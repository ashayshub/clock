#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <math.h>
#include <signal.h>
#include <ncurses.h>

#define DEGTORAD(deg) (deg * (M_PI/180.0f))
#define TWHOUR(hour)  hour > 12 ? hour - 12 : hour
#define CONVHOUR(hour)  hour == 0 ? 12 : hour

/* ASCII Characters */
#define HASH 35
#define DOT 46
#define AT 64

#define ONE_SECOND 1
#define BASE_WIDTH 32
#define BASE_HEIGHT 0
#define DEG_SHIFT 90.0f
/* 
For minute and second hand movement, 
in one min/sec, min/sec hand moves 360/60 deg
*/
#define SIXTY_PACER 360/60.0f
/*
In one hour, hour hand moves 360/12 = 30 deg
In one minute, hour hand moves 360/(12*60) = 30/60 = 0.5 deg
In one second, hour hand moves 360/(12*60*60) = 1/120 deg
*/
#define HOURLY_PACER 30.0f

void endwin_exit(void);
void sig_handler(int signo);
int draw_circle(WINDOW *mainwin, int x_coord, int y_coord, int radius);
int plot_time(void);
int draw_line(WINDOW *win, float deg, int len_buff, int x, int y, char plot_char);
void get_time(struct tm *timeinfo, char *result);
