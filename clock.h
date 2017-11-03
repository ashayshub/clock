#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <math.h>
#include <signal.h>
#include <ncurses.h>

#define DEGTORAD(deg) (deg * (M_PI/180.0f))
#define HASH 35
#define DOT 46
#define AT 64

#define ONE_SECOND 1
#define BASE_WIDTH 31
#define BASE_HEIGHT 32
#define DEG_SHIFT 90.0
#define SIXTY_PACER 360/60
/* ToDo: Make HOURLY_PACER MORE FINE GRAINED */
#define HOURLY_PACER 360/12

void endwin_exit(void);
void sig_handler(int signo);
int draw_circle(WINDOW *mainwin, int x_coord, int y_coord, int radius);
int plot_time(int x, int y);
int draw_line(WINDOW *win, float deg, int len_buff, int x, int y, char plot_char);
