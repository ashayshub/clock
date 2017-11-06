#include "clock.h"

void endwin_exit(void){
  refresh();
  endwin();
  exit(EXIT_SUCCESS);
}

void sig_handler(int signo)
{
  if (signo == SIGUSR1){
    endwin_exit();
  }
  else if (signo == SIGINT){
    endwin_exit();
  }
}

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
  
  /* Distance between two points: sqrt ([x1-x2]^2+[y1-y2]^2)*/
  /* ToDo: correct and get proper distance */
  float p = (width)^2;
  float q = (height)^2;
  float radius = sqrt(p + q);

  for (int i = 0; i < radius+len_buff; i++){
    line_coord.x = width + (int)(i * cos(DEGTORAD(deg)));
    line_coord.y = height + (int)(i * sin(DEGTORAD(deg)));
    mvwaddch(win, line_coord.y, line_coord.x, plot_char);
  }
  
  return 0;
}


int plot_time(void){
  int width = 30;
  int height = 12;
  struct tm *timeinfo;
  char *result;
  time_t rawtime;
  struct tm timeinfoBuffer;

  /* Plot second, minute and hour hand for the clock */
  while (signal(SIGINT, sig_handler) != SIG_ERR){
  /* while (1){      */

    /* 
       Collect time information, could be collected 
       every second or minute  along with a 
       local counter variable depending on accuracy required 
    */    
    time(&rawtime);
    timeinfo = localtime_r(&rawtime , &timeinfoBuffer);
    result = malloc(26 * sizeof(char));    
    result = asctime_r(timeinfo, result);
    
    /* Need 12 hour format */
    int hour = CONVHOUR(TWHOUR(timeinfo->tm_hour));

    /*
      Fine grained shift in hour hand
      hour hand moves every 15 min
    */
    float incr = timeinfo->tm_min*(0.5);
    
    /* Convert time information to corresponding degrees */    
    float seconds_deg = (float) (timeinfo->tm_sec*SIXTY_PACER) - DEG_SHIFT;
    float min_deg = (float) (timeinfo->tm_min*SIXTY_PACER) - DEG_SHIFT;
    float hour_deg = (float) (hour*HOURLY_PACER) + incr - DEG_SHIFT;    
    
    /* Plot analog information */
    WINDOW *win = newwin(30, 60, 2, 20);
    wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');
    /* Draw Seconds hand */
    draw_line(win, seconds_deg, 5, width, height, DOT);
    /* Draw Minute hand */
    draw_line(win, min_deg, 5, width, height, HASH);
    /* Draw hour hand */
    draw_line(win, hour_deg, 5, width, height, AT);
    /* Print time     */
    WINDOW *win2 = newwin(5, 60, 27, 20);

    /* Plot ASCII information */
    wborder(win2, '|', '|', '-', '-', '+', '+', '+', '+');
    mvwprintw(win2, BASE_HEIGHT+1, BASE_WIDTH-10, "%s", result);
    mvwprintw(win2, BASE_HEIGHT+3, BASE_WIDTH-8, "Press Ctrl+c to exit");
    wrefresh(win);
    wrefresh(win2);

    sleep(ONE_SECOND);
    delwin(win);
    delwin(win2);
    free(result);
  }

  return 0;
}


int main(int argc, char **argv){
  
  /*Suitable height and width */
  
  /*  Initialize ncurses  */
  if ( initscr() == NULL ) {
    fprintf(stderr, "Error initialising ncurses.\n");
    exit(EXIT_FAILURE);
  }
  noecho();
  plot_time();
  endwin_exit();
}
