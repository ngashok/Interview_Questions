#include <stdio.h>
#include <stdlib.h>
#include<strings.h>

#define TRUE 1
#define FALSE 0
#define MAX_VERTEX  10

struct priority_queue_node
{
    int weight;
    int vertex;
    int edge[2];
};
typedef struct priority_queue_node pq_node;

struct binary_heap
{
    int q_index;
    pq_node pq[MAX_VERTEX];
};

typedef struct binary_heap bin_heap;
/* Linked list node of an Adjacency list */
struct edge_node
{
    int dest;    /* Destination vertex of this edge */
    int weight;  /* Weight of this Vertex */
    struct edge_node * next;
    int directional;
};
typedef struct edge_node edge;

/* Graph stats with Vertex 0 to Vertex N */
struct my_graph
{
  /* arrage of edges */
  edge* adj[MAX_VERTEX-1];
  int weight[MAX_VERTEX-1];
  int in_tree[MAX_VERTEX-1];
  int num_vertices;
  bin_heap heap;
};
typedef struct my_graph graph;

void insert_edge(graph *g, int src, int dest,int directional,int weight)
{
    edge *e = malloc(sizeof(edge));
    if (e == NULL) {
	return;
    }
    /* Insert this node in begining of source*/
    e->dest = dest;
    e->weight = weight;
    e->next = g->adj[src];
    g->adj[src] = e;

    if (directional == FALSE) {
	insert_edge(g,dest,src,TRUE,weight);  /* Insert one more node if it is not directinoal */
    }
    (g->num_vertices)++;
}

void display_adjacency_list(graph *g)
{
    int i;
    edge * e;
    for (i = 0; i< sizeof(g->adj)/sizeof(edge*); i++) {
	e = g->adj[i];
	if (e == NULL) {
	    continue;
	}
	printf(" Vertex: %d :",i);
	while(e != NULL) {
	    printf("->%d(%dw)",e->dest,e->weight);
	    e = e->next;
	}
	printf("\n");
    }
}

void destroy_graph(graph *g)
{

    int i ;
    edge *e = NULL;
    edge *temp = NULL;
    for (i = 0; i< sizeof(g->adj)/sizeof(edge*); i++) {
	e = g->adj[i];
	while(e != NULL) {
	    temp = e->next;
	    free(e);
	    e = temp;
	}
    }

}

void init_graph(graph *g)
{
    int i = 0;
    for (i = 0; i< sizeof(g->adj)/sizeof(edge*); i++) {
	g->adj[i] = NULL;
	g->weight[i] = 64000;
	g->in_tree[i] = FALSE;
    }

}


void init_binary_heap(bin_heap *b_heap)
{
    int i = 0;
    b_heap->q_index = 1;
    for ( i = 1; i< MAX_VERTEX; i++)
    {
	b_heap->pq[i].weight =  0;
	b_heap->pq[i].vertex =  0;
	b_heap->pq[i].edge[0] = -1;
	b_heap->pq[i].edge[1] = -1;
    }
}

void swap(pq_node *node1, pq_node *node2)
{
	pq_node temp = *node1;
	*node1 = *node2;
	*node2 = temp;
}


void bubble_up(bin_heap *heap,int start_index)
{
	if (start_index <= 1) {
	    return;
	}
	int parent = (int)start_index/2;
	if (heap->pq[parent].weight > heap->pq[start_index].weight) {
		swap(&heap->pq[parent], &heap->pq[start_index]);
		/* may have to bubble_up as I dont know whether I broke heap structure */
		bubble_up(heap,parent);
	}
}


void bubble_down(bin_heap *heap, int start_index)
{
	if (start_index > heap->q_index) {
	    return;
	}

	int child1 = 2 * start_index;
	int child2 = child1 + 1;
	int swap_index  = child1;

	if (child2 >= heap->q_index) {
	    swap_index = child1;
	}
	else if (heap->pq[child2].weight < heap->pq[child1].weight) {
	    swap_index = child2;
	}
	if (heap->pq[swap_index].weight < heap->pq[start_index].weight) {
		swap(&heap->pq[swap_index], &heap->pq[start_index]);
	}
}

pq_node extract_min(bin_heap *heap)
{
	pq_node min_node = heap->pq[1];
	//printf("\nEdge %d-%d",min_node.edge[0],min_node.edge[1]);
	/* put last node to first node, rebuild heap */
	heap->pq[1] = heap->pq[heap->q_index-1];
	(heap->q_index)--;
	bubble_down(heap,1);
	return min_node;
}

void insert_to_heap(bin_heap *heap,pq_node *pq)
{
    	heap->pq[heap->q_index] = *pq;
	bubble_up(heap, heap->q_index);
	heap->q_index++;
}
void print_heap(bin_heap *heap)
{
    int i = 1;
    printf("\n Priority Queue ordering \n");
    for (i = 1; i< heap->q_index; i++) {
	printf("\n%d",heap->pq[i].weight);
    }

}


void prims_algorithm(graph *g)
{
    int next_vertex  = 0;
    edge * e = NULL;
    int dest = 0;
    init_binary_heap(&(g->heap));
    while ( g->in_tree[next_vertex] == FALSE) 
    {
	g->in_tree[next_vertex] = TRUE;	
	e = g->adj[next_vertex];
	while( e != NULL ) {
	    /* update weight addition if we select vertex V */   
	    dest = e->dest;
	    if ( g->in_tree[dest] == FALSE) {
		if (e->weight < g->weight[dest] ) {
		    g->weight[dest] = e->weight;
		    pq_node pq;
		    pq.weight = g->weight[dest];
		    pq.vertex = dest;
		    pq.edge[0] = next_vertex;
		    pq.edge[1] = dest;
		    //Put to priority queue to select min
		    insert_to_heap(&(g->heap),&pq);
		}
	    }
	    e = e->next;
	}
	pq_node min_node = extract_min(&(g->heap));

	/* select the min weight giving vertex */
	next_vertex = min_node.vertex;
	if ( g->in_tree[next_vertex] == FALSE) {
	    printf("\nEdge %d-%d\n",min_node.edge[0],min_node.edge[1]);
	}
    }

}

int main()
{
    int i = 0;
    graph g;
    init_graph(&g);
    insert_edge(&g,0,1,FALSE,1);
    insert_edge(&g,0,3,FALSE,10);
    insert_edge(&g,1,3,FALSE,5);
    insert_edge(&g,1,2,FALSE,1);
    insert_edge(&g,3,2,FALSE,15);
    display_adjacency_list(&g);

    /* Prims algorithm to find the min-weight spanning tree, using priority queues */
    prims_algorithm(&g);
    destroy_graph(&g);
}

