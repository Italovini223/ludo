#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>

#ifdef _WIN32 || _WIN64
    #include <Windows.h>
#else
    #include <unistd.h>
#endif

int tabuleiro[61];
int areaSegura[6] = {1, 9, 22, 25, 35, 40};



// (1) => comeca a vermelha - 9 - 22 - 35 - (40) => comeca azul - 48 
// 

struct peca {
    int posicao;
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
void zeraPosicaoDasPecasETabuleiro(struct jogador jogadores[], int qunatidadeDeJogadores);
void zeraAsPecasQuePodemJogar(struct jogador *jogador);
void MovimentaPeca(int quantidadeASerMexida, struct jogador jogadores[], int posicaoJogador, int pecaASerMexida);
void mostraPosicaoDasPecas(struct jogador *jogador);
bool verificaSePosicaoESegura(int posicao);







int main(){
    srand(time(NULL));
    bool pecaEValida, haVencedor;
    int dado, quantidadeDeJogadores = 2, possibilidadeDeJogadas = 0;
    int pecaMexida; // para o jogador digitar sua opcao de peca;
    char stringVazia[3]; // somente para o jogdor apertar enter 
    
    struct jogador jogadores[quantidadeDeJogadores];


    preencheDadosDosJogadores(jogadores, quantidadeDeJogadores);
    zeraPosicaoDasPecasETabuleiro(jogadores, quantidadeDeJogadores);

    for(int i = 0; haVencedor != true; i++){
        if (i > 1){
            i = 0;
        }
        jogadores[i].jogasRepetidas = 0;

        do{
            possibilidadeDeJogadas = 0;
            zeraAsPecasQuePodemJogar(&jogadores[i]);
            dado = jogarDado();

            if(jogadores[i].cor == 1){
                system("color 4");
            } else if(jogadores[i].cor == 2){
                system("color 1");
            }
            
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

            mostraPosicaoDasPecas(&jogadores[i]);

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

                    MovimentaPeca(dado, jogadores, i, pecaMexida);
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

            printf("\n\nJogadas repetidas => %d\n\n", jogadores[i].jogasRepetidas);
            
            Sleep(2000);
            system("cls");

        }while(dado == 6);


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
          
            printf("\n%s escolha uma das cores\n", jogadores[i].nome);
            printf("(1) - vermelho\n");
            printf("(2) - azul\n");
            printf("Digite sua opcao: ");
            scanf("%d", &jogadores[i].cor);
  
            corValida = verificaCor(jogadores[i].cor, jogadores, quantidade);

            if(corValida == false){
                printf("\n\ncor ja selecionada ou invalida, por favor escolha outra! \n\n");
            }

        }while(corValida != true);
        Sleep(1000);
        system("cls");
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

    if(corRepetida == 1){
        corEValida = true;
    } else{
        corEValida = false;
    }

    return corEValida;
}

void zeraPosicaoDasPecasETabuleiro(struct jogador jogadores[], int qunatidadeDeJogadores){
    for(int i = 0; i < qunatidadeDeJogadores; i++){
        for(int j = 0; j < 4; j++){
            jogadores[i].pecas[j].posicao = 0;
        }

    }

    for(int k = 0; k < 56; k++){
        tabuleiro[k] = 0;
    }


}

void zeraAsPecasQuePodemJogar(struct jogador *jogador){
    for(int i = 0; i < 4; i++){
        jogador->pecasParaJogar[i] = 0;
    }
}

bool verificaSePosicaoESegura(int posicao){
    bool posicaoEsegura;
    int eSegura = 0;

    for(int i = 0; i < 6; i++){
        if(posicao == areaSegura[i]){
            eSegura++;
        }
    }

    if(eSegura == 1){
        posicaoEsegura = true;
    } else {
        posicaoEsegura = false;
    }

    return posicaoEsegura;
}

void MovimentaPeca(int quantidadeASerMexida, struct jogador jogadores[],  int posicaoJogador, int pecaASerMexida){

    bool posicaoEsegura;
    int posicaoAnterior = jogadores[posicaoJogador].pecas[pecaASerMexida - 1].posicao;


    if(jogadores[posicaoJogador].pecas[pecaASerMexida - 1].posicao == 0 && quantidadeASerMexida == 6){

        if(jogadores[posicaoJogador].cor == 1){
            jogadores[posicaoJogador].pecas[pecaASerMexida - 1].posicao = 1;
        } else if(jogadores[posicaoJogador].cor == 2){
            jogadores[posicaoJogador].pecas[pecaASerMexida - 1].posicao = 40;
        }
        
    } else {

        if((jogadores[posicaoJogador].pecas[pecaASerMexida -1].posicao + quantidadeASerMexida) == 53 && jogadores[posicaoJogador].cor == 2){
            jogadores[posicaoJogador].pecas[pecaASerMexida -1].posicao = 1;

        } else if((jogadores[posicaoJogador].pecas[pecaASerMexida -1].posicao + quantidadeASerMexida) >= 53 && jogadores[posicaoJogador].cor == 2){
            quantidadeASerMexida = (jogadores[posicaoJogador].pecas[pecaASerMexida -1].posicao += quantidadeASerMexida) - 52;

            jogadores[posicaoJogador].pecas[pecaASerMexida - 1].posicao = quantidadeASerMexida;
        } else {
            jogadores[posicaoJogador].pecas[pecaASerMexida - 1].posicao += quantidadeASerMexida;
        }

        
    }

    
    

    if(tabuleiro[jogadores[posicaoJogador].pecas[pecaASerMexida - 1].posicao] == 0){
        tabuleiro[jogadores[posicaoJogador].pecas[pecaASerMexida - 1].posicao] = jogadores[posicaoJogador].pecas[pecaASerMexida - 1].posicao;

        printf("A peca %d agora esta na posicao %d\n\n", pecaASerMexida, jogadores[posicaoJogador].pecas[pecaASerMexida - 1].posicao);

    } else{
        for(int i = 0; i < 4; i++){
            posicaoEsegura = verificaSePosicaoESegura(jogadores[posicaoJogador].pecas[i].posicao);

            for(int j = 0; j < 4; j++){

                if(posicaoJogador == 0){
                    if(jogadores[posicaoJogador].pecas[i].posicao == jogadores[1].pecas[j].posicao && posicaoEsegura == false){
                        jogadores[1].pecas[j].posicao = 0;

                        printf("Jogador %s sua peca %d foi comida e voltou para a posicao %d\n", jogadores[1].nome, j+1, jogadores[1].pecas[j].posicao);
                        Sleep(2000);
                        return;
                    }
                } else if(posicaoJogador == 1){
                    if(jogadores[posicaoJogador].pecas[i].posicao == jogadores[0].pecas[j].posicao && posicaoEsegura == false){
                        jogadores[0].pecas[j].posicao = 0;
                        
                        system("cls");
                        printf("Jogador %s sua peca %d foi comida e voltou para a posicao %d\n", jogadores[0].nome, j+1, jogadores[0].pecas[j].posicao);
                        Sleep(2000);
                        return;
                
                    }
                }
            }
        }
    }
    
    tabuleiro[posicaoAnterior] -= posicaoAnterior;

    printf(" VALOR NA POSICAO %d NO VETOR DEPOIS DA PECA MEXIDA => %d", posicaoAnterior, tabuleiro[posicaoAnterior]);

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

void mostraPosicaoDasPecas(struct jogador *jogador){
    printf("\n");

    printf("Jogador %s, esta e a posicao das suas pecas: \n", jogador->nome);

    for(int i = 0; i < 4; i++){
        printf("A peca %d esta na posicao %d\n", i+1, jogador->pecas[i].posicao);
    }

    printf("\n");
}
