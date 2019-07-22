#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#define MAX 20000
#define MAXV 20000
#define INF 9999999
typedef struct ANode {
	int adjvex;
	struct ANode *nextarc;
	double weight;
}ArcNode;

typedef struct Vnode {
	int info;
	ArcNode *firstarc;
}VNode;

typedef struct {
	VNode adjlist[MAXV];
	int n, e;
}AdjGraph;
int visit[MAXV];


int *Dijkstra(AdjGraph *G, int u, int uv) {
	int tmp[MAXV];
	int *temp;
	temp = (int*)malloc(sizeof(int)*MAXV);
	for (int i = 0; i < MAXV; i++)
	{
		temp[i] = -2;
	}
	ArcNode *p = (ArcNode*)malloc(sizeof(ArcNode));
	double dist[MAXV];
	 int 	path[MAXV];
	int S[MAXV];
	double  MINdis;
   int 	i, j, v;
	for (int i = 0; i < G->n; i++) {
		S[i] = 0;
		if (u != i) {
			dist[i] = INF;
		}
		else {
			dist[i] = -1;
		}
		path[i] = -1;
	}
	p = G->adjlist[u].firstarc;
	while (p != NULL) {
		dist[p->adjvex] = p->weight;
		path[p->adjvex] = u;
		p = p->nextarc;
	}
	S[u] = 1;
	path[u] = -1;
	for (int i = 0; i < G->n - 1; i++) {
		MINdis = INF;
		for (int j = 0; j < G->n; j++) {
			if (S[j] == 0 && dist[j] < MINdis) {
				v = j;
				MINdis = dist[j];
			}
		}
		S[v] = 1;
		p = G->adjlist[v].firstarc;
		while (p != NULL) {
			if (dist[p->adjvex] > p->weight + dist[v]) {
				dist[p->adjvex] = p->weight + dist[v];
				path[p->adjvex] = v;
			}
			p = p->nextarc;
		}
	}
	int k = 0;
	while (path[uv] != -1) {
		tmp[k] = uv;
		uv = path[uv];
		k++;
	}
	temp[0] = u;
	for (int i = 1; i <= k; i++) {
		temp[i] = tmp[k - i];
	}
	if(k < 1)
		return NULL;
	return temp;
}


void Create_list(game_state_t state,AdjGraph **G)
{
	int x;
	int y;
	ArcNode *p;
	int e=0;
	for (int i = 0; i < state.m*state.n; i++)
	{
		(*G)->adjlist[i].firstarc = NULL;
	}
	for (int i = 0; i < state.m*state.n; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			x = i / state.m;
			y = i % state.m;
			if (state.grid[x][y] == 1)
				break;//当前位置是墙，不做路径考虑
			if (j == 0)//向右寻找
			{
				if (state.grid[x][y+1]==0)//右边有路径
				{
					p = (ArcNode*)malloc(sizeof(ArcNode));
					p->adjvex = i + 1;
					p->weight = state.cost[x][y + 1];
					p->nextarc = (*G)->adjlist[i].firstarc;
					(*G)->adjlist[i].firstarc = p;
					e++;
				}
			}
			if (j == 1)//向左寻找
			{
				if (state.grid[x][y-1] == 0)//左边有路径
				{
					p = (ArcNode*)malloc(sizeof(ArcNode));
					p->adjvex = i - 1;
					p->weight = state.cost[x][y - 1];
					p->nextarc = (*G)->adjlist[i].firstarc;
					(*G)->adjlist[i].firstarc = p;
					e++;
				}
			}
			if (j == 2)//向上寻找
			{
				if (state.grid[x - 1][y] == 0)//上边有路径
				{
					p = (ArcNode*)malloc(sizeof(ArcNode));
					p->adjvex = i - state.m;
					p->weight = state.cost[x - 1][y];
					p->nextarc = (*G)->adjlist[i].firstarc;
					(*G)->adjlist[i].firstarc = p;
					e++;
				}
			}
			if (j == 3)//向下寻找
			{
				if (state.grid[x + 1][y] == 0)//下边有路径
				{
					p = (ArcNode*)malloc(sizeof(ArcNode));
					p->adjvex = i + state.m;
					p->weight = state.cost[x + 1][y];
					p->nextarc = (*G)->adjlist[i].firstarc;
					(*G)->adjlist[i].firstarc = p;
					e++;
				}
			}
		}
	}
	(*G)->n=state.m*state.n;
	(*G)->e=e;
}

void printDir(int *path, int m){
    char dir[MAXV];
    int i = 0;
    for(i = 0; i <= m * 100;i++){
        if(path[i+1]-path[i]==1){
            dir[i] = 'E';
        }else if(path[i+1]==path[i]-1){
            dir[i] = 'W';
        }else if(path[i+1]==path[i]+m){
            dir[i] = 'S';
        }else if(path[i+1]==path[i]-m){
            dir[i] = 'N';
        }
    }
    dir[i] = '\0';
    printf("%s",dir);
}

int main(){
    game_state_t state;
    memset(&state, 0, sizeof(state));
    init(&state);
    // write your code here
    AdjGraph *G;
    G = (AdjGraph *)malloc(sizeof(AdjGraph));
    // 
    Create_list(state, &G);
    int u = state.start_x*state.m + state.start_y;
    int v = state.goal_x*state.m + state.goal_y;
    int* path;
    path = (int *)malloc(sizeof(int)*20000);
    path = Dijkstra(G,u,v);
    int m = state.m;
    printDir(path,m);
    destroy(&state);
    
    return 0;
}

