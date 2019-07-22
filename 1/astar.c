#include "util.h"
#include <stdio.h>
#include <string.h>
#define max 400
int dx[4] = { 0,1,0,-1 };
int dy[4] = { 1,0,-1,0 };

typedef struct node {//queue中每个点的定义
	int x, y;//坐标
}node;

typedef struct queue {
	int front, rear;
	node data[max];
}queue;

void qinit(queue *q) {
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

int min(int a, int b) {
	return a < b ? a : b;
}

int queue_size(queue *q) {
	return q->rear - q->front;
}

int queue_cmp(queue* a, queue* b) { // 队列字典序的比较 
	int n = min(queue_size(a), queue_size(b));
	for (int i = 0; i < n; i++) {
		if (a->data[a->front + i].x != b->data[b->front + i].x) {
			return a->data[a->front + i].x < b->data[b->front + i].x;
		}
	}
	return queue_size(a) < queue_size(b);
}

// priority_queue
typedef struct NODE { // priority_queue 中每个点的定义
	node location;//坐标(x,y)
	int g, h;//g起点到(x,y),h(x,y)到终点      
	queue path;       // q:起点->(x,y) 的 path
} NODE;

typedef struct priority_queue {
	NODE data[max * max];
	int size;
} priority_queue;

void pq_init(priority_queue* q) {
	q->size = 0;
}

void swap(NODE *x, NODE *y) {
	struct NODE tmp = *x;
	*x = *y;
	*y = tmp;
}

int pq_cmp(NODE a, NODE b) { // priority_queue 的 比较函数 
	if (a.g + a.h != b.g + b.h) {
		return a.g + a.h < b.g + b.h;
	}
	return queue_cmp(&a.path, &b.path);
}

void pq_push(priority_queue *q, NODE val) {
	int p = ++q->size;
	q->data[p] = val;
	while (p > 1 && pq_cmp(q->data[p], q->data[p / 2])) {
		swap(&q->data[p / 2], &q->data[p]);
		p /= 2;
	}
}

NODE pq_pop(priority_queue *q) {
	if (q->size <= 0) {
		printf("pq_pop\n");
		exit(0);
	}
	NODE res = q->data[1];
	q->data[1] = q->data[q->size--];
	int p = 1, t;
	while (p * 2 <= q->size) {
		if (p * 2 + 1 > q->size || pq_cmp(q->data[p * 2], q->data[p * 2 + 1])) {
			t = p * 2;
		}
		else {
			t = p * 2 + 1;
		}
		if (pq_cmp(q->data[t], q->data[p])) {
			swap(&q->data[p], &q->data[t]);
			p = t;
		}
		else {
			break;
		}
	}
	return res;
}

int pq_size(priority_queue *q) {
	return q->size;
}

// A*
priority_queue pq;
int ok(node e)
{
	for (int i = 0; i < pq.size; i++) {
		if (e.x == pq.data[i].location.x&&e.y == pq.data[i].location.y) {
			return 0;
		}
	}
	return 1;
}

int jdz(int x)
{
	if (x < 0)
		return 0 - x;
	else
		return x;
}

queue A(game_state_t state, int k) { //start -> end 短路 
	queue q; // 路径
	qinit(&q);
	node start, des;
	des.x = state.goal_x;
	des.y = state.goal_y;
	start.x = state.start_x;
	start.y = state.start_y;
	push(&q, start);

	NODE tmp = { start,0,jdz(start.x - state.goal_x) + jdz(start.y - state.goal_y),q };
	pq_init(&pq);
	pq_push(&pq, tmp);
	int visit[max][max];
	for (int i = 0; i < max; i++)
		for (int j = 0; j < max; j++)
			visit[i][j] = 0;
	int cnt = 0;
	while (pq_size(&pq)) {
		tmp = pq_pop(&pq);
		visit[tmp.location.x][tmp.location.y] = 1;
		if (tmp.location.x == des.x&&tmp.location.y == des.y) {
			cnt++;
			if (cnt == k) {
				return tmp.path;
			}
			continue;
		}
		for (int i = 0; i < 4; i++) {
			NODE to = tmp;
			to.location.x = tmp.location.x + dx[i];
			to.location.y = tmp.location.y + dy[i];
			if (state.grid[to.location.x][to.location.y] == 0 && ok(to.location)
				&& to.location.x >= 0 && to.location.x < state.n
				&&to.location.y >= 0 && to.location.y < state.m&&visit[to.location.x][to.location.y]==0) {
				to.h = jdz(to.location.x - start.x) + jdz(to.location.y - start.y);
				to.g = jdz(to.location.x - des.x) + jdz(to.location.y - des.y);
				push(&to.path, to.location);
				pq_push(&pq, to);
			}
		}
	}
}

int main() {
	game_state_t state;
	memset(&state, 0, sizeof(state));
	init(&state);

	queue q = A(state, 1);
	node p1, p2;
	while (q.front != q.rear) {
		p1 = q.data[q.front];
		p2 = q.data[q.front + 1];
		if (p2.x - p1.x == 1)
			printf("S");
		else if (p2.x - p1.x == -1)
			printf("N");
		else if (p2.y - p1.y == 1)
			printf("E");
		else if (p2.y - p1.y == -1)
			printf("W");
		q.front++;
	}
	printf("\n");
	destroy(&state);
	return 0;
}
