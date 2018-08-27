// Written by John Shepherd, May 2013

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "graph.h"
#include "pagerank.h"


int validV(Graph g, Page p)
{
	return (g != NULL && p.v >= 0 && p.v < g->nP);
}


// p->q <=> q is outgoing link of p
void insertEdge(Graph g, Page p, Page q, int wt)//wt
{
	assert(g != NULL && validV(g,p) && validV(g,q));
	if (g->edges[p.v][q.v] == 0) {
		g->edges[p.v][q.v] = wt;
		g->nE++;
	}
}


// unset p->q
void removeEdge(Graph g, Page p, Page q)
{
	assert(g != NULL && validV(g,p) && validV(g,q));
	if (g->edges[p.v][q.v] != 0) {
		g->edges[p.v][q.v] = 0;
		g->nE--;
	}
}


Graph newGraph(int nP)
{
	assert(nP > 0);
	int p, q;
	Graph new = malloc(sizeof(GraphRep));
	assert(new != 0);
	new->nP = nP; new->nE = 0;
	new->edges = malloc(nP*sizeof(int *));
	assert(new->edges != 0);
	for (p = 0; p < nP; p++) {
		new->edges[p] = malloc(nP*sizeof(int));
		assert(new->edges[p] != 0);
		for (q = 0; q < nP; q++)
		new->edges[p][q] = 0;
	}
	return new;
}



void showGraph(Graph g)
{
	assert(g != NULL);
	printf("#vertices=%d, #edges=%d\n\n", g->nP, g->nE);
	int p, q;
	for (p = 0; p < g->nP; p++) {
		printf("%d %s\n", p, (g->page[p]).url);
		for (q = 0; q < g->nP; q++) {
			if (g->edges[p][q]) {
				printf("\t%s => %d\n", (g->page[q]).url, g->edges[p][q]);
			}
		}
		printf("\n");
	}
}
