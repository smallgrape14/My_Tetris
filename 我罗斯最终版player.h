class Player
{
private:
    int x0, y0, num, newblock[4][4], nextblock[4][4], type, check;
    Block block;
    int map[21][12], OriginX, OriginY;
public:
    Player(int xx, int yy, int t);
    void setMap();
    void randBlock();
    void PrintScreen();//打印窗口
    void clearBlock(int block[4][4], int a, int b);
    int BlockDown(int y);
    void BlockLeft(int x);
    void BlockRight(int x);
    void BlockUp(int x);
    void printBlock(int block[4][4], int a, int b);//打印 
    void Eliminate_Row_Add_Row(int& val, int map2[21][12]);
    bool step1(int& val, Player& opponent);
    int step3();
    bool Detect_Collision(int block[4][4], int x, int y);
    friend class Game;
};
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
        }
    }
    else return false;

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