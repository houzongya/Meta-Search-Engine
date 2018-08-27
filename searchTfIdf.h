#define BUFFER 100

void searchTfIdf(int argc, char* argv[]);

int countURLs(char *word);

int countWord(char *word, char*url);

int totalWordCount(char *url);

int totalURls();

void sort(char url[BUFFER][BUFFER], double tfidf[BUFFER], int terms[BUFFER], int count);

double calculate_tfidf(char *word, char *str);

void normalise(char *str);
