#ifndef CARTAS_H
#define CARTAS_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX 52

//Representação dos valores das cartas
typedef enum {
    DOIS = 2, TRES = 3, QUATRO = 4, CINCO =  5, SEIS = 6, SETE = 7, 
    OITO = 8, NOVE = 9, DEZ = 10, VALETE = 11, DAMA = 12, REI = 13, AS = 14
} tp_valor;

//Representação dos naipes das cartas
typedef enum{
    ESPADAS, PAUS, COPAS, OUROS
} tp_naipe;

const char* nomes_valores[] = {
    "", "", "2", "3", "4", "5", "6", "7", "8", "9", "10",
    "Valete", "Dama", "Rei", "As"
};

const char* nomes_naipes[] = {
    "Espadas", "Paus", "Copas", "Ouros"
};


//Struct da carta
typedef struct{
    tp_valor valor;
    tp_naipe naipe;
} tp_carta;

//Estrutura da pilha de cartas do baralho
typedef struct{
    tp_carta cartas[MAX];
    int topo;
} Pilha;

//------------------------------------------------------------
//Funções da Pilha de cartas 
void inicializaPilha(Pilha *p){
    p->topo = -1;
}

int pilhaVazia(Pilha *p){
    return p->topo == -1;
}

int pilhaCheia(Pilha *p){
    return p->topo == MAX-1;
}

void push(Pilha *p, tp_carta carta){
    if(pilhaCheia(p)){
        printf("Baralho cheio!\n");
        return;
    }
    p->cartas[++(p->topo)] = carta;
}

tp_carta pop(Pilha *p){
    if (pilhaVazia(p)){
        printf("\nBaralho vazio!\n");
        tp_carta cartaVazia = {0, 0};
        return cartaVazia;
    }
    return p->cartas[(p->topo)--];
}

tp_carta topo(Pilha *p){
    if(pilhaVazia(p)){
        printf("\nBaralho vazio!\n");
        tp_carta cartaVazia = {0, 0};
        return cartaVazia;
    }    
    return p->cartas[p->topo];
}
//--------------------------------------------------------


//Função para gerar o baralho de cartas
void criarBaralho(Pilha *p){
    inicializaPilha(p);
    for(int naipe = ESPADAS; naipe <= OUROS; naipe++){
        for(int valor = DOIS; valor <= AS; valor ++){
        tp_carta novaCarta;
        novaCarta.valor = valor;
        novaCarta.naipe = naipe;
        push(p, novaCarta);
        }
    }
}


//Função para embaralhar o baralho
void embaralhar(Pilha *p){
    if (pilhaVazia(p) || p->topo == 0) return;

    tp_carta cartasTemp[MAX];
    int total = p->topo + 1;

    // Copia as cartas da pilha para um vetor
    for (int i = 0; i < total; i++) {
        cartasTemp[i] = pop(p);
    }

    // Embaralha o vetor com Fisher-Yates
    srand(time(NULL));
    for (int i = total - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        tp_carta temp = cartasTemp[i];
        cartasTemp[i] = cartasTemp[j];
        cartasTemp[j] = temp;
    }

    // Coloca as cartas de volta na pilha
    for (int i = 0; i < total; i++) {
        push(p, cartasTemp[i]);
    }
}

//---------------------------------------------------------------------

//ANALISE DE COMBINAÇÕES JOGADOR/MESA

// Função para contar frequências dos valores
void contar_valores(tp_carta cartas[], int total, int freq[]) {
    for(int i = 0; i < total; i++) {
        freq[cartas[i].valor]++;
    }
}

// Função para contar naipes (flush)
void contar_naipes(tp_carta cartas[], int total, int freq[]) {
    for(int i = 0; i < total; i++) {
        freq[cartas[i].naipe]++;
    }
}

// Verifica straight (sequência)
int tem_straight(int freq[]) {
    for(int i = DOIS; i <= DEZ; i++) {
        int cont = 0;
        for(int j = 0; j < 5; j++) {
            if(freq[i + j] > 0) cont++;
        }
        if(cont == 5) return 1;
    }
    // Verifica A-2-3-4-5
    if(freq[AS] && freq[DOIS] && freq[TRES] && freq[QUATRO] && freq[CINCO]) return 1;
    return 0;
}

int classificar_mao(tp_carta mao[], tp_carta mesa[], int tamanho_mesa) {
    tp_carta total[7];
    for(int i = 0; i < 2; i++) total[i] = mao[i];
    for(int i = 0; i < tamanho_mesa; i++) total[i + 2] = mesa[i];

    int freq_valor[15] = {0};
    int freq_naipe[4] = {0};

    contar_valores(total, 2 + tamanho_mesa, freq_valor);
    contar_naipes(total, 2 + tamanho_mesa, freq_naipe);

    int pares = 0, trincas = 0;

    for(int i = DOIS; i <= AS; i++) {
        if(freq_valor[i] == 4) return 7; // Quadra (não solicitada mas pode ser adicionada)
        if(freq_valor[i] == 3) trincas++;
        if(freq_valor[i] == 2) pares++;
    }

    for(int i = 0; i < 4; i++) {
        if(freq_naipe[i] >= 5) return 6; // Flush
    }

    if(tem_straight(freq_valor)) return 5; // Straight
    if(trincas >= 1) return 3; // Trinca
    if(pares == 2) return 2; // Dois pares
    if(pares == 1) return 1; // Um par

    return 0; // Carta alta
}


#endif