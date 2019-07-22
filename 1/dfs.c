#include "util.h"
#include <stdio.h>
#include <string.h>
#define MAX 500
int dx[4] = { 0,1,0,-1 };
int dy[4] = { 1,0,-1,0 };

int flag;
int visit[MAX][MAX];//visit记录结点是否遍历过
void dfs(game_state_t state, int nowx, int nowy, char path[], int length)
{
	if (flag == 0) {
		if (nowx == state.goal_x&&nowy == state.goal_y)
		{
			printf("%s\n", path);
			flag = 1;
		}
		else
		{
			int di;
			visit[nowx][nowy] = 1;
			for (di = 0; di < 4; di++)
			{
				int tx = nowx + dx[di];
				int ty = nowy + dy[di];
				if (tx >= 0 && tx < state.n&&ty >= 0 && ty < state.m&&state.grid[tx][ty] != 1 && visit[tx][ty] == 0&&flag==0)
				{
					if (di == 0)
						path[length] = 'E';
					else if (di == 1)
						path[length] = 'S';
					else if (di == 2)
						path[length] = 'W';
					else
						path[length] = 'N';
					dfs(state, tx, ty, path, length+1);
				}
			}
			visit[nowx][nowy] = 0;
		}
	}
}


int main() {
	game_state_t state;
	memset(&state, 0, sizeof(state));
	init(&state);
	// write your code here
	char path[MAX];
	for (int i = 0; i < MAX; i++)
		path[i] = '\0';
	dfs(state, state.start_x, state.start_y, path, 0);

	destroy(&state);
	return 0;
}

