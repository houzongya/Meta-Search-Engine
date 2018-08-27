#include "graph.h"
#include "readData.h"

#define word_size 50


void normalise(char *content);

void inverted(Graph g, char *content, Index *index, int i);

void createIndex(Graph g, Index *index, char *word, int url);

void printIndex(Graph g, Index *index);
