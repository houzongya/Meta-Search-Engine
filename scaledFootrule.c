#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "scaledFootrule.h"

double min;
urlRep urlC;
int num_of_arg;


int main(int argc, char **argv)
{
  //initialize C set (Union of rank lists)
  num_of_arg = argc;
  urlC = getUrlC(num_of_arg, argv);
  int array[urlC->counter];
  for (int i = 1; i < urlC->counter + 1; i++) {
    array[i-1] = i;
  }

  //set minimum result
  int minP[urlC->counter];
  min = 1000;
  permute(array, 0, urlC->counter, minP, argv);

  //print out minimum result
  printf("%.6f\n", min);
  //print out corresponding urls
  for (int i = 0; i < urlC->counter; i++) {
    url curr = urlC->first;
    int j = 0;
    while(j < minP[i]-1) {
      j++;
      curr = curr->next;
    }
    printf("%s\n", curr->url_name);
  }
  return 0;
}


//Using linked list to store information here
urlRep getUrlC(int num_of_arg, char **argv)
{
  urlRep urlC = newUrlRep();
  char URL[BUFFER][BUFFER];

  for (int i = 1; i < num_of_arg; i++) {
    FILE *fp = fopen(argv[i],"r");
    int j = 0;
    while (fgets(URL[j], BUFFER, fp) != NULL) {
      unsigned long ln = strlen(URL[j]) - 1;
      if (ln > 0 && URL[j][ln] == '\n') {
        URL[j][ln] = '\0';
      }
      if (checkUrl(urlC, URL[j]) == 0) {
        insertUrl(urlC, newUrl(URL[j]));
      }
      j++ ;
    }
    fclose(fp);
  }
  return urlC;
}


//check if Url already exists
int checkUrl(urlRep urlC, char *URL)
{
  int found = 0;
  url curr = urlC->first;
  if (curr == NULL) {
    return found;
  }
  while (curr != NULL) {
    if (strncmp(curr->url_name, URL, strlen(URL)) == 0) {
      found = 1;
      break;
    }
    curr = curr->next;
  }

  return found;
}


//swap and permute are used for generating permutation of C (Union Set of rank lists)
void swap(int *a, int *b)
{
  int temp;
  temp = *a;
  *a = *b;
  *b = temp;
}
//calculate distance of every P in permute function
void permute(int *num, int j, int length, int *minP, char** argv)
{
  int i;
  if(j == length) {
    double distance = calculate_distance(num, argv);
    if (distance < min) {
      for (int i = 0; i < length; i++) {
        minP[i] = num[i];
      }
      min = distance;
    }
  }
  else
  {
    for(i = j; i < length; i++) {
      swap(&num[j], &num[i]);
      permute(num, j + 1, length, minP, argv);
      swap(&num[j], &num[i]);
    }
  }
}


double calculate_distance(int *num, char **argv)
{
  int n = urlC->counter;
  double distance = 0;
  for (int i = 1; i < num_of_arg; i++ ) {
    FILE *fp = fopen(argv[i], "r");
    int t_size = 0;
    char URL[BUFFER];
    while (fgets(URL, BUFFER, fp) != NULL) {
      t_size++ ;
    }
    fclose(fp);
    for (int j = 0; j < n; j++) {
      int p = num[j];
      int position = findPosition(j+1, argv[i]);
      if (position == -1) {
        continue;
      }
      else {
        distance = distance + fabs((double)position/t_size - (double)p/urlC->counter);
      }
    }

  }

  return distance;
}


int findPosition(int num, char *txt)
{
  int position = 0;
  char URL[BUFFER];
  url curr = urlC->first;
  int i = 0;
  while(i < num) {
    strcpy(URL, curr->url_name);
    i++;
    curr = curr->next;
  }

  FILE *fp = fopen(txt, "r");
  char Url[BUFFER];
  int found = 0;
  while (fgets(Url, BUFFER, fp) != NULL) {
    position++ ;
    if (strncmp(Url, URL, strlen(URL)) == 0) {
      found = 1;
      break;
    }
  }
  fclose(fp);

  if (found == 0) {
    position = -1;
  }

  return position;
}


url newUrl(char *url_name)
{
	url new;
	new = malloc(BUFFER * sizeof(url_));
	assert(new != NULL);
  //copy url_name to linked list node
  int i;
  for (i = 0; url_name[i] != '\0'; i++) {
    new->url_name[i] = url_name[i];
  }
  new->url_name[i] = '\0';
	new->next = NULL;
	return new;
}


urlRep newUrlRep()
{
	urlRep urlC;
	urlC = malloc(sizeof (urlRep_));
	assert (urlC != NULL);
	urlC->first = NULL;
  urlC->last = NULL;
  urlC->counter = 1;
	return urlC;
}


void insertUrl(urlRep urlC, url URL)
{

  if (urlC->first == NULL) {
    urlC->first = URL;
    urlC->last = URL;
  }
  else {
    urlC->last->next = URL;
    urlC->last = URL;//
    urlC->counter = urlC->counter +1;
  }
  URL->order = urlC->counter;
  return;
}
