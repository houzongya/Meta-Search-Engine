typedef struct url_ *url;
typedef struct urlRep_ *urlRep;
#define BUFFER 1000


typedef struct url_{
  int order;
  char url_name[BUFFER];
  url next;
} url_;

typedef struct urlRep_ {
  url first;
  url last;
  int counter;
} urlRep_;

urlRep getUrlC(int num_of_arg, char **argv);

void permute(int *num, int j, int length, int *minP, char **argv);

double calculate_distance(int *num, char **argv);

int findPosition(int num, char *txt);


url newUrl(char *url_name);
urlRep newUrlRep();
void insertUrl(urlRep C, url URL);
//void printUrlC(urlRep C);
int checkUrl(urlRep C, char *URL);
