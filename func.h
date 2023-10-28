#include "vars.h"

int init(struct everything *);
int input(char *, char *, int);
int gettasklistsize(struct tarefa *);
int getCatSizeSize(char [][MAXTAREFAS]);
int strcompare(char *, char *);
int doesCatExist(char *[MAXTAREFAS], char *);
int intinput(char *);
int createtask(struct tarefa *, char *, char [][MAXTAREFAS]);
int reverseArray(struct tarefa *, int);
int viewtask(struct tarefa *, char *, char [][MAXTAREFAS], int *);
int edittask(struct tarefa *, char *, char [][MAXTAREFAS]);
int deletetask(struct tarefa *, char *, char [][MAXTAREFAS]);
int savedata(char *, struct everything *);
int loaddata(char *, struct everything *);