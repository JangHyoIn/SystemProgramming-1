#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <curses.h>

#include <unistd.h>
#include <termios.h>


#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80

#define PAUSE 112
#define ESC 27

#define default_ch 'X'
#define HEAD '@'
#define BLANK ' '
#include "bounce.h"

void bound(),set_cr_noecho_mode(), wrap_up(), set_up();
int set_ticker(int);
int bounce_or_lose(struct ppball*);

int dir =1;
int length=5;
int cur_x;
int cur_y;

//struct ppball the_ball;
struct ppball snake[100];


int main(){
	char ch;
	clear();
	
	bound();
	
	set_up();
//	move_snake();

	//ch = getch();
	while(ch = getch()){
		
		switch(ch){
		case LEFT:
		case RIGHT:
		case UP:
		case DOWN:
			if(  (dir == LEFT && ch !=RIGHT) || (dir == RIGHT && ch != LEFT) || (dir == DOWN && ch != UP) || (dir == UP && ch != DOWN) )
				dir = ch;
			ch = 0;
		break;
		case 'q':
		case ESC: endwin();

		}
		move_snake(dir);
	}
	wrap_up();

	return 0;
}

void move_snake( int dir){
	//char blank[] = "     ";
	//static int dir = +1;
	//static int pos = MAP_XINIT+3;

	initscr();

	mvaddch(snake[length-1].y_pos, snake[length-1].x_pos, BLANK);
	for(i = length-1; i>0; i--){
		snake[i].x_pos = snake[i-1].x_pos;
		snake[i].y_pos = snake[i-1].y_pos;
	}

	mvaddch(snake[0].y_pos, snake[0].x_pos, DFT_SYMBOL);
	if(dir == LEFT) --snake[0].x_pos;
	if(dir == RIGHT) ++snake[0].x_pos;
	if(dir == UP) --snake[0].y_pos;
	if(dir == DOWN) ++snake[0].y_pos;
	
  	 mvaddch(snake[0].y_pos, snake[0].x_pos, HEAD);

/*	mvaddch(

		move(5,pos);
		addstr(snake);
		move(5-1,5);
		refresh();
		sleep(1);
		move(5,pos);
		addstr(blank);
		pos+=dir;
		if(pos>=LEFT_EDGE)
			dir = -1;
		if(pos<=RIGHT_EDGE)
			dir = +1;
*/		
		//cur_x=5;
		//cur_y=pos;
	
}

void move_DOWN()
{
	
}

void bound(){
	int i, j;
	initscr();

	set_cr_noecho_mode();
	for(i = 0; i < MAP_Y; i++){
		mvaddch(MAP_YINIT + i, MAP_XINIT, default_ch);

		mvaddch(MAP_YINIT + i, MAP_XINIT+MAP_X, default_ch);
		
	}

	
	for(j = 0; j <= MAP_X; j++){
		mvaddch(MAP_YINIT, MAP_XINIT+j, default_ch);

		mvaddch(MAP_YINIT + MAP_Y, MAP_XINIT+j, default_ch);
		
	}
	
	move(MAP_YINIT -1, MAP_XINIT+MAP_X-15);
	addstr("score: ");
	//move(MAP_YINIT + MAP_Y+1, MAP_XINIT + MAP_X);
	refresh();



}

void set_up(){
	int i;
 	initscr();
	//void move_snake(int, int);
	for(i = 0; i < length; i++){
	snake[i].x_pos = X_INIT+i;
	snake[i].y_pos = Y_INIT;
		if(i == 0) snake[i].symbol = HEAD;
		else snake[i].symbol = DFL_SYMBOL;
	dir = LEFT;
	
	}
        
	/*the_ball.y_pos = 1;
        the_ball.x_pos = X_INIT;
        the_ball.y_ttg = the_ball.y_ttm = Y_TTM;
        the_ball.x_ttg = the_ball.x_ttm = X_TTM;
        the_ball.y_dir = LEFT;
        the_ball.x_dir = LEFT;
        the_ball.symbol = DFL_SYMBOL;
        */
	
        set_cr_noecho_mode();

        signal(SIGINT, SIG_IGN);
        for(i = 0; i < length; i++){
	mvaddch(snake[i].y_pos, snake[i].x_pos, snake[i].symbol);

	}

	//mvaddch(the_ball.y_pos, the_ball.x_pos, the_ball.symbol);
        refresh();

       	//signal(SIGALRM, move_snake);
        set_ticker(1000/TICKS_PER_SEC);

}

void wrap_up(){
        set_ticker(0);
        endwin();
}


void ball_move(int signum){
        int y_cur, x_cur, moved;
        signal(SIGALRM, SIG_IGN);
        y_cur = the_ball.y_pos;
        x_cur = the_ball.x_pos;
        moved = 0;

        if(the_ball.y_ttm >0 && the_ball.y_ttg-- == 1){
                the_ball.y_pos += the_ball.y_dir;
                the_ball.y_ttg = the_ball.y_ttm;
                moved = 1;
        }

        if(the_ball.x_ttm > 0 && the_ball.x_ttg-- == 1){
                the_ball.x_pos += the_ball.x_dir;
                the_ball.x_ttg = the_ball.x_ttm;
                moved = 1;
        }
        if(moved){
                mvaddch(y_cur, x_cur, BLANK);
                mvaddch(y_cur, x_cur, BLANK);
                mvaddch(the_ball.y_pos, the_ball.x_pos, the_ball.symbol);
                bounce_or_lose(&the_ball);
                move(LINES -1, COLS-1);
                refresh();

        }
        signal(SIGALRM, ball_move);
}

int bounce_or_lose(struct ppball *bp){

        int return_val = 0;

        if(bp->y_pos == TOP_ROW){
                bp->y_dir = 1;
                return_val = 1;

        }
        else if(bp->y_pos == BOT_ROW){
                bp->y_dir = -1;
                return_val = 1;
        }
        if(bp->x_pos == LEFT_EDGE){
                bp->x_dir = 1;
                return_val = 1;
        }

         else if(bp->x_pos == RIGHT_EDGE){
                 bp->x_dir = -1;
                return_val = 1;
         }
        return return_val;

}


void set_cr_noecho_mode(){

        struct termios ttystate;

        tcgetattr(0, &ttystate);
        ttystate.c_lflag &= ~ICANON;
        ttystate.c_lflag &= ~ECHO;
        ttystate.c_cc[VMIN] = 1;
        tcsetattr(0, TCSANOW, &ttystate);

}


