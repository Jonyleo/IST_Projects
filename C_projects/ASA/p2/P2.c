#include <stdio.h>
#include <stdlib.h>

#define NODE(A) ((node *) A)



enum nodeType {SHOP, PERSON, BOTH, NONE};

typedef struct listNode {
	struct listNode *next;
	void * value;
	int flag;
} listNode;

typedef struct list {
	listNode *end, *start;
} list;

typedef struct node {
	int x, y;
	listNode *in, *out;
	struct node * prev;
	int full;
	int tmp;
	char type;
} node;

typedef struct corner {
	node *left, *right;
	enum nodeType type;
} corner;


void initList(list *s) {
	s->end = NULL;
	s->start = NULL;
}


node *sink;
node *source;

corner **map;
int M, N,S,P;
int total;

void join(list *s,node * v) {
	if(!s)
		return;

	listNode *tmp = malloc(sizeof(listNode));
	tmp->value = v;
	tmp->next = NULL;

	if(s->end) {
		s->end->next = tmp;
		s->end = tmp;
	}
	else {
		s->start = s->end = tmp;
	}
}

node * query(list *s) {
	if(!s || !s->start)
		return NULL;

	listNode * tmp = s->start;
	node * tmpN = tmp->value;

	s->start = s->start->next;
	if(!s->start)
		s->end = NULL;

	free(tmp);
	return tmpN;
}

node * createNode(int x, int y, char t) {
	node *tmp = malloc(sizeof(node));

	tmp->x = x;
	tmp->y = y;
	tmp->prev = NULL;
	tmp->in = tmp->out = NULL;
	tmp->full = 0;
	tmp->tmp = 0;
	tmp->type = t;

	return tmp;
}


int bfs() {
	list s;
	node * u;
	listNode * tmp;

	initList(&s);
	
	join(&s, source);
	while(s.start) {
		u = query(&s);					
		
		if(u->tmp) {
			tmp = u->out;
			while(tmp) {
				if(NODE(tmp->value)->prev == NULL && NODE(tmp->value) != source) {
					
					NODE(tmp->value)->prev = u;

					if(NODE(tmp->value) == sink) {
						while(query(&s));
						return 1;
					}
					else {
						join(&s, NODE(tmp->value));
					}
				}
				tmp = tmp->next;
			}
		}	

		if(u->full)
			tmp = u->in;
		else 
			tmp = u->out;
		
		while(tmp) {
			if(NODE(tmp->value)->prev == NULL && NODE(tmp->value) != source) {
				NODE(tmp->value)->prev = u;
				if(u->full) {
					NODE(tmp->value)->tmp = 1;
				}
				if(NODE(tmp->value) == sink) {
					while(query(&s));
					return 1;
				}
				else {
					join(&s, NODE(tmp->value));
				}
			}
			tmp = tmp->next;
		}

		
	}
	return 0;
}

void connect(node * n1, node * n2) {
	listNode *tmp = malloc(sizeof(listNode));

	tmp->value = n2;
	tmp->next = n1->out;
	n1->out = tmp;
}

int	isConnected(node * n1, node * n2) {
	listNode *tmp = n1->out;
	while(tmp) {
		if(NODE(tmp->value) == n2)
			return 1;
		tmp = tmp->next;
	}
	return 0;
}

void pair(node *n1, node *n2) {
	if(!n1 || !n2)
		return;

	listNode * tmp;

	if(n1->in) {
		if(NODE(n1->in->value) == n2) {
			free(NODE(n1->in));
			n1->in = NULL;
			return;
		}
	}

	tmp = malloc(sizeof(listNode));

	tmp->value = n1;
	tmp->next = n2->in;
	n2->in = tmp;
}

void scanMap() {
	int x,y,m,n;

	scanf("%d %d\n", &M, &N);

	map = malloc(sizeof(corner*) * M);

	for(x = 0; x < M; x++) {
		map[x] = malloc(sizeof(corner) * N);
		for(y = 0; y < N; y++) {
			map[x][y].left = createNode(x,y,'l');
			map[x][y].right = createNode(x,y, 'r');

			map[x][y].type = NONE;

			connect(map[x][y].right, map[x][y].left);
		}
	}

	for(x = 0; x < M; x++) {
		for(y = 0; y < N; y++) {
			if(x > 0)
				connect(map[x][y].left, map[x-1][y].right);
	
			if(x < M -1)
				connect(map[x][y].left, map[x+1][y].right);

			if(y > 0)
				connect(map[x][y].left, map[x][y-1].right);
			
			if(y < N -1)
				connect(map[x][y].left, map[x][y+1].right);
		}
	}
	
	scanf("%d %d\n", &S, &P);

	for(x = 0; x < S; x++) {
		scanf("%d %d\n", &m, &n);

		if(map[m-1][n-1].type != SHOP)
			connect(map[m-1][n-1].left, sink);

		map[m-1][n-1].type = SHOP;
	}

	for(x = 0; x < P; x++) {
		scanf("%d %d\n", &m, &n);

		if(map[m-1][n-1].type == PERSON || map[m-1][n-1].type == BOTH)
			continue;

		if(map[m-1][n-1].type == NONE)
			map[m-1][n-1].type = PERSON;
		else
			map[m-1][n-1].type = BOTH;
		
		connect(source, map[m-1][n-1].right);
		
	}
}

void freeAdj(node * n) {
	listNode *tmp = n->in, *tmpN;
	while(tmp) {
		tmpN = tmp->next;
		free(tmp);
		tmp = tmpN;
	}
	tmp = n->out;
	while(tmp) {
		tmpN = tmp->next;
		free(tmp);
		tmp = tmpN;
	}
}

void freeMap() {
	int x, y;
	
	for(x = 0; x < M; x++) {
		for(y = 0; y < N; y++) {
			freeAdj(map[x][y].right);
			freeAdj(map[x][y].left);

			free(map[x][y].right);
			free(map[x][y].left);
		}

		free(map[x]);
	}
	free(map);
}

int main() {
	total = 0;
	int x, y;

	sink = createNode(-1,-1,'t');
	source = createNode(-2,-2, 's');

	scanMap();

	node * tmp;
	node * tmpN;

	while(bfs()) {
		total++;
		tmp = sink->prev;
		tmpN = sink;

		while(tmp) {
					
			pair(tmp, tmpN);
			tmpN = tmp;
			tmp = tmp->prev;
		}



		for(x = 0; x < M; x++) {
			for(y = 0; y < N; y++) {
				map[x][y].left->prev = NULL;
				map[x][y].right->prev = NULL;

				map[x][y].right->tmp = 0;
				map[x][y].left->tmp = 0;

				if(map[x][y].left->in)
					map[x][y].left->full = 1;
				else
					map[x][y].left->full = 0;


				if(map[x][y].right->in)
					map[x][y].right->full = 1;
				else
					map[x][y].right->full = 0;

			}
		}

		sink->full = 0;
		source->full = 0;
		
		sink->prev = NULL;
		source->prev = NULL;

	}

	

	printf("%d\n", total);

	freeMap();
	freeAdj(sink);
	freeAdj(source);
	free(sink);
	free(source);

	return 0;
}