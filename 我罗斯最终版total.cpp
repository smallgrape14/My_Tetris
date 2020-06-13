#include <iostream>
#include <windows.h>
#include<string>
#include<ctime>
#include<cstdlib>
#include<conio.h>
#include<stdio.h>
#include<easyx.h>
#include <graphics.h>
#include "resource.h"
#include<thread>

#pragma comment(lib,"Winmm.lib")  //这个决不能少
#define x1 0
#define y1 0
#define x2 48
#define y2 0
using namespace std;

int block0[21][4][4] = { {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}},
{ { 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 } },
{ { 2,0,0,0 },{ 1,1,1,1 },{ 0,0,0,0 },{ 0,0,0,0 } },
{ { 3,0,1,0 },{ 0,0,1,0 },{ 0,0,1,0 },{ 0,0,1,0 } },
{ { 4,0,0,0 },{ 0,0,0,0 },{ 1,1,1,0 },{ 0,1,0,0 } },
{ { 5,0,0,0 },{ 0,1,0,0 },{ 1,1,0,0 },{ 0,1,0,0 } },
{ { 6,0,0,0 },{ 0,0,0,0 },{ 0,1,0,0 },{ 1,1,1,0 } },
{ { 7,0,0,0 },{ 0,1,0,0 },{ 0,1,1,0 },{ 0,1,0,0 } },
{ { 8,0,0,0 },{ 0,0,0,0 },{ 1,1,1,0 },{ 1,0,0,0 } },
{ { 9,0,0,0 },{ 1,1,0,0 },{ 0,1,0,0 },{ 0,1,0,0 } },
{ { 10,0,0,0 },{ 0,0,0,0 },{ 0,0,1,0 },{ 1,1,1,0 } },
{ { 11,0,0,0 },{ 0,1,0,0 },{ 0,1,0,0 },{ 0,1,1,0 } },
{ { 12,0,0,0 },{ 0,0,0,0 },{ 1,1,1,0 },{ 0,0,1,0 } },
{ { 13,0,0,0 },{ 0,1,0,0 },{ 0,1,0,0 },{ 1,1,0,0 } },
{ { 14,0,0,0 },{ 0,0,0,0 },{ 1,0,0,0 },{ 1,1,1,0 } },
{ { 15,0,0,0 },{ 0,1,1,0 },{ 0,1,0,0 },{ 0,1,0,0 } },
{ { 16,0,0,0 },{ 0,0,0,0 },{ 0,1,1,0 },{ 1,1,0,0 } },
{ { 17,0,0,0 },{ 1,0,0,0 },{ 1,1,0,0 },{ 0,1,0,0 } },
{ { 18,0,0,0 },{ 0,0,0,0 },{ 1,1,0,0 },{ 0,1,1,0 } },
{ { 19,0,0,0 },{ 0,0,1,0 },{ 0,1,1,0 },{ 0,1,0,0 } },
{ { 20,0,0,0 },{ 0,0,0,0 },{ 1,1,0,0 },{ 1,1,0,0 } } };

HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
void SetPos(int x, int y);//移动光标
class Block
{
private:
    int feature[4][4];
public:
};

class Player
{
private:
    int x0, y0, num, newblock[4][4], nextblock[4][4], type, check;
    Block block;
    int map[21][12], OriginX, OriginY;
public:
    Player(int xx, int yy, int t);

    void randBlock();
    void printBlock(int block[4][4], int a, int b);//打印 
    void clearBlock(int block[4][4], int a, int b);

    int BlockDown(int y);
    void BlockLeft(int x);
    void BlockRight(int x);
    void BlockUp(int x);

    void Eliminate_Row_Add_Row(int& val, int map2[21][12]);
    void PrintScreen();//打印窗口
    bool Detect_Collision(int block[4][4], int x, int y);
    bool step1(int& val, Player& opponent);
    int step3();
    void setMap();

    friend class Game;
};
class Game
{
public:
    void Move();
    void SetWindow();//初始界面 
    void GameOver(Player& player, Player& opponent);
    void GamePause();
    int step2(Player& player1, Player& player2);
};
void music()
{
    while (1)
    {
        PlaySound(LPWSTR(IDR_WAVE2), GetModuleHandle(NULL), SND_RESOURCE | SND_LOOP);
        PlaySound(LPWSTR(IDR_WAVE3), GetModuleHandle(NULL), SND_RESOURCE | SND_LOOP);
        PlaySound(LPWSTR(IDR_WAVE4), GetModuleHandle(NULL), SND_RESOURCE | SND_LOOP);
        PlaySound(LPWSTR(IDR_WAVE5), GetModuleHandle(NULL), SND_RESOURCE | SND_LOOP);
    }

}
int main()
{
    //	SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    Game game;
    game.SetWindow();
    initgraph(840, 560);

    IMAGE a;
    loadimage(&a, L"jpg", MAKEINTRESOURCE(IDB_BITMAP1));
    putimage(0, 0, &a);
    _getch();
    closegraph();
    //
    thread t1(music);
    thread t2((&Game::Move), game);
    t1.join();
    t2.join();
    return 0;
}

Player::Player(int xx, int yy, int t)
{

    x0 = xx;
    y0 = yy;
    type = t;

}
void Player::setMap()
{
    for (int i = 0; i <=19; i++)
    {
        for (int j = 1; j <= 10; j++)
        {
            map[i][j] = 0;

        }
    }
    for (int i = 0; i <= 19; i++)
    {
        map[i][0] = map[i][11] = 1;

    }
    for (int i = 0; i <= 10; i++)
    {
        map[20][i] = 1;

    }
}

void Player::randBlock()
{
    clearBlock(nextblock, 4, 15);
    int m, i;
    m = rand() % 19;
    for (i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            nextblock[i][j] = block0[m + 2][i][j];
        }
    }
    printBlock(nextblock, 4, 15);
}

void Player::PrintScreen()//打印窗口
{
    SetPos(x0 + 8, y0 + 23);
    cout << "Player" << type;
    SetPos(x0, y0);
    for (int i = 0; i < 20; i++)
    {
        SetPos(x0, y0 + i);
        cout << "◆                    ◆                    ◆" << endl;
    }
    SetPos(x0 + 0, y0 + 20);
    cout << "◆◆◆◆◆◆◆◆◆◆◆◆";
    SetPos(x0 + 24, y0);
    cout << "☆☆☆☆☆☆☆☆☆☆◆";
    SetPos(x0 + 24, y0 + 20);
    cout << "☆☆☆☆☆☆☆☆☆☆◆";
    //    SetPos(hOut, x0+26, y0+1);
    //    cout << "分    数：      ";
    SetPos(x0 + 26, y0 + 3);
    cout << "下一方块：";
    SetPos(x0 + 26, y0 + 11);
    cout << "操作方法:";
    if (type == 1)
    {
        SetPos(x0 + 26, y0 + 12);
        cout << "旋转：w";
        SetPos(x0 + 26, y0 + 13);
        cout << "左移：a";
        SetPos(x0 + 26, y0 + 14);
        cout << "右移：d";
        SetPos(x0 + 26, y0 + 15);
        cout << "加速：s";
    }
    else
    {
        SetPos(x0 + 26, y0 + 12);
        cout << "旋转：↑";
        SetPos(x0 + 26, y0 + 13);
        cout << "左移：←";
        SetPos(x0 + 26, y0 + 14);
        cout << "右移：→";
        SetPos(x0 + 26, y0 + 15);
        cout << "加速：↓";
    }
    SetPos(x0 + 26, y0 + 16);
    cout << "空格键暂停";
    SetPos(x0 + 26, y0 + 17);
    cout << "Esc键退出";
}

void Game::SetWindow()
{
    SetConsoleTitle(L"俄罗斯方块");
    COORD size = { 100, 30 };
    SetConsoleScreenBufferSize(hOut, size);
    SMALL_RECT rc = { 0, 0, 99, 30 };
    SetConsoleWindowInfo(hOut, true, &rc);
    CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };
    SetConsoleCursorInfo(hOut, &cursor_info);
    system("color 7e");
}
void SetPos(int x, int y)//移动光标
{
    COORD pos = { x,y };
    SetConsoleCursorPosition(hOut, pos);
}

void Player::clearBlock(int block[4][4], int a, int b)
{
    for (int i = 0; i < 4; i++)
    {
        if (i + a >= 0)
        {
            for (int j = 0; j < 4; j++)
            {
                if (block[i][j] == 1)
                {
                    SetPos(2 * (x0 / 2 + b + j), y0 + a + i);
                    SetConsoleTextAttribute(hOut, BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN);
                    cout << "  ";
                }
            }
        }
    }
}
void Game::GameOver(Player& player, Player& opponent)
{
    SetConsoleTextAttribute(hOut, BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
    SetPos(player.x0 + 10, 8);
    cout << "LOSE";
    SetPos(48 - player.x0 + 10, 8);
    cout << "WIN";
    SetConsoleTextAttribute(hOut, BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
    SetPos(38, 22);
    cout << "空格键重新开始";
    SetPos(40, 23);
    cout << "Esc键退出";
    char key;
    while (true)
    {
        key = _getch();
        if (key == 32)
        {
            SetConsoleTextAttribute(hOut, BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
            player.PrintScreen();
            opponent.PrintScreen();
            return;
        }
        if (key == 27)
        {
            SetPos(0, 25);
            exit(0);
        }
    }
}

int Player::BlockDown(int y)
{
    if (Detect_Collision(newblock, OriginX + 1, y))
    {
        clearBlock(newblock, OriginX, y);
        OriginX++;
        return 0;
    }
    if (OriginX < 0)
    {
        return 2;
    }
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (newblock[i][j] == 1)
            {
                map[OriginX + i][y + j] = 1;
                SetConsoleTextAttribute(hOut, BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
                SetPos(2 * (y + x0 / 2 + j), y0 + OriginX + i);
                cout << "█";
            }
        }
    }
    return 1;
}

void Player::BlockLeft(int x)
{
    if (Detect_Collision(newblock, x, OriginY - 1))
    {
        clearBlock(newblock, x, OriginY);
        OriginY--;
    }
}

void Player::BlockRight(int x)
{
    if (Detect_Collision(newblock, x, OriginY + 1))
    {
        clearBlock(newblock, x, OriginY);
        OriginY++;
    }
}

void Player::BlockUp(int x)
{
    int k1[5] = { 0,-1,1,-2,2 }, k2[3] = { 0,-1,1 };
    if (newblock[0][0] == 2)
    {
        if (Detect_Collision(block0[3], x, OriginY))
        {
            clearBlock(newblock, x, OriginY);
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    newblock[i][j] = block0[3][i][j];
                }
            }
        }
    }
    else if (newblock[0][0] == 3)
    {
        int e;
        for (e = 0; e <= 4; e++)
        {
            if (Detect_Collision(block0[2], x, OriginY + k1[e]))
            {
                clearBlock(newblock, x, OriginY);
                for (int i = 0; i < 4; i++)
                {
                    for (int j = 0; j < 4; j++)
                    {
                        newblock[i][j] = block0[2][i][j];
                    }
                }
                OriginY += k1[e];
                break;
            }
        }
    }
    else if (newblock[0][0] == 20);
    else
    {
        int e, k;
        if (newblock[0][0] == 17 || newblock[0][0] == 19)
            k = newblock[0][0] - 1;
        else if ((newblock[0][0] % 4) < 3)
            k = newblock[0][0] + 1;
        else
            k = newblock[0][0] - 3;
        for (e = 0; e < 3; e++)
        {
            if (Detect_Collision(block0[k], x, OriginY + k2[e]))
            {
                clearBlock(newblock, x, OriginY);
                for (int i = 0; i < 4; i++)
                {
                    for (int j = 0; j < 4; j++)
                    {
                        newblock[i][j] = block0[k][i][j];
                    }
                }
                OriginY += k2[e];
                break;
            }
        }
    }

}
void Player::printBlock(int block[4][4], int a, int b)
{
    switch (block[0][0])
    {
    case 2: case 3:
        SetConsoleTextAttribute(hOut, BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        break;
    case 4: case 5:case 6: case 7:
        SetConsoleTextAttribute(hOut, BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
        break;
    case 8:case 9: case 10: case 11:
        SetConsoleTextAttribute(hOut, BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        break;
    case 12: case 13: case 14:case 15:
        SetConsoleTextAttribute(hOut, BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        break;
    case 16: case 17:
        SetConsoleTextAttribute(hOut, BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        break;
    case 18: case 19:
        SetConsoleTextAttribute(hOut, BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        break;
    case 20:
        SetConsoleTextAttribute(hOut, BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
        break;

    }
    for (int i = 0; i < 4; i++)
    {
        if (i + a >= 0)//OriginX, OriginY
        {
            for (int j = 0; j < 4; j++)
            {
                if (block[i][j] == 1)
                {
                    SetPos(2 * (x0 / 2 + b + j), y0 + a + i);
                    cout << "█";
                }
            }
        }
    }
}
void Game::GamePause()
{
    SetConsoleTextAttribute(hOut, BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    SetPos(44, 22);
    cout << "游戏暂停";
    char key;
    while (true)
    {
        key = _getch();
        if (key == 32)
        {
            SetPos(44, 22);
            cout << "        ";
            return;
        }
        if (key == 27)
        {
            exit(0);
        }
    }
}

void Player::Eliminate_Row_Add_Row(int& val, int map2[21][12])
{
    int checking = 0;
    SetConsoleTextAttribute(hOut, BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
    int i, j, m, n, x, f, k;
    for (i = 19; i >= 0; i--)
    {
        x = 0;
        for (j = 1; j < 11; j++)
        {
            x += map[i][j];
        }

        if (x == 10)
        {
            checking = 1;
            for (m = i; m > 0; m--)
            {
                for (n = 1; n < 11; n++)
                {
                    map[m][n] = map[m - 1][n];
                    SetPos(2 * (n + x0 / 2), y0 + m);
                    if (map[m][n] == 1)
                    {
                        cout << "█";
                    }
                    else
                    {
                        cout << "  ";
                    }
                }
            }
            i++;
        }
    }
    if (checking == 1)
    {
        for (i = 0; i < 20; i++)
        {
            f = 0;
            for (j = 1; j < 11; j++)
            {
                if (map2[i][j] != 0)
                {
                    f = 1;
                }
                if (f == 1)
                    break;
            }
            if (f == 0)break;
        }
        int y = i;
        for (m = i; m < 19; m++)
        {
            for (j = 1; j < 11; j++)
            {
                map2[m][j] = map2[m + 1][j];
            }
        }
        map2[19][1] = 1;
        map2[19][10] = 0;
        for (i = 2; i < 10; i++)
        {
            k = rand() % 2;
            map2[19][i] = k;
        }
        for (i = y; i < 20; i++)
        {
            for (j = 1; j < 11; j++)
            {
                SetPos(2 * (j + (48 - x0) / 2), y0 + i);
                if (map2[i][j] == 1)
                {
                    cout << "█";
                }
                else
                {
                    cout << "  ";
                }
            }
        }
    }

}
bool Player::step1(int& val, Player& opponent)
{
    if (check)
    {
        Eliminate_Row_Add_Row(val, opponent.map);
        check = false;
        OriginX = -3;
        OriginY = 4;
        if (Detect_Collision(nextblock, OriginX, OriginY))
        {
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    newblock[i][j] = nextblock[i][j];
                }
            }
            randBlock();

            return false;
        }
        else
        {
            return true;
            // goto initial;

        }
    }
    else return false;

}
int Game::step2(Player& player1, Player& player2)
{
    int flag = 0;
    if (_kbhit())
    {
        switch (_getch())
        {
        case 'w':case 'W':
            player1.BlockUp(player1.OriginX);
            break;
        case 'a':case 'A':
            player1.BlockLeft(player1.OriginX);
            break;
        case 'd':case 'D':
            player1.BlockRight(player1.OriginX);
            break;
        case 's':case 'S':
            switch (player1.BlockDown(player1.OriginY))
            {
            case 0:
                player1.check = false;
                break;
            case 1:
                player1.check = true;
                break;
            case 2:
                GameOver(player1, player2);
                flag = 1;
                //goto initial;
            }
            break;
        case 72:
            player2.BlockUp(player2.OriginX);
            break;
        case 75:
            player2.BlockLeft(player2.OriginX);
            break;
        case 77:
            player2.BlockRight(player2.OriginX);
            break;
        case 80:
            switch (player2.BlockDown(player2.OriginY))
            {
            case 0:
                player2.check = false;
                break;
            case 1:
                player2.check = true;
                break;
            case 2:
                GameOver(player2, player1);
                flag = 1;
                //goto initial;
            default:
                break;
            }
            break;
        case 32:
            GamePause();
            break;
        case 27:
            SetPos(0, 25);
            exit(0);
        default:
            break;
        }
    }
    return flag;
}
int Player::step3()
{
    switch (BlockDown(OriginY))
    {
    case 0:
        check = false;

        break;
    case 1:
        check = true;

        break;
    case 2:
        return 1;

    }
    return 0;
}
void Game::Move()
{
    //Player player1(x1, y1, 1), player2(x2, y2, 2);
    //player1.PrintScreen();
    //player2.PrintScreen();
    char key;//用来存储按键
    int val;//用来控制下落速度
    int times;
initial:
    SetConsoleTextAttribute(hOut, BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
    Player player1(x1, y1, 1), player2(x2, y2, 2);
    player1.PrintScreen();
    player2.PrintScreen();
    SetPos(38, 22);
    cout << "               ";
    SetPos(40, 23);
    cout << "         ";
    player1.check = player2.check = true;
    val = 50;
    times = val;
    player1.setMap();
    player2.setMap();
    srand((unsigned)time(NULL));
    player1.randBlock();
    player2.randBlock();
    while (true)
    {
        if (player1.step1(val, player2))
        {
            GameOver(player1, player2);
            goto initial;

        }

        player1.printBlock(player1.newblock, player1.OriginX, player1.OriginY);

        if (player2.step1(val, player1))
        {
            GameOver(player2, player1);
            goto initial;
        }

        player2.printBlock(player2.newblock, player2.OriginX, player2.OriginY);

        if (step2(player1, player2))
            goto initial;

        Sleep(10);
        if (0 == (--times))
        {
            if (player1.step3())
            {
                GameOver(player1, player2);
                goto initial;
            }

            if (player2.step3())
            {
                GameOver(player2, player1);
                goto initial;
            }

            times = val;
        }

    }
    cin.get();
}

bool Player::Detect_Collision(int block[4][4], int x, int y)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (x + i >= 0 && y + j >= 0 && map[x + i][y + j] && block[i][j] == 1)
            {

                return false;
            }
        }
    }

    return true;
}
