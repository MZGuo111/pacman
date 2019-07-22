#include "util.h"
#include <stdio.h>
#include <string.h>

typedef struct {
	int x;
	int y;
} Point;
struct {
	Point location;
	int parent;
} q[1000];

int dx[4] = {1, 0, -1, 0};
int dy[4] = {0, 1, 0, -1};
char direction[4] = {'N', 'E', 'S', 'W'};
int cnt=0;
int src,des;
int ans[1000], res = 0x3f3f3f3f;
int vis[1000]={0};//dfs
int x[50], y[50];
int visited[100][100];//bfs
Point path[1000];

game_state_t state;
int dis[100][100];

int bfs(int sx, int sy, int tx, int ty) {
	memset(path,0,sizeof( path));
	memset(q,0,sizeof(q));
	int i,j;
	for(i=0;i<100;i++){
		for(j=0;j<100;j++){
			visited[i][j]=0;
		}
	}
	int front = -1;
	int rear = -1;
	rear++;
	q[rear].location.x = sx;
	q[rear].location.y = sy;
	q[rear].parent = -1;
	visited[sx][sy] = 1;
	int count = 0;
	while (rear != front) {
		front++;
		Point e = q[front].location;
		if (e.x == tx && e.y == ty) {
			int k = q[front].parent;
			count++;
			path[count] = q[front].location;
			while (k != -1) {
				count++;
				path[count] = q[k].location;
				k = q[k].parent;
			}
			break;
		}
		
		for (i = 0; i < 4; i++) {
			int g = e.x + dx[i];
			int h = e.y + dy[i];
			if (visited[g][h] == 0 &&
				state.grid[g][h] == 0) {
				visited[g][h] = 1;
				rear++;
				q[rear].location.x = g;
				q[rear].location.y = h;
				q[rear].parent = front;
			}
		}
	}
	return count;//path此时保存的是逆路径
}
void dfs(int now, int road[]) {
    if (now == cnt) {
	   int sum = dis[road[0]][road[0]];
        for (int i = 1; i < cnt; i++) {
			sum +=dis[road[i-1]][road[i]];
        }
        if(sum>=res){
			return;
		}
        if (sum < res) {
			res = sum;
            for (int i = 0; i < cnt; i++){
                ans[i] = road[i];
            }
        }
        return;
    }
    for (int i = 0; i < cnt; i++) {
        if (!vis[i]) {
			vis[i]=1;
            road[now] = i;
            dfs(now + 1, road);
			vis[i]=0;
        }
    }
}
int main() {
    memset(&state, 0, sizeof(state));
    init(&state);
  
    int n = state.n;
	int m = state.m;
	src = state.start_x;
	des = state.start_y;
  
    for (int i = 0; i < n; i++){
        for (int j = 0; j <m; j++){
            if (state.food[i][j] == 1) {
                x[cnt] = i;
                y[cnt++] = j;
            }
        }
    }
	for(int i=0;i<cnt;i++){
		for(int j=i;j<cnt;j++){
			if(i==j){
				dis[i][j]=bfs(src,des,x[i],y[i]);
				continue;
			}
			dis[i][j]=bfs(x[i],y[i],x[j],y[j]);
			dis[j][i]=dis[i][j];
		}
	}
	int road[1000]={0};
    dfs(0,road);
	int count;
    count = bfs(src,des,x[ans[0]],y[ans[0]]);
    for (int i = count; i >= 1; i--) {
	    if (path[i].x == path[i - 1].x) {
		    if (path[i].y == path[i - 1].y - 1) {
			    printf("%c", direction[1]);
		    }
		    else if (path[i].y == path[i - 1].y + 1) {
			    printf("%c", direction[3]);
		    }
	    }
	    if (path[i].y == path[i - 1].y) {
		    if (path[i].x == path[i - 1].x - 1)
			    printf("%c", direction[2]);
		    if (path[i].x == path[i - 1].x + 1)
			    printf("%c", direction[0]);
	    }
    } 
	for(int i=1;i<cnt;i++) {
        count= bfs(x[ans[i-1]], y[ans[i-1]],x[ans[i]], y[ans[i]]);

	    for (int i = count; i >= 1; i--) {
      	    if (path[i].x == path[i - 1].x) {
		       if (path[i].y == path[i - 1].y - 1) {
			       printf("%c", direction[1]);
		       }
	     	   else if (path[i].y == path[i - 1].y + 1) {
			       printf("%c", direction[3]);
		       }
	        }  
     	    if (path[i].y == path[i - 1].y) {
	     	   if (path[i].x == path[i - 1].x - 1)
			      printf("%c", direction[2]);
		       if (path[i].x == path[i - 1].x + 1)
			      printf("%c", direction[0]);
	        }
        }
	}



    destroy(&state);
    return 0;
}
