#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

int getch(){
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

int kbhit(){
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF){
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}


// Função para converter direção em texto
string directionToText(char dir) {
    switch (dir) {
        case 'W': return "Cima";
        case 'A': return "Esquerda";
        case 'S': return "Baixo";
        case 'D': return "Direita";
        default: return "";
    }
}

// Função principal
int main() {
    vector<char> sequence;
    vector<char> playerInput;     
    char directions[] = {'W', 'A', 'S', 'D'}; 
    bool gameOver = false;        
    int round = 1;                

    srand(static_cast<unsigned>(time(0))); // Seed aleatória

    cout << "=== Jogo da Memoria ===\n";
    cout << "Use W (Cima), A (Esquerda), S (Baixo), D (Direita) para jogar.\n";
    cout << "Replique a sequencia gerada pelo computador.\n";
    cout << "Pressione qualquer tecla para comecar...\n";
    getch();
    system("clear");
    usleep(500000);
    

    while (!gameOver) {
        // Gerar nova direção e adicionar à sequência
        char newDirection = directions[rand() % 4];
        sequence.push_back(newDirection);

        // Mostrar sequência ao jogador, direção por direção
        for (int i = 0; i < sequence.size(); ++i) {
            cout << "Rodada " << round << ". Memorize a sequencia:" << endl;
            cout << directionToText(sequence[i]) << endl;
            cout.flush();
            usleep(500000); 
            system("clear");
            cout << "Rodada " << round << ". Memorize a sequencia:" << endl;
            usleep(500000); 
            system("clear");
        }

        // Pedir entrada do jogador
        playerInput.clear();
        cout << "Digite a sequencia usando W, A, S, D:\n";
        for (int i = 0; i < sequence.size(); ++i) 
        {
            char input = toupper(getch()); // Captura tecla sem pressionar Enter
            if (input == 'W' || input == 'A' || input == 'S' || input == 'D') 
            {
                cout << directionToText(input) << endl;
                playerInput.push_back(input);
            } 
            else 
            {
                cout << "Entrada invalida! Use apenas W, A, S ou D.\n";
                gameOver = true;
                break;
            }
        }
        usleep(500000);


        // Verificar se a entrada do jogador está correta
        if (!gameOver) {
            for (int i = 0; i < sequence.size(); ++i) {
                if (playerInput[i] != sequence[i]) {
                    gameOver = true;
                    break;
                }
            }

            if (gameOver) {
                cout << "\nSequencia incorreta! Fim de jogo.\n";
            } else {
                cout << "\nCorreto! Prepare-se para a proxima rodada...\n";
                ++round;
                usleep(1000); // Pausa antes da próxima rodada
                system("clear");
            }
        }
    }

    cout << "Voce completou " << round - 1 << " rodada(s). Parabens!\n";
    return 0;
}
