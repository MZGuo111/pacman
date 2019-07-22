#include "util.h"
#include <stdio.h>
#include<stdlib.h>
#include<time.h>
#include <string.h>
#include<stdbool.h>
#define MaxSize 100
#define INF 32767
#define MaxV 100
#define MaxStep 20
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

typedef struct
{
	int i;						//当前方块的行号
	int j;						//当前方块的列号
} Box;

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
	for (int i = 0; i < Originlen; i++) {
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
	}
	free(q);
}
int greedy(node bean[], game_state_t state, ALGraph*G,int vision[],int cnt) {
	int key;
	int distance[500];
	int flag;
	int j = 0;
	int len = 0;
	node src = { state.start_x,state.start_y };
	while (1) {
		flag = 0;
		int disMin = 32767;
		memset(distance, 0, sizeof(distance));
		for (int i = 0; i< cnt; i++) {
			if (bean[i].x != 0 && bean[i].y != 0) {
				distance[i] = spfa(G, state, src, bean[i]);
				if (distance[i] < disMin) {
					disMin = distance[i];
					key = i;
				}
				flag = 1;
			}
		}
		len+=spfa(G, state, src, bean[key]);
		src = bean[key];
		if (flag == 0) {
			break;
		}
		vision[j++] = key;
		bean[key].x = 0;
		bean[key].y = 0;
	}
	return len;
}

void swap(int*a, int*b) {
	int tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;
}
void reverse(int left, int right, int vision[])
{
	for (int i = 0; (left + i != right - i)&&(left +1 +  i!=right - i); i++) {
		swap(&vision[left + i],&vision[right - i]);
	}
}
int main() {
	srand((int)time(NULL));
	game_state_t state;
	memset(&state, 0, sizeof(state));
	init(&state);
	node src = {state.start_x,state.start_y };
	int cnt = 0;
	node bean[500];
	node bean1[500];
	int dist[250][250];
	memset(dist, INF, sizeof(dist));
	memset(bean, -1, sizeof(bean));
	for (int i = 0; i < state.n; i++) {
		for (int j = 0; j < state.m; j++) {
			if (state.food[i][j] == 1) {
				bean[cnt].x = i;
				bean[cnt].y = j;
				bean1[cnt] = bean[cnt];
				cnt++;
			}
		}
	}
	bean[cnt] = src;
	bean1[cnt] = src;
	ALGraph*G = CreateAdj(state);
	for (int i = 0; i <=cnt; i++) {
		for (int j = i; j <=cnt; j++) {
			dist[i][j] = spfa(G, state, bean[i], bean[j]);
			dist[j][i]=dist[i][j];
		}
	}
	int vision[500] = { -1 };
	int Minvision[500] = { -1 };
	int minlen = greedy(bean, state, G, vision,cnt);
	for (int k = 0; k < cnt; k++) {
		Minvision[k] = vision[k];
	}
	int lenNow;
	for (int i = 0; i < 100; i++) {
		lenNow = 0;
		int num1 = rand() % (cnt-1)+1;
		int num2 = rand() % (cnt-1)+1;
		int left = num1 < num2 ? num1 : num2;
		int right = num1 > num2 ? num1 : num2;
		reverse(left, right, vision);
		for (int j = cnt ; j > 0; j--) {
			lenNow += dist[vision[j]][vision[j - 1]];
		}
		if (lenNow < minlen) {
			minlen = lenNow;
			for (int k = 0; k < cnt; k++) {
				Minvision[k] = vision[k];
			}
		}
		else {
			reverse(left, right, vision);
		}
	}
	spfa1(G, state, src, bean1[Minvision[0]]);
	for (int j = 0; j < cnt; j++) {
		spfa1(G, state, bean1[Minvision[j]], bean1[Minvision[j + 1]]);
	}
	return 0;
}