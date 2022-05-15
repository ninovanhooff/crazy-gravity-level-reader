#ifndef LUA_WRITER_H
#define LUA_WRITER_H

#include "cgl.h"
#include <stdio.h>
#include <stdint.h>

void write_lua(FILE *fp, struct cgl *cgl);
void write_table_start(FILE *fp);



#endif