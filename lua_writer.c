#include "cgl.h"
#include "mathgeom.h"
#include "lua_writer.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <errno.h>

void write_lua(FILE *fp, struct cgl *cgl)
{
    write_table_start(fp);
    write_platforms(fp, cgl);
    write_table_end(fp);
}

void write_table_start(FILE *fp)
{
    fprintf(fp,
            "-- CGLLUA1\n"
            "return {\n");
}
void write_platforms(FILE *fp, struct cgl *cgl)
{
    struct airport *ap = cgl->airports;

    for (struct airport *start = ap; ap < start + cgl->nairports; ++ap)
    {
        fprintf(fp, "{\n");

        enum pTypes pType = map_cg_platform(ap->type);

        // common
        write_int_entry(fp, "sType", PLATFORM);
        write_int_entry(fp, "pType", pType);
        write_int_entry(fp, "x", ap->base->x / LUA_UNIT_PX);
        write_int_entry(fp, "y", ap->base->y / LUA_UNIT_PX);
        write_int_entry(fp, "w", ap->base->w / LUA_UNIT_PX);
        write_int_entry(fp, "h", LUA_PLATFORM_HEIGHT);
        write_int_entry(fp, "amount", ap->num_cargo);

        if (pType == HOME)
        {
            write_int_entry(fp, "arrows", ap->has_left_arrow);
        }

        if (pType == FREIGHT)
        {
            write_int_entry(
                fp, "type",
                ap->c.freight[0].f
            );
        }

        if (pType == EXTRAS)
        {
            int extras_idx = ap->c.extras[0];
            write_int_entry(
                fp, "type",
                extras_idx
            );
        }

        if (pType == KEY)
        {
            write_int_entry(
                fp, "color",
                ap->c.key
            );
        }

        fprintf(fp, "},\n");
        // fprintf("num_cargo: %zu\n", aps->num_cargo);
    }
}

void write_table_end(FILE *fp)
{
    fprintf(fp, "}\n");
}

void write_int_entry(FILE *fp, char *key, int value)
{
    fprintf(fp, "[\"%s\"]=%d,\n", key, value);
};

enum pTypes map_cg_platform(int cgType)
{
    switch (cgType)
    {
    case Homebase:
        return HOME;
    case Key:
        return KEY;
    case Fuel:
        return FUEL;
    case Freight:
        return FREIGHT;
    case Extras:
        return EXTRAS;
    default:
        assert(!"Not a valid platform type!");
        return -1;
    }
}
