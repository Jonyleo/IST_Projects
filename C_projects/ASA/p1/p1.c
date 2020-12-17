#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define getSCC(V) SCCgraph[SCCdict[graph[V].l]]


typedef struct node {
	struct node *next;
	int v;
} node;

typedef struct stack {
	node *top;
	node *next;
} stack;

typedef struct vertex {
	int grade;
	stack con;
	int d, l;
} vertex;

typedef struct SCC {
	stack members;
	stack con;
	int maxGrade;
	int d;
} SCC;

int N;
stack tarjanStack;
vertex *graph;
SCC *SCCgraph;
int visit = 0;

void initStack(stack *L) {
	L->top = NULL;
	L->next = NULL;
}

void push(stack *L, int v) {
	if(!L)
		return;
	node *tmp = malloc(sizeof(node));
	tmp->next = L->top;
	tmp->v = v;	
	L->top = tmp;
}

int pop(stack *L) {
	if(!L->top)
		return -1;

	node *tmp = L->top;
	int v = tmp->v;

	L->top = tmp->next;
	free(tmp);

	return v;
}

int next(stack *L) {
	if(!L)
		return -1;
	if(!L->next)
		return -1;

	node *tmp = L->next;
	L->next = L->next->next;
	
	return tmp->v;
}

void reset(stack *L) {
	L->next = L->top;
}


int contains(stack *L, int v) {
	node *tmp = L->top;
	while(tmp) {
		if(v == tmp->v)
			return 1;
		tmp = tmp->next;
	}

	return 0;
}

void visitRecur(int s) {
	graph[s].d = visit++;
	graph[s].l = graph[s].d;

	reset(&graph[s].con);
	int v;

	push(&tarjanStack, s);

	while((v = next(&graph[s].con)) != -1) {
		
		if(graph[v].d == -1 || contains(&tarjanStack, v)) {
			if(graph[v].d == -1)
				visitRecur(v);

			if(graph[v].l < graph[s].l)
				graph[s].l = graph[v].l;
		}
	}

	int x;

	if(graph[s].d == graph[s].l)
		while((x = pop(&tarjanStack)) != s);		
}

void SCC_DFS(int u) {
	int v;
	SCCgraph[u].d = 0;
	reset(&SCCgraph[u].con);
	while((v = next(&SCCgraph[u].con)) != -1) {
		if(SCCgraph[v].d == -1)
			SCC_DFS(v);

		if(SCCgraph[u].maxGrade < SCCgraph[v].maxGrade)
			SCCgraph[u].maxGrade = SCCgraph[v].maxGrade;
	}
}

void printGraph() {
	int x, v;
	for(x = 0; x < N; ++x)  {
		printf("V: %d - %d\n", x, graph[x].grade);
		reset(&graph[x].con);
		while((v = next(&graph[x].con)) != -1)
			printf(" Adj: %d\n", v);
	}
}

void printSCCgraph(int nSCC) {
	int x, v;
	for(x = 0; x < nSCC; ++x)  {
		printf("SCC: %d - %d\n", x, SCCgraph[x].maxGrade);
		reset(&SCCgraph[x].con);
		while((v = next(&SCCgraph[x].con)) != -1)
			printf(" Adj: %d\n", v);

		reset(&SCCgraph[x].members);
		while((v = next(&SCCgraph[x].members)) != -1)
			printf(" Mem: %d\n", v);
	}
}


int main() {
	int M, u, v, x;

	initStack(&tarjanStack);

	scanf("%d,%d", &N, &M);

	graph = malloc(sizeof(vertex) * N);

	if(!graph)
		perror("malloc failed");

	for(x = 0; x < N; ++x) {
		scanf("%d",&graph[x].grade);
		initStack(&graph[x].con);
		graph[x].d = -1;
		graph[x].l = 0;
	}

	for(x = 0; x < M; ++x) {
		scanf("%d %d", &u ,&v);
		push(&graph[u-1].con, v-1);
	}

	for(x = 0; x < N; ++x) 
		if(graph[x].d == -1)
			visitRecur(x);

	int SCCdict[N];
	int nSCC = 0;

	memset(SCCdict, 0, N * sizeof(int));

	for(x = 0; x < N; ++x)
		SCCdict[graph[x].l]++;

	for(x = 0; x < N; ++x)
		if(SCCdict[x] > 0)
			nSCC++;

	SCCgraph = malloc(sizeof(SCC) * nSCC);

	memset(SCCdict, -1, N * sizeof(int));
	nSCC = 0;

	for(x = 0; x < N; ++x) {
		if(SCCdict[graph[x].l] == -1) {
			SCCdict[graph[x].l] = nSCC++;
			initStack(&getSCC(x).members);
			initStack(&getSCC(x).con);
			getSCC(x).d = -1;
		}
	}

	for(x = 0; x < N; ++x) {
		if(!contains(&getSCC(x).members, x)) 
			push(&getSCC(x).members, x);
		
		reset(&getSCC(x).members);
		while((v = next(&getSCC(x).members)) != -1);
	}


	for(x = 0; x < nSCC; ++x) {
		u = 0;
		reset(&SCCgraph[x].members);
		while((v = next(&SCCgraph[x].members)) != -1)
			if(graph[v].grade > u) 
				u = graph[v].grade;

		SCCgraph[x].maxGrade = u;
	}

	for(x = 0; x < N; ++x) {
		reset(&graph[x].con);
		while((v = next(&graph[x].con)) != -1) {
			if(!contains(&getSCC(x).con, SCCdict[graph[v].l]) && SCCdict[graph[x].l] != SCCdict[graph[v].l])
				push(&getSCC(x).con, SCCdict[graph[v].l]);
		}
	}

	for(x = 0; x < nSCC; ++x)
		SCC_DFS(x);

	for(x = 0; x < nSCC; ++x) {
		while((v = pop(&SCCgraph[x].members)) != -1);
	}

	for(x = 0; x < N; ++x) {
		printf("%d\n", getSCC(x).maxGrade);

		reset(&graph[x].con);

		while((v = pop(&graph[x].con)) != -1);
	}

	free(graph);

	return 0;
}