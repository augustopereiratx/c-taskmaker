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