#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "graph.h"
#include "readData.h"
#include "pagerank.h"


int main(int argc, char** argv)
{
  Graph g = GetGraph();
  printPageRank(g, atof(argv[1]), atof(argv[2]), atoi(argv[3]));
  return 0;
}

//order and print pagerank to txt file
int printPageRank(Graph g, double d, double diffPR, int maxIterations)
{
  calculatePageRank(g, d, diffPR, maxIterations);
  int num_of_url = g->nP;
  int rank[num_of_url];
  for(int p = 0; p < num_of_url; p++) {
    rank[p] = p;
  }

  int i, j;
  for (i = 0; i < num_of_url; i++) {
    for (j = 0; j < num_of_url; j++) {
      if ((g->page[rank[j]]).pagerank < (g->page[rank[j+1]]).pagerank) {
        int temp = rank[j+1];
        rank[j+1] = rank[j];
        rank[j] = temp;
      }
    }
  }

  FILE *f = fopen("pagerankList.txt", "w");
  int links[num_of_url];
  for(int p = 0; p < num_of_url; p++) {
    fprintf(f, "%s, %d, %.7f\n",(g->page[rank[p]]).url, outlinksFrom(g,rank[p],links), (g->page[rank[p]]).pagerank);
  }

  fclose(f);
  return 0;
}


void calculatePageRank(Graph g, double d, double diffPR, int maxIterations)
{
  int num_of_url = g->nP;
  int iteration = 0;
  double diff = diffPR;
  double pagerank_t, pagerank_tt;
  while (iteration < maxIterations && diff >= diffPR) {
    iteration++;
    diff = 0;
    for (int Pi = 0; Pi < num_of_url; Pi++) {
      pagerank_tt = pagerank(g, Pi, d, iteration + 1);
      pagerank_t = pagerank(g, Pi, d, iteration);
      (g->page[Pi]).pagerank = pagerank_tt;
      diff = diff + fabs(pagerank_tt - pagerank_t);
    }
  }

  return;
}


double pagerank(Graph g, int Pi, double d, int iteration)
{
  int num_of_url = g->nP;
  if (iteration == 1) {
    return 1/(double)num_of_url;
  }

  int set_of_Pj[num_of_url];
  int num_of_Pj = inlinksTo(g, Pi, set_of_Pj);

  double sum = 0;
  for (int j = 0; j < num_of_Pj; j++) {
    int Pj = set_of_Pj[j];
    double wpk = weightedPageRank(g, Pj, Pi);
    sum = sum + pagerank(g, Pj, d, iteration-1) * wpk;
  }
  double rank = (1-d)/num_of_url + d*sum;
  return rank;
}


double weightedPageRank(Graph g, int j, int i)
{
  int num_of_url = g->nP;
  int inlinks[num_of_url], outlinks[num_of_url];
  int referencePages[num_of_url];
  int num_of_referencePages = outlinksFrom(g, j, referencePages);

  double sum_of_inlinks = 0;
  double sum_of_outlinks = 0;
  for (int k = 0; k < num_of_referencePages; k++) {
    sum_of_outlinks = sum_of_outlinks + outlinksFrom(g, referencePages[k], outlinks);
    sum_of_inlinks = sum_of_inlinks + inlinksTo(g, referencePages[k], inlinks);
  }

  if (sum_of_outlinks == 0) {
    sum_of_outlinks = 0.5;
  }
  if (sum_of_inlinks == 0) {
    sum_of_inlinks = 0.5;
  }

  double Win = inlinksTo(g, i, inlinks)/sum_of_inlinks;
  double Wout = outlinksFrom(g, i, outlinks)/sum_of_outlinks;

  return Win*Wout;
}


int inlinksTo(Graph g, int v, int *links)
{
  int num_of_url = g->nP;
  int counter = 0;
  for (int j = 0; j < num_of_url; j++) {
    if (g->edges[j][v]) {
      links[counter] = j;
      counter++ ;
    }
  }
  return counter;
}


int outlinksFrom(Graph g, int v, int *links)
{
  int num_of_url = g->nP;
  int i = 0;
  for (int j = 0; j < num_of_url; j++) {
    if (g->edges[v][j]) {
      links[i] = j;
      i++ ;
    }
  }
  return i;
}
