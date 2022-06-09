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
    write_fans(fp, cgl);
    write_airgens(fp, cgl);
    write_magnets(fp, cgl);
    write_gates(fp, cgl);
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
    }
}

void write_fans(FILE *fp, struct cgl *cgl){
    struct fan *fan = cgl->fans;

    for (struct fan *start = fan; fan < start + cgl->nfans; ++fan)
    {
        enum directions egDir = map_cg_direction(fan->dir);
        enum gratings egGrating = map_cg_power(fan->power);
        int distance, x, y, w, h;

        switch (egDir)
        {
        case UP:
        case LEFT:
            x = fan->act->x / LUA_UNIT_PX + 1;
            y = fan->act->y / LUA_UNIT_PX + 1;
            break;
        case DOWN:
        case RIGHT:
            x = fan->base->x / LUA_UNIT_PX + 1;
            y = fan->base->y / LUA_UNIT_PX + 1;
            break;
        default:
            assert(!"Not a valid direction type!");
            break;
        }

        switch (egDir)
        {
        case UP:
        case DOWN:
            distance = ceil(fan->act->h / LUA_UNIT_PX);
            w = fan->act->w / LUA_UNIT_PX;
            h = (fan->base->h + fan->pipes->h + fan->act->h) / LUA_UNIT_PX;
            break;
        case LEFT:
        case RIGHT:
            distance = ceil(fan->act->w / LUA_UNIT_PX);
            w = (fan->base->w + fan->pipes->w + fan->act->w) / LUA_UNIT_PX;
            h = fan->base->h / LUA_UNIT_PX;
            break;
        default:
            assert(!"Not a valid direction type!");
            break;
        }

        fprintf(fp, "{\n");

        write_int_entry(fp, "sType", BLOWER);
        write_int_entry(fp, "x", x);
        write_int_entry(fp, "y", y);
        write_int_entry(fp, "w", w);
        write_int_entry(fp, "h", h);
        write_int_entry(fp, "direction", egDir);
        write_int_entry(fp, "grating", egGrating);
        write_int_entry(fp, "distance", distance);

        fprintf(fp, "},\n");
    }
}

void write_airgens(FILE *fp, struct cgl *cgl){
    struct airgen *airgen = cgl->airgens;

    for (struct airgen *start = airgen; airgen < start + cgl->nairgens; ++airgen)
    {
        enum directions egDir = map_cg_direction(airgen->dir);
        enum rotates egRotates = map_cg_spin(airgen->spin);
        int distance, x, y, w, h;

        switch (egDir)
        {
        case UP:
        case LEFT:
            x = airgen->act->x / LUA_UNIT_PX + 1;
            y = airgen->act->y / LUA_UNIT_PX + 1;
            break;
        case DOWN:
        case RIGHT:
            x = airgen->base->x / LUA_UNIT_PX + 1;
            y = airgen->base->y / LUA_UNIT_PX + 1;
            break;
        default:
            assert(!"Not a valid direction type!");
            break;
        }

        switch (egDir)
        {
        case UP:
        case DOWN:
            distance = ceil(airgen->act->h / LUA_UNIT_PX);
            w = airgen->act->w / LUA_UNIT_PX;
            h = (airgen->base->h + airgen->pipes->h + airgen->act->h) / LUA_UNIT_PX;
            break;
        case LEFT:
        case RIGHT:
            distance = ceil(airgen->act->w / LUA_UNIT_PX);
            w = (airgen->base->w + airgen->pipes->w + airgen->act->w) / LUA_UNIT_PX;
            h = airgen->base->h / LUA_UNIT_PX;
            break;
        default:
            assert(!"Not a valid direction type!");
            break;
        }

        fprintf(fp, "{\n");

        write_int_entry(fp, "sType", ROTATOR);
        write_int_entry(fp, "x", x);
        write_int_entry(fp, "y", y);
        write_int_entry(fp, "w", w);
        write_int_entry(fp, "h", h);
        write_int_entry(fp, "direction", egDir);
        write_int_entry(fp, "rotates", egRotates);
        write_int_entry(fp, "distance", distance);

        fprintf(fp, "},\n");
    }
}


void write_magnets(FILE *fp, struct cgl *cgl){
    struct magnet *magnet = cgl->magnets;

    for (struct magnet *start = magnet; magnet < start + cgl->nmagnets; ++magnet)
    {
        enum directions egDir = map_cg_direction(magnet->dir);
        int distance, x, y, w, h;

        switch (egDir)
        {
        case UP:
        case LEFT:
            x = magnet->act->x / LUA_UNIT_PX + 1;
            y = magnet->act->y / LUA_UNIT_PX + 1;
            break;
        case DOWN:
        case RIGHT:
            x = magnet->base->x / LUA_UNIT_PX + 1;
            y = magnet->base->y / LUA_UNIT_PX + 1;
            break;
        default:
            assert(!"Not a valid direction type!");
            break;
        }

        switch (egDir)
        {
        case UP:
        case DOWN:
            distance = ceil(magnet->act->h / LUA_UNIT_PX);
            w = magnet->act->w / LUA_UNIT_PX;
            h = (magnet->base->h + magnet->magn->h + magnet->act->h) / LUA_UNIT_PX;
            break;
        case LEFT:
        case RIGHT:
            distance = ceil(magnet->act->w / LUA_UNIT_PX);
            w = (magnet->base->w + magnet->magn->w + magnet->act->w) / LUA_UNIT_PX;
            h = magnet->base->h / LUA_UNIT_PX;
            break;
        default:
            assert(!"Not a valid direction type!");
            break;
        }

        fprintf(fp, "{\n");

        write_int_entry(fp, "sType", MAGNET);
        write_int_entry(fp, "x", x);
        write_int_entry(fp, "y", y);
        write_int_entry(fp, "w", w);
        write_int_entry(fp, "h", h);
        write_int_entry(fp, "direction", egDir);
        write_int_entry(fp, "distance", distance);

        fprintf(fp, "},\n");
    }
}


/** One-way gates */
void write_gates(FILE *fp, struct cgl *cgl){
    struct gate *gate = cgl->gates;

    for (struct gate *start = gate; gate < start + cgl->ngates; ++gate)
    {
        // only Up or Down for gates
        enum directions egDir = map_cg_direction(gate->dir);
        int direction, distance, x, y, w, h;
        int max_len = gate->max_len / LUA_UNIT_PX;
        int xToY = -1; // -1 indicates invalid / uninitialized

        printf("gate egDir:%d orientation:%d gateType: %d\n", egDir, gate->orient, gate->type);

        switch (gate->type)
        {
        case GateLeft:
            direction = RIGHT;
            if (egDir == DOWN)
            {   
                // horizontal gate, pass direction from top to bottom
                xToY = 1;
            } else { 
                // horizontal gate, pass direction from bottom to top
                xToY = 2;
            }
            break;
        case GateTop:
            direction = DOWN;
            if (egDir == DOWN)
            {   
                // vertical gate, pass direction from right to left
                xToY = 2;
            } else { 
                // vertical gate, pass direction from left to right
                xToY = 1;
            }
            break;
        case GateRight:
            direction = LEFT;
            if (egDir == DOWN)
            {   
                // horizontal gate, pass direction from top to bottom
                xToY = 2;
            } else { 
                // horizontal gate, pass direction from bottom to top
                xToY = 1;
            }
            break;
        case GateBottom:
            direction = UP;
            if (egDir == DOWN)
            {   
                // vertical gate, pass direction from right to left
                xToY = 1; //todo
            } else { 
                // vertical gate, pass direction from left to right
                xToY = 2;
            }
            break;
        default:
            assert(!"Not a valid gate type!");
            break;
        }

        printf("direction: %d\n", direction);
        printf("gate base0X:%d base1X:%d base2X: %d base3X: %d base4X: %d\n", gate->base[0]->x, gate->base[1]->x, gate->base[2]->x, gate->base[3]->x, gate->base[4]->x);    
        struct tile* topLeftTile;
        switch (direction)
        {
        case UP:
            topLeftTile = gate->base[0];
            break;
        case LEFT:
        case DOWN:
            topLeftTile = gate->base[2];
            break;
        case RIGHT:
            topLeftTile = gate->base[0];
            break;
        default:
            assert(!"Not a valid direction type!");
            break;
        }
        x = topLeftTile->x / LUA_UNIT_PX + 1;
        y = topLeftTile->y / LUA_UNIT_PX + 1;

        switch (egDir)
        {
        case UP:
        case DOWN:
            distance = ceil(gate->act->h / LUA_UNIT_PX);
            w = 6;
            h = max_len + 6;
            break;
        case LEFT:
        case RIGHT:
            distance = ceil(gate->act->w / LUA_UNIT_PX);
            w = max_len + 6;
            h = 6;
            break;
        default:
            assert(!"Not a valid direction type!");
            break;
        }

        int endStone = gate->has_end;
        // set the position of the gate to closed (maximum extension)
        int pos = distance*LUA_UNIT_PX-endStone*16-4;

        fprintf(fp, "{\n");

        write_int_entry(fp, "sType", ONE_WAY);
        write_int_entry(fp, "x", x);
        write_int_entry(fp, "y", y);
        write_int_entry(fp, "w", w);
        write_int_entry(fp, "h", h);
        write_int_entry(fp, "direction", direction);
        write_int_entry(fp, "distance", distance);
        write_int_entry(fp, "pos", pos);
        write_int_entry(fp, "endStrone", endStone);
        write_int_entry(fp, "XtoY", xToY);

        fprintf(fp, "},\n");
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

enum directions map_cg_direction(int cgDir){
    switch (cgDir)
    {
    case Up:
        return UP;
    case Down:
        return DOWN;
    case Left: 
        return LEFT;
    case Right:
        return RIGHT;
    default:
        assert(!"Not a valid direction type!");
        return -1;
    }
}


enum gratings map_cg_power(int cgPower)
{
    switch (cgPower)
    {
    case Hi:
        return NO;
    case Low: 
        return YES; // grating indicates low power

    default:
        assert(!"Not a valid power type!");
        return -1;
    }
}

enum rotates map_cg_spin(int cgSpin)
{
    switch (cgSpin)
    {
    case CCW:
        return COUNTER_CLOCKWISE;
    case CW: 
        return CLOCKWISE; 
    default:
        assert(!"Not a valid spin type!");
        return -1;
    }
}

