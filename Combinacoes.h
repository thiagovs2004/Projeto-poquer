#ifndef COMBINACOES_H
#define COMBINACOES_H


int avaliar_mao(tp_jogador jogador, tp_mesa mesa) {
    tp_carta combinadas[7];
    int cont_valores[15] = {0};
    int cont_naipes[4] = {0};

    for (int i = 0; i < 2; i++) combinadas[i] = jogador.mao.cartas[i];
    for (int i = 0; i < 5; i++) combinadas[i + 2] = mesa.cartas[i];

    for (int i = 0; i < 7; i++) {
        cont_valores[combinadas[i].valor]++;
        cont_naipes[combinadas[i].naipe]++;
    }

    int tem_flush = 0;
    for (int i = 0; i < 4; i++) {
        if (cont_naipes[i] >= 5) {
            tem_flush = 1;
            break;
        }
    }

    int valores[15] = {0};
    for (int i = 0; i < 7; i++) valores[combinadas[i].valor] = 1;
    valores[1] = valores[14]; 

    int sequencia = 0, tem_straight = 0;
    for (int i = 1; i <= 14; i++) {
        if (valores[i]) {
            sequencia++;
            if (sequencia >= 5) {
                tem_straight = 1;
                break;
            }
        } else {
            sequencia = 0; 
        }
    }

    int pares = 0, trinca = 0, quadra = 0;
    for (int i = 2; i <= 14; i++) {
        if (cont_valores[i] == 2) pares++;
        if (cont_valores[i] == 3) trinca++;
        if (cont_valores[i] == 4) quadra = 1;
    }

    if (quadra) return 7;
    if (trinca && pares >= 1) return 6;
    if (tem_flush) return 5;
    if (tem_straight) return 4;
    if (trinca) return 3;
    if (pares >= 2) return 2;
    if (pares == 1) return 1;
    return 0;
}


#endif 