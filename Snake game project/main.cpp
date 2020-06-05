#include <stdlib.h> 
#include <stdio.h>
#include "main.h"
#include <time.h>
#include <miosix.h>
#include <termios.h>
#include <unistd.h>

using namespace std;
using namespace miosix;


#define clear() printf("\033[H\033[J")

//static Direction direct;

Direction Snake:: direct;


void tty_raw_mode(void)
{
	struct termios tty_attr;
     
	tcgetattr(0,&tty_attr);

	/* Set raw mode. */
	tty_attr.c_lflag &= (~(ICANON|ECHO));
	tty_attr.c_cc[VTIME] = 0;
	tty_attr.c_cc[VMIN] = 1;
     
	tcsetattr(0,TCSANOW,&tty_attr);
}




Snake::Snake(int _large, int _tall) {
	large = _large;
	tall = _tall;	
	length = 1;
	xpos = large / 2;
	ypos = tall / 2;
	appleX = rand() % large;
	appleY = rand() % tall;
	score = 0;
	game_over = 0;
	direct = Right;
	score = 1;
}

void Snake::moveUp(int positions) {
	printf("\x1b[%dA", positions);
}

void Snake::moveDown(int positions) {
	printf("\x1b[%dB", positions);
}

void Snake::moveRight(int positions) {
	printf("\x1b[%dC", positions);
}

void Snake::moveLeft(int positions) {
	printf("\x1b[%dD", positions);
}

void Snake::moveTo(int row, int col) {
	printf("\x1b[%d;%df", row, col);
}

void green() {
	printf("\x1b[30;42m");
}

void red() {
	printf("\x1b[30;41m");
}


void save_cursor() {
	printf("\x1b%d", 7);
}

void restore_cursor() {
	printf("\x1b%d", 8);
}

void reset() {
	printf("\033[0m");
	printf("\x1b[0m");
}

int Snake::get_game_over() {
	return game_over;
}



void Snake::board_print(){
	 reset();
	 clear();
	 printf("\r&");
	 for (int i = 0; i < large-1;i++) {
        	printf("&");
    }
    printf("\n");
    for (int i = 0; i < tall;i++){
		for (int j = 0; j < large; j++) {
			if (j == 0) {
				printf("\r&");
			}
			else if (i == ypos && j == xpos) {
				printf("O");
			}
			else if (i == appleY && j == appleX) {
				red();
				printf("F");
				reset();
			}
			else{
				int w = 0;
				for (w = 0; w < length; w++) {
					if (i == tail_pos_y[w] && j == tail_pos_x[w]) {
						printf("o");
						break;
					}
				}
				if (w == length) {
					moveRight(1);
				}
			}
	   }
	printf("& \n");
    }
	printf("\r&");
	for (int i = 0; i < large-1;i++) {
        printf("&");
    }
	printf("\n \rScore: %d", score);
	save_cursor();
}


void Snake::recogn_key(void *) {
	while (1){
		int a = getchar();
		switch (a)
		{
		//Snake Direction::direct
		case 97:
			direct = Left;
			break;
		case 100:
			direct = Right;
			break;
		case 119:
			direct = Up;
			break;
		case 115:
			direct = Down;
			break;
		default:
			break;
		}
	}
}


void Snake::print_movement(int aux_x[], int aux_y[], int current_xpos,int current_ypos){
	moveTo(current_ypos+2,current_xpos+1);
	printf(" ");
	for (int i = 0; i < length;i++){
		moveTo(aux_y[i]+2,aux_x[i]+1);
		printf(" ");
	}
	moveTo(ypos+2,xpos+1);
	green();
	printf("O");
	for (int i = 0; i < length;i++){
		moveTo(tail_pos_y[i]+2,tail_pos_x[i]+1);
		printf("o");
	}
	reset();
}

void Snake::movement() {
	int current_xpos = xpos;
	int current_ypos = ypos;
	int current_xtail[70], current_ytail[70];
	for (int i = 0; i < length;i++){
		 current_xtail[i] = tail_pos_x[i];
		 current_ytail[i] = tail_pos_y[i];
	}
	int aux_x = tail_pos_x[0];
	int aux_y = tail_pos_y[0];
	tail_pos_x[0] = xpos;
	tail_pos_y[0] = ypos;
	int aux_x2, aux_y2;
	for (int i = 1; i < length; i++) {
		aux_x2 = tail_pos_x[i];
		aux_y2 = tail_pos_y[i];
		tail_pos_x[i] = aux_x;
		tail_pos_y[i] = aux_y;
		aux_x = aux_x2;
		aux_y = aux_y2;
	}


	switch (direct)
	{
	case Up:
		ypos--;
		break;
	case Down:
		ypos++;
		break;
	case Right:
		xpos++;
		break;
	case Left:
		xpos--;
		break;
	}


	//Test if its game over or not
	if (xpos >= large || xpos == 0 || ypos >= tall || ypos == 0) {
		game_over = 1;
	}

	for (int i = 0; i < length; i++) {
		if (xpos == tail_pos_x[i] && ypos == tail_pos_y[i]) {
			game_over = 1;
			break;
		}
	}

	if (xpos == appleX && ypos == appleY) {
		length++;
		score++;
		tail_pos_x[length - 1] = appleX;
		tail_pos_y[length - 1] = appleY;
		appleX = rand() % large;
		appleY = rand() % tall;
		if (appleX == 0) {
			appleX = 1;
		}
		if (appleY == 0) {
			appleY = 1;
		}
		moveTo(appleY+2, appleX+1);
		red();
		printf("F");
		reset();

	}

	print_movement(current_xtail,current_ytail,current_xpos,current_ypos);
	restore_cursor();
	moveLeft(1);
	printf("%d", score);
}


int main()
{
	Snake snake(40,10);
	srand(time(NULL));
	tty_raw_mode();
	snake.board_print();
	Thread::create(snake.recogn_key,STACK_MIN);
	while (snake.get_game_over()==0) {
		//snake.recogn_key();
		snake.movement();
		fflush(stdout);
		Thread::sleep(450);
	}
	restore_cursor();
	printf("\r\nGame over!\n");
	while(1){
		ledOn();
		Thread::sleep(450);
		ledOff();
		Thread::sleep(450);
	}
}
