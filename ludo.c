#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>

struct peca {
    int posicao;
    bool chegouPrimeiroNaPosicao;
};

struct jogador {
    char nome[20];
    struct peca pecas[4];
    int pecasParaJogar[4], cor, jogasRepetidas;; 
};


int jogarDado();
void preencheDadosDosJogadores( struct jogador *jogadores, int quantidade);
bool verificaCor(int cor, struct jogador jogador[], int quantidadeDeJogadores);
bool pecaMeixaEValida(struct jogador *jogador, int pecaSelecionada);
bool varificaSeAVencedor(struct jogador jogadores[], int quantidadeDeJogadores);
void zeraPosicaoDasPecas(struct jogador jogadores[], int qunatidadeDeJogadores);
void zeraAsPecasQuePodemJogar(struct jogador jogadores[], int posicaoDoJogador);
void MovimentaPeca(int quantidadeASerMexida, struct jogador *jogador, int pecaASerMexida);




int main(){
    srand(time(NULL));
    bool pecaEValida, haVencedor;
    int tabuleiro[15][15];
    int dado, quantidadeDeJogadores = 2, possibilidadeDeJogadas = 0;
    int pecaMexida;
    char stringVazia[3];
    struct jogador jogadores[quantidadeDeJogadores];

    preencheDadosDosJogadores(jogadores, quantidadeDeJogadores);
    zeraPosicaoDasPecas(jogadores, quantidadeDeJogadores);

    for(int i = 0; haVencedor != true; i++){
        if (i > 1){
            i = 0;
        }
        jogadores[i].jogasRepetidas = 0;

        do{
            zeraAsPecasQuePodemJogar(jogadores, i);
            dado = jogarDado();
            possibilidadeDeJogadas = 0;
            
            printf("Jogador %s, voce triou %d no dado!\n ", jogadores[i].nome, dado);

            if(dado == 6){

                jogadores[i].jogasRepetidas++;

                
                for(int j = 0; j < 4; j++){
                    if(jogadores[i].pecas[j].posicao == 0){
                        jogadores[i].pecasParaJogar[j] = j+1;
                        possibilidadeDeJogadas++;
                    } else {
                        jogadores[i].pecasParaJogar[j] = j+1;
                        possibilidadeDeJogadas++;
                    }
                }

            } else {
                for(int j = 0; j < 4; j++){
                    if(jogadores[i].pecas[j].posicao > 0){
                       jogadores[i].pecasParaJogar[j] = j+1;
                       possibilidadeDeJogadas++;
                    }
                    
                }

            }

            if(possibilidadeDeJogadas > 0){
                for(int j = 0; j < 4; j++){
                    if(jogadores[i].pecasParaJogar[j] > 0 && jogadores[i].pecasParaJogar[j] <= 4){
                        printf("digite %d para mover a peca %d por %d casas!\n", jogadores[i].pecasParaJogar[j], jogadores[i].pecasParaJogar[j], dado);
                    }
                }

                do {
                    printf("Digite sua opcao: ");
                    scanf("%d", &pecaMexida);

                    pecaEValida = pecaMeixaEValida(&jogadores[i], pecaMexida);

                    if(pecaEValida == false ){
                        printf("\n\nEscolha uma peca Valida!\n\n");
                    }

                    MovimentaPeca(dado, jogadores, pecaMexida);
                } while(pecaEValida != true);
                
            } else {
                printf("Voce nao pode movimentar nenhuma peca!\n");
                printf("Aperte qualquer tecla para continuar para continuar");
                setbuf(stdin, NULL);
                gets(stringVazia);
            }

            if(jogadores[i].jogasRepetidas == 3){
                break;
            }

            printf("%d", jogadores[i].jogasRepetidas);
            
        }while(dado == 6);

        printf("\n\nsaiu do do\n\n");

        haVencedor = varificaSeAVencedor(jogadores, quantidadeDeJogadores);

    }

    
}

int jogarDado(){

    int numero;

    do {
        numero = (rand()%6) + 1;
    } while(numero == 0);
    

    return numero;
}

void preencheDadosDosJogadores( struct jogador *jogadores, int quantidade){
    bool corValida;

    for(int i = 0; i < quantidade; i++){
        printf("jogador %d digite o seu nome: ", i+1);
        setbuf(stdin, NULL);
        fgets(jogadores[i].nome, 20, stdin);

        for(int j = 0; jogadores[i].nome[j] != '\0'; j++){
            if(jogadores[i].nome[j] == '\n'){
                jogadores[i].nome[j] = '\0';
            }
        }

        do{
            do{
                printf("\n%s escolha uma das cores\n", jogadores[i].nome);
                printf("(1) - vermelho\n");
                printf("(2) - azul\n");
                printf("Digite sua opcao: ");
                scanf("%d", &jogadores[i].cor);

                if(jogadores[i].cor != 1 && jogadores[i].cor != 2){
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

void zeraPosicaoDasPecas(struct jogador jogadores[], int qunatidadeDeJogadores){
    for(int i = 0; i < qunatidadeDeJogadores; i++){
        for(int j = 0; j < 4; j++){
            jogadores[i].pecas[j].posicao = 0;
        }
    }
}

void zeraAsPecasQuePodemJogar(struct jogador jogadores[], int posicaoDoJogador){
    for(int i = 0; i < 4; i++){
        jogadores[posicaoDoJogador].pecasParaJogar[i] = 0;
    }
}

void MovimentaPeca(int quantidadeASerMexida, struct jogador *jogador, int pecaASerMexida){

    if(jogador->pecas[pecaASerMexida].posicao == 0 && quantidadeASerMexida == 6){
        jogador->pecas[pecaASerMexida].posicao = 1;

    } else {
        jogador->pecas[pecaASerMexida].posicao += quantidadeASerMexida;
    }

   

   printf("A peca %d agora esta na posicao %d\n\n", pecaASerMexida, jogador->pecas[pecaASerMexida].posicao);
}

bool pecaMeixaEValida(struct jogador *jogador, int pecaSelecionada){
    bool pecaVailda;
    int quantidadeDePecasValidas = 0;

    for(int i = 0; i < 4; i++){
        if(jogador->pecasParaJogar[i] == pecaSelecionada){
            quantidadeDePecasValidas++;
        }
    }

    if(quantidadeDePecasValidas == 1){
        pecaVailda = true;
    } else {
        pecaVailda = false;
    }

    return pecaVailda;
}

bool varificaSeAVencedor(struct jogador jogadores[], int quantidadeDeJogadores){
    bool haVencedor;
    int pecasFinalizadas = 0;


    for(int i = 0; i < quantidadeDeJogadores; i++){
        pecasFinalizadas = 0;
        for(int j = 0; j < 4; j++){
            if(jogadores[i].pecas[j].posicao == 56){
                pecasFinalizadas++;

                if(pecasFinalizadas == 4){
                    break;
                }
            }
        }

        if(pecasFinalizadas == 4){
            haVencedor = true;
        } else{
            haVencedor = false;
        }
    }

    return haVencedor;
}