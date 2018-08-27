#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "searchTfIdf.h"


int main (int argc, char *argv[]) {
   searchTfIdf(argc, argv);
   return 0;
}


void searchTfIdf(int argc, char* argv[]) {
   double tfidf[BUFFER] = {0};
   int terms[BUFFER];
   char url[BUFFER][BUFFER];
   char str[BUFFER];
   char copy[BUFFER];
   char *word;
   char ch;
   int n = 1;
   int i = 0;
   int flag = 0;
   int count = 0;

   //ERROR if no words are in the command line
   if (argc <= 1) {
      printf("Please type a word to search in the command line.\n");
      return;

   } else {
      for (n = 1; n<argc; n++) {
         normalise(argv[n]);
         word = argv[n];
         FILE *fp = fopen("invertedIndex.txt","r+");
         if (fp == NULL) {
            printf("Failed to open file.\n");
            return;
         }
         while (!feof(fp)) {
            fscanf(fp, "%s", str);
            if (strcmp(str,word) == 0) {
               while ((ch=getc(fp)) != '\n') {
                  if (ch == ' ') {
                     fscanf(fp, "%s", str);
                     strcpy(copy,str);
                     if (n > 1){
                        int m = 0;
                        flag = 0;
                        while (m<i && flag == 0) {
                           if (strcmp(url[m],copy)==0) {
                              strcat(str, ".txt");
                              tfidf[m] = (float)tfidf[m] + (float)calculate_tfidf(word,str);
                              terms[m] = countWord(word,str) + terms[m];
                              flag = -1;
                           }
                           m++;
                        }
                     }
                     if (flag >= 0 ) {
                        strcpy(url[i], copy);
                        strcat(str, ".txt");
                        tfidf[i] = calculate_tfidf(word,str);
                        terms[i] = countWord(word,str);
                        count++;
                        i++;
                     }
                  }
               }
            }
         }
      }
      if (n == argc) {
         if (count > 30) {
            count = 30;
         }
         sort(url,tfidf,terms,count);
      }
   }
}


void sort(char url[BUFFER][BUFFER], double tfidf[BUFFER], int terms[BUFFER], int count) {
   int curr = 0;
   int max_term = 0;
   double max_tfidf = 0;
   int n = 0;
   int max_position = 0;

   while (curr < count) {
      max_term = 0;
      max_tfidf = 0;
      max_position = 0;
      n = 0;
      while (n < count) {
         if ((terms[n] > max_term || (terms[n] == max_term && tfidf[n] > max_tfidf))) {
            max_tfidf = tfidf[n];
            max_term = terms[n];
            max_position = n;
         }
         n++;
      }
      printf("%s %.6f\n", url[max_position], tfidf[max_position]);
      if (curr == 30) {
         return;
      }
      terms[max_position] = -1;
      tfidf[max_position] = -1;
      curr++;
   }
}


double calculate_tfidf(char *word, char *str)
{
   double tf;
   double idf;
   double tfidf;
   int totalWords = totalWordCount(str);
   int numWords = countWord(word,str);
   int totalUrls = totalURls();
   int numUrls = countURLs(word);
   if (totalWords != 0) {
      tf = ((float)(numWords)/((float)(totalWords)));
   }
   if (numUrls != 0) {
      idf = log10((float)totalUrls/(float)numUrls);
   }
   tfidf = tf*idf;
   return tfidf;
}


// number of URLs associated with the word
int countURLs(char *word)
{
   FILE *fp = fopen("invertedIndex.txt", "r");
   if (fp == NULL) {
      printf("Failed to open file.\n");
      return 0;
   }
   char str[100];
   int count = 0;
   char ch;
   while (!feof(fp)) {
      fscanf(fp, "%s", str);
      if (strcmp(str,word) == 0) {
         while ((ch=getc(fp)) != '\n') {
            if (ch == ' ') {count++;}
            if (ch == EOF) {break;}
         }
      }
   }
   return count;
}


// number of times the specific word appears in the URL
int countWord(char *word, char*url)
{
   FILE *fp = fopen(url, "r");
   char str[100];
   int count = 0;
   while (!feof(fp)) {
      fscanf(fp, "%s", str);
      normalise(str);
      if (strcmp(str,word) == 0) {
         count++;
      }
   }
   return count;
}


// total number of words in URL
//this function does not count the #start Section-1 and urls etc....
int totalWordCount(char *url)
{
   int nwords = -2;
   FILE *fp = fopen(url, "r");
   char str[100];
   int check = 0;
   while (!feof(fp)) {
      fscanf(fp, "%s", str);
      if (str[0]=='#') {
         check++;
      }
      if (check == 3) {
         nwords++;
      }
      if (check == 4) {
         break;
      }
   }
   if (nwords > 0) {
      return nwords;
   } else {
      return 0;
   }
}

//counts the total number of URLS from collection.txt
int totalURls()
{
   FILE *fp = fopen("collection.txt","r");
   if (fp == NULL) {
      printf("Failed to open file.\n");
      return 0;
   }
   char str[100];
   int count = -1;
   while (!feof(fp)) {
      fscanf(fp, "%s", str);
      if (strcmp(str, " ") != 0) {
         count++;
      }
   }
   if (count > 0) {
      return count;
   } else {
      return 0;
   }
}

void normalise(char *str)
{
   int n = 0;
   for (n = 0; n<strlen(str); n++) {
      if ('A' <= str[n] && str[n] <= 'Z') {
         str[n] = str[n] + 32;
      } else if (str[n] <= 'z' && 'a' <= str[n]) {
         continue;
      } else {
         str[n] = '\0';
      }
   }
}
