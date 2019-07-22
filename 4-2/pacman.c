#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


void mark(game_state_t state)//将怪周围及自己五个方块标记为墙
{
	for (int i = 0; i < state.n; i++)
	{
		for (int j = 0; j < state.m; j++)
		{
			for (int k = 0; k < state.num_ghosts; k++) {

				if (i == state.ghost_x[k] && j == state.ghost_y[k])
				{
					state.grid[i][j] = 1;
					if (i - 1 >= 0)		state.grid[i - 1][j] = 1;
					if (i + 1 < state.n)	state.grid[i + 1][j] = 1;
					if (j - 1 >= 0)		state.grid[i][j - 1] = 1;
					if (j + 1 < state.m)	state.grid[i][j + 1] = 1;
				}

			}
		}
	}
}

int dx[] = { -1, 0, 0, 1 };
int dy[] = { 0, -1, 1, 0 };
char d_name[] = { NORTH, WEST, EAST, SOUTH };

typedef struct {
	int x;
	int y;
	int fa;
}node;

node BFSbean(game_state_t state)
{
	int visit[100][100] = { 0 };
	node data[500];
	int front, rear;
	front = rear = -1;
	node tmp;
	tmp.x = state.start_x;
	tmp.y = state.start_y;
	tmp.fa = -1;
	rear++;
	data[rear] = tmp;
	visit[tmp.x][tmp.y] = 1;
	while (front != rear)
	{
		front++;
		tmp = data[front];
		if (state.food[tmp.x][tmp.y] == 1)
		{
			node pre;
			while (tmp.fa != -1)
			{
				pre = tmp;
				tmp = data[tmp.fa];
			}
			return pre;
		}
		for (int d = 0; d < 4; d++)
		{
			node now = tmp;
			now.x = tmp.x + dx[d];
			now.y = tmp.y + dy[d];
			now.fa = front;
			if (now.x >= 0 && now.y >= 0 && now.x < state.n&&now.y < state.m)
			{
				if (state.grid[now.x][now.y] == 0 && visit[now.x][now.y] == 0)
				{
					rear++;
					data[rear] = now;
					visit[now.x][now.y] = 1;
				}
			}
		}
	}
	tmp.x = -1;
	return tmp;
}

node BFScapsule(game_state_t state)
{
	int visit[100][100] = { 0 };
	node data[500];
	int front, rear;
	front = rear = -1;
	node tmp;
	tmp.x = state.start_x;
	tmp.y = state.start_y;
	tmp.fa = -1;
	rear++;
	data[rear] = tmp;
	visit[tmp.x][tmp.y] = 1;
	while (front != rear)
	{
		front++;
		tmp = data[front];
		for (int j = 0; j < state.num_capsule; j++) {
			if (tmp.x == state.capsule_x[j] && tmp.y == state.capsule_y[j]) 
			{
				node pre;
				while (tmp.fa != -1)
				{
					pre = tmp;
					tmp = data[tmp.fa];
				}
				return pre;
			}
		}
		for (int d = 0; d < 4; d++)
		{
			node now = tmp;
			now.x = tmp.x + dx[d];
			now.y = tmp.y + dy[d];
			now.fa = front;
			if (now.x >= 0 && now.y >= 0 && now.x < state.n&&now.y < state.m)
			{
				if (state.grid[now.x][now.y] == 0 && visit[now.x][now.y] == 0)
				{
					rear++;
					data[rear] = now;
					visit[now.x][now.y] = 1;
				}
			}
		}
	}
	tmp.x = -1;
	return tmp;
}

int main() {
	game_state_t state;
	memset(&state, 0, sizeof(state));
	init(&state);
	int flag = 0;
	int key[2] = { -1 };
	for (int i = 0; i < state.num_ghosts; i++) {
		if (state.timer[i] == 0) {
			flag = 1;
			key[i] = i;
		}
	}
	if (flag == 0 || state.num_capsule == 0) {
		if (flag = 1) {
			mark(state);
		}
		srand(time(0));
		int d = rand();
		while (1) {
			node tmp = BFSbean(state);
			if (tmp.x != -1)
			{
				if (tmp.x - state.start_x==1)
					d = 3;
				if (tmp.x - state.start_x==-1)
					d = 0;
				if (tmp.y - state.start_y==1)
					d = 2;
				if (tmp.y - state.start_y==-1)
					d = 1;
			}
			int count = 0;
			while (d >= 0 && d < 4 && count < 4)
			{
				int tx = state.start_x + dx[d];
				int ty = state.start_y + dy[d];
				if (tx >= 0 && tx < state.n && ty >= 0 && ty < state.m && !state.grid[tx][ty]) {
					putchar(d_name[d]);
					break;
				}
				d = (d + 1) % 4;
				count++;
			}
			break;
		}
		destroy(&state);
		return 0;
	}
	else {
		mark(state);
		srand(time(0));
		int d = rand();
		while (1) {
			node tmp = BFScapsule(state);
			if (tmp.x != -1)
			{
				if (tmp.x - state.start_x == 1)
					d = 3;
				if (tmp.x - state.start_x == -1)
					d = 0;
				if (tmp.y - state.start_y == 1)
					d = 2;
				if (tmp.y - state.start_y == -1)
					d = 1;
			}
			int count = 0;
			while (d >= 0 && d < 4 && count < 4)
			{
				int tx = state.start_x + dx[d];
				int ty = state.start_y + dy[d];
				if (tx >= 0 && tx < state.n && ty >= 0 && ty < state.m && !state.grid[tx][ty]) {
					putchar(d_name[d]);
					break;
				}
				d = (d + 1) % 4;
				count++;
			}
			break;
		}
		destroy(&state);
		return 0;
	}
}
