#include "graph.h"
#include "readData.h"

void calculatePageRank(Graph g, double d, double diffPR, int maxIterations);

double pagerank(Graph g, int Pi, double d, int iteration);

double weightedPageRank(Graph g, int j, int i);

int inlinksTo(Graph g, int v, int *links);

int outlinksFrom(Graph g, int v, int *links);

int printPageRank(Graph g, double d, double diffPR, int maxIterations);
