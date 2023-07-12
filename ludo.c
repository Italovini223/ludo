#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>
#include <Windows.h>


int tabuleiro[53];
int areaSegura[8] = {1, 9, 14, 22, 27, 35, 40, 48};

int areaFinalAzul[6];
int areaFinalVermelho[6];



// (1) => comeca a vermelha - 9 - 22 - 35 - (40) => comeca azul - 48 
// 

struct peca {
    int posicao;
    bool completouUmaVoltaNoTabuleiro;
    bool estaNaAreaFinal;
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
bool varificaSeAVencedor(struct jogador jogadores[], int quantidadeDeJogadores, FILE *historico);
void zeraPosicaoDasPecasETabuleiro(struct jogador jogadores[], int qunatidadeDeJogadores);
void zeraAsPecasQuePodemJogar(struct jogador *jogador);
void MovimentaPeca(int quantidadeASerMexida, struct jogador jogadores[], int posicaoJogador, int pecaASerMexida, FILE *historico);
void mostraPosicaoDasPecas(struct jogador *jogador);
bool verificaSePosicaoESegura(int posicao);



int main(){
    srand(time(NULL));
    bool pecaEValida, haVencedor;
    int dado, quantidadeDeJogadores = 2, possibilidadeDeJogadas = 0;
    int pecaMexida; // para o jogador digitar sua opcao de peca;
    char stringVazia[3]; // somente para o jogdor apertar enter;
    FILE *historico;

    struct jogador jogadores[quantidadeDeJogadores];
    historico = fopen("historico.txt","w");

    if(historico == NULL){
        printf("Erro ao abrir o arquivo");
        exit(1);
    }


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

                if(jogadores[i].jogasRepetidas == 3){
                    printf("\n Jogador %s como voce tirou 6 por %d vezes seguidas perdeu a vez!", jogadores[i].nome, jogadores[i].jogasRepetidas);
                    break;
                }

                
                for(int j = 0; j < 4; j++){
                    if(((jogadores[i].pecas[j].posicao + dado) <= 31 || jogadores[i].pecas[j].estaNaAreaFinal == false) && jogadores[i].cor == 2){ // azul
                        jogadores[i].pecasParaJogar[j] = j+1;
                        possibilidadeDeJogadas++;
                    } else if((jogadores[i].pecas[j].posicao + dado) <= 57 && jogadores[i].cor == 1){ // vermelho
                        jogadores[i].pecasParaJogar[j] = j+1;
                        possibilidadeDeJogadas++;
                    }
                }

            } else {
                for(int j = 0; j < 4; j++){
                    if(((jogadores[i].pecas[j].posicao + dado) <= 31 || jogadores[i].pecas[j].estaNaAreaFinal == false) && jogadores[i].pecas[j].posicao > 0 && jogadores[i].cor == 2){ // azul
                        jogadores[i].pecasParaJogar[j] = j+1;
                        possibilidadeDeJogadas++;
                    } else if((jogadores[i].pecas[j].posicao + dado) <= 57 && jogadores[i].pecas[j].posicao > 0 && jogadores[i].cor == 1){ // vermelho
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

                    
                } while(pecaEValida != true);

                MovimentaPeca(dado, jogadores, i, pecaMexida, historico);
                
            } else {
                printf("Voce nao pode movimentar nenhuma peca!\n");
                printf("Aperte ENTER para continuar para continuar");
                setbuf(stdin, NULL);
                gets(stringVazia);
            }

            printf("\n\nJogadas repetidas => %d\n\n", jogadores[i].jogasRepetidas);
            
            Sleep(2000);
            system("cls");


        }while(dado == 6);

        haVencedor = varificaSeAVencedor(jogadores, quantidadeDeJogadores, historico);
    }

    fclose(historico);
    return 0;

    
}

int jogarDado(){

    int numero;
    numero = (rand()%6) + 1;
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
        if(jogadores[i].cor == cor && (cor == 1 || cor == 2)){
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

            jogadores[i].pecas[j].completouUmaVoltaNoTabuleiro = false;
            jogadores[i].pecas[j].estaNaAreaFinal = false;

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

void MovimentaPeca(int quantidadeASerMexida, struct jogador jogadores[],  int posicaoJogador, int pecaASerMexida, FILE *historico){

    bool posicaoEsegura;
    int posicaoAnterior = jogadores[posicaoJogador].pecas[pecaASerMexida - 1].posicao;


    if(jogadores[posicaoJogador].pecas[pecaASerMexida - 1].posicao == 0 && quantidadeASerMexida == 6){

        if(jogadores[posicaoJogador].cor == 1){
            jogadores[posicaoJogador].pecas[pecaASerMexida - 1].posicao = 1; // coloca a peca na posicao 1 se a cor for vermelha
        } else if(jogadores[posicaoJogador].cor == 2){
            jogadores[posicaoJogador].pecas[pecaASerMexida - 1].posicao = 27; // coloca a peca na posicao 27 se for da cor azul
        }
        
    } else {

        if((jogadores[posicaoJogador].pecas[pecaASerMexida -1].posicao + quantidadeASerMexida) == 53 && jogadores[posicaoJogador].cor == 2){ // verifica se a proxia casa do peao azul for a 53
            jogadores[posicaoJogador].pecas[pecaASerMexida -1].posicao = 1; // se for ele coloca a peca na casa 1
            jogadores[posicaoJogador].pecas[pecaASerMexida -1].completouUmaVoltaNoTabuleiro = true;

        } else if((jogadores[posicaoJogador].pecas[pecaASerMexida -1].posicao + quantidadeASerMexida) >= 53 && jogadores[posicaoJogador].cor == 2){ //verifica se a soma da posicao da peca + a quantidada a ser mexia e maior que 53, se a cor do jogador for a azul
            quantidadeASerMexida = (jogadores[posicaoJogador].pecas[pecaASerMexida -1].posicao + quantidadeASerMexida) - 52; // se for ele pega essa quantidade e subtrai 52 
            jogadores[posicaoJogador].pecas[pecaASerMexida -1].completouUmaVoltaNoTabuleiro = true;
            jogadores[posicaoJogador].pecas[pecaASerMexida - 1].posicao = quantidadeASerMexida; // e coloca esse peao na casa certa
        } else {
            jogadores[posicaoJogador].pecas[pecaASerMexida - 1].posicao += quantidadeASerMexida; // se nao se encaixar nas condicoes acima ele somente soma a proxima casa do peao
        }

        
    }

    if(jogadores[posicaoJogador].pecas[pecaASerMexida - 1].posicao >= 26 && jogadores[posicaoJogador].pecas[pecaASerMexida -1].completouUmaVoltaNoTabuleiro == true  && jogadores[posicaoJogador].cor == 2){ // verifica se a proxima casa do peao do jogador azul e maior ou igual a 26

        if(posicaoAnterior <= 25){ // se for igual a 26 qur dizer que ele veio da posicao 25 do tabuleiro
            tabuleiro[posicaoAnterior] -= posicaoAnterior; // com isso limpamos a posicao 25 para que funcione a logica de comer a peca;

        }

        jogadores[posicaoJogador].pecas[pecaASerMexida - 1].estaNaAreaFinal = true;

        areaFinalAzul[quantidadeASerMexida - 25] = jogadores[posicaoJogador].pecas[pecaASerMexida - 1].posicao; // aqui ele coloca a peca do jogador azul na area final que e segura
    

        printf("A peca %d agora esta na posicao %d na area Segura\n\n", pecaASerMexida, jogadores[posicaoJogador].pecas[pecaASerMexida -1 ].posicao - 25);
        fprintf(historico, "Jogador %s A peca %d agora esta na posicao %d na area Segura\n", jogadores[posicaoJogador].nome, pecaASerMexida, jogadores[posicaoJogador].pecas[pecaASerMexida -1 ].posicao - 25);
        return; // retornamos porque como a logica abaixo se refere ao tabuleiro

    } else if(jogadores[posicaoJogador].pecas[pecaASerMexida - 1].posicao >= 52 && jogadores[posicaoJogador].cor == 1){ // aqui acontece a mesma coisa porem com as pecas do jogador vermelho;

        if(posicaoAnterior <= 52){
            tabuleiro[posicaoAnterior] -= posicaoAnterior;
        }

        areaFinalVermelho[jogadores[posicaoJogador].pecas[pecaASerMexida - 1].posicao - 52] = jogadores[posicaoJogador].pecas[pecaASerMexida - 1].posicao;
        jogadores[posicaoJogador].pecas[pecaASerMexida - 1].estaNaAreaFinal = true;


        printf("A peca %d agora esta na posicao %d na area Segura\n", pecaASerMexida, jogadores[posicaoJogador].pecas[pecaASerMexida -1 ].posicao - 51);
        fprintf(historico, "Jogador %s a peca %d agora esta na posicao %d na area Segura\n", jogadores[posicaoJogador].nome, pecaASerMexida, jogadores[posicaoJogador].pecas[pecaASerMexida -1 ].posicao - 52); 
        return;// retornamos porque como a logica abaixo se refere ao tabuleiro
    } 

    
    

    if(tabuleiro[jogadores[posicaoJogador].pecas[pecaASerMexida - 1].posicao] == 0){ // aqui e verificado se a posicao do tabulerio para onde a peca do jogador ira esta vazia
        tabuleiro[jogadores[posicaoJogador].pecas[pecaASerMexida - 1].posicao] += jogadores[posicaoJogador].pecas[pecaASerMexida - 1].posicao; // se estiver a peca va para a posicao do tabuleiro normalmente;

        printf("A peca %d agora esta na posicao %d\n\n", pecaASerMexida, jogadores[posicaoJogador].pecas[pecaASerMexida - 1].posicao);
        fprintf(historico, "Jogador %s a peca %d agora esta na posicao %d\n", jogadores[posicaoJogador].nome, pecaASerMexida, jogadores[posicaoJogador].pecas[pecaASerMexida - 1].posicao);

    } else{ // agora se a posicao ja estiver ocupada
        posicaoEsegura = verificaSePosicaoESegura(jogadores[posicaoJogador].pecas[pecaASerMexida - 1].posicao); // primeiro verificamos se essa peca nao e uma area segura do tabuleiro;

        tabuleiro[jogadores[posicaoJogador].pecas[pecaASerMexida - 1].posicao] += jogadores[posicaoJogador].pecas[pecaASerMexida - 1].posicao; // colaca a peca na nova posicao;
        
        printf("A peca %d agora esta na posicao %d\n\n", pecaASerMexida, jogadores[posicaoJogador].pecas[pecaASerMexida - 1].posicao);
        fprintf(historico, "Jogador %s A peca %d agora esta na posicao %d\n", jogadores[posicaoJogador].nome, pecaASerMexida, jogadores[posicaoJogador].pecas[pecaASerMexida - 1].posicao);

        for(int i = 0; i < 4; i++){
            if(posicaoJogador == 0){ // aqui se o jogador que esta mexendo a peca e o jogador 1
                if(jogadores[posicaoJogador].pecas[pecaASerMexida - 1].posicao == jogadores[1].pecas[i].posicao && posicaoEsegura == false && jogadores[posicaoJogador].pecas[pecaASerMexida - 1].estaNaAreaFinal == false){ // verificamos se ha alguma peca do jogador 2 nessa posicao e se essa posicao nao e segura
                    tabuleiro[jogadores[1].pecas[i].posicao] -= jogadores[1].pecas[i].posicao; // tira a peca a ser comida do tabuleiro
                    
                    jogadores[1].pecas[i].posicao = 0; // se estiver alguma peca na posicao voltamos ela para fora do tabuleiro da jogo
                    jogadores[1].pecas[i].estaNaAreaFinal = false; // 

                    if(jogadores[1].cor == 2){
                        jogadores[1].pecas[i].completouUmaVoltaNoTabuleiro = false; // se a cor escolhida pelo usuario que a peca foi comida for azul a variavel para ver se a peca deu a volta no tabueiro recebe false
                    }
                    
                    


                    system("cls");
                    printf("Jogador %s sua peca %d foi comida e voltou para a posicao %d\n", jogadores[1].nome, i+1, jogadores[1].pecas[i].posicao);
                    fprintf(historico, "Jogador %s sua peca %d foi comida e voltou para a posicao %d\n", jogadores[1].nome, i+1, jogadores[1].pecas[i].posicao);
                    Sleep(2000);

                    
                }
            } else if(posicaoJogador == 1){ // aqui a mesma logica acima porem, se o jogador que estiver jogando for o 2
                if(jogadores[posicaoJogador].pecas[pecaASerMexida - 1].posicao == jogadores[0].pecas[i].posicao && posicaoEsegura == false && jogadores[posicaoJogador].pecas[pecaASerMexida - 1].estaNaAreaFinal == false){
                    
                    tabuleiro[jogadores[1].pecas[i].posicao] -= jogadores[1].pecas[i].posicao; // tira a peca a ser comida do tabuleiro
                    
                    jogadores[0].pecas[i].posicao = 0; // a peca do jogador 1 e comida
                    jogadores[0].pecas[i].estaNaAreaFinal = false; // a variavel para checar se o peao esta na area final recebe false

                    if(jogadores[0].cor == 2){
                        jogadores[0].pecas[i].completouUmaVoltaNoTabuleiro = false; // se a cor escolhida pelo usuario que a peca foi comida for azul a variavel para ver se a peca deu a volta no tabueiro recebe false
                    }
                        
                    system("cls");
                    printf("Jogador %s sua peca %d foi comida e voltou para a posicao %d\n", jogadores[0].nome, i+1, jogadores[0].pecas[i].posicao);
                    fprintf(historico, "Jogador %s sua peca %d foi comida e voltou para a posicao %d\n", jogadores[0].nome, i+1, jogadores[0].pecas[i].posicao);
                    Sleep(2000);
                    
                
                }
            }
        }
    }
    
    tabuleiro[posicaoAnterior] -= posicaoAnterior; // aqui pegamos a pocisao anterior que a peca jogada estava e limpamos ela 


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

bool varificaSeAVencedor(struct jogador jogadores[], int quantidadeDeJogadores, FILE *historico){

    bool haVencedor;
    int pecasFinalizadas = 0;
    int i;


    for(i = 0; i < quantidadeDeJogadores; i++){
        pecasFinalizadas = 0;
        for(int j = 0; j < 4; j++){
            if(jogadores[i].pecas[j].posicao == 57 && jogadores[i].cor == 1){
                pecasFinalizadas++;
            } else if(jogadores[i].pecas[j].posicao == 31 && jogadores[i].pecas[j].estaNaAreaFinal == true && jogadores[i].cor == 2){
                pecasFinalizadas++;
            }
        }

        if(pecasFinalizadas == 4){
            haVencedor = true;

            printf("Parabens jogador %s voce ganhou a partida!!!!!", jogadores[i].nome);
            fprintf(historico, "Parabens jogador %s voce ganhou a partida!!!!!\n", jogadores[i].nome);
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
        if(jogador->pecas[i].estaNaAreaFinal == true){
            if(jogador->cor == 1){ //vermelho
                printf("A peca %d esta na posicao %d do quadrante final\n", i+1, jogador->pecas[i].posicao - 51);
            } else if(jogador->cor == 2){ // azul 
                printf("A peca %d esta na posicao %d do quadrante final\n", i+1, jogador->pecas[i].posicao - 25);
            } 
        } else {
            printf("A peca %d esta na posicao %d\n", i+1, jogador->pecas[i].posicao);
        }
    }

    printf("\n");
}
