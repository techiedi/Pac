#include <SFML/Graphics.hpp>
#include <time.h>

using namespace sf;

const int hight = 29;               // Высота карты
const int width = 29;               // Ширина карты
const int block = 25;               // Размер объектов

struct GameState {
    int chicken_num = 0;
    int total_chicken = 0;          // Общее количество кусочков курицы
    bool life = true;
    bool game_over = false;
    bool win = false;
} gameState;

String Map[hight] = {
    "WWWWWWWWWWWWWWVWWWWWWWWWWWWWW",
    "W            WVW            W",
    "W WWWWW WWWW VVV WWWW WWWWW W",
    "W W   W W    WVW    W W   W W",
    "W W W W WWWWWWVWWWWWW W W W W",
    "W   W   W           W   W   W",
    "WWW WWWWW WWWWWWWWW WWWWW WWW",
    "W         W       W         W",
    "W WWWWW WWWW W W WWWW WWWWW W",
    "W     W      W W      W     W",
    "WWWWW WWWWW WWWWW WWWWW WWWWW",
    "W     W               W     W",
    "W W WWW WWWW WVW WWWW WWW W W",
    "WWW W        WVW        W WWW",
    "VVV   WW WWW VPV WWW WW   VVV",
    "WWW W        WVW        W WWW",
    "W W WWW WWWW WVW WWWW WWW W W",
    "W     W               W     W",
    "WWWWW WWWWW WWWWW WWWWW WWWWW",
    "W     W      W W      W     W",
    "W WWWWW WWWW W W WWWW WWWWW W",
    "W         W       W         W",
    "WWW WWWWW WWWWWWWWW WWWWW WWW",
    "W   W   W           W   W   W",
    "W W W W WWWWWWVWWWWWW W W W W",
    "W W   W W    WVW    W W   W W",
    "W WWWWW WWWW VVV WWWW WWWWW W",
    "W            WVW            W",
    "WWWWWWWWWWWWWWVWWWWWWWWWWWWWW"
};

class Player {
public:
    float frame = 0;
    int x = 14, y = 14;
    int changex = 0, changey = 0;
    int rotate = 0, del = 0;

    void reset() {
        frame = 0;
        x = 14; y = 14;
        changex = 0; changey = 0;
        rotate = 0; del = 0;
    }

    void changeP() {
        frame += 0.03;
        if (frame > 6) frame -= 6;
        del++;

        if (del >= 100) {
            switch (rotate) {
            case 0: if (Map[y][changex + 1] != 'W') changex += 1; break;
            case 1: if (Map[y][changex - 1] != 'W') changex -= 1; break;
            case 2: if (Map[changey - 1][x] != 'W') changey -= 1; break;
            case 3: if (Map[changey + 1][x] != 'W') changey += 1; break;
            }
            del = 0;
        }

        if (Map[changey][changex] == ' ' || Map[changey][changex] == 'V') {
            if (Map[changey][changex] == ' ') {
                gameState.chicken_num++;
                if (gameState.chicken_num >= gameState.total_chicken) {
                    gameState.win = true;
                    return;  // Добавлено для немедленного выхода при победе
                }
            }

            if (Map[changey][changex] == '1' || Map[changey][changex] == '2' ||
                Map[changey][changex] == '3' || Map[changey][changex] == '4') {
                gameState.life = false;
                gameState.game_over = true;
                return;
            }

            Map[y][x] = 'V';
            Map[changey][changex] = 'P';
            x = changex;
            y = changey;
        }

        // Горизонтальный телепорт
        if (changey == 14 && (changex == 0 || changex == 28)) {
            changex = (changex == 0) ? 27 : 1;
            Map[y][x] = 'V';
            Map[changey][changex] = 'P';
            x = changex;
            y = changey;
        }
        // Вертикальный телепорт
        if (changex == 14 && (changey == 0 || changey == 28)) {
            changey = (changey == 0) ? 27 : 1;
            Map[y][x] = 'V';
            Map[changey][changex] = 'P';
            x = changex;
            y = changey;
        }
    }
};

class Enemy {
public:
    int x[4] = { 5, 23 , 5, 23 }, y[4] = { 4, 4, 24, 24 };
    int changex[4] = { 0 , 0 , 0, 0 }, changey[4] = { 0, 0, 0, 0 };
    int rotate[4] = { 1, 1, 1, 1 }, del = 0;

    void reset() {
        x[0] = 5; x[1] = 23; x[2] = 5; x[3] = 23;
        y[0] = 4; y[1] = 4; y[2] = 24; y[3] = 24;
        for (int i = 0; i < 4; i++) {
            changex[i] = 0;
            changey[i] = 0;
            rotate[i] = 1;
        }
        del = 0;
    }

    void changeE() {
        del++;
        if (del >= 100) {
            for (int i = 0; i < 4; i++) {
                rotate[i] = rand() % 4 + 1;
                changex[i] = x[i];
                changey[i] = y[i];

                switch (rotate[i]) {
                case 1: if (Map[y[i]][changex[i] + 1] != 'W') changex[i] += 1; break;
                case 2: if (Map[y[i]][changex[i] - 1] != 'W') changex[i] -= 1; break;
                case 3: if (Map[changey[i] - 1][x[i]] != 'W') changey[i] -= 1; break;
                case 4: if (Map[changey[i] + 1][x[i]] != 'W') changey[i] += 1; break;
                }
            }
            del = 0;
        }

        for (int i = 0; i < 4; i++) {
            if (Map[changey[i]][changex[i]] == ' ' || Map[changey[i]][changex[i]] == 'V' ||
                Map[changey[i]][changex[i]] == 'P') {

                if (Map[changey[i]][changex[i]] == 'V') Map[y[i]][x[i]] = 'V';
                else if (Map[changey[i]][changex[i]] == ' ') Map[y[i]][x[i]] = ' ';
                else if (Map[changey[i]][changex[i]] == 'P') {
                    gameState.life = false;
                    gameState.game_over = true;
                }

                Map[changey[i]][changex[i]] = '1' + i;
                x[i] = changex[i];
                y[i] = changey[i];
            }

            // Телепорты для врагов
            if (changey[i] == 14 && (changex[i] == 0 || changex[i] == 28)) {
                changex[i] = (changex[i] == 0) ? 27 : 1;
                Map[y[i]][x[i]] = 'V';
                Map[changey[i]][changex[i]] = '1' + i;
                x[i] = changex[i];
                y[i] = changey[i];
            }
            if (changex[i] == 14 && (changey[i] == 0 || changey[i] == 28)) {
                changey[i] = (changey[i] == 0) ? 27 : 1;
                Map[y[i]][x[i]] = 'V';
                Map[changey[i]][changex[i]] = '1' + i;
                x[i] = changex[i];
                y[i] = changey[i];
            }
        }
    }
};

void countTotalChicken() {
    gameState.total_chicken = 0;
    for (int i = 0; i < hight; i++) {
        for (int j = 0; j < width; j++) {
            if (Map[i][j] == ' ') {
                gameState.total_chicken++;
            }
        }
    }
}

void resetGame() {
    String newMap[hight] = {
        "WWWWWWWWWWWWWWVWWWWWWWWWWWWWW",
        "W            WVW            W",
        "W WWWWW WWWW VVV WWWW WWWWW W",
        "W W   W W    WVW    W W   W W",
        "W W W W WWWWWWVWWWWWW W W W W",
        "W   W   W           W   W   W",
        "WWW WWWWW WWWWWWWWW WWWWW WWW",
        "W         W       W         W",
        "W WWWWW WWWW W W WWWW WWWWW W",
        "W     W      W W      W     W",
        "WWWWW WWWWW WWWWW WWWWW WWWWW",
        "W     W               W     W",
        "W W WWW WWWW WVW WWWW WWW W W",
        "WWW W        WVW        W WWW",
        "VVV   WW WWW VPV WWW WW   VVV",
        "WWW W        WVW        W WWW",
        "W W WWW WWWW WVW WWWW WWW W W",
        "W     W               W     W",
        "WWWWW WWWWW WWWWW WWWWW WWWWW",
        "W     W      W W      W     W",
        "W WWWWW WWWW W W WWWW WWWWW W",
        "W         W       W         W",
        "WWW WWWWW WWWWWWWWW WWWWW WWW",
        "W   W   W           W   W   W",
        "W W W W WWWWWWVWWWWWW W W W W",
        "W W   W W    WVW    W W   W W",
        "W WWWWW WWWW VVV WWWW WWWWW W",
        "W            WVW            W",
        "WWWWWWWWWWWWWWVWWWWWWWWWWWWWW"
    };

    for (int i = 0; i < hight; i++) {
        Map[i] = newMap[i];
    }

    gameState.chicken_num = 0;
    countTotalChicken();
    gameState.life = true;
    gameState.game_over = false;
    gameState.win = false;
}

int main() {
    srand(time(0));
    RenderWindow window(VideoMode(width * block, hight * block), "Pacman");

    // Загрузка текстур
    Texture w, pac, c, e, win_t, lose_t, teleport_left, teleport_right, teleport_top, teleport_bottom;
    w.loadFromFile("C:/Users/matve/source/repos/Pac/Paint/BrickWall2.png");
    pac.loadFromFile("C:/Users/matve/source/repos/Pac/Paint/pacman.png");
    c.loadFromFile("C:/Users/matve/source/repos/Pac/Paint/Chicken.png");
    e.loadFromFile("C:/Users/matve/source/repos/Pac/Paint/Enemy.png");
    win_t.loadFromFile("C:/Users/matve/source/repos/Pac/Paint/Win1.png");
    lose_t.loadFromFile("C:/Users/matve/source/repos/Pac/Paint/Lose1.png");
    teleport_left.loadFromFile("C:/Users/matve/source/repos/Pac/Paint/YellowPortal1.png");
    teleport_right.loadFromFile("C:/Users/matve/source/repos/Pac/Paint/YellowPortal2.png");
    teleport_top.loadFromFile("C:/Users/matve/source/repos/Pac/Paint/BluePortal1.png");
    teleport_bottom.loadFromFile("C:/Users/matve/source/repos/Pac/Paint/BluePortal2.png");

    Sprite wall(w), pacman(pac), chicken(c), enemy(e), win(win_t), lose(lose_t);
    Sprite left_portal(teleport_left), right_portal(teleport_right),
        top_portal(teleport_top), bottom_portal(teleport_bottom);

    // Установка размеров для спрайтов победы и поражения
    win.setScale((float)(width * block) / win.getLocalBounds().width,
        (float)(hight * block) / win.getLocalBounds().height);
    lose.setScale((float)(width * block) / lose.getLocalBounds().width,
        (float)(hight * block) / lose.getLocalBounds().height);

    left_portal.setPosition(0, 14 * block);
    right_portal.setPosition(28 * block, 14 * block);
    top_portal.setPosition(14 * block, 0);
    bottom_portal.setPosition(14 * block, 28 * block);

    Player pl;
    Enemy en;
    countTotalChicken();  // Подсчет кусочков курицы при запуске

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Escape)
                    window.close();

                // Перезапуск игры при нажатии Enter
                if ((gameState.game_over || gameState.win) && event.key.code == Keyboard::Enter) {
                    resetGame();
                    pl.reset();
                    en.reset();
                }

                // Управление игроком
                if (gameState.life && !gameState.game_over && !gameState.win) {
                    pl.changex = pl.x;
                    pl.changey = pl.y;

                    if (event.key.code == Keyboard::D || event.key.code == Keyboard::Right) pl.rotate = 0;
                    if (event.key.code == Keyboard::A || event.key.code == Keyboard::Left) pl.rotate = 1;
                    if (event.key.code == Keyboard::W || event.key.code == Keyboard::Up) pl.rotate = 2;
                    if (event.key.code == Keyboard::S || event.key.code == Keyboard::Down) pl.rotate = 3;
                }
            }
        }

        // Игровая логика
        if (gameState.life && !gameState.game_over && !gameState.win) {
            pl.changeP();
            en.changeE();
        }

        // Отрисовка
        window.clear(Color::Black);

        // Фон (телепорты)
        window.draw(left_portal);
        window.draw(right_portal);
        window.draw(top_portal);
        window.draw(bottom_portal);

        // Стены
        for (int i = 0; i < hight; i++)
            for (int j = 0; j < width; j++)
                if (Map[i][j] == 'W') {
                    wall.setPosition(j * block, i * block);
                    window.draw(wall);
                }

        // Курица
        for (int i = 0; i < hight; i++)
            for (int j = 0; j < width; j++)
                if (Map[i][j] == ' ') {
                    chicken.setPosition(j * block, i * block);
                    window.draw(chicken);
                }

        // Враги
        for (int i = 0; i < hight; i++)
            for (int j = 0; j < width; j++)
                if (Map[i][j] >= '1' && Map[i][j] <= '4') {
                    int idx = Map[i][j] - '1';
                    enemy.setTextureRect(IntRect(block * en.rotate[idx], 0, block, block));
                    enemy.setPosition(j * block, i * block);
                    window.draw(enemy);
                }

        // Игрок - пакман
        if (Map[pl.y][pl.x] == 'P') {
            pacman.setTextureRect(IntRect(block * int(pl.frame), block * pl.rotate, block, block));
            pacman.setPosition(pl.x * block, pl.y * block);
            window.draw(pacman);
        }

        // Сообщения о победе/поражении
        if (gameState.win) {
            window.draw(win);
        }
        else if (gameState.game_over) {
            window.draw(lose);
        }

        window.display();
    }

    return 0;
}