#include "util.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#define MAX 1000

int grid_ghost[MAX][MAX];

typedef struct
{
	int x, y;
	int fa;         //father
	int di;         //direction
}Queue;

void BFS(const game_state_t *s, char path[], int gxout ,int gyout);

int abs_s(int xx);


int main() {
    game_state_t state;
    memset(&state, 0, sizeof(state));
    init(&state);
    int d_x[]={0};
    int d_y[]={0};
  //
   
    int food_count = 0;
    int goal_x[MAX];
    int goal_y[MAX];
    int food_eatern[MAX][MAX];
    int xw,yw,x_enemy,y_enemy;
    if(state.is_red == 1)
    {
         xw = state.red_x;
	     yw = state.red_y;
         x_enemy = state.blue_x;
	     y_enemy = state.blue_y;
	    	
    }
	else
	{
         xw = state.blue_x;
	     yw = state.blue_y;
         x_enemy = state.red_x;
	     y_enemy = state.red_y;
    }

	for (int i = 0; i < MAX; i++)
    {
      for(int j = 0; j < MAX; j++)
           grid_ghost[i][j] = 0;
    }

  
    for(int i = 0; i < state.num_ghosts; i++)
    {
  	    for(int j = 0; j < 1; j++)
  	    {
  		    int x_wall = state.ghost_x[i] + d_x[j];
  		    int y_wall = state.ghost_y[i] + d_y[j];
  		    grid_ghost[x_wall][y_wall] = 1;
	    }
    }
  
    for(int i = 0; i < MAX; i++)
    {
        for(int j = 0; j < MAX; j++)
        {
            food_eatern[i][j] = 0;
        }
    }

    for(int i = 0; i < state.n; i++)
    {
        for(int j = 0; j < state.m; j++)
        {
            if(state.grid[i][j] == 0 && state.food[i][j] == 1)
             { 
                goal_x[food_count] = i;
                goal_y[food_count] = j;
                food_count ++;
            }
        }
    }
    int food_xunhuan = food_count;
    int center_eat = 0;
    while(food_count >= 0)
    { 
      
        srand(time(NULL));
        int adjust_weight = rand() % 2;
        int goal_x_now; 
        int goal_y_now;
        int min = 60000;
        for(int i = 0; i < food_xunhuan; i++)
        {
            if(food_eatern[goal_x[i]][goal_y[i]]  != 0)
                 continue;
          
            int manhattan = abs_s(xw - goal_x[i]) + abs_s(yw - goal_y[i]);
            if(abs_s(xw - goal_x[i]) + abs_s(yw - goal_y[i]) <= 1)
               manhattan = manhattan - 100;
         
            if(abs_s(goal_x[i] - x_enemy) + abs_s(goal_y[i] - y_enemy) < 3) 
            {
                manhattan = manhattan + 500;
            }
            if(abs_s(goal_x[i] - 8) + abs_s(goal_y[i] -16) < 4)
            {
                 center_eat = 1;
                 manhattan = manhattan - 6;
            }
            if(abs_s(goal_x[i] - 8) + abs_s(goal_y[i] -16) < 2)
            {
				 center_eat = 1;
                 manhattan = manhattan - 8;
            }
            if(abs_s(goal_x[i] - 8) + abs_s(goal_y[i] -16) < 1)
            {
                 manhattan = manhattan - 10;
            }
			else   
				manhattan = manhattan - adjust_weight*2;
            if(center_eat == 0)
            {
                if(goal_y[i] - 1 <= 1 || state.m - goal_y[i] <= 2 || (state.n - goal_x[i] >= state.n - 2 && goal_y[i] <= 10  && goal_y[i] >= 1))
                manhattan = manhattan - 25;
            }
          //manhattan = manhattan + adjust_weight;
            if(min > manhattan)
            {
                min  = manhattan;
                goal_x_now = goal_x[i];
                goal_y_now = goal_y[i];
            }
        }
        food_eatern[goal_x_now][goal_y_now] = 1;
        char path[state.n * state.m];
        BFS(&state,path,goal_x_now,goal_y_now);
        if(path[0] == 'S' || path[0] == 'W' || path[0] == 'E' || path[0] =='N')
        {
            printf("%c", path[0]);
        }
		else 
		{
			return 0;
		}

        food_count --;
        if(state.is_red == 1)
        {
            state.red_x = goal_x_now;
            state.red_y = goal_y_now;
	    	
        }
	    else
	    {
           state.blue_x = goal_x_now;
           state.blue_y = goal_y_now;
        }
      
    }

  //BFS(&state, path);
  
    destroy(&state);
    return 0;
}

void BFS(const game_state_t *s, char path[], int gxout ,int gyout)
{
	int vis[s->n][s->m];
	memset(vis, -1, sizeof vis);//init
    int xact[] = {1, 0, -1, 0};
	int yact[] = {0, 1, 0, -1};
    int x,y;
    if(s->is_red == 1)
    {
             x = s->red_x;
	     y = s->red_y;
	    	
    }
    else
	{
            x = s->blue_x;
	        y = s->blue_y;
    }
    vis[x][y] = 0;	
	int di = 0;
	Queue queue[MAX];
	int rear = 0;
	int front = -1;
	int xi;
	int yi;
	queue[rear].x = x;
	queue[rear].y = y;
	queue[rear].fa = -1;
	queue[rear].di = -1;
	rear++;
	while((rear - front + MAX) % MAX > 1)
	{
		
		front++;
		//printf("front:%d  ", front);
		if(front >= MAX)
		  front %= MAX;
		x = queue[front].x;
		y = queue[front].y;
		//printf("%d, %d\n", x, y);
		if (x == gxout && y == gyout)
			break;
		for(di = 0; di < 4; di++)
		{
			xi = x + xact[di];
			yi = y + yact[di];
			if(xi >= 0 && yi >= 0 && xi < s->n && yi < s->m 
				&& vis[xi][yi] == -1 && s->grid[xi][yi] == 0 && grid_ghost[xi][yi] != 1)
			{
				queue[rear].x = xi;
				queue[rear].y = yi;
				queue[rear].fa = front;
				vis[xi][yi] = vis[x][y] + 1;
				queue[rear].di = di;
				rear++;
			}
		}
	}
	int count = vis[x][y];
	//path[count] = '\n';
	path[count] = '\0';
	int trace = front;
	//printf("trace:%d count:%d\n", trace, count);
	while(count >= 0)
	{
		count--;
		switch(queue[trace].di)
		{
			case 0:path[count] = 'S';	break;
			case 1:path[count] = 'E';	break;
			case 2:path[count] = 'N';	break;
			case 3:path[count] = 'W';	break;
		}
		trace = queue[trace].fa;
	}
}

int abs_s(int xx)
{
    if(xx < 0)
        return -xx;
    return xx;
}

