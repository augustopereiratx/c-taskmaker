#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include "vars.h"

// Função para incicializar acentos em português, não funciona no Windows, mas no meu Linux em casa deu certo
int init(struct everything *all)
{
    setlocale(LC_ALL, "Portuguese");
    for (int i = 0; i < MAXTAREFAS; i++)
    {
        all->tasklist[i].prio = -1;
        strcpy(all->categorias[i], " ");
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
    for (int i = 0; i < MAXTAREFAS; i++)
    {
        if (tasklist[i].prio == -1)
        {
            return i;
        }
    }
    return MAXTAREFAS;
}

// pegar tamanho de categorias
int getCatSizeSize(char categorias[][MAXTAREFAS])
{
    for (int i = 0; i < MAXTAREFAS; i++)
    {
        if (!strcmp(categorias[i], " "))
        {
            return i;
        }
    }
    return MAXTAREFAS;
}

// vê se strings são idênticas
int strcompare(char *str1, char *str2)
{
    int result = strcmp(str1, str2);
    if (!result)
    {
        for (int i = 0; i < strlen(str1); i++)
        {
            if (str1[i] != str2[i])
            {
                return 0;
            }
        }
    }
    return (result == 0) ? 1 : 0;
}

// ver se categoria já existe
int doesCatExist(char categorias[][MAXTAREFAS], char *categoria)
{
    int catsize = getCatSizeSize(categorias);
    for (int i = 0; i < catsize; i++)
    {
        if (strcompare(categoria, categorias[i]))
        {
            return 0;
        }
    }
    // caso seja categoria nova
    strcpy(categorias[catsize], categoria);
    return 1;
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
int createtask(struct tarefa *tasklist, char *str, char categorias[][MAXTAREFAS])
{
    int maxtask = gettasklistsize(tasklist);
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
                int catsize = getCatSizeSize(categorias);
                doesCatExist(categorias, tasklist[maxtask].cat);
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

// necessario pra filtros
int reverseArray(struct tarefa *tarefas, int length)
{
    int start = 0;
    int end = length - 1;
    struct tarefa temp;

    while (start < end)
    {
        temp = tarefas[start];
        tarefas[start] = tarefas[end];
        tarefas[end] = temp;

        start++;
        end--;
    }
    return 0;
}

// exportar lista filtrada
int exportfilter(struct tarefa *tasklist, int listsize)
{
    int input = intinput("\nDeseja exportar essa lista para um arquivo .txt?\n0 - não\n1 - sim\n-> ");
    if (input)
    {
        FILE *f = fopen("lista.txt", "w");
        for (int i = 0; i < listsize; i++)
        {
            fprintf(f,"Tarefa %d:\n", i + 1);
            fprintf(f,"Prioridade: %d\n", tasklist[i].prio);
            fprintf(f,"Estado: ");
            switch (tasklist[i].state)
            {
            case nao_iniciado:
                fprintf(f,"não iniciada\n");
                break;
            case em_andamento:
                fprintf(f,"em andamento\n");
                break;
            case completa:
                fprintf(f,"completa\n");
                break;
            default:
                break;
            }
            fprintf(f,"Categoria: %s\n", tasklist[i].cat);
            fprintf(f,"Descrição: %s\n\n", tasklist[i].desc);
        }
        fclose(f);
    }

    return 0;
}

// Vizualizar tarefas
int viewtask(struct tarefa *tasklist, char *str, char categorias[][MAXTAREFAS], int *specific)
{
    int input;
    int maxtask = gettasklistsize(tasklist);
    int i;
    // Se a função for chamada pelo usuário
    if (!specific)
    {
        struct tarefa *tasklist_filtered = malloc(maxtask * sizeof(struct tarefa));
        int cont = 0;
        int *has_user_filtered = malloc(sizeof(int));
        *has_user_filtered = 0;
        int c;
        while (1)
        {
            // quer filtrar?
            c = intinput("Gostaria de filtrar as tarefas?\n1 - não\n2 - por prioridade\n3 - por estado\n4 - por categoria\n5 - por prioridade e categoria\n6 - por prioridade escolhida pelo usuário\n7 - por estado escolhido pelo usuário\n8 - por categoria escolhida pelo usuário\n9 - por categoria e prioridade escolhidas pelo usuário\n-> ");
            if (c >= 1 && c <= 9)
            {
                break;
            }
            printf("\n");
        }
        switch (c)
        {
        case 1:
            // sem filtro
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
                for (int i = 0; i < maxtask; i++)
                {
                    viewtask(tasklist, str, categorias, &i);
                }
            }
            else
            {
                c -= 1;
                viewtask(tasklist, str, categorias, &c);
                c++;
            }
            exportfilter(tasklist, maxtask);
            break;
        case 2:
            *has_user_filtered = 1;
            // por prioridade
            for (int i = 0; i < 11; i++)
            {
                for (int j = 0; j < maxtask; j++)
                {
                    if (tasklist[j].prio == i)
                    {
                        tasklist_filtered[cont++] = tasklist[j];
                    }
                }
            }

            break;
        case 3:
            *has_user_filtered = 1;
            // filtrar por estado
            for (int i = nao_iniciado; i < completa + 1; i++)
            {
                for (int j = 0; j < maxtask; j++)
                {
                    if (tasklist[j].state == i)
                    {
                        tasklist_filtered[cont++] = tasklist[j];
                    }
                }
            }
            break;
        case 4:
            *has_user_filtered = 1;
            // filtrar por categoria
            for (int i = 0; i < getCatSizeSize(categorias); i++)
            {
                for (int j = 0; j < maxtask; j++)
                {
                    if (strcompare(tasklist[j].cat, categorias[i]))
                    {
                        tasklist_filtered[cont++] = tasklist[j];
                    }
                }
            }
            break;
        case 5:
            *has_user_filtered = 1;
            // filtrar por prioridade e categoria

            for (int i = 0; i < getCatSizeSize(categorias); i++)
            {
                for (int j = 0; j < 11; j++)
                {
                    for (int k = 0; k < maxtask; k++)
                    {
                        if (tasklist[k].prio == j && strcompare(tasklist[k].cat, categorias[i]))
                        {
                            tasklist_filtered[cont++] = tasklist[k];
                        }
                    }
                }
            }
            break;
        case 6:
            // por prioridade escolhida pelo usuário
            input = intinput("\nDigite a prioridade\n-> ");
            for (int j = 0; j < maxtask; j++)
            {
                if (tasklist[j].prio == input)
                {
                    tasklist_filtered[cont++] = tasklist[j];
                }
            }
            for (int i = 0; i < cont; i++)
            {
                viewtask(tasklist_filtered, str, categorias, &i);
            }
            exportfilter(tasklist_filtered,cont);
            break;
        case 7:
            // por estado escolhido pelo usuário
            input = intinput("Escolha o estado de tarefa:\n1 - não iniciada\n2 - em andamento\n3 - completa\n-> ") - 1;
            for (int j = 0; j < maxtask; j++)
            {
                if (tasklist[j].state == input)
                {
                    tasklist_filtered[cont++] = tasklist[j];
                }
            }
            for (int i = 0; i < cont; i++)
            {
                viewtask(tasklist_filtered, str, categorias, &i);
            }
            exportfilter(tasklist_filtered,cont);
            break;
        case 8:
            // por categoria escolhida pelo usuário
            printf("Selecione uma categoria:\n");
            for (int i = 0; i < getCatSizeSize(categorias); i++)
            {
                printf("%d - %s\n", i + 1, categorias[i]);
            }
            input = intinput("-> ") - 1;
            for (int j = 0; j < maxtask; j++)
            {
                if (strcompare(tasklist[j].cat, categorias[input]))
                {
                    tasklist_filtered[cont++] = tasklist[j];
                }
            }
            for (int i = 0; i < cont; i++)
            {
                viewtask(tasklist_filtered, str, categorias, &i);
            }
            exportfilter(tasklist_filtered,cont);
            break;
        case 9:
            // por categoria e prioridade escolhidas pelo usuário;
            input = intinput("\nDigite a prioridade\n-> ");
            printf("Selecione uma categoria:\n");
            for (int i = 0; i < getCatSizeSize(categorias); i++)
            {
                printf("%d - %s\n", i + 1, categorias[i]);
            }
            int catinput = intinput("-> ") - 1;
            for (int k = 0; k < maxtask; k++)
            {
                if (tasklist[k].prio == input && strcompare(tasklist[k].cat, categorias[catinput]))
                {
                    tasklist_filtered[cont++] = tasklist[k];
                }
            }
            for (int i = 0; i < cont; i++)
            {
                viewtask(tasklist_filtered, str, categorias, &i);
            }
            exportfilter(tasklist_filtered,cont);
            break;
        default:
            printf("\n");
            break;
        }
        // se o usuario escolheu filtrar, mostrar
        if (*has_user_filtered)
        {
            reverseArray(tasklist_filtered, maxtask);
            for (int i = 0; i < maxtask; i++)
            {
                viewtask(tasklist_filtered, str, categorias, &i);
            }
            exportfilter(tasklist_filtered,maxtask);
        }
        free(has_user_filtered);
        free(tasklist_filtered);
    }
    // Se a função for chamada pelo código (acha direto)
    else
    {
        if (*specific == -1)
        {
            for (i = 0; i < maxtask; i++)
            {
                printf("Tarefa %d:\n", i + 1);
                printf("Prioridade: %d\n", tasklist[i].prio);
                printf("Estado: ");
                switch (tasklist[i].state)
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
                printf("Categoria: %s\n", tasklist[i].cat);
                printf("Descrição: %s\n\n", tasklist[i].desc);
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
int edittask(struct tarefa *tasklist, char *str, char categorias[][MAXTAREFAS])
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
    viewtask(tasklist, str, categorias, &i);
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
                        tasklist[i].prio = c;
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
                        tasklist[i].state = c - 1;
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
                        strcpy(tasklist[i].cat, str);
                        int catsize = getCatSizeSize(categorias);
                        doesCatExist(categorias, tasklist[i].cat);
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
                        strcpy(tasklist[i].desc, str);
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
int deletetask(struct tarefa *tasklist, char *str, char categorias[][MAXTAREFAS])
{
    int maxtask = gettasklistsize(tasklist);
    int c;
    int *temp = malloc(sizeof(int));
    *temp = -1;
    while (1)
    {
        viewtask(tasklist, str, categorias, temp);
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
int savedata(char *filename, struct everything *all)
{
    FILE *f = fopen(filename, "wb");
    if (f == NULL)
    {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return 1;
    }
    if (!fwrite(all, sizeof(struct everything), 1, f))
    {
        printf("Erro ao escrever no arquivo.\n");
        fclose(f);
        return 1;
    }
    fclose(f);
    return 0;
}

// Carregar arquivo bin
int loaddata(char *filename, struct everything *all)
{
    FILE *f = fopen(filename, "rb");
    if (f == NULL)
    {
        printf("Erro ao abrir o arquivo para leitura.\n");
        return 1;
    }
    if (!fread(all, sizeof(struct everything), 1, f))
    {
        printf("Erro ao ler o arquivo.\n");
        fclose(f);
        return 1;
    }
    fclose(f);
    return 0;
}
