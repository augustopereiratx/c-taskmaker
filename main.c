#include <stdio.h>
#include "vars.h"
#include "func.h"

int main()
{
    init();
    struct tarefa tasklist[100];
    loaddata("tasks.bin",tasklist);
    char str[SIZE];
    int choice = 0;
    while(1)
    {
        if(choice == 5)
        {
            break;
        }
        choice = intinput("Bem vindo(a) ao TaskMaker!\n\nSelecione:\n1 - Criar tarefa\n2 - Editar tarefa\n3 - Excluir tarefa\n4 - Ver tarefas\n5 - Sair\n\n-> ");
        switch (choice)
        {
        case 1:
            createtask(tasklist,str);
            break;
        case 2:
            edittask(tasklist,str);
            break;
        case 3:
            break;
        case 4:
            viewtask(tasklist,str,NULL);
            break;
        case 5:
            savedata("tasks.bin",tasklist);
            printf("Até logo!\n");
            break;
        default:
            break;
        }
    }
    return 0;
}