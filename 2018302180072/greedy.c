#include "util.h"
#include <stdio.h>
#include <string.h>
#define MaxSize 300
#define INF 32767

int dx[4] = { 0,1,0,-1 };
int dy[4] = { 1,0,-1,0 };
int cnt = 1;//目标个数
int x[MaxSize], y[MaxSize];//目标位置
int ans[MaxSize], min = 0x3f3f3f3f;//ans[]枚举最短路径顺序,min为最短路径长度

typedef struct node {
	int x, y;//(x,y)表示坐标
}node;

typedef struct NODE { // priority_queue 中每个点的定义
	node location;//坐标(x,y)
	int h;//起点到到终点距离 
	int fa;//前一结点
	int step;
} NODE;

int BFS(game_state_t state,NODE start,NODE to,int function)
{
	if (function == 1)
		start.fa = -1;
	start.step = 0;
	NODE q[MaxSize];//队列初始化
	int front, rear;
	front = rear = -1;
	int visit[MaxSize][MaxSize] = { 0 };//记录是否访问过
	int di;//di表示方向
	rear++;
	q[rear] = start;
	visit[start.location.x][start.location.y] = 1;
	while (front != rear)
	{
		front++;
		NODE tmp = q[front];
		if (tmp.location.x == to.location.x&&tmp.location.y == to.location.y)
		{
			char a[MaxSize];
			int length = -1;
			int count = tmp.step;
			while (tmp.fa != -1 && function == 1)
			{
				length++;
				NODE p1 = q[tmp.fa];
				if (p1.location.x - tmp.location.x == 1)
					a[length] = 'N';
				else if (p1.location.x - tmp.location.x == -1)
					a[length] = 'S';
				else if (p1.location.y - tmp.location.y == 1)
					a[length] = 'W';
				else
					a[length] = 'E';
				tmp = p1;
			}
			while (length != -1)
			{
				printf("%c", a[length]);
				length--;
			}
			return count;
		}
		for (di = 0; di < 4; di++)
		{
			NODE now = tmp;
			now.location.x += dx[di];
			now.location.y += dy[di];
			if (visit[now.location.x][now.location.y] == 0 && state.grid[now.location.x][now.location.y] == 0
				&& now.location.x > 0 && now.location.x < state.n&&now.location.y>0 && now.location.y < state.m)
			{
				rear++;
				now.step++;
				if (function == 1)
					now.fa = front;
				q[rear] = now;
				visit[now.location.x][now.location.y] = 1;
			}
		}
	}
}

int main() 
{
	game_state_t state;
	memset(&state, 0, sizeof(state));
	init(&state);
	x[0] = state.start_x;
	y[0] = state.start_y;
	for (int i = 0; i < state.n; i++) {//统计目标位置
		for (int j = 0; j < state.m; j++) {
			if (state.food[i][j] == 1) {
				x[cnt] = i;
				y[cnt++] = j;
			}
		}
	}

	int length[MaxSize][MaxSize] = { 0 };//统计目标之间距离
	for (int i = 0; i < cnt; i++)
	{
		for (int j = 0; j < cnt; j++)
		{
			if (i != j)
			{
				NODE from, to;
				from.location.x = x[i];
				from.location.y = y[i];
				to.location.x = x[j];
				to.location.y = y[j];
				length[i][j] = BFS(state, from, to, 0);
			}
		}
	}

	int count = 1;//贪心算法找较优路径
	int now = 0, tmp = 0;
	ans[0] = 0;
	int visit[MaxSize] = { 0 };//记录目标是否访问
	visit[0] = 1;
	while (count != cnt)
	{
		int minnow = min;
		for (int i = 0; i < cnt; i++)
		{
			if (length[now][i] < minnow&&i != tmp && visit[i] == 0)
			{
				minnow = length[now][i];
				tmp = i;
			}
		}
		now = tmp;
		visit[now] = 1;
		ans[count] = now;
		count++;
	}
	for (int i = 0; i < cnt - 1; i++)
	{
		NODE from, to;
		from.location.x = x[ans[i]];
		from.location.y = y[ans[i]];
		to.location.x = x[ans[i + 1]];
		to.location.y = y[ans[i + 1]];
		BFS(state, from, to, 1);
	}
	printf("\n");
	destroy(&state);
	return 0;
}