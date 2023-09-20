#include <stdio.h>
#include "vars.h"
#include "func.h"

int main()
{
    init();
    struct tarefa tasklist[MAXTAREFAS];
    int qntd = 0;
    loaddata("tasks.bin", tasklist, &qntd);
    char str[SIZE];
    int choice = 0;
    while (1)
    {
        if (choice == 5)
        {
            break;
        }
        choice = intinput("Bem vindo(a) ao TaskMaker!\n\nSelecione:\n1 - Criar tarefa\n2 - Editar tarefa\n3 - Excluir tarefa\n4 - Ver tarefas\n5 - Sair\n\n-> ");
        switch (choice)
        {
        case 1:
            if (!(createtask(tasklist, str, qntd)))
            {
                qntd++;
            }
            break;
        case 2:
            edittask(tasklist, str, qntd);
            break;
        case 3:
            if (!(deletetask(tasklist, str, qntd)))
            {
                qntd--;
            }
            break;
        case 4:
            viewtask(tasklist, str, 0, qntd);
            break;
        case 5:
            savedata("tasks.bin", tasklist, &qntd);
            printf("Até logo!\n");
            break;
        default:
            break;
        }
    }
    return 0;
}