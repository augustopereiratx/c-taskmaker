#include <stdio.h>
#include <stdlib.h>
#include "vars.h"
#include "func.h"

int main()
{
    struct everything *all = malloc(sizeof(struct everything));
    all->qntd = 0;
    init(all);
    loaddata("tasks.bin", all);
    char str[SIZE];
    int choice = 0;
    all->qntd = gettasklistsize(all->tasklist);
    printf("Bem vindo(a) ao TaskMaker!\n\n");
    while (1)
    {
        if (choice == 5)
        {
            break;
        }
        choice = intinput("\nSelecione:\n1 - Criar tarefa\n2 - Editar tarefa\n3 - Excluir tarefa\n4 - Ver tarefas\n5 - Sair\n\n-> ");
        switch (choice)
        {
        case 1:
            if (!(createtask(all->tasklist, str, all->categorias)))
            {
                all->qntd++;
            }
            break;
        case 2:
            edittask(all->tasklist, str, all->categorias);
            break;
        case 3:
            if (!(deletetask(all->tasklist, str, all->categorias)))
            {
                all->qntd--;
            }
            break;
        case 4:
            viewtask(all->tasklist, str, all->categorias, 0);
            break;
        case 5:
            savedata("tasks.bin", all);
            printf("Até logo!\n");
            break;
        default:
            break;
        }
    }
    free(all);
    return 0;
}