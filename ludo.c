#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>

struct jogador {
    char nome[20], cor[20];
    int peoes[4];
};


int jogarDado();


int main(){
    srand(time(NULL));

    int dado;

    dado = jogarDado();

    

    return 0;
}

int jogarDado(){

    // função responsavel pela parte de jogar o dado;
    int numero;

    numero = rand()%6;

    return numero;
}
