#include "util.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_LENGTH 10000

typedef struct 
{	int i,j;						
	int parent;						
	char direction;
} Box;								
typedef struct
{
	Box data[MAX_LENGTH];
	int front,rear;					
} QUERE;							


void Printpath(QUERE *qu,int front)	
{
	int k=front, m=0;
	int j;
	do				
	{	j=k;
		k=qu->data[k].parent;
		qu->data[j].parent = -1;
	} while (k!=0);
	k=0;
	while (k<MAX_LENGTH)  
	{	if (qu->data[k].parent == -1)
		{	m++;
			if(m>1)
				printf("%c",qu->data[k].direction);
		}
		k++;
	}
	printf("\n");
}
void bfs(int xi,int yi,int xe,int ye, int** mg)	
{
	Box e;
	int i,j,di,i1,j1;
	QUERE *qu;						
	qu=(QUERE *)malloc (sizeof(QUERE));
	qu->front = qu->rear = -1;					
	e.i = xi; e.j = yi; e.parent = -1;
	qu->rear++;						
	qu->data[qu->rear]=e;							
	mg[xi][yi]=-1;					
	while (qu->front != qu->rear)			
	{	
		qu->front++;
		e = qu->data[qu->front];				
		i = e.i; 
		j = e.j;
		if (i == xe && j == ye)			
		{	
			Printpath(qu, qu->front);	
			free(qu);		
			return;			
		}
		for (di = 0; di < 4; di++)	
		{	
			switch(di)
			{
			case 0:i1=i-1; j1=j;   break;
			case 1:i1=i;   j1=j+1; break;
			case 2:i1=i+1; j1=j;   break;
			case 3:i1=i;   j1=j-1; break;
			}

			
			if (mg[i1][j1]==0)
			{
				e.i = i1; e.j = j1; 
				e.parent = qu->front;	
				if(di == 0)	e.direction = 'N';
				if(di == 1)	e.direction = 'E';
				if(di == 2)	e.direction = 'S';
				if(di == 3)	e.direction = 'W';
				
				qu->rear++;						
				qu->data[qu->rear]=e;				
				mg[i1][j1]=-1;		
			}
		}
     }
	free(qu);			
	return;			
}

int main() {
  game_state_t state;
  memset(&state, 0, sizeof(state));
  init(&state);
  // write your code here
  bfs(state.start_x, state.start_y, state.goal_x, state.goal_y, state.grid);
  //printf("SSWSWWSW\n");
  
  // 
  destroy(&state);
  return 0;
}


