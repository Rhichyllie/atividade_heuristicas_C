# Atividade de Busca Heurística

![Linguagem](https://img.shields.io/badge/Linguagem-C-blue)
![Universidade](https://img.shields.io/badge/Universidade-UNIVALI-red)
![Disciplina](https://img.shields.io/badge/Disciplina-Teoria%20da%20Computa%C3%A7%C3%A3o-lightgrey)
![Status](https://img.shields.io/badge/Status-Conclu%C3%ADdo-brightgreen)

Atividade desenvolvida na disciplina de **Teoria da Computação** da **Universidade do Vale do Itajaí - UNIVALI**.

O trabalho utiliza algoritmos de busca heurística para distribuir tarefas entre máquinas, buscando minimizar o **makespan**, que representa o maior tempo de processamento entre as máquinas.

## Heurísticas

- Busca Local Monótona Randomizada
- Têmpera Simulada

## Como executar

### Busca Local Monótona Randomizada

Compile:

```bash
gcc busca_monotona.c -o busca_monotona
```

Execute no Windows:

```bash
busca_monotona.exe
```

### Têmpera Simulada

Compile:

```bash
gcc tempera_simulada.c -o tempera_simulada -lm
```

Execute no Windows:

```bash
tempera_simulada.exe
```

Os resultados das execuções são salvos no arquivo `resultados.txt`.

## Professor

**Alex Rese**

## Alunos

- Rhichyllie Stefen
- Gabriel Silveira
- Lucas Borba