#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include "vars.h"

// Função para incicializar acentos em português, não funciona no Windows,
// mas no meu Linux em casa deu certo
int init()
{
    setlocale(LC_ALL, "Portuguese");
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
        if ((tasklist + i)->prio == NULL)
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
int createtask(struct tarefa *tasklist, char *str)
{
    int maxtask = gettasklistsize(tasklist);
    if (maxtask == 99)
    {
        printf("Limite de tarefas atingido.\n");
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
        maxtask++;
        printf("\n");
    }
    return 0;
}

// Vizualizar tarefas
int viewtask(struct tarefa *tasklist, char *str, int *specific)
{
    // Se a função for chamada pelo usuário
    if (specific == NULL)
    {
        int maxtask = gettasklistsize(tasklist);
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
        int i;
        if (c == 0)
        {
            for (i = 0; i < maxtask; i++)
            {
                printf("Tarefa %d:\n", i + 1);
                printf("Prioridade: %d\n", (tasklist + i)->prio);
                printf("Categoria: %s\n", (tasklist + i)->cat);
                printf("Descrição: %s\n\n", (tasklist + i)->desc);
            }
        }
        else
        {
            printf("Tarefa %d:\n", c);
            printf("Prioridade: %d\n", (tasklist + c - 1)->prio);
            printf("Categoria: %s\n", (tasklist + c - 1)->cat);
            printf("Descrição: %s\n\n", (tasklist + c - 1)->desc);
        }
    }
    // Se a função for chamada pelo código (acha direto)
    else
    {
        printf("Tarefa %d:\n", *specific + 1);
        printf("Prioridade: %d\n", tasklist[*specific].prio);
        printf("Categoria: %s\n", tasklist[*specific].cat);
        printf("Descrição: %s\n\n", tasklist[*specific].desc);
    }
}

// Editar tarefas
int edittask(struct tarefa *tasklist, char *str)
{
    int maxtask = gettasklistsize(tasklist);
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
    viewtask(tasklist, str, &i);
    c = 0;
    while (1)
    {
        // if c == 4 porque 4 é pra cancelar/sair
        if (c != 4)
        {
            c = intinput("O que deseja editar?\n1 - Prioridade\n2 - Categoria\n3 - Descrição\n4 - Sair\n-> ");
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
                break;
            case 2:
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
            case 3:
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

// Salvar/Criar arquivo bin
int savedata(char *filename, struct tarefa *tasklist)
{
    FILE *f = fopen(filename, "wb");
    if (f == NULL)
    {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return 1;
    }
    // Escrever o array tasklist no arquivo (inclusive os NULLs)
    // O arquivo deu em torno de 40Kb então achei ok colocar tudo de uma vez
    if (fwrite(tasklist, sizeof(struct tarefa), 100, f) != 100)
    {
        printf("Erro ao escrever no arquivo.\n");
        fclose(f);
        return 1;
    }
    fclose(f);
    return 0;
}

// Carregar arquivo bin
int loaddata(char *filename, struct tarefa *tasklist)
{
    FILE *f = fopen(filename,"rb");
    if (f == NULL)
    {
        printf("Erro ao abrir o arquivo para leitura.\n");
        return 1;
    }
    // Lê os dados do arquivo direto para o array tasklist
    if(fread(tasklist, sizeof(struct tarefa), 100, f) != 100)
    {
        printf("Erro ao ler o arquivo.\n");
        fclose(f);
        return 1;
    }
    fclose(f);
    return 0;
}
