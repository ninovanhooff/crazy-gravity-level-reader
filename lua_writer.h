#ifndef LUA_WRITER_H
#define LUA_WRITER_H

#include "cgl.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/** pixels per unit in lua files) */
#define LUA_UNIT_PX 8

/** CG uses 32px blocks, lua uses 8px tiles */
#define LUA_UNITS_PER_CG_BLOCK 4

/** platform height in lua units (8px) includes space for cago, fuel, etc. */
#define LUA_PLATFORM_HEIGHT 6

/** special objec types */
enum sTypes {
    PLATFORM = 8,
    BLOWER = 9, // CG: Fan
    MAGNET = 10,
    ROTATOR = 11,
    CANNON = 12,
    ROD = 13,
    ONE_WAY = 14,
    BARRIER = 15 
};

enum pTypes {
    HOME = 1,
    FREIGHT = 2,
    FUEL = 3,
    EXTRAS = 4,
    KEY = 5
};


void write_lua(FILE *fp, struct cgl *cgl);
void write_file_start(FILE *fp);
void write_level_props(FILE *fp, struct cgl *cgl);
/** name may be null */
void write_table_start(FILE *fp, char *name);
void write_platforms(FILE *fp, struct cgl *cgl);
void write_table_end(FILE *fp, bool isLast);
void write_int_entry(FILE *fp, char* key, int value);
enum pTypes map_cg_platform(int cgType);


#endif