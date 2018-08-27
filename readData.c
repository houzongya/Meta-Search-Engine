#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "readData.h"
#include "graph.h"
#include "pagerank.h"
#include "inverted.h"
#include "searchPagerank.h"


// get all URLS from collection.txt
int GetCollection(List_of_Urls url)
{
  FILE *fp = fopen("collection.txt","r+");

  //convert \n into spaces
  char c = fgetc(fp);
  while ((c = fgetc(fp)) != EOF) {
    if (c == ' ') {
      fseek(fp, -1, SEEK_CUR);
      fputc('\n',fp);
      fseek(fp, 0, SEEK_CUR);
    }
  }
  fclose(fp);

  fp = fopen("collection.txt","r");//
  int i = 0;
  while (fgets(url[i], MAX_URL, fp)!= NULL) {
    int ln = strlen(url[i]) - 1;
    if (ln > 0 && url[i][ln] == '\n') {
      url[i][ln] = '\0';
    }
    if (strcmp("\n", url[i]) != 0) {
      i++ ;
    }
  }
  fclose(fp);
  return i;
}


//puts all urls into graph
Graph GetGraph(void)
{
  List_of_Urls url;
  int num_of_url = GetCollection(url);
  Graph g = newGraph(num_of_url);
  Page *p = g->page;
  int i = 0;
  while (i < num_of_url){
    p[i].v = i;
    p[i].url = url[i];
    i++ ;
  }

  //content is a set containing all the lines for each url.txt
  char ***content = malloc(BUFFER * sizeof (char **));
  for(int i = 0; i < BUFFER; i++) {
    content[i] = malloc(BUFFER * sizeof (char *));
    for(int j = 0; j < BUFFER; j++) {
      content[i][j] = malloc(BUFFER * sizeof (char));
    }
  }

  char **URL = malloc(BUFFER * sizeof (char *));
  for(int j = 0; j < BUFFER; j++) {
    URL[j] = malloc(BUFFER * sizeof (char));
  }

  g->index_size = 0;

  FILE *fp;
  List_of_Urls txt;
  for(int i = 0; i < num_of_url; i++) {
    strcpy(txt[i], url[i]);
    strcat(txt[i], ".txt");
    fp = fopen(txt[i], "r");

    int flag = 0;
    int line = 0;
    while (fgets(content[i][line], MAX_LINE, fp)!= NULL) {
      if (content[i][line][0] == '#') {
        flag++ ;
      }
      //exit loop after fgets reached two #ends
      if (flag == 4) {
        break;
      }
      if (content[i][line][0] != '#' && content[i][line][0] != '\n') {
        line++;
      }
    }

    //insert edges
    int length_of_url = readUrl(content[i][0], URL);
    for (int k = 0; k < length_of_url; k++) {
      for(int j = 0; j < num_of_url; j++) {
        if (strcmp(URL[k], url[j]) == 0) {
          insertEdge(g, p[i], p[j], 1);
          //avoid self loop
          if (i == j) {
            removeEdge(g, p[i], p[j]);
          }
        }
      }
    }

  }
  fclose(fp);

  return g;
}


//puts urls/words from .txt file in a line into an array of strings
int readUrl(char *line, char **URL)
{
  //convert all spaces into '\n'
  int i = 0;
  while (line[i] != '\n') {
    if (line[i] == ' ') {
      line[i] = '\n';
    }
    i++ ;
  }
  line[i] = '\0';

  int j = 0, k = 0;
  int flag = 0;
  i = 0;
  while (line[i] != '\0') {
    if (flag == 1 && line[i] == 'u') {
      j++;
      k = 0;
    }
    if (line[i] != '\n') {
      URL[j][k] = line[i];
      k++;
      flag = 1;
    }
    i++;
  }
  return j+1;
}
