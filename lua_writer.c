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
    write_file_start(fp);
    write_table_start(fp, NULL);
    
    write_table_start(fp, "levelProps");
    write_level_props(fp, cgl);
    write_table_end(fp, false);
    
    write_table_start(fp, "specialT");
    write_platforms(fp, cgl);
    write_table_end(fp, true);

    write_table_end(fp, true);
}

void write_level_props(FILE *fp, struct cgl *cgl)
{
    write_int_entry(fp, "sizeX", cgl->width * LUA_UNITS_PER_CG_BLOCK);
    write_int_entry(fp, "sizeY", cgl->height * LUA_UNITS_PER_CG_BLOCK);
    write_int_entry(fp, "lives", 5); // not sure if stored in cgl
    write_int_entry(fp, "fuel", 6000);
    write_int_entry(fp, "bg", 0);
    write_int_entry(fp, "tLimit", 300);
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
        write_int_entry(fp, "x", ap->base->x / LUA_UNIT_PX + 1);
        write_int_entry(fp, "y", ap->base->y / LUA_UNIT_PX - 2);
        write_int_entry(fp, "w", ap->base->w / LUA_UNIT_PX);
        write_int_entry(fp, "h", LUA_PLATFORM_HEIGHT);
        write_int_entry(fp, "amnt", ap->num_cargo);

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
                extras_idx + 1
            );
        }

        if (pType == KEY)
        {
            write_int_entry(
                fp, "color",
                ap->c.key + 1
            );
        }

        fprintf(fp, "},\n");
        // fprintf("num_cargo: %zu\n", aps->num_cargo);
    }
}



void write_file_start(FILE *fp)
{
    fprintf(fp, 
    "-- CGLLUA1\n"
    "return\n"
    );
}
void write_table_start(FILE *fp, char *name)
{
    if (name)
    {
        fprintf(fp, "%s={\n", name);
    } else {
        fprintf(fp, "{\n");
    }
}

void write_table_end(FILE *fp, bool isLast)
{
    if (isLast)
    {
        fprintf(fp, "}\n");    
    } else {
        fprintf(fp, "},\n");
    }
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
