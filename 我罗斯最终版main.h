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