#include <iostream>
#include <ctime>
#include <cstdlib>
#include <conio.h> // Для GetKeyState
#include <windows.h>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

// константы для работы с прогой
const char Food = '*';
const char Snake = '0';
const int WIDTH = 20;  // Ширина игрового поля
const int HEIGHT = 10; // Высота игрового поля
const string LEADERBOARD_FILE = "leaderboard.txt";

// переменные для работы с прогой
enum Direction { UP, DOWN, LEFT, RIGHT };
int SnakeOX[100], SnakeOY[100];
int SnakeLEN = 1;
int FoodOX, FoodOY;
int score = 1;
Direction SNAKE_NAPRAVLENIE = RIGHT;
bool isRunning = true;
char map[HEIGHT][WIDTH];

void gotOXY(int x, int y) {
    COORD pos = { short(x), short(y) };
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(output, pos);
}

void initializeMap() {
    // Заполнение карты
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1) {
                map[i][j] = '#'; // Границы
            }
            else {
                map[i][j] = ' '; // тута змейка ползает
            }
        }
    }
}

// отрисовка карты при помощи FOR
void drawMap() {
    gotOXY(0, 0);
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            cout << map[i][j];
        }
        cout << endl;
    }
}

// функция окончания игры
void endGame() {
    isRunning = false;
    cout << "Игра окончена! Ваш счет: " << SnakeLEN << endl;

    // Запрос имени игрока
    string playerName;
    cout << "Введите ваше имя для таблицы лидеров: ";
    cin >> playerName;

    // Сохранение результата в таблицу лидеров
    ofstream outfile;
    outfile.open(LEADERBOARD_FILE, ios::app);
    if (outfile.is_open()) {
        outfile << playerName << " " << SnakeLEN << endl;
        outfile.close();
    }
}

int main() {

    setlocale(LC_ALL, "Russian"); // русская мова
    srand(time(0)); // с сишки спиздил(рандомчик)
     //чтоб змейка ползала красиво(не пон как работает) 
    SnakeOX[0] = WIDTH / 2; 
    SnakeOY[0] = HEIGHT / 2;
    double time = clock();

    // Генератор еды 
    FoodOX = 1 + (rand() % (WIDTH - 3));
    FoodOY = 1 + (rand() % (HEIGHT - 2));

    initializeMap();

    while (isRunning) {
        if (GetKeyState('A') & 0x8000) {
            if (SNAKE_NAPRAVLENIE != RIGHT) {
                SNAKE_NAPRAVLENIE = LEFT;
            }
        }
        if (GetKeyState('S') & 0x8000) {
            if (SNAKE_NAPRAVLENIE != UP) {
                SNAKE_NAPRAVLENIE = DOWN;
            }
        }
        if (GetKeyState('D') & 0x8000) {
            if (SNAKE_NAPRAVLENIE != LEFT) {
                SNAKE_NAPRAVLENIE = RIGHT;
            }
        }
        if (GetKeyState('W') & 0x8000) {
            if (SNAKE_NAPRAVLENIE != DOWN) {
                SNAKE_NAPRAVLENIE = UP;
            }
        }

        if ((clock() - time) / CLOCKS_PER_SEC >= 0.2) { // вот так работает норм что за часики я хз)
            time = clock();
            if (SnakeOX[0] == FoodOX && SnakeOY[0] == FoodOY) {
                ++SnakeLEN;
                FoodOX = 1 + (rand() % (WIDTH - 3));
                FoodOY = 1 + (rand() % (HEIGHT - 2));
            }

            // Сдвигаем тело змейки
            for (int i = SnakeLEN - 2; i >= 0; --i) {
                SnakeOX[i + 1] = SnakeOX[i];
                SnakeOY[i + 1] = SnakeOY[i];
            }

            // Движение змейки
            if (SNAKE_NAPRAVLENIE == UP) {
                --SnakeOY[0];
            }
            if (SNAKE_NAPRAVLENIE == DOWN) {
                ++SnakeOY[0];
            }
            if (SNAKE_NAPRAVLENIE == RIGHT) {
                ++SnakeOX[0];
            }
            if (SNAKE_NAPRAVLENIE == LEFT) {
                --SnakeOX[0];
            }

            // Проверка на столкновение с краями карты
            if (SnakeOX[0] < 0 || SnakeOY[0] < 0 || SnakeOX[0] >= WIDTH || SnakeOY[0] >= HEIGHT) {
                isRunning = false;
            }

            // Проверка на столкновение с самим собой
            for (int i = 1; i < SnakeLEN; ++i) {
                if (SnakeOX[0] == SnakeOX[i] && SnakeOY[0] == SnakeOY[i]) {
                    isRunning = false;
                    break;
                }
            }

            initializeMap(); // Обновляем карту
            map[FoodOY][FoodOX] = Food; // Размещаем еду
            for (int i = 0; i < SnakeLEN; i++) {
                map[SnakeOY[i]][SnakeOX[i]] = Snake; // Размещаем змейку
            }
            drawMap(); // Рисуем карту
            cout << "Длина змейки: " << SnakeLEN << endl;
        }
    }
    gotOXY(1, HEIGHT / 2);
    cout << "ТВОЙ СЧЕТ:" << SnakeLEN << endl;
    gotOXY(WIDTH, HEIGHT);
    endGame();

    return 0;
}