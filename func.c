#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include "vars.h"

// Função para incicializar acentos em português, não funciona no Windows,
// mas no meu Linux em casa deu certo
int init(struct tarefa *tasklist)
{
    setlocale(LC_ALL, "Portuguese");
    for (int i = 0; i < MAXTAREFAS; i++)
    {
        (tasklist + i)->prio = -1;
    }
    
    return 0;
}

// Entrada de strings
int input(char *text, char *str, int max_len)
{
    strcpy(str, "");
    printf("%s", text);
    printf("");
    if (fgets(str, max_len, stdin) != NULL)
    {
        for (int i = 0; i < max_len; i++)
        {
            if (str[i] == '\n')
            {
                str[i] == '\0';
                break;
            }
        }
        // Retorna 1 porque eu uso como comparador em ifs
        return 1;
    }
    else
    {
        return 0;
    }
}

// Pegar número de tarefas já preechidas
int gettasklistsize(struct tarefa *tasklist)
{
    int i;
    for (i = 0; i < MAXTAREFAS; i++)
    {
        printf("%d\n",(tasklist + i)->prio);
        if ((tasklist + i)->prio == -1)
        {
            return i;
        }
    }
    return 99;
}

// Entrada de ints
// (Obrigado ChatGPT)
int intinput(char *text)
{
    char input_buffer[50];
    printf("%s", text);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) != NULL)
    {
        int number = atoi(input_buffer);
        if (number != 0 || input_buffer[0] == '0')
        {
            return number;
        }
    }
    return -1;
}

// Criar tarefa
int createtask(struct tarefa *tasklist, char *str, int maxtask)
{
    if (maxtask == MAXTAREFAS)
    {
        printf("Limite de tarefas atingido.\n");
        return 1;
    }
    else
    {
        // Categoria, Descrição e Prioridade

        // Whiles para prender o usuário caso ele
        // escreva errado ou dê erro no input
        while (1)
        {
            if (input("Digite a categoria da tarefa\n-> ", str, MAXTAREFAS))
            {
                strcpy((tasklist + maxtask)->cat, str);
                break;
            }
            printf("\n");
        }
        while (1)
        {
            if (input("Digite a descrição da tarefa\n-> ", str, SIZE))
            {
                strcpy((tasklist + maxtask)->desc, str);
                break;
            }
            printf("\n");
        }
        int p;
        while (1)
        {
            p = intinput("Digite a prioridade (0 a 10) da tarefa\n-> ");
            if (p >= 0 && p <= 10)
            {
                (tasklist + maxtask)->prio = p;
                break;
            }
            printf("\n");
        }
        (tasklist + maxtask)->state = nao_iniciado;
        printf("\n");
    }
    return 0;
}

// Vizualizar tarefas
int viewtask(struct tarefa *tasklist, char *str, int *specific, int maxtask)
{
    int i;
    // Se a função for chamada pelo usuário
    if (!specific)
    {
        int c;
        while (1)
        {
            printf("Qual tarefa você gostaria de ver? (0 para todas, %d para a última)\n", maxtask);
            c = intinput("-> ");
            if (!(c < 0) && !(c > maxtask))
            {
                break;
            }
            printf("\n");
        }
        if (!c)
        {
            for (i = 0; i < maxtask; i++)
            {
                printf("Tarefa %d:\n", i + 1);
                printf("Prioridade: %d\n", (tasklist + i)->prio);
                printf("Estado: ");
                switch ((tasklist + i)->state)
                {
                case nao_iniciado:
                    printf("não iniciada\n");
                    break;
                case em_andamento:
                    printf("em andamento\n");
                    break;
                case completa:
                    printf("completa\n");
                    break;
                default:
                    break;
                }
                printf("Categoria: %s\n", (tasklist + i)->cat);
                printf("Descrição: %s\n\n", (tasklist + i)->desc);
            }
        }
        else
        {
            printf("Tarefa %d:\n", c);
            printf("Prioridade: %d\n", (tasklist + c - 1)->prio);
            printf("Estado: ");
            switch ((tasklist + c - 1)->state)
            {
            case nao_iniciado:
                printf("não iniciada\n");
                break;
            case em_andamento:
                printf("em andamento\n");
                break;
            case completa:
                printf("completa\n");
                break;
            default:
                break;
            }
            printf("Categoria: %s\n", (tasklist + c - 1)->cat);
            printf("Descrição: %s\n\n", (tasklist + c - 1)->desc);
        }
    }
    // Se a função for chamada pelo código (acha direto)
    else
    {
        if (*specific == -1)
        {
            for (i = 0; i < maxtask; i++)
            {
                printf("Tarefa %d:\n", i + 1);
                printf("Prioridade: %d\n", (tasklist + i)->prio);
                printf("Estado: ");
                switch ((tasklist + i)->state)
                {
                case nao_iniciado:
                    printf("não iniciada\n");
                    break;
                case em_andamento:
                    printf("em andamento\n");
                    break;
                case completa:
                    printf("completa\n");
                    break;
                default:
                    break;
                }
                printf("Categoria: %s\n", (tasklist + i)->cat);
                printf("Descrição: %s\n\n", (tasklist + i)->desc);
            }
        }
        else
        {
            printf("Tarefa %d:\n", *specific + 1);
            printf("Prioridade: %d\n", tasklist[*specific].prio);
            printf("Estado: ");
            switch (tasklist[*specific].state)
            {
            case nao_iniciado:
                printf("não iniciada\n");
                break;
            case em_andamento:
                printf("em andamento\n");
                break;
            case completa:
                printf("completa\n");
                break;
            default:
                break;
            }
            printf("Categoria: %s\n", tasklist[*specific].cat);
            printf("Descrição: %s\n\n", tasklist[*specific].desc);
        }
    }
}

// Editar tarefas
int edittask(struct tarefa *tasklist, char *str, int maxtask)
{
    int c;
    while (1)
    {
        printf("Qual tarefa você gostaria de editar? (%d para a última)\n", maxtask);
        c = intinput("-> ");
        if (!(c > maxtask) && !(c < 1))
        {
            break;
        }
        printf("\n");
    }
    int i = c - 1;
    viewtask(tasklist, str, &i, maxtask);
    c = 0;
    while (1)
    {
        // if c == 4 porque 4 é pra cancelar/sair
        if (c != 5)
        {
            c = intinput("O que deseja editar?\n1 - Prioridade\n2 - Estado\n3 - Categoria\n4 - Descrição\n5 - Sair\n-> ");
            switch (c)
            {
            case 1:
                while (1)
                {
                    c = intinput("Digite a prioridade da tarefa (0 a 10)\n-> ");
                    if (c >= 0 && c <= 10)
                    {
                        (tasklist + i)->prio = c;
                        break;
                    }
                    printf("\n");
                }
                c = 1;
                break;
            case 2:
                while (1)
                {
                    c = intinput("Escolha o estado da tarefa:\n1 - não iniciada\n2 - em andamento\n3 - completa\n-> ");
                    if (c > 0 && c < 4)
                    {
                        (tasklist + i)->state = c - 1;
                        break;
                    }
                    printf("\n");
                }
                c = 2;
                break;
            case 3:
                while (1)
                {
                    if (input("Digite a categoria da tarefa\n-> ", str, MAXTAREFAS))
                    {
                        strcpy((tasklist + i)->cat, str);
                        break;
                    }
                    printf("\n");
                }
                break;
            case 4:
                while (1)
                {
                    if (input("Digite a descrição da tarefa\n-> ", str, SIZE))
                    {
                        strcpy((tasklist + i)->desc, str);
                        break;
                    }
                    printf("\n");
                }
                break;
            default:
                break;
            }
        }
        else
        {
            break;
        }
    }
}

// Deletar tarefas
int deletetask(struct tarefa *tasklist, char *str, int maxtask)
{
    int c;
    int *temp = malloc(sizeof(int));
    *temp = -1;
    while (1)
    {
        viewtask(tasklist, str, temp, maxtask);
        printf("Qual tarefa você gostaria de deletar? (digite um número)\n");
        c = intinput("-> ");
        if (!(c < 0) && !(c > maxtask))
        {
            break;
        }
        printf("\n");
    }
    free(temp);
    c--;
    for (; c < maxtask; c++)
    {
        (tasklist + c)->prio = (tasklist + c + 1)->prio;
        strcpy((tasklist + c)->cat, (tasklist + c + 1)->cat);
        strcpy((tasklist + c)->desc, (tasklist + c + 1)->desc);
    }
    return 0;
}

// Salvar/Criar arquivo bin
int savedata(char *filename, struct tarefa *all, int *qntd)
{
    FILE *f = fopen(filename, "wb");
    if (f == NULL)
    {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return 1;
    }
    // Escrever o array tasklist no arquivo (inclusive os NULLs)
    // O arquivo deu em torno de 40Kb então achei ok colocar tudo de uma vez
    if (fwrite(all, sizeof(struct tarefa), MAXTAREFAS, f) != MAXTAREFAS)
    {
        printf("Erro ao escrever no arquivo.\n");
        fclose(f);
        return 1;
    }
    if (fwrite(qntd, sizeof(int), 1, f) != 1)
    {
        printf("Erro ao escrever no arquivo.\n");
        fclose(f);
        return 1;
    }
    fclose(f);
    return 0;
}

// Carregar arquivo bin
int loaddata(char *filename, struct tarefa *all, int *qntd)
{
    FILE *f = fopen(filename, "rb");
    if (f == NULL)
    {
        printf("Erro ao abrir o arquivo para leitura.\n");
        return 1;
    }
    // Lê os dados do arquivo direto para o array tasklist
    if (fread(all, sizeof(struct tarefa), MAXTAREFAS, f) != MAXTAREFAS)
    {
        printf("Erro ao ler o arquivo.\n");
        fclose(f);
        return 1;
    }
    if (fread(qntd, sizeof(int), 1, f) != 1)
    {
        printf("Erro ao ler o arquivo.\n");
        fclose(f);
        return 1;
    }
    fclose(f);
    return 0;
}