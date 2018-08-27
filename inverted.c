#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "readData.h"
#include "graph.h"
#include "inverted.h"


int main(int argc, char** argv)
{
  Index indexx[BUFFER];

  //create graph
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
  g->index_size = 0;


  //content contains all the lines for each url.txt
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

    //normalise and inverted every line(content part) in txt
    for (int l = 1; l<line; l++) {
      normalise(content[i][l]);
      inverted(g, content[i][l], indexx, i);
    }

  }
  fclose(fp);
  printIndex(g, indexx);

  return 0;
}


void normalise(char *content)
{
  int n = 0;
  while(content[n] != '\n') {
    if ('a' <= content[n] && content[n] <= 'z') {
      content[n] = content[n];
    } else if ('A' <= content[n] && content[n] <= 'Z') {
      content[n] = content[n] - ('A' - 'a');
    } else {
      content[n] = ' ';
    }
    n++;
  }
  content[n] = '\0';
  return ;
}


void inverted(Graph g, char *content, Index *index, int url)
{

  char word[BUFFER][BUFFER];
  int i = 0;
  int j = 0, k = 0;
  int flag = 0;

  while (content[i] != '\0') {
    if (flag == 1 && content[i] == ' ') {
      //extract word to create index
      word[j][k] = '\0';
      if (strcmp(word[j],"\0")!= 0) {
        createIndex(g, index, word[j], url);
      }

      if (content[i - 1] != ' ') {
        j++ ;
      }
      k = 0;
    }
    else if (content[i] >= 'a' && content[i] <= 'z') {
      word[j][k] = content[i];
      k++ ;
      flag = 1;
    }
    i++;

  }

  //extract the last word in a line
  word[j][k] = '\0';
  if (strcmp(word[j],"\0")!= 0) {
    createIndex(g, index, word[j], url);
  }

  return;
}


void createIndex(Graph g, Index* index, char *word, int url)
{

  int index_size = g->index_size;
  int flag = 0;
  int which_index = -1;

  //skip if word has already been stored
  for (int i = 0; i < index_size; i++) {
    if (strcmp(word, index[i].word) == 0) {
      which_index = i;
      flag++ ;
    }
  }
  if (flag == 0) {
    char* word1 = malloc(word_size*sizeof(char));
    strcpy(word1, word);
    (index[index_size]).word = word1;
    index[index_size].url_size = 0;
    which_index = index_size;
    g->index_size = g->index_size +1;
  }

  //skip if url has already been stored
  int url_size = index[which_index].url_size;
  flag = 0;
  for (int i = 0; i < url_size; i++) {
    if (index[which_index].url[i] == url) {
      flag++ ;
    }
  }
  if (flag == 0 ) {
    (index[which_index].url)[url_size] = url;
    index[which_index].url_size = index[which_index].url_size+1;
  }

  return;

}


//order and print index to txt file
void printIndex(Graph g, Index *index)
{
  int index_size = g->index_size;

  char **term = malloc(index_size * sizeof(char *));
  for (int i = 0; i < index_size; i++) {
    term[i] = malloc(BUFFER * sizeof(char));
    strcpy(term[i], index[i].word);
  }

  char t[BUFFER];
  for (int i = 1; i < index_size; i++) {
    for (int j = 1; j < index_size; j++) {
      if (strcmp(term[j - 1], term[j]) > 0) {
        strcpy(t, term[j - 1]);
        strcpy(term[j - 1], term[j]);
        strcpy(term[j], t);
      }
    }
  }

  FILE *f;
  f = fopen("invertedIndex.txt", "w");
  for (int i = 0; i < index_size; i++) {
    for (int k = 0; k < index_size; k++) {
      if (strcmp(index[k].word,term[i]) == 0) {
        fprintf(f, "%s", index[k].word);
        for (int j = 0; j < index[k].url_size; j++) {
          int url = index[k].url[j];
          fprintf(f, " %s", g->page[url].url);
        }
      }
    }
    fprintf(f,"\n");
  }

  //free memory
  for (int i = 0; i < index_size; i++) {
    free(term[i]);
  }
  free(term);

  fclose(f);

  return;
}
