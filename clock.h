#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <math.h>
#include <ncurses.h>

#define DEGTORAD(deg) (deg * (M_PI/180.0f))
#define HASH 35
#define DOT 46
#define ONE_SECOND 1
#define BASE_WIDTH 31
#define BASE_HEIGHT 32

int draw_circle(WINDOW *mainwin, int x_coord, int y_coord, int radius);
int plot_time(int x, int y);
int draw_line(WINDOW *win, struct tm *timeinfo, int x, int y);
