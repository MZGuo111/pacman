#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct node
{
	int x, y;//(x,y)表示坐标
	int fa;//fa表示当前坐标前一坐标的位置
}node;

int jdz(int x)
{
	if (x < 0)
		return  -x;
	else
		return x;
}

node bfs(game_state_t state, int dx[], int dy[])
{
	node data[500];//队列
	int front, rear;
	int visit[100][100] = { 0 };//记录坐标是否访问过
	front = rear = -1;
	node tmp;
	tmp.fa = -1; tmp.x = state.ghost_x[0]; tmp.y = state.ghost_y[0];
	rear++;
	data[rear] = tmp;//起始点进队
	visit[tmp.x][tmp.y] = 1;
	while (front != rear)
	{
		front++;
		tmp = data[front];
		if (tmp.x == state.start_x&&tmp.y == state.start_y)
		{
			node pre;
			while (tmp.fa != -1)
			{
				pre = tmp;
				tmp = data[tmp.fa];
			}
			return pre;
		}
		else
		{
			for (int d = 0; d < 4; d++)
			{
				node now = tmp;
				now.x = now.x + dx[d];
				now.y = now.y + dy[d];
				now.fa = front;
				if (now.x > 0 && now.y > 0 && now.x < state.n-1 && now.y < state.m - 1)
				{
					if (visit[now.x][now.y] == 0&&!(now.x== state.ghost_x[1]&&now.y== state.ghost_y[1]))
					{
						rear++;
						data[rear] = now;
						visit[now.x][now.y] = 1;
					}
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
	// write your code here
	srand(time(0));
	int dx[] = { -1, 0, 1, 0 };
	int dy[] = { 0, 1, 0, -1 };
	char d_name[] = { NORTH,EAST, SOUTH , WEST };
	if (state.index == 0)//第一个鬼卡人
	{
		int d;//鬼走的方向
		node tmp = bfs(state, dx, dy);//tmp为鬼应走往人的坐标
		if (tmp.x != -1)//若找到最短路径
		{
			if (tmp.y - state.ghost_y[0] == 1)
				d = 1;
			else if (tmp.y - state.ghost_y[0] == -1)
				d = 3;
			else if (tmp.x - state.ghost_x[0] == 1)
				d = 2;
			else
				d = 0;
			putchar(d_name[d]);
		}
		else//若没有找到最短路径
		{
			while (1)
			{
				int d = rand() % 4;
				int tx = state.ghost_x[1] + dx[d];
				int ty = state.ghost_y[1] + dy[d];
				if (tx > 0 && ty > 0 && tx < state.n - 1 && ty < state.m - 1)
				{
					putchar(d_name[d]);
					break;
				}
				d = rand() % 4;
			}
		}
	}
	else if (state.index == 1)//第二个鬼吃人
	{
		int flag = 0;
		while (1)
		{
			int d = rand() % 4;
			int tx = state.ghost_x[1] + dx[d];
			int ty = state.ghost_y[1] + dy[d];
			if (jdz(state.ghost_x[1] - state.start_x) + jdz(state.ghost_y[1] - state.start_y)
				> jdz(tx - state.start_x) + jdz(ty - state.start_y) && tx > 0 && ty > 0 &&
				tx < state.n - 1 && ty < state.m - 1 && !(tx == state.ghost_x[0] && ty == state.ghost_y[0]))
			{
				putchar(d_name[d]);
				flag = 1;
				break;
			}
		}
		if (flag == 0)
		{
			while (1)
			{
				int d = rand() % 4;;
				int tx = state.ghost_x[1] + dx[d];
				int ty = state.ghost_y[1] + dy[d];
				if (tx > 0 && ty > 0 && tx < state.n - 1 && ty < state.m - 1)
				{
					putchar(d_name[d]);
					break;
				}
			}
		}
	}
	destroy(&state);
	return 0;
}



/*#include "util.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#define MaxSize 100
#define INF 32767
#define MaxV 100

typedef struct
{
	int x, y;
	//int in;
}node;
typedef struct queue {
	int front, rear;
	node data[100000];
}queue;

void Init(queue *q) {
	q->front = q->rear = -1;
}

void push(queue *q, node e) {
	q->rear++;
	q->data[q->rear] = e;
}

node pop(queue *q) {
	q->front++;
	return q->data[q->front];
}

int empty(queue *q) {
	return q->rear - q->front;
}

typedef struct ANode           	//边的节点结构类型
{
	node no;
	struct ANode *nextarc; 		//指向下一条边的指针
} ArcNode;
typedef struct Vnode      		//邻接表头节点的类型
{
	ArcNode *firstarc;     		//指向第一个相邻点
} VNode;
typedef struct
{
	VNode adjlist[MaxV][MaxV];    //邻接表头节点数组
} ALGraph;                   	//图的邻接表类型

int abs(int x) {
	if (x > 0) {
		return x;
	}
	else {
		return -x;
	}
}
ALGraph* CreateAdj(game_state_t state) //建立迷宫数组对应的邻接表G
{
	int i, j, i1, j1, di;
	ArcNode *p;
	ALGraph* G = (ALGraph *)malloc(sizeof(ALGraph));
	for (i = 0; i < state.n; i++)					//给邻接表中所有头节点的指针域置初值
		for (j = 0; j < state.m; j++)
			G->adjlist[i][j].firstarc = NULL;
	for (i = 0; i < state.n; i++)					//检查mg中每个元素
		for (j = 0; j < state.m; j++)
			if (state.grid[i][j] == 0)
			{
				di = 0;
				while (di < 4)
				{
					switch (di)
					{
					case 0:i1 = i - 1; j1 = j; break;
					case 1:i1 = i; j1 = j + 1; break;
					case 2:i1 = i + 1; j1 = j; break;
					case 3:i1 = i, j1 = j - 1; break;
					}
					if (state.grid[i1][j1] == 0)							//(i1,j1)为可走方块
					{
						p = (ArcNode *)malloc(sizeof(ArcNode));	//创建一个节点*p
						p->no.x = i1; p->no.y = j1;
						p->nextarc = G->adjlist[i][j].firstarc;	//将*p节点链到链表后
						G->adjlist[i][j].firstarc = p;
					}
					di++;
				}
			}
	return G;
}
int spfa(ALGraph*G, game_state_t state, node src, node to) {
	node start = { src.x,src.y };
	node end = { to.x,to.y };
	queue *q = (queue*)malloc(sizeof(queue));
	Init(q);
	int vis[MaxSize][MaxSize] = { 0 };
	int dis[MaxSize][MaxSize];
	node fa[200][200];
	node road[1000];
	int cnt = 0;
	memset(fa, -1, sizeof(fa));
	for (int i = 0; i < state.n; i++) {
		for (int j = 0; j < state.m; j++) {
			if (i == start.x&&j == start.y) {
				dis[i][j] = 0;
			}
			else {
				dis[i][j] = INF;
			}
		}
	}
	push(q, start);
	vis[start.x][start.y] = 1;
	while (empty(q) != 0) {
		node out = pop(q);
		vis[out.x][out.y] = 0;
		ArcNode*p;
		p = G->adjlist[out.x][out.y].firstarc;
		while (p != NULL) {
			node to = p->no;
			if (dis[to.x][to.y] == INF || (dis[to.x][to.y]) > (dis[out.x][out.y] + 1)) {
				dis[to.x][to.y] = dis[out.x][out.y] + 1;
				fa[to.x][to.y].x = out.x;
				fa[to.x][to.y].y = out.y;

				if (vis[to.x][to.y] == 0) {
					vis[to.x][to.y] = 1;
					push(q, to);
				}
			}
			p = p->nextarc;
		}
	}
	int length = 0;
	while (fa[end.x][end.y].x != -1 && fa[end.x][end.y].y != -1) {
		road[length].x = fa[end.x][end.y].x;
		road[length].y = fa[end.x][end.y].y;
		end = fa[end.x][end.y];
		length++;
	}
	free(q);
	return length;
}
void spfa1(ALGraph*G, game_state_t state, node src, node to) {
	node start = { src.x,src.y };
	node end = { to.x,to.y };
	queue *q = (queue*)malloc(sizeof(queue));
	Init(q);
	int vis[MaxSize][MaxSize] = { 0 };
	double dis[MaxSize][MaxSize];
	node fa[MaxSize][MaxSize];
	node road[MaxSize];
	node path[MaxSize];
	int cnt = 0;
	memset(fa, -1, sizeof(fa));
	for (int i = 0; i < state.n; i++) {
		for (int j = 0; j < state.m; j++) {
			if (i == start.x&&j == start.y) {
				dis[i][j] = 0;
			}
			else {
				dis[i][j] = INF;
			}
		}
	}
	push(q, start);
	vis[start.x][start.y] = 1;
	while (empty(q) != 0) {
		node out = pop(q);
		vis[out.x][out.y] = 0;
		ArcNode*p;
		p = G->adjlist[out.x][out.y].firstarc;
		while (p != NULL) {
			node to = p->no;
			if (dis[to.x][to.y] == INF || (dis[to.x][to.y]) > (dis[out.x][out.y] + 1)) {
				dis[to.x][to.y] = dis[out.x][out.y] + 1;
				fa[to.x][to.y].x = out.x;
				fa[to.x][to.y].y = out.y;

				if (vis[to.x][to.y] == 0) {
					vis[to.x][to.y] = 1;
					push(q, to);
				}
			}
			p = p->nextarc;
		}
	}
	int length = 0;
	while (fa[end.x][end.y].x != -1 && fa[end.x][end.y].y != -1) {
		road[length].x = fa[end.x][end.y].x;
		road[length].y = fa[end.x][end.y].y;
		end = fa[end.x][end.y];
		length++;
	}
	path[length].x = to.x;
	path[length].y = to.y;
	int Originlen = length;
	int j = 0;
	while (length--) {
		path[j] = road[length];
		//printf("(%d,%d)->", path[j].x, path[j].y);
		j++;
	}
	int i = 0;
	if (path[i + 1].x - path[i].x == 1) {
		printf("S");
	}
	else if (path[i + 1].x - path[i].x == -1)
	{
		printf("N");
	}
	else if (path[i + 1].y - path[i].y == 1) {
		printf("E");
	}
	else if (path[i + 1].y - path[i].y == -1) {
		printf("W");
	}
	free(q);
	return Originlen;
}

int dx[] = { -1, 0, 0, 1 };
int dy[] = { 0, -1, 1, 0 };
char d_name[] = { NORTH, WEST, EAST, SOUTH };
int main() {
	srand((int)time(NULL));
	game_state_t state;
	memset(&state, 0, sizeof(state));
	init(&state);
	ALGraph*G = CreateAdj(state);
	if (state.index == 0) {
		int i = 0;
		while (1) {
			int d = i % 4;
			int tx = state.ghost_x[0] + dx[d];
			int ty = state.ghost_y[0] + dy[d];
			int len = abs(tx - state.start_x) + abs(ty - state.start_y) - abs(state.ghost_x[0] - state.start_x) - abs(state.ghost_y[0] - state.start_y);
			if (tx >= 2 && tx < state.n - 2 && ty >= 2 && ty <= state.m - 2 && state.grid[tx][ty] == 0 && len < 0) {
				putchar(d_name[d]);
				break;
			}
			i++;
		}
	}
	else {
		node src = { state.ghost_x[state.index],state.ghost_y[state.index] };
		node end = { state.start_x,state.start_y };
		spfa1(G, state, src, end);
		return 0;
	}
}*/










//#include "util.h"
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <time.h>


//typedef struct 
{
//	int x;
//	int y;
//} Node;
//struct 
{
//	Node posi;
//	int parent;
//} q[1000];
//int dx[4] = {-1, 0, 1, 0};
//int dy[4] = {0, 1, 0, -1};
//char direct[4] = {'N', 'E', 'S', 'W'};
//void BFS(game_state_t state, int xi, int yi, int xt, int yt)
//{
//	int vis[100][100];
  //	memset(vis, 0, sizeof vis);
//	int rear = -1;
   // int front = -1;
   // rear++;
//    q[rear].posi.x = xi;
//    q[rear].posi.y = yi;
//    q[rear].parent = -1;
 //   vis[xi][yi] = 1;
//    Node path[1000];
 //   int count = 0;
 //   while(rear != front)
 //{
//	    front++;
//  	  	Node e = q[front].posi;
//	  	if (e.x == xt && e.y == yt)
//{
//			    int k = q[front].parent;
//				count++;
//				path[count] = q[front].posi;
//				while (k != -1) 
//{
//					count++;
//					path[count] = q[k].posi;
//					k = q[k].parent;
//				}
//				break;
//	  	}
//	 	xi = e.x;
//	 	yi = e.y;
//	 	for (int i = 0; i < 4; i++)
//{
//		 	 int xx = xi + dx[i];
//		  	 int yy = yi + dy[i];
//		  	if (vis[xx][yy] == 0 &&
//				state.grid[xx][yy] != 1 /*&&
//				xx != state.ghost_x[0] &&
//				yy != state.ghost_y[0]*/) 
//{
//			  	vis[xx][yy] = 1;
//			  	rear++;
//			    q[rear].posi.x = xx;
//			    q[rear].posi.y = yy;
//			    q[rear].parent = front;
//		  	}
//	  	}
// 	 }
  //  for (int i = count; i>=1; i--)  
  //{
	//  	  if (path[i].x == path[i - 1].x) 
	//{
	//			if (path[i].y == path[i - 1].y - 1) 
	//{
	//				printf("%c", direct[1]);
	//			} else if (path[i].y == path[i - 1].y +1)
	//{
	//				printf("%c", direct[3]);
	//			}
	//		}
	//	    if (path[i].y == path[i - 1].y)
	//{
	//			if (path[i].x == path[i - 1].x - 1)
	//				printf("%c", direct[2]);
	//			if (path[i].x == path[i - 1].x + 1)
	//				printf("%c", direct[0]);
	//		}
  	//}
//}

//int main() 
//{
  //game_state_t state;
 // memset(&state, 0, sizeof(state));
  //init(&state);
  // write your code here
 
 
 
 // srand(time(0));
 // int dx[] = {-1, 0, 0, 1};
 // int dy[] = {0, -1, 1, 0};
  //char d_name[] = {NORTH, WEST, EAST, SOUTH};
  //state.grid[state.ghost_x[0]][state.ghost_y[0]] = 1;
  //state.grid[state.ghost_x[1]][state.ghost_y[1]] = 1;
/* 
 if (state.index == 0)
 {
  int i = 0;
  while (1)
  {
	 while(1) 
	 {
	int flag = 0;
	int d1 = rand() % 4;
    int idx1 = 0;
    int tx1 = state.ghost_x[idx1] + dx[d1];
    int ty1 = state.ghost_y[idx1] + dy[d1];
	if (i == 4 && tx1 >= 2  && tx1 < state.n - 2 && ty1 >= 2 && ty1 <state.m  - 2&&
        !state.grid[tx1][ty1]) 
        {
      putchar(d_name[d1]);
	  flag = 1;
      break;
     }
	if (flag == 1 || i != 4)
	{
		break;
	}
	}
	if (i == 4)
	{
		break;
	}
    int d = i % 4;
    int idx = 0;
    int tx = state.ghost_x[idx] + dx[d];
    int ty = state.ghost_y[idx] + dy[d];
	int len = abs(tx - state.start_x) + abs(ty - state.start_y) - abs(state.ghost_x[idx] - state.start_x) - abs(state.ghost_y[idx] - state.start_y);
    if (tx >= 2 && tx < state.n - 2 && ty >= 2 && ty < state.m  - 2&&
        !state.grid[tx][ty] &&
		len < 0) 
		{
      putchar(d_name[d]);
      break;
    }
	i++;
  }
 }
 if (state.index == 1)
 {
  	BFS(state, state.ghost_x[1], state.ghost_y[1], state.start_x, state.start_y);
	
 } 
  //
  destroy(&state);
  return 0;
}
*/