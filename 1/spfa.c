#include "util.h"
#include <stdio.h>
#include <string.h>
#include<stdbool.h>
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
	node data[10000];
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

typedef struct ANode           	//�ߵĽڵ�ṹ����
{
	node no;
	double weight;//�ñߵ��յ�λ��(i,j)
	struct ANode *nextarc; 		//ָ����һ���ߵ�ָ��
} ArcNode;
typedef struct Vnode      		//�ڽӱ�ͷ�ڵ������
{
	ArcNode *firstarc;     		//ָ���һ�����ڵ�
} VNode;
typedef struct
{
	VNode adjlist[MaxV][MaxV];    //�ڽӱ�ͷ�ڵ�����
} ALGraph;                   	//ͼ���ڽӱ�����

typedef struct
{
	int i;						//��ǰ������к�
	int j;						//��ǰ������к�
} Box;

ALGraph* CreateAdj(game_state_t state) //�����Թ������Ӧ���ڽӱ�G
{
	int i, j, i1, j1, di;
	ArcNode *p;
	ALGraph* G = (ALGraph *)malloc(sizeof(ALGraph));
	for (i = 0; i < state.n; i++)					//���ڽӱ�������ͷ�ڵ��ָ�����ó�ֵ
		for (j = 0; j < state.m; j++)
			G->adjlist[i][j].firstarc = NULL;
	for (i = 0; i < state.n; i++)					//���mg��ÿ��Ԫ��
		for (j = 0; j < state.m; j++)
			if (state.grid[i][j]==0)
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
					if (state.grid[i1][j1]==0)							//(i1,j1)Ϊ���߷���
					{
						p = (ArcNode *)malloc(sizeof(ArcNode));	//����һ���ڵ�*p
						p->no.x = i1; p->no.y = j1;
						p->weight = state.cost[i1][j1];
						p->nextarc = G->adjlist[i][j].firstarc;	//��*p�ڵ����������
						G->adjlist[i][j].firstarc = p;
					}
					di++;
				}
			}
	return G;
}
void spfa(ALGraph*G, game_state_t state) {
	node start = { state.start_x,state.start_y };
	node end = { state.goal_x,state.goal_y };
	queue *q = (queue*)malloc(sizeof(queue));
	Init(q);
	int vis[MaxSize][MaxSize] = { 0 };
	double dis[MaxSize][MaxSize];
	node fa[MaxSize][MaxSize];
	node road[MaxSize],road1[MaxSize];
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
			if (dis[to.x][to.y] == INF || (dis[to.x][to.y]) > (dis[out.x][out.y] + p->weight)) {
				dis[to.x][to.y] = dis[out.x][out.y] + p->weight;
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
	road1[length].x =state.goal_x ;
	road1[length].y = state.goal_y;
	int Originlen = length;
	int j = 0;
	while (length--) {
		road1[j] = road[length];
		//printf("(%d,%d)->", road1[j].x, road1[j].y);
		j++;
	}
	for (int i = 0; i < Originlen - 1; i++) {
		if (road1[i + 1].x - road1[i].x == 1) {
			printf("S");
		}
		else if (road1[i + 1].x - road1[i].x == -1)
		{
			printf("N");
		}else if (road1[i + 1].y - road1[i].y == 1) {
			printf("E");
		}
		else if (road1[i + 1].y - road1[i].y == -1) {
			printf("W");
		}
	}
	printf("W");
	printf("\n");

}
int main() {
	game_state_t state;
	memset(&state, 0, sizeof(state));
	init(&state);
	ALGraph *G = CreateAdj(state);
	spfa(G, state);
	destroy(&state);
	return 0;
}
