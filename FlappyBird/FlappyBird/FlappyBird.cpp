#include <iostream>
#include <conio.h>   
#include <windows.h> 
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

const int width = 30;
const int height = 20;
const int gapSize = 5;
const int pipeWidth = 2;
const int pipeSpeed = 1;

class Bird {
public:
    int x, y;

    Bird(int startX, int startY) {
        x = startX;
        y = startY;
    }

    void moveUp() {
        y -= 3;
        if (y < 0) y = 0;
    }

    void fall() {
        y++;
    }
};

class Pipe {
public:
    int x, gapStart;

    Pipe(int startX) {
        x = startX;
        gapStart = rand() % (height - gapSize - 1) + 1;
    }

    void move() {
        x--;
    }

    bool isOffScreen() {
        return x < -pipeWidth;
    }
};

class Game {
private:
    Bird bird;
    vector<Pipe> pipes;
    int score;
    int highscore;
    bool gameOver;
    int pipeDelay;

public:
    Game() : bird(width / 4, height / 2) {
        score = 0;
        highscore = 0;
        gameOver = false;
        pipeDelay = 0;
        pipes.push_back(Pipe(width));
    }

    void draw() {
        system("cls");
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                if (x == bird.x && y == bird.y)
                    cout << 'O'; 
                else {
                    bool isPipe = false;
                    for (const Pipe& pipe : pipes) {
                        if (x >= pipe.x && x < pipe.x + pipeWidth) { 
                            if (y < pipe.gapStart || y > pipe.gapStart + gapSize)  
                                cout << '#'; 
                            else
                                cout << ' ';
                            isPipe = true;
                            break;
                        }
                    }
                    if (!isPipe) {
                        if (y == height - 1)
                            cout << '_'; 
                        else
                            cout << ' ';
                    }
                }
            }
            cout << endl;
        }
        cout << "Score: " << score << endl;
        if (score > highscore) highscore = score;
        cout << "High Score: " << highscore << endl;
    }

    void input() {
        if (_kbhit()) {
            char key = _getch();
            if (key == 'w' || key == 'W') {
                bird.moveUp();
            }
        }
    }

    void updateLogic() {
        bird.fall();

        
        if (pipeDelay++ % pipeSpeed == 0) {
            for (Pipe& pipe : pipes) {
                pipe.move();
            }
        }

        
        if (pipes.back().x < width / 2) {
            pipes.push_back(Pipe(width));
        }

        
        if (pipes.front().isOffScreen()) {
            pipes.erase(pipes.begin());
        }

        
        if (bird.y >= height - 1) {
            gameOver = true;
        }
        for (Pipe& pipe : pipes) {
            if (bird.x >= pipe.x && bird.x < pipe.x + pipeWidth) {
                if (bird.y < pipe.gapStart || bird.y > pipe.gapStart + gapSize) {
                    gameOver = true;
                }
            }

            
            if (bird.x == pipe.x + pipeWidth && !gameOver) {
                score++; 
            }
        }
    }


    void gameLoop() {
        while (!gameOver) {
            draw();
            input();
            updateLogic();
            Sleep(100);
        }
        gameOverMenu();
    }

    void gameOverMenu() {
        char choice;
        do {
            cout << "Game Over! Your score is: " << score << endl;
            cout << "1. Restart Game" << endl;
            cout << "2. Return to Menu" << endl;
            cout << "Select an option (1-2): ";
            choice = _getch();

            switch (choice) {
            case '1':
                reset();
                gameLoop();
                break;
            case '2':
                return;
            default:
                cout << "Invalid choice. Please select again." << endl;
            }
        } while (choice != '2');
    }

    void reset() {
        bird = Bird(width / 4, height / 2);
        pipes.clear();
        pipes.push_back(Pipe(width));
        score = 0;
        gameOver = false;
        pipeDelay = 0;
    }
};

void displayMenu() {
    Game game;
    char choice;
    do {
        system("cls");
        cout << "===== Flappy Bird Console Version =====" << endl;
        cout << "1. Start Game" << endl;
        cout << "2. Exit" << endl;
        cout << "Select an option (1-2): ";
        choice = _getch();

        switch (choice) {
        case '1':
            game.reset();
            game.gameLoop();
            break;
        case '2':
            cout << "Exiting game..." << endl;
            break;
        default:
            cout << "Invalid choice. Please select again." << endl;
            Sleep(1000);
        }
    } while (choice != '2');
}

int main() {
    srand(time(0));
    displayMenu();
    return 0;
}
