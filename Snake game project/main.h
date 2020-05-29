#pragma once
#ifndef _main_h
#define _main_h
#include <vector>

enum Direction {Left, Right, Up, Down};
const int MAX = 70;

class Snake {
private:
	int large;
	int tall;
	int score;
	int length;
	int xpos;
	int ypos;
	int tail_pos_x[MAX];
	int tail_pos_y[MAX];
	int appleX;
	int appleY;
	//std::vector <snake> tail;
	Direction direc;
	bool game_over;
public:
	Snake(int tall, int large);
	void moveUp(int positions);
	void moveDown(int positions);
	void moveLeft(int positions);
	void moveRight(int positions);
	void moveTo(int row, int column);
	void board_print();
	static void recogn_key(void *);
	void movement();
	void print_movement(int aux_x[], int aux_y[], int current_xpos,int current_ypos);
	bool get_game_over();
};


#endif // !main.h
