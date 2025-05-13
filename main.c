#include <stdio.h>
#include "Cartas.h"
#include <stdlib.h>

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

//Função para determinar a maior mao apos a segunda rodada
void determinar_vencedor(tp_jogador jogadores[], int n_jogadores, tp_carta mesa[], int tamanho_mesa, Pote *pote) {
    int melhor_mao = -1;
    int indice_vencedor = -1;

    for(int i = 0; i < n_jogadores; i++) {
        if(!jogadores[i].ativo) continue;
        int forca_mao = classificar_mao(jogadores[i].mao.cartas, mesa, tamanho_mesa);
        printf("%s tem mao com valor %d\n", jogadores[i].nome, forca_mao);

        if(forca_mao > melhor_mao) {
            melhor_mao = forca_mao;
            indice_vencedor = i;
        }
    }

    if(indice_vencedor != -1) {
        jogadores[indice_vencedor].fichas += pote->quantidade;
        printf("\n%s venceu a rodada e ganhou %d fichas!\n", jogadores[indice_vencedor].nome, pote->quantidade);
    }
}


int main(){

    //Inicialização dos jogadores, da mesa e do baralho
    int n_jogadores;
    Pilha baralho;
    tp_mesa mesa;
    
    printf("Ola, seja bem vindo ao TecPoquer. Esperamos que a sua sorte esteja em dia. \n");
    do{
        printf("\nEscolha o numero de jogadores: (2 - 4)\n");
        scanf("%d", &n_jogadores);

        if(n_jogadores < 2 || n_jogadores > 4){ 
            printf("Numero de jogadores invalido. Escolha entre 2 - 4 participantes.\n");
        }
    } while(n_jogadores < 2 || n_jogadores > 4);

    tp_jogador jogadores[n_jogadores];

    printf("\nOtimo. Hora de escolher os nomes dos jogadores! \n");
    for(int i=0; i<n_jogadores; i++){
    printf("Insira o nome do jogador %d: \n", i+1);
    scanf(" %[^\n]s", jogadores[i].nome);
    }
    
    int avancar = 0;
    do{
        printf("\nTudo certo para avancar para o jogo? ( 1- Iniciar    2- Visualizar nome de jogadores    3- Alterar nome de jogador)\n");
        scanf("%d", &avancar);
        if(avancar == 2){
            printf("\nNome dos jogadores: \n");
        for(int i=0; i<n_jogadores; i++){
        printf("Jogador %d: %s\n", i+1, jogadores[i].nome);
        }
        }
        else if(avancar == 3){
            int alterar_nome;
            do{
                printf("Qual numero do jogador deseja alterar o nome? \n");
                scanf("%d", &alterar_nome);
                if(alterar_nome < 1 || alterar_nome > n_jogadores){
                    printf("Numero invalido! Escolha um jogador entre 1 e %d.\n", n_jogadores);
                }
            }while(alterar_nome < 1 || alterar_nome > n_jogadores);
            
            printf("Insira o novo nome do jogador: \n");
            scanf(" %[^\n]s", jogadores[alterar_nome-1].nome);
        }
        else if(avancar != 1 && avancar != 2 && avancar != 3){
            printf("Opcao incorreta. Insira um numero de 1 a 3.");
        }
    }while(avancar != 1);


    //INICIALIZAÇÃO DO JOGO
    
    criarBaralho(&baralho);
    embaralhar(&baralho);
    Pote pote;
    int escolha;

    for(int i = 0; i<n_jogadores; i++){
        inicializa_mao(&jogadores[i].mao, 2);
        jogadores[i].mao.cartas[0] = pop(&baralho);
        jogadores[i].mao.cartas[1] = pop(&baralho);
        jogadores[i].fichas = 1000;
        jogadores[i].ativo = 1;
    }

    inicializa_mesa(&mesa, 5);
    for(int i=0; i<mesa.tamanho; i++){
        mesa.cartas[i] = pop(&baralho);
    }


    printf("\nCartas e fichas inicias distribuidas!\n");

    printf("\nRODADA INICIAL!!!\n");

    int apostaAtual = 0;
    pote.quantidade = 0;

    for(int i=0; i<n_jogadores; i++){
        int turnoJogador = i;
        if(turnoJogador == 0){
            printf("\nTurno de %s: \n", jogadores[i].nome);
            printf("Insira o valor da sua aposta inicial: \n");
            scanf("%d", &jogadores[i].aposta);
            apostaAtual = jogadores[i].aposta;
        }
        else{
            printf("\n\nTurno de %s: \n", jogadores[i].nome);
            printf("Aposta atual: %d", apostaAtual);
            do{
                printf("\nO que deseja fazer? (1-cobrir aposta  2-Aumentar aposta)\n");
                scanf("%d", &escolha);
                if(escolha != 1 && escolha != 2)
                    printf("Opcao invalida. Escolha entre as opcoes 1 e 2. \n");
            }while(escolha != 1 && escolha != 2);

            if(escolha == 1){
                jogadores[i].aposta = apostaAtual;
                printf("%s cobriu a aposta!!\n", jogadores[i].nome);
            }
            else{
                int novaAposta;
                do{
                    printf("Insira o valor da nova aposta:  (aposta atual = %d)\n", apostaAtual);
                    scanf("%d", &novaAposta);
                    if(novaAposta <= apostaAtual)
                        printf("O valor da sua aposta deve ser maior que o da aposta atual. (%d fichas)\n", apostaAtual);
                }while(novaAposta <= apostaAtual);
                jogadores[i].aposta = novaAposta;
                apostaAtual = novaAposta;
                printf("%s aumentou a aposta para %d.\n", jogadores[i].nome, novaAposta);
            }
        }
        jogadores[i].fichas -= jogadores[i].aposta;
        pote.quantidade += jogadores[i].aposta;
    }
    printf("Rodada inicial encerrada. \n");



    //SEGUNDA RODADA (JOGADORES VISUALIZAM A MAO E AS CARTAS NA MESA E PODEM COMEÇAR A FAZER COMBINAÇÕES)
    int turno = 0;
    int jogaramNaFase = 0;

    while(turno <= 2){
        for(int i=0; i<n_jogadores; i++){
            if(!jogadores[i].ativo) continue;
            printf("\nCartas reveladas: \n");
            for(int j=0; j<3+turno; j++){
                printf("Carta %d-> %s de %s\n", j+1, nomes_valores[mesa.cartas[j].valor], nomes_naipes[mesa.cartas[j].naipe]);
            }

            int opcao;
            int novaAposta;

            printf("\nTurno de %s: \n", jogadores[i].nome);
            printf("Sua mao: %s de %s  e  %s de %s\n", nomes_valores[jogadores[i].mao.cartas[0].valor], nomes_naipes[jogadores[i].mao.cartas[0].naipe], nomes_valores[jogadores[i].mao.cartas[1].valor], nomes_naipes[jogadores[i].mao.cartas[1].naipe]);
            do{
                printf("\nSelecione uma opcao: (1-> cobrir aposta (%d fichas) // 2-> Aumentar aposta // 3-> Desistir)\n", apostaAtual);
                scanf("%d", &opcao);
                if(opcao!=1 && opcao!=2 && opcao!=3)
                    printf("Opcao invalida.");
                }while(opcao!=1 && opcao!=2 && opcao!=3);

            switch(opcao){
                case 1:
                jogadores[i].aposta = apostaAtual;
                jogadores[i].fichas -= jogadores[i].aposta;
                pote.quantidade += jogadores[i].aposta;
                printf("\n%s cobriu a aposta!!\n", jogadores[i].nome);
                break;

                case 2:
                do{
                printf("\nInsira o valor da sua aposta: (aposta atual = %d fichas)\n", apostaAtual);
                scanf("%d", &novaAposta);
                if(novaAposta <= apostaAtual)
                    printf("A nova aposta deve ser maior do que a atual (%d fichas) ", apostaAtual);
                }while(novaAposta <= apostaAtual);
                jogadores[i].aposta = novaAposta;
                jogadores[i].fichas -= jogadores[i].aposta;
                pote.quantidade += jogadores[i].aposta;
                apostaAtual = novaAposta;
                printf("%s aumentou a aposta para %d!\n", jogadores[i].nome, novaAposta);
                break;

                case 3:
                jogadores[i].ativo = 0;
                printf("%s desistiu da rodada!\n", jogadores[i].nome);
                continue;
            }

            jogaramNaFase++;
        }

        int ativos = 0;
        for(int i=0; i<n_jogadores; i++){
            if(jogadores[i].ativo) ativos++;
        }

        if(jogaramNaFase == ativos){
            turno ++;
            jogaramNaFase = 0;

            if(turno == 1){
                printf("\nQuarta carta na mesa revelada: %s de %s\n", nomes_valores[mesa.cartas[3].valor], nomes_naipes[mesa.cartas[3].naipe]);
            } else if(turno == 2){
                printf("\nQuinta carta na mesa revelada: %s de %s\n", nomes_valores[mesa.cartas[4].valor], nomes_naipes[mesa.cartas[4].naipe]);
            }
        }
    }

    printf("\nRodada finalizada.\n");
    determinar_vencedor(jogadores, n_jogadores, mesa.cartas, mesa.tamanho, &pote);
}