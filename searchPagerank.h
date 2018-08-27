#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "graph.h"


void searchEngine(Graph g,int searching_word_size, char **word);

void sortByPagerank(Graph g, int *url);

void sortByTerm(Graph g, int *url,int searching_word_size, char **word);

void getFirstTerm(char *line, char *term);

void getIndexUrl(char *line, int *url, Graph g);

int findUrlInt(char *url, Graph g);

double getPagerank(Graph g, int url);
