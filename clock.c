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

int plot_time(int width, int height){
  time_t rawtime;
  struct tm *timeinfo;
  struct tm timeinfoBuffer;
  char *result;

  /* Plot second, minute and hour hand for the clock */
  while (signal(SIGINT, sig_handler) != SIG_ERR){
  /* while (1){      */
    time(&rawtime);

    /* Collect time information, could be collected 
       every seconds or 60 minutes along with a 
       local counter variable depending on accuracy required */
    timeinfo = localtime_r(&rawtime , &timeinfoBuffer);
    result = malloc(26 * sizeof(char));
    result = asctime_r(timeinfo, result);
    int hour = CONVHOUR(TWHOUR(timeinfo->tm_hour));

    /* Convert time information to corresponding degrees */
    
    float seconds_deg = (float) (timeinfo->tm_sec*SIXTY_PACER) - DEG_SHIFT;
    float min_deg = (float) (timeinfo->tm_min*SIXTY_PACER) - DEG_SHIFT;
    float hour_deg = (float) (hour*HOURLY_PACER) - DEG_SHIFT;


    WINDOW *win = newwin(0, 0, 0, 0);
    /* Draw Seconds hand */
    draw_line(win, seconds_deg, 8, width, height, DOT);
    /* Draw Minute hand */
    draw_line(win, min_deg, 8, width, height, HASH);
    /* Draw hour hand */
    draw_line(win, hour_deg, 8, width, height, AT);
    /* Print time     */
    mvwprintw(win, BASE_HEIGHT, BASE_WIDTH, "%s", result);
    mvwprintw(win, BASE_HEIGHT+5, BASE_WIDTH, "PRESS CTRL+c to exit");
    wrefresh(win);

    sleep(ONE_SECOND);
    delwin(win);
    free(result);
  }

  return 0;
}


int main(int argc, char **argv){
  
  /*Suitable height and width */
  int width = 43;
  int height = 15;

  
  /*  Initialize ncurses  */
  if ( initscr() == NULL ) {
    fprintf(stderr, "Error initialising ncurses.\n");
    exit(EXIT_FAILURE);
  }

  plot_time(width, height);
  endwin_exit();
}
