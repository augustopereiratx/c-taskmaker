# TaskMaker
Projeto de 2º semestre no curso de ciência da computação da FEI. Feito em C.
## Mudanças desde o projeto inicial

Para o professor: eu esqueci de fazer um branch para o projeto 3, então o branch `toques_finais` é pra não perder nota.

Mudanças:

1 - Alterar tarefas: o usuário deve escolher uma tarefa e qual campo deseja alterar. O seu programa deve sobrescrever o dado que foi informado pelo usuário;
2 - Filtrar tarefas por prioridade: seu programa deve listar na tela as tarefas que possuem uma prioridade escolhida pelo usuário;
3 - Filtrar tarefas por estado: da mesma forma que o anterior, mas apenas o estado que o usuário selecionar;
4 - Filtrar tarefas por categoria: idem, mas com o filtro por categoria e com a prioridade ordenada da maior para a menor;
5 - Filtrar tarefas por prioridade e categoria: junção das duas funções anteriores em que o usuário escolhe a categoria e a prioridade e o seu programa deve exibir as tarefas que passarem pelos dois filtros;
6 - Exportar tarefas por prioridade: seu programa deve gerar um arquivo texto listando apenas as tarefas da prioridade escolhida pelo usuário. Cada tarefa deve estar em uma linha do arquivo na sequência: prioridade, categoria, estado e descrição;
7 - Exportar tarefas por categoria: idem ao anterior, inclusive no formato do arquivo exportado, sendo que as tarefas devem estar ordenadas por prioridade;
8 - Exportar tarefas por prioridade e categoria: junção dos filtros anteriores, mantendo o formato do arquivo que deve ser exportado;

Entre outros que eu adicionei porque quis (ou por acidente).

## Compilação

Para compilar, use o [GCC](https://gcc.gnu.org/).

GCC no Arch Linux e seus derivados (provavelmente já está instalado na sua distro):
```bash
  sudo pacman -S gcc
```
## Imagens

![Meu desktop com um terminal rodando o programa e o outro rodando o nvim com código do programa](assets/desktop.png)

