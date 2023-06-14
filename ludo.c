#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>

struct jogador {
    char nome[20], cor[20];
    int pecas[4];
};


int jogarDado();
void preencheDadosDosJogadores( struct jogador *jogadores, int quantidade);
bool verificaCor(char cor[]);



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

    // função responsavel pela parte de jogar o dado;
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
            printf("%s digite uma cor\n", jogadores[i].nome);
            printf("varmelho, azul, verde ou amarelo: ");
            setbuf(stdin, NULL);
            gets(jogadores[i].nome);
            //fgets(jogadores[i].cor, 20, stdin);
            
            corValida = verificaCor(jogadores[i].cor);

            if(corValida == false){
                printf("Digite um cor valida! \n\n");
            }
        }while(corValida != true);
    }

}

bool verificaCor(char cor[]){
    char coresValidas[4][20] = { "vermelho", "amarelo", "verde", "azul" };
    int quantidadeDeValida = 0;
    bool corEValida;


    for(int i = 0; i < 4; i++){
        quantidadeDeValida = 0;

        quantidadeDeValida = strcmp(coresValidas[i], cor);

        printf("%d\n\n", quantidadeDeValida);

        if(quantidadeDeValida == 0){
            break;
        }
    }

    printf("%d", quantidadeDeValida);

    if(quantidadeDeValida == 0){
        corEValida = true;
    } else {
        corEValida = false;
    }

    return corEValida;


}
