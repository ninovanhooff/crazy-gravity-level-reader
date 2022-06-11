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
    write_magnets(fp, cgl);
    write_airgens(fp, cgl);
    write_cannons(fp, cgl);
    write_bars(fp, cgl);
    write_gates(fp, cgl);
    write_lgates(fp, cgl);
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

void write_bars(FILE *fp, struct cgl *cgl){
    struct bar *bar = cgl->bars;

    for (struct bar *start = bar; bar < start + cgl->nbars; ++bar)
    {
        enum orientations egOrientation = map_cg_orientation(bar->orientation);
        int distance = bar->len / LUA_UNIT_PX + 3;
        int x,y,w,h, speedMin, speedMax, fixedGap, gapSize, changeOften;
        x = bar->beg->x / LUA_UNIT_PX + 1;
        y = bar->beg->y / LUA_UNIT_PX + 1;
        speedMin = bar->sspeed;
        speedMax = bar->fspeed;
        fixedGap = map_cg_gap_type(bar->gap_type);
        gapSize = bar->gap / LUA_UNIT_PX;
        changeOften = bar->freq; // freq is not the frequency, but a bool (int 0 or 1) indicating whether to change often
        if (changeOften == 0)
        {
            // 1: yes; 2 = no
            changeOften = 2;
        }

        switch (egOrientation)
        {
        case HORIZONTAL:
            w = distance + 6;
            h = 2;
            break;
        case VERTICAL:
            h = distance + 6;
            w = 2;
            break;
        default:
            assert(!"Not a valid orientation type!");
            break;
        }

        fprintf(fp, "{\n");

        write_int_entry(fp, "sType", ROD);
        write_int_entry(fp, "x", x);
        write_int_entry(fp, "y", y);
        write_int_entry(fp, "w", w);
        write_int_entry(fp, "h", h);
        write_int_entry(fp, "direction", egOrientation);
        write_int_entry(fp, "distance", distance);
        write_int_entry(fp, "speedMin", speedMin);
        write_int_entry(fp, "speedMax", speedMax);
        write_int_entry(fp, "fixdGap", fixedGap); // not a spelling mistake, foxdGap is without e in output format
        write_int_entry(fp, "gapSize", gapSize);
        write_int_entry(fp, "chngOften", changeOften);
        write_int_entry(fp, "pos1", 20);
        write_int_entry(fp, "pos2", 20); // todo?

        fprintf(fp, "},\n");
    }
}

void write_cannons(FILE *fp, struct cgl *cgl){
    struct cannon *cannon = cgl->cannons;

    for (struct cannon *start = cannon; cannon < start + cgl->ncannons; ++cannon)
    {
        printf("speedx: %d, speedy: %d, dir: %d\n", cannon->speed_x, cannon->speed_y, cannon->dir);
        int direction = map_cg_direction(cannon->dir);
        printf("direction: %d, dir: %d\n", direction, cannon->dir);
        int x,y,w,h,distance;
        int startX = cannon->beg_base->x / LUA_UNIT_PX + 1;
        int startY = cannon->beg_base->y / LUA_UNIT_PX + 1;
        int endX = cannon->end_base->x / LUA_UNIT_PX + 1;
        int endY = cannon->end_base->y / LUA_UNIT_PX + 1;
        int speed = max(abs(cannon->speed_x), abs(cannon->speed_y));
        int rate = cannon->fire_rate;

        switch (direction)
        {
        case UP:
            x = endX;
            y = endY;
            w = 3;
            h = startY - endY - 2;
            distance = h;
            break;
        case DOWN:
            x = startX;
            y = startY;
            w = 3;
            h = endY - startY - 3;
            distance = h;
            break;
        case LEFT:
            x = endX;
            y = endY;
            w = startX - endX - 2;
            h = 3;
            distance = w;
            break;
        case RIGHT:
            x = startX;
            y = startY;
            w = endX - startX - 3;
            h = 3;
            distance = w;
            break;
        default:
            assert(!"Not a valid direction");
            break;
        }

        fprintf(fp, "{\n");

        write_int_entry(fp, "sType", CANNON);
        write_int_entry(fp, "x", x);
        write_int_entry(fp, "y", y);
        write_int_entry(fp, "w", w);
        write_int_entry(fp, "h", h);
        write_int_entry(fp, "direction", direction);
        write_int_entry(fp, "distance", distance);
        write_int_entry(fp, "speed", speed);
        write_int_entry(fp, "rate", rate);

        fprintf(fp, "},\n");
    }
}


/** Colr-key gates */
void write_lgates(FILE *fp, struct cgl *cgl){
    struct lgate *lgate = cgl->lgates;

    for (struct lgate *start = lgate; lgate < start + cgl->nlgates; ++lgate)
    {
        int direction, distance, x, y, w, h, pos;
        int endStone = lgate->has_end;

        int actW = ceil(lgate->act->w / LUA_UNIT_PX);
        int actH = ceil(lgate->act->h / LUA_UNIT_PX);
        int red = lgate->keys[0];
        int green = lgate->keys[1];
        int blue = lgate->keys[2];
        int yellow = lgate->keys[3];

        // printf("lgate egDir:%d orientation:%d lgateType: %d lgate_max_len: %f\n", egDir, lgate->orient, lgate->type, lgate->max_len);

        switch (lgate->type)
        {
        case GateLeft:
            direction = RIGHT;
            break;
        case GateTop:
            direction = DOWN;
            break;
        case GateRight:
            direction = LEFT;
            break;
        case GateBottom:
            direction = UP;
            break;
        default:
            assert(!"Not a valid gate type!");
            break;
        }

        printf("direction: %d\n", direction);
        printf("lgate base0X:%d base1X:%d base2X: %d base3X: %d base4X: %d\n", lgate->base[0]->x, lgate->base[1]->x, lgate->base[2]->x, lgate->base[3]->x, lgate->base[4]->x);    
        printf("lgate base0Y:%d base1Y:%d base2Y: %d base3Y: %d base4Y: %d\n", lgate->base[0]->y, lgate->base[1]->y, lgate->base[2]->y, lgate->base[3]->y, lgate->base[4]->y);    

        struct tile* topLeftTile;
        struct tile* topLeftTileY;
        switch (direction)
        {
        case UP:
            topLeftTile = lgate->base[0];
            topLeftTileY = lgate->base[4];
            break;
        case DOWN:
            topLeftTile = lgate->base[1];
            topLeftTileY = lgate->base[0];
            break;
        case LEFT:
            topLeftTile = lgate->base[4];
            topLeftTileY = lgate->base[1];
            break;
        case RIGHT:
            topLeftTile = lgate->base[0];
            topLeftTileY = lgate->base[0];
            break;
        default:
            assert(!"Not a valid direction type!");
            break;
        }
        x = topLeftTile->x / LUA_UNIT_PX + 1;
        y = topLeftTileY->y / LUA_UNIT_PX + 1;

        distance = lgate->max_len/LUA_UNIT_PX + 1 + endStone * 2;
        if (endStone == 1)
        {
            pos = lgate->max_len + 4; // pos is in pixels
        } else {
            pos = lgate->max_len;
        }

        switch (direction)
        {
        case UP:
        case DOWN:
            w = 6;
            h = distance + 6;
            break;
        case LEFT:
        case RIGHT:
            w = distance + 6;
            h = 6;
            break;
        default:
            assert(!"Not a valid direction type!");
            break;
        }

        fprintf(fp, "{\n");

        write_int_entry(fp, "sType", BARRIER);
        write_int_entry(fp, "x", x);
        write_int_entry(fp, "y", y);
        write_int_entry(fp, "w", w);
        write_int_entry(fp, "h", h);
        write_int_entry(fp, "actW", actW);
        write_int_entry(fp, "actH", actH);
        write_int_entry(fp, "direction", direction);
        write_int_entry(fp, "distance", distance);
        write_int_entry(fp, "pos", pos);
        write_int_entry(fp, "endStone", endStone);
        write_int_entry(fp, "red", red);
        write_int_entry(fp, "green", green);
        write_int_entry(fp, "blue", blue);
        write_int_entry(fp, "yellow", yellow);

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
        int direction, distance, x, y, w, h, pos;
        int xToY = -1; // -1 indicates invalid / uninitialized
        int endStone = gate->has_end;

        int actW = ceil(gate->act->w / LUA_UNIT_PX);
        int actH = ceil(gate->act->h / LUA_UNIT_PX);


        // printf("gate egDir:%d orientation:%d gateType: %d gate_max_len: %f\n", egDir, gate->orient, gate->type, gate->max_len);

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

        // printf("direction: %d\n", direction);
        // printf("gate base0X:%d base1X:%d base2X: %d base3X: %d base4X: %d\n", gate->base[0]->x, gate->base[1]->x, gate->base[2]->x, gate->base[3]->x, gate->base[4]->x);    
        // printf("gate base0Y:%d base1Y:%d base2Y: %d base3Y: %d base4Y: %d\n", gate->base[0]->y, gate->base[1]->y, gate->base[2]->y, gate->base[3]->y, gate->base[4]->y);    

        struct tile* topLeftTile;
        struct tile* topLeftTileY;
        switch (direction)
        {
        case UP:
            topLeftTile = gate->base[0];
            topLeftTileY = gate->base[4]; //todo y= baseY -  distance
            break;
        case LEFT:
            topLeftTile = gate->base[4];
            topLeftTileY = gate->base[2];
            break;
        case DOWN:
            topLeftTile = gate->base[2];
            topLeftTileY = gate->base[2];
            break;
        case RIGHT:
            topLeftTile = gate->base[0];
            topLeftTileY = gate->base[0];
            break;
        default:
            assert(!"Not a valid direction type!");
            break;
        }
        x = topLeftTile->x / LUA_UNIT_PX + 1;
        y = topLeftTileY->y / LUA_UNIT_PX + 1;

        distance = gate->max_len/LUA_UNIT_PX + 1 + endStone * 2;
        if (endStone == 1)
        {
            pos = gate->max_len + 4; // pos is in pixels
        } else {
            pos = gate->max_len;
        }

        switch (direction)
        {
        case UP:
        case DOWN:
            w = 6;
            h = distance + 6;
            break;
        case LEFT:
        case RIGHT:
            w = distance + 6;
            h = 6;
            break;
        default:
            assert(!"Not a valid direction type!");
            break;
        }

        fprintf(fp, "{\n");

        write_int_entry(fp, "sType", ONE_WAY);
        write_int_entry(fp, "x", x);
        write_int_entry(fp, "y", y);
        write_int_entry(fp, "w", w);
        write_int_entry(fp, "h", h);
        write_int_entry(fp, "actW", actW);
        write_int_entry(fp, "actH", actH);
        write_int_entry(fp, "direction", direction);
        write_int_entry(fp, "distance", distance);
        write_int_entry(fp, "pos", pos);
        write_int_entry(fp, "endStone", endStone);
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

enum orientations map_cg_orientation(int cgOrientation)
{
    switch (cgOrientation)
    {
    case Horizontal:
        return HORIZONTAL;
    case Vertical:
        return VERTICAL;
    default:
        assert(!"Not a valid orientation type!");
        return -1;
    }
}

enum gapTypes map_cg_gap_type(int cgGapType)
{
    switch (cgGapType)
    {
    case Constant:
        return FIXED;
    case Variable:
        return VARIABLE;
    default:
        assert(!"Not a valid gap type!");
        return -1;
    }
}

