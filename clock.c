#include "clock.h"

int draw_circle(int width, int height, int radius){
  float deg;
  int x, y;

  for(deg = 0; deg < 360.0f; deg += 1.0f){
    x = width + (int)(width * cos(DEGTORAD(deg)));
    y = height + (int)(height * sin(DEGTORAD(deg)));
    mvaddch(y, x, HASH);
  }

  /* Draw center to the circle */
  mvaddch(height, width, HASH);
  
  return 0;
}

/* int draw_hand(int x, ) */
int draw_line(int width, int height, float deg, int erase){
  struct coord {
    int x;
    int y;
  };
  WINDOW *win = newwin(0, 0, 0, 0);
  struct coord line_coord;
  /* sqrt ([x1-x2]^2+[y1-y2]^2)*/
  float p = (width)^2;
  float q = (height)^2;
  float radius = sqrt(p + q);
  
  for (int i = 0; i < radius+8; i++){
      line_coord.x = width + (int)(i * cos(DEGTORAD(deg*6)));
      line_coord.y = height + (int)(i * sin(DEGTORAD(deg*6)));
      mvwaddch(win, line_coord.y, line_coord.x, DOT);
      /* printw("deg: %f, cos: %f, sin: %f", deg, cos(DEGTORAD(deg*6)), sin(DEGTORAD(deg*6))); */
  }
  wrefresh(win);
  return 0;
}

int plot_time(int width, int height){
  time_t rawtime;
  struct tm *timeinfo;
  struct tm   timeinfoBuffer;
  char *result;
  float deg = 0;
  
  while (1){
    if (deg == 360.0f)
      deg = 0;
    deg += 1.0;
    
    time(&rawtime);
    /*Plot second, minute and hour hand for the clock*/
    
    draw_line(width, height, deg, 1);
    
    /*Plot base time in ASCII*/
    timeinfo = localtime_r(&rawtime , &timeinfoBuffer);
    result = malloc(26 * sizeof(char));
    result = asctime_r(timeinfo, result);
    mvprintw(BASE_HEIGHT, BASE_WIDTH, "%s", result);
    refresh();
    sleep(ONE_SECOND);
    free(result);
  }
  
  return 0;
}


int main(int argc, char **argv){
  int width = 43;
  int height = 15;

  WINDOW * mainwin;
  
  /*  Initialize ncurses  */
  if ( (mainwin = initscr()) == NULL ) {
    fprintf(stderr, "Error initialising ncurses.\n");
    exit(EXIT_FAILURE);
  }
  draw_circle(width, height, 10);  
  plot_time(width, height);
  
  refresh();
  /* draw_seconds_hand(); */
  endwin();
  return EXIT_SUCCESS;
}
