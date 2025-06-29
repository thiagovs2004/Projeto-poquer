#ifndef FLUXO_DE_JOGO_H
#define FLUXO_DE_JOGO_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cartas.h"

//Estrutura da mão do jogador
typedef struct{
    tp_carta *cartas;
    int tamanho;
} tp_mao;

//Estrutura dos jogadores
typedef struct{
    char nome[MAX];
    int fichas;
    tp_mao mao;
    int aposta;
    int ativo;
}tp_jogador;

//Estrutura da mesa do jogo
typedef struct{
    tp_carta *cartas;
    int tamanho;
}tp_mesa;

//Estrutura do pote de apostas
typedef struct{
    int quantidade;
}Pote;

//Estrutura para armazenar estatisticasde combinações
typedef struct {
    int quadra;
    int full_house;
    int flush;
    int straight;
    int trinca;
    int dois_pares;
    int par;
    int carta_alta;
} EstatisticasCombinacoes;
EstatisticasCombinacoes estatisticas = {0};

//Função para inicializar a mão dos jogadores usando alocação dinâmica
void inicializa_mao(tp_mao *mao, int tamanho){
    mao->cartas = (tp_carta *)malloc(sizeof(tp_carta) * tamanho);
    mao->tamanho = tamanho;
}

//Função para inicializar a mesa de cartas usando alocação dinâmica
void inicializa_mesa(tp_mesa *mesa, int tamanho){
    mesa->cartas = (tp_carta *)malloc(sizeof(tp_carta) * tamanho);
    mesa->tamanho = tamanho;
}

//Função para determinar o(s) vencedor(es) da rodada com base na combinação das cartas e entregar o pote ao vencedor ou dividi-lo aos vencedores em caso de empate 
void determinar_vencedor(tp_jogador jogadores[], int n_jogadores, tp_mesa mesa, Pote* pote) {
    int melhor_mao = -1;
    int codigo_mao[4] = {0}; 

    printf("\n=== Combinacoes dos jogadores ===\n");
    for (int i = 0; i < n_jogadores; i++) {
        if (!jogadores[i].ativo) continue;

        codigo_mao[i] = avaliar_mao(jogadores[i], mesa);

        printf("%s: %s\n", jogadores[i].nome, nomes_combinacoes(codigo_mao[i]));

        if (codigo_mao[i] > melhor_mao) {
            melhor_mao = codigo_mao[i];
        }
    }

    int vencedores[4];
    int qtd_vencedores = 0;
    for (int i = 0; i < n_jogadores; i++)
        if (codigo_mao[i] == melhor_mao) vencedores[qtd_vencedores++] = i;

    if (qtd_vencedores == 0) {
        puts("\nNenhum jogador ativo na rodada.");
        return;
    }

    int quota = pote->quantidade / qtd_vencedores;
    int sobra = pote->quantidade % qtd_vencedores;   /* fichas que não dividem */

    for (int k = 0; k < qtd_vencedores; k++)
        jogadores[vencedores[k]].fichas += quota;

    if (sobra)
        jogadores[vencedores[0]].fichas += sobra;

    if (qtd_vencedores == 1) {
        printf("\n%s vence a rodada com %s e ganha %d fichas!\n",
               jogadores[vencedores[0]].nome,
               nomes_combinacoes(melhor_mao),
               pote->quantidade);
    } else {
        printf("\nEmpate! %d jogadores dividem o pote de %d fichas:\n",
               qtd_vencedores, pote->quantidade);
        for (int k = 0; k < qtd_vencedores; k++)
            printf(" %s (%s) recebe %d\n", jogadores[vencedores[k]].nome, nomes_combinacoes(melhor_mao), quota + (k==0 ? sobra : 0));
    }

    pote->quantidade = 0;
}

//------------------------------- A R M A Z E N A M E N T O    D E    J O G A D A S------------------------------------------------

// Função para salvar as combinações de uma mão em arquivo
void salvar_combinacoes_arquivo(const char* nome_arquivo, tp_jogador jogadores[], int n_jogadores, tp_mesa mesa) {
    FILE *arquivo = fopen(nome_arquivo, "a");
    if (!arquivo) {
        perror("Erro ao abrir arquivo de historico");
        return;
    }

    fprintf(arquivo, "\n=== Nova Mao ===\n");
    for (int i = 0; i < n_jogadores; i++) {
        if (jogadores[i].ativo) {
            int combinacao = avaliar_mao(jogadores[i], mesa);
            fprintf(arquivo, "%s: %s\n", jogadores[i].nome, nomes_combinacoes(combinacao));
            
            // Atualiza estatísticas em memória
            switch(combinacao) {
                case 7: estatisticas.quadra++; break;
                case 6: estatisticas.full_house++; break;
                case 5: estatisticas.flush++; break;
                case 4: estatisticas.straight++; break;
                case 3: estatisticas.trinca++; break;
                case 2: estatisticas.dois_pares++; break;
                case 1: estatisticas.par++; break;
                default: estatisticas.carta_alta++; break;
            }
        }
    }
    fclose(arquivo);
}

// Função para mostrar estatísticas ao final do jogo
void mostrar_estatisticas_finais() {
    printf("\n=== Estatisticas de Combinacoes ===\n");
    printf("Quadras: %d\n", estatisticas.quadra);
    printf("Full Houses: %d\n", estatisticas.full_house);
    printf("Flushes: %d\n", estatisticas.flush);
    printf("Straights: %d\n", estatisticas.straight);
    printf("Trincas: %d\n", estatisticas.trinca);
    printf("Dois Pares: %d\n", estatisticas.dois_pares);
    printf("Pares: %d\n", estatisticas.par);
    printf("Cartas Altas: %d\n", estatisticas.carta_alta);
}

// Função para carregar histórico do arquivo
void carregar_historico(const char* nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        
        return;
    }

    printf("\n=== Historico de Jogadas ===\n");
    char linha[256];
    while (fgets(linha, sizeof(linha), arquivo)) {
        printf("%s", linha);
    }
    fclose(arquivo); 
}

#endif 