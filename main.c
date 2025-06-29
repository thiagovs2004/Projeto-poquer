#include <stdio.h>
#include <stdlib.h>
#include "Cartas.h"
#include "FluxoDeJogo.h"
#include "Combinacoes.h"


int main(){

    //Inicialização dos jogadores, da mesa e do baralho
    int n_jogadores;
    Pilha baralho;
    tp_mesa mesa;

    //Variaveis para registrar as combinações ao longo do jogo
    int historico[8] = {0};
    static int numero_mao = 1;

  carregar_historico("historico.txt");


    //INICIO DO JOGO
    printf("Ola, seja bem vindo ao TecPoquer. Esperamos que a sua sorte esteja em dia. \n");
    do{
        printf("\nEscolha o numero de jogadores: (2 - 4)\n");
        scanf("%d", &n_jogadores);

        if(n_jogadores < 2 || n_jogadores > 4){ 
            printf("Numero de jogadores invalido. Escolha entre 2 - 4 participantes.\n");
        }
    } while(n_jogadores < 2 || n_jogadores > 4);

    tp_jogador jogadores[n_jogadores];
    int codigos[n_jogadores];

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

    for(int i=0; i<n_jogadores;i++){
        inicializa_mao(&jogadores[i].mao,2);
        jogadores[i].fichas = 1000;
        jogadores[i].ativo = 1;
    }
    inicializa_mesa(&mesa, 5);

    while (1) {

    int ativos = 0;
    int indice_vencedor = -1;

    for (int i = 0; i < n_jogadores; i++){
        if (jogadores[i].ativo && jogadores[i].fichas > 0){
            ativos++;
            indice_vencedor = i;
        }
    }

    if (ativos == 1) {
        printf("\n***** %s vence a partida com %d fichas! *****\n", jogadores[indice_vencedor].nome, jogadores[indice_vencedor].fichas);
        break;                                 
    }
    else if(ativos == 0){
        printf("\n***** Todos os jogadores ficaram sem fichas! *****\n");
        break;
    }
  
    criarBaralho(&baralho);
    embaralhar(&baralho);
    Pote pote;
    int escolha;

    
    int apostaAtual = 0;
    pote.quantidade = 0;

    for (int i = 0; i < n_jogadores; i++) {
        jogadores[i].ativo  = (jogadores[i].fichas > 0);
        jogadores[i].aposta = 0;
        if (!jogadores[i].ativo) continue;

        jogadores[i].mao.cartas[0] = pop(&baralho);
        jogadores[i].mao.cartas[1] = pop(&baralho);
    }

    for (int c = 0; c < 5; c++)
        mesa.cartas[c] = pop(&baralho);
    
    
    printf("\nCartas e fichas inicias distribuidas!\n");

    printf("\n========== TURNO: PRE-FLOP (primeiro turno) ==========\n");


    for(int i=0; i<n_jogadores; i++){
        int turnoJogador = i;

        if(!jogadores[i].ativo) continue;

        if(turnoJogador == 0){
            printf("\nTurno de %s: \n", jogadores[i].nome);
            printf("Sua mao: %s de %s e %s de %s\n", nomes_valores[jogadores[i].mao.cartas[0].valor], nomes_naipes[jogadores[i].mao.cartas[0].naipe], nomes_valores[jogadores[i].mao.cartas[1].valor], nomes_naipes[jogadores[i].mao.cartas[1].naipe]);
            printf("Insira o valor da sua aposta inicial: \n");
            scanf("%d", &jogadores[i].aposta);

            if (jogadores[i].fichas < jogadores[i].aposta) {
                puts("Fichas insuficientes. O jogador desistiu.");
                jogadores[i].ativo = 0;
                continue;
            }

            apostaAtual = jogadores[i].aposta;
        }
        else{
            printf("\n\nTurno de %s: \n", jogadores[i].nome);
            printf("Sua mao: %s de %s e %s de %s\n", nomes_valores[jogadores[i].mao.cartas[0].valor], nomes_naipes[jogadores[i].mao.cartas[0].naipe], nomes_valores[jogadores[i].mao.cartas[1].valor], nomes_naipes[jogadores[i].mao.cartas[1].naipe]);
            printf("Aposta atual: %d", apostaAtual);
            do{
                printf("\nO que deseja fazer? (1-Cobrir aposta | 2-Aumentar aposta | 3-Desistir)\n");
                scanf("%d", &escolha);
                if(escolha != 1 && escolha != 2 && escolha != 3)
                    printf("Opcao invalida. Escolha entre as opcoes 1 e 2. \n");
            }while(escolha != 1 && escolha != 2 && escolha != 3);

            if(escolha == 1){
                jogadores[i].aposta = apostaAtual;
                if (jogadores[i].fichas < jogadores[i].aposta) {
                puts("Fichas insuficientes. O jogador desistiu.");
                jogadores[i].ativo = 0;
                continue;
            }
                printf("%s cobriu a aposta!!\n", jogadores[i].nome);
            }
            else if(escolha == 2){
                int novaAposta;
                do{
                    printf("Insira o valor da nova aposta:  (aposta atual = %d)\n", apostaAtual);
                    scanf("%d", &novaAposta);
                    if(novaAposta <= apostaAtual)
                        printf("O valor da sua aposta deve ser maior que o da aposta atual. (%d fichas)\n", apostaAtual);
                }while(novaAposta <= apostaAtual);
                jogadores[i].aposta = novaAposta;
                if (jogadores[i].fichas < jogadores[i].aposta) {
                    puts("Fichas insuficientes. O jogador desistiu.");
                    jogadores[i].ativo = 0;
                    continue;
                }
                apostaAtual = novaAposta;
                printf("%s aumentou a aposta para %d.\n", jogadores[i].nome, novaAposta);
            }
            else{
                printf("%s desistiu da rodada!\n", jogadores[i].nome);
                jogadores[i].ativo = 0;
                continue;
            }
        }
        jogadores[i].fichas -= jogadores[i].aposta;
        pote.quantidade += jogadores[i].aposta;
    }
    printf("Rodada inicial encerrada. \n");

    //JOGADORES VISUALIZAM A MAO E AS CARTAS NA MESA E PODEM COMEÇAR A FAZER COMBINAÇÕES
    int turno = 0;

    while(turno <= 2){

        int ativos = 0;
        for(int k=0;k<n_jogadores;k++){
            if(jogadores[k].ativo && jogadores[k].fichas > 0){
                ativos++;
            }
        }
        if(ativos <= 1) break;

        if(turno == 0)
            printf("\n========== TURNO: FLOP (segundo turno) ==========\n");
        else if(turno == 1)
            printf("\n========== TURNO: TURN (terceiro turno) ==========\n");
        else if(turno == 2)
            printf("\n========== TURNO: RIVER (quarto turno) ==========\n");    

            int jogaramNaFase = 0;
            for(int i=0;i<n_jogadores;i++){
                if(!jogadores[i].ativo || jogadores[i].fichas <= 0) continue;
                
                printf("\nCartas reveladas: \n");
                for(int j=0; j<3+turno; j++){
                    printf("Carta %d-> %s de %s\n", j+1, nomes_valores[mesa.cartas[j].valor], nomes_naipes[mesa.cartas[j].naipe]);
                }

                printf("\nTurno de %s: \n", jogadores[i].nome);
                printf("Sua mao: %s de %s  e  %s de %s\n", nomes_valores[jogadores[i].mao.cartas[0].valor], nomes_naipes[jogadores[i].mao.cartas[0].naipe], nomes_valores[jogadores[i].mao.cartas[1].valor], nomes_naipes[jogadores[i].mao.cartas[1].naipe]);
                int opcao, novaAposta;
                do{
                    printf("\nSelecione uma opcao: (1-> cobrir aposta (%d fichas) // 2-> Aumentar aposta // 3-> Desistir)\n", apostaAtual);
                    scanf("%d", &opcao);
                    if(opcao!=1 && opcao!=2 && opcao!=3)
                        printf("Opcao invalida.");
                    }while(opcao!=1 && opcao!=2 && opcao!=3);

                switch(opcao){
                    case 1:
                    jogadores[i].aposta = apostaAtual;
                    if (jogadores[i].fichas < jogadores[i].aposta) {
                    puts("Fichas insuficientes. O jogador desistiu.");
                    jogadores[i].ativo = 0;
                    continue;
                }
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

                    if (jogadores[i].fichas < jogadores[i].aposta) {
                        puts("Fichas insuficientes. O jogador desistiu.");
                        jogadores[i].ativo = 0;
                        continue;
                    }

                    jogadores[i].fichas -= jogadores[i].aposta;
                    pote.quantidade += jogadores[i].aposta;
                    apostaAtual = novaAposta;
                    printf("%s aumentou a aposta para %d!\n", jogadores[i].nome, novaAposta);
                    break;

                    case 3:
                    jogadores[i].ativo = 0;
                    ativos--;
                    printf("%s desistiu da rodada!\n", jogadores[i].nome);
                    continue;
                }

                jogaramNaFase++;
            }


        if(jogaramNaFase >= ativos){
            turno ++;
           

            if(turno == 1){
                printf("\nQuarta carta na mesa revelada: %s de %s\n", nomes_valores[mesa.cartas[3].valor], nomes_naipes[mesa.cartas[3].naipe]);
            } else if(turno == 2){
                printf("\nQuinta carta na mesa revelada: %s de %s\n", nomes_valores[mesa.cartas[4].valor], nomes_naipes[mesa.cartas[4].naipe]);
            }
        }
    }

    printf("\nRodada finalizada.\n");
   
    for (int i = 0; i < n_jogadores; i++) {
    if (jogadores[i].ativo)               
        codigos[i] = avaliar_mao(jogadores[i], mesa);
    else
        codigos[i] = -1;                 
}

    determinar_vencedor(jogadores, n_jogadores, mesa, &pote);

    salvar_combinacoes_arquivo("historico.txt", jogadores, n_jogadores, mesa);
    numero_mao++;

    for (int i = 0; i < n_jogadores; i++)
        if (jogadores[i].fichas <= 0)
            jogadores[i].ativo = 0;

    puts("\n--- Situacao de fichas ---");
    for (int i = 0; i < n_jogadores; i++)
        printf("%s : %d\n", jogadores[i].nome, jogadores[i].fichas);

}

mostrar_estatisticas_finais();

for (int i = 0; i < n_jogadores; i++)
    free(jogadores[i].mao.cartas);
free(mesa.cartas);


return 0;
} 