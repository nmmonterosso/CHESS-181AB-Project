#ifndef XBOARD_H_
#define XBOARD_H_

//MACROS for xboard()
#define OK       0
#define NO_INPUT 1
#define TOO_LONG 2

#include "board.h"

static int getline(char *buff, size_t size);
void rmSubstr(char *str, const char *toRemove);
void xboard(Board *board);

#endif // !XBOARD_H_
#pragma once

