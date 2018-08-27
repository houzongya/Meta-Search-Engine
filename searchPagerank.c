#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "readData.h"
#include "graph.h"
#include "inverted.h"
#include "searchPagerank.h"
#define max_printed_size 30

int printed_Urls;


int main(int argc, char** argv)
{
  Graph g = GetGraph();
  searchEngine(g, argc, argv);
  return 0;
}

//sort by term(words found) first then by pagerank
void searchEngine(Graph g, int searching_word_size, char **word)
{
  printed_Urls = 0;
  int *url = malloc(g->nP * sizeof(int));
  for (int i = 0; i < g->nP; i++) {
    url[i] = 0;
  }
  sortByTerm(g, url, searching_word_size, word);

  for (int j = searching_word_size; j >= 0; j--) {
    int *URL = malloc(g->nP * sizeof(int));
    int k = 0;
    for (int i = 0; i < g->nP; i++) {
      if (url[i] == j) {
        URL[k] = i;
        k++ ;
      }
      URL[k] = EOF;
    }
    sortByPagerank(g, URL);
    free(URL);
  }
  free(url);
  
  return;
}


//print to file searchPagerank.txt
void sortByPagerank(Graph g, int *url)
{
  for (int i = 0; url[i] != EOF; i++) {
       for (int j = 0; url[j] != EOF; j++) {
           if (getPagerank(g, url[i])>getPagerank(g, url[j])) {
               double temp = url[i];
               url[i] = url[j];
               url[j] = temp;
           }
       }
   }

   for (int i = 0; url[i] != EOF && printed_Urls < max_printed_size; i++) {
     printf("%s\n",g->page[url[i]].url);
     printed_Urls = printed_Urls + 1;
   }

  return;
}


//get pagerank of url
double getPagerank(Graph g, int url)
{
  FILE *fp;
  fp = fopen("pagerankList.txt", "r");
  char line[BUFFER];
  double ranking;
  while (fgets(line, BUFFER, fp) != NULL) {
    char *term = malloc(BUFFER * sizeof(char));
    getFirstTerm(line, term);
    if (strcmp(term, g->page[url].url) == 0) {
      char *rank = malloc(BUFFER*sizeof(char));
      int j = 0;
      int comma = 0;
      int found = 0;
      for (int i = 0; line[i] != '\n'; i++) {
        if (line[i] == ',') {
          comma++ ;
        }
        if (comma == 2 && found == 0) {
          found = 1;
          continue;
        }
        if (found == 1) {
          rank[j] = line[i];
          j++;
        }
      }
      rank[j] = '\0';
      ranking = atof(rank);
      free(rank);
    }
  }
  fclose(fp);

  return ranking;
}


//sort by the matched words found
void sortByTerm(Graph g, int *url,int searching_word_size, char **word)
{
  FILE *fp = fopen("invertedIndex.txt", "r");
  char line[BUFFER];
  while (fgets(line, BUFFER, fp) != NULL) {
    char *term = malloc(BUFFER * sizeof(char));
    getFirstTerm(line,term);
    for (int i = 1; i < searching_word_size; i++) { //
      if (strcmp(word[i], term) == 0) { //
        getIndexUrl(line, url, g);
      }
    }
    free(term);
  }
  fclose(fp);

  return;
}


//get word which is the first term in a line
void getFirstTerm(char *line, char *term)
{
  int i;
  for (i = 0; line[i] != ' ' && line[i] != ','; i++) {
  }
  term = strncpy(term, line, i);

  return;
}


//url[n] means the number of words nth url(in graph) has
void getIndexUrl(char *line, int *url, Graph g)
{
  char URL[BUFFER][BUFFER];
  int urlInt;
  int j = 0, k = 0, i = 0;
  int flag = 0;
  for (i = 0; line[i] != '\0'; i++) {
    if (line[i] == ' ' && flag == 0) {
      flag++ ;
      continue;
    }
    if (flag != 0) {
      if (line[i] != ' ') {
        URL[j][k] = line[i];
        k++ ;
      }
      else if (line[i] == ' ') {
        j++ ;
        URL[j-1][k] = '\0';
        urlInt = findUrlInt(URL[j-1], g);
        url[urlInt]++ ;
        k = 0;
      }
    }
  }
  URL[j][k-1] = '\0';
  urlInt = findUrlInt(URL[j],g);
  url[urlInt]++ ;

  return;
}


//find out the order(number) of url stored in graph
int findUrlInt(char *url, Graph g)
{
  int i;
  for (i = 0; i < g->nP; i++) {
    if (strcmp(url, g->page[i].url) == 0) {
      return i;
    }
  }
  return -1;
}
