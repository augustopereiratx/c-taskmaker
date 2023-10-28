#ifndef VARS_H
#define VARS_H
#define SIZE 300
#define MAXTAREFAS 100

enum estado
{
    nao_iniciado,
    em_andamento,
    completa
};

struct tarefa
{
    int prio;
    enum estado state;
    char desc[SIZE];
    char cat[MAXTAREFAS];
};

struct everything 
{
    struct tarefa tasklist[MAXTAREFAS];
    int qntd;
    char categorias[MAXTAREFAS][MAXTAREFAS];
};
#endif