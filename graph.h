// Graph.h ... interface to Graph ADT
// Written by John Shepherd, March 2013

#ifndef GRAPH_H
#define GRAPH_H
#define BUFFER 1000

#include <stdio.h>

typedef struct Index {
  char *word;
  int url[BUFFER];//*
  int url_size;
} Index;

typedef struct GraphRep *Graph;

typedef struct Page {
  int v;// vertices denoted by integers 0..N-1
  char *url;
  double pagerank;
} Page;//

typedef struct GraphRep {
	int    nP;    // #vertices
	int    nE;    // #edges
	Page	page[BUFFER];
	int  **edges; // matrix of weights (0 == no edge)
  Index *index;
  int index_size;//index size
} GraphRep;

int validV(Graph, Page);
typedef struct { Page p; Page q; } Edge;
void insertEdge(Graph, Page, Page, int);
void removeEdge(Graph, Page, Page);


// operations on graphs
Graph newGraph(int nV);
Graph makeGraph(int, int**);
void showGraph(Graph);
#endif
