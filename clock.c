#include "clock.h"

int draw_circle(WINDOW *mainwin, int width, int height, int radius){
  float deg;
  int x, y;

  for(deg = 0; deg < 360.0f; deg += 1.0f){
    x = width + (int)(width * cos(DEGTORAD(deg)));
    y = height + (int)(height * sin(DEGTORAD(deg)));
    mvwaddch(mainwin, y, x, HASH);
  }

  /* Draw center to the circle */
  mvwaddch(mainwin, height, width, HASH);
  
  return 0;
}

/* int draw_hand(int x, ) */
int draw_line(WINDOW *win, float deg, int len_buff, int width, int height, char plot_char){
  struct coord {
    int x;
    int y;
  };
  struct coord line_coord;

  
  /* distance between two points: sqrt ([x1-x2]^2+[y1-y2]^2)*/
  float p = (width)^2;          /* I know, wrong */
  float q = (height)^2;
  float radius = sqrt(p + q);
  
  
  for (int i = 0; i < radius+len_buff; i++){
    line_coord.x = width + (int)(i * cos(DEGTORAD(deg)));
    line_coord.y = height + (int)(i * sin(DEGTORAD(deg)));
    mvwaddch(win, line_coord.y, line_coord.x, plot_char);
  }
  
  //printw("deg: %f, timeinsec: %d", deg, (int)timeinfo->tm_sec);
  
  return 0;
}

int plot_time(int width, int height){
  time_t rawtime;
  struct tm *timeinfo;
  struct tm timeinfoBuffer;
  char *result;

  while (1){
    
    time(&rawtime);
    /* Plot second, minute and hour hand for the clock */
        
    /* Collect time information */
    timeinfo = localtime_r(&rawtime , &timeinfoBuffer);
    result = malloc(26 * sizeof(char));
    result = asctime_r(timeinfo, result);

    /* Convert time information to corresponding degrees */
    float seconds_deg = (float) timeinfo->tm_sec*6 - DEG_SHIFT;
    float min_deg = (float) timeinfo->tm_min*6 - DEG_SHIFT;
    float hour_deg = (float) timeinfo->tm_hour*30 - DEG_SHIFT;
    

    WINDOW *win = newwin(0, 0, 0, 0);
    /* Draw Seconds hand */
    draw_line(win, seconds_deg, 8, width, height, DOT);
    /* Draw Minute hand */
    draw_line(win, min_deg, 8, width, height, AT);
    /* Draw hour hand */
    draw_line(win, hour_deg, 8, width, height, HASH);
    /* Print time     */
    mvwprintw(win, BASE_HEIGHT, BASE_WIDTH, "%s", result);
    wrefresh(win);
    refresh();
    
    sleep(ONE_SECOND);
    delwin(win);
    free(result);
  }
  
  return 0;
}


int main(int argc, char **argv){
  int width = 43;
  int height = 15;

  
  /*  Initialize ncurses  */
  if ( initscr() == NULL ) {
    fprintf(stderr, "Error initialising ncurses.\n");
    exit(EXIT_FAILURE);
  }

  plot_time(width, height);

  endwin();
  
  return EXIT_SUCCESS;
}
