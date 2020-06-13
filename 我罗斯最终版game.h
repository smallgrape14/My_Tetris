class Game
{
public:
    void Move();
    void SetWindow();//初始界面 
    void GameOver(Player& player, Player& opponent);
    void GamePause();
    int step2(Player& player1, Player& player2);
};
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
void Game::Move()
{
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