#include <stdio.h>
#include "graph.h"

#define MAX_URL 1000
#define MAX_LINE 1000


typedef char List_of_Urls[BUFFER][BUFFER];

int GetCollection(List_of_Urls);

int readUrl(char *line, char **URL);

Graph GetGraph(void);
