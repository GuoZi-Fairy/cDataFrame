#include <string.h>
#include <stdio.h>
#include "dataframe.h"
#ifndef CSVREADER_H
    #define CSVREADER_H
    #include "csvReader.h"
#endif
#define STDCAL(type) _cdecl type
#define EXPR_PARSE_H
extern STDCAL(cell) eval(const char* expr);