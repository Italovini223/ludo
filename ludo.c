#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>

struct jogador {
    char nome[20];
    int pecas[4], cor;
};


int jogarDado();
void preencheDadosDosJogadores( struct jogador *jogadores, int quantidade);
bool verificaCor(int cor, struct jogador jogador[], int quantidadeDeJogadores);



int main(){
    srand(time(NULL));
    int tabuleiro[52], dado, quantidadeDeJogadores;

    do {
        printf("Digite a quantidade de jogadorees (2 ou 4): ");
        scanf("%d",&quantidadeDeJogadores);

        if(quantidadeDeJogadores != 2 && quantidadeDeJogadores != 4){
            printf("Sao aceitos somente 2 ou 4 jogadores!\n");
        }
    }while(quantidadeDeJogadores != 2 && quantidadeDeJogadores != 4);

    struct jogador jogadores[quantidadeDeJogadores];

    preencheDadosDosJogadores(jogadores, quantidadeDeJogadores);

    
}

int jogarDado(){

    int numero;

    numero = rand()%6;

    return numero;
}

void preencheDadosDosJogadores( struct jogador *jogadores, int quantidade){
    bool corValida;

    for(int i = 0; i < quantidade; i++){
        printf("jogador %d digite o seu nome: ", i+1);
        setbuf(stdin, NULL);
        fgets(jogadores[i].nome, 20, stdin);

        do{
            do{
                printf("\n%s escolha uma das cores\n", jogadores[i].nome);
                printf("(1) - vermelho\n");
                printf("(2) - azul\n");
                printf("(3) - amarelo\n");
                printf("(4) - verde\n");
                printf("Digite sua opcao: ");
                scanf("%d", &jogadores[i].cor);

                if(jogadores[i].cor != 1 && jogadores[i].cor != 2 && jogadores[i].cor != 3 && jogadores[i].cor != 4){
                    printf("\n\nDigite uma opcao de cor valida!\n\n");
                }
        
            }while(jogadores[i].cor != 1 && jogadores[i].cor != 2 && jogadores[i].cor != 3 && jogadores[i].cor != 4);
         
            
            corValida = verificaCor(jogadores[i].cor, jogadores, quantidade);

            if(corValida == false){
                printf("\n\ncor ja selecionada, por favor escolha outra! \n\n");
            }

        }while(corValida != true);
    }

}

bool verificaCor(int cor, struct jogador jogadores[], int quantidadeDeJogadores){
    int corRepetida = 0;
    bool corEValida;


    for(int i=0; i < quantidadeDeJogadores; i++){
        if(jogadores[i].cor == cor){
            corRepetida++;
        }
    }

    if(corRepetida < 2){
        corEValida = true;
    } else if(corRepetida >= 2) {
        corEValida = false;
    }

    return corEValida;
}

