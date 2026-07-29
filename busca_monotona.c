#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_TAREFAS 100
#define MAX_MAQUINAS 50
#define QTD_INSTANCIAS 6

int calcularMakespan(int solucao[], int tarefas[], int n, int m) {
    int cargas[MAX_MAQUINAS] = {0};
    int i;
    int maior = 0;

    for (i = 0; i < n; i++) {
        cargas[solucao[i]] += tarefas[i];
    }

    for (i = 0; i < m; i++) {
        if (cargas[i] > maior) {
            maior = cargas[i];
        }
    }

    return maior;
}


void criarSolucaoInicial(int solucao[], int n, int m) {
    int i;

    for (i = 0; i < n; i++) {
        solucao[i] = rand() % m;
    }
}


void copiarSolucao(int origem[], int destino[], int n) {
    int i;

    for (i = 0; i < n; i++) {
        destino[i] = origem[i];
    }
}


void gerarVizinhoAleatorio(int atual[], int vizinho[], int n, int m) {
    int tarefa;
    int novaMaquina;

    copiarSolucao(atual, vizinho, n);

    tarefa = rand() % n;
    novaMaquina = rand() % m;

    while (novaMaquina == atual[tarefa]) {
        novaMaquina = rand() % m;
    }

    vizinho[tarefa] = novaMaquina;
}


/*
   Procura um vizinho melhor de forma simples.
   Sao feitas algumas tentativas aleatorias.
   Se encontrar uma melhora, ela e devolvida.
*/
int gerarVizinhoMelhor(int atual[], int vizinho[], int tarefas[],
                       int n, int m, int valorAtual) {
    int tentativa;
    int candidato[MAX_TAREFAS];
    int valorCandidato;

    for (tentativa = 0; tentativa < n * 2; tentativa++) {
        gerarVizinhoAleatorio(atual, candidato, n, m);
        valorCandidato = calcularMakespan(candidato, tarefas, n, m);

        if (valorCandidato < valorAtual) {
            copiarSolucao(candidato, vizinho, n);
            return 1;
        }
    }

    copiarSolucao(atual, vizinho, n);
    return 0;
}


int buscaMonotonaRandomizada(int tarefas[], int n, int m,
                             double alfa, int *iteracoes) {
    int atual[MAX_TAREFAS];
    int vizinho[MAX_TAREFAS];
    int melhor[MAX_TAREFAS];

    int valorAtual;
    int valorVizinho;
    int melhorValor;
    int semMelhora = 0;
    double sorteio;

    criarSolucaoInicial(atual, n, m);

    valorAtual = calcularMakespan(atual, tarefas, n, m);
    melhorValor = valorAtual;
    copiarSolucao(atual, melhor, n);

    *iteracoes = 0;

    while (semMelhora < 1000) {
        (*iteracoes)++;

        sorteio = (double) rand() / RAND_MAX;

        /*
           Com probabilidade alfa faz uma caminhada aleatoria.
           Caso contrario, tenta encontrar um vizinho melhor.
        */
        if (sorteio < alfa) {
            gerarVizinhoAleatorio(atual, vizinho, n, m);
            valorVizinho = calcularMakespan(vizinho, tarefas, n, m);

            copiarSolucao(vizinho, atual, n);
            valorAtual = valorVizinho;
        }
        else {
            if (gerarVizinhoMelhor(atual, vizinho, tarefas,
                                   n, m, valorAtual)) {
                valorVizinho = calcularMakespan(vizinho, tarefas, n, m);
                copiarSolucao(vizinho, atual, n);
                valorAtual = valorVizinho;
            }
        }

        if (valorAtual < melhorValor) {
            melhorValor = valorAtual;
            copiarSolucao(atual, melhor, n);
            semMelhora = 0;
        }
        else {
            semMelhora++;
        }
    }

    return melhorValor;
}


void gerarInstancias(int tarefas[][MAX_TAREFAS], int ns[], int ms[]) {
    int valoresM[3] = {10, 20, 50};
    double valoresR[2] = {1.5, 2.0};

    int instancia = 0;
    int i, j, k;

    /*
       Semente fixa somente para que os dois programas gerem
       exatamente as mesmas tarefas.
    */
    srand(1234);

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 2; j++) {
            ms[instancia] = valoresM[i];
            ns[instancia] = (int) (valoresM[i] * valoresR[j]);

            for (k = 0; k < ns[instancia]; k++) {
                tarefas[instancia][k] = 1 + rand() % 100;
            }

            instancia++;
        }
    }
}


int arquivoExiste(char nome[]) {
    FILE *arquivo = fopen(nome, "r");

    if (arquivo != NULL) {
        fclose(arquivo);
        return 1;
    }

    return 0;
}


int main() {
    int tarefas[QTD_INSTANCIAS][MAX_TAREFAS];
    int ns[QTD_INSTANCIAS];
    int ms[QTD_INSTANCIAS];

    double alfas[9] = {0.1, 0.2, 0.3, 0.4, 0.5,
                       0.6, 0.7, 0.8, 0.9};

    int i, j, repeticao;
    int valor;
    int iteracoes;

    clock_t inicio, fim;
    double tempo;

    FILE *arquivo;
    int jaExiste;

    gerarInstancias(tarefas, ns, ms);

    /*
       Depois de gerar as instancias, usa outra semente
       para as escolhas aleatorias da busca.
    */
    srand((unsigned) time(NULL));

    jaExiste = arquivoExiste("resultados.txt");

    arquivo = fopen("resultados.txt", "a");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de resultados.\n");
        return 1;
    }

    if (!jaExiste) {
        fprintf(arquivo,
                "heuristica,n,m,replicacao,tempo,iteracoes,valor,parametro\n");
    }

    for (i = 0; i < QTD_INSTANCIAS; i++) {
        for (j = 0; j < 9; j++) {
            for (repeticao = 1; repeticao <= 10; repeticao++) {

                inicio = clock();

                valor = buscaMonotonaRandomizada(
                    tarefas[i],
                    ns[i],
                    ms[i],
                    alfas[j],
                    &iteracoes
                );

                fim = clock();

                tempo = (double) (fim - inicio) / CLOCKS_PER_SEC;

                fprintf(
                    arquivo,
                    "monotona,%d,%d,%d,%.6f,%d,%d,%.2f\n",
                    ns[i],
                    ms[i],
                    repeticao,
                    tempo,
                    iteracoes,
                    valor,
                    alfas[j]
                );

                printf(
                    "Monotona | n=%d m=%d rep=%d alfa=%.1f valor=%d\n",
                    ns[i],
                    ms[i],
                    repeticao,
                    alfas[j],
                    valor
                );
            }
        }
    }

    fclose(arquivo);

    printf("\nBusca monótona finalizada.\n");
    return 0;
}
