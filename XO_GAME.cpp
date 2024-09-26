#include <iostream>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <chrono>
#include <thread>
#include <fstream>
#include <iomanip>

using namespace std;

HWND console = GetConsoleWindow(); //For control the console window
HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE); //For control the output
COORD cursor_position; //For control the cursor position

struct playars
{
    char XO;
    string name;
    int **map;
}
playar[2];

struct save_game
{
    int game_mode;
    int game_size;
    char **map;
    int i, j, k;
    playars ps[2];
    int AILevel;
    int movments;
}
saved_game;

void set_cursor_pos(int x, int y)
{
    cursor_position.X = x;
    cursor_position.Y = y;
    SetConsoleCursorPosition(output, cursor_position);
}

void choose_option_error()
{
    set_cursor_pos(31,1);
    printf("\x1b[41mPlease select one of the available options\x1b[0m");
}

void game_history()
{
    system("cls");
    ifstream gamehistory;
    gamehistory.open("game_history.txt");
    string line;
    for(int i = 0; i < 10; i++)
    {
        if(gamehistory >> line)
        {
            set_cursor_pos(20,3 + i);
            cout << i + 1 << ". \x1b[32m" << "Game between " << "\x1b[31m" << line << "\x1b[0m";
            gamehistory >> line;
            cout << " & " << "\x1b[31m" << line << "\x1b[0m";
            gamehistory >> line;
            if(line == "EQUAL")
                cout << "\x1b[36m" << "       the result was " << "\x1b[35m" << line << "\x1b[0m";
            else
                cout << "\x1b[36m" << "       the winner was " << "\x1b[35m" << line << "\x1b[0m";
        }
    }
    set_cursor_pos(0,0);
    cout << "1.First page\n";
    char c;
    do
    {
        c = getch();
        if(c != '1')
            choose_option_error();
    } while(c != '1');

}

bool check_full(int a, int b)
{
    if(saved_game.map[a][b] == ' ')
        return true;
    return false;
}

string color(int i, int j)
{
    string colors[2] = {"\x1b[33m", "\x1b[36m"};
    if(playar[0].map[i][j] == 1)
        return colors[0];
    return colors[1];
}

void reload_page(int a, int b, int c, bool &check2)
{
    check2 = true;
    saved_game.i = a;
    saved_game.j = b;
    saved_game.k = c;
    system("cls");
    set_cursor_pos(0,0);
    cout << "1.First page\n";
    set_cursor_pos(45 - playar[c % 2].name.length() / 2, 3);
    cout << "\x1b[32m" << playar[c % 2].name << "!" << "\x1b[0m" << " Its your turn!";
    bool check = check_full(a,b);
    for(int i = 0; i < saved_game.game_size; i++)
    {

        set_cursor_pos(51 - saved_game.game_size / 2, i * 2 + 5);
        for(int j = 0; j < saved_game.game_size; j++)
        {
            if(i == a && j == b)
            {
                set_cursor_pos(51 - saved_game.game_size / 2 + j * 2, i * 2 + 5);
                if(check)
                    printf("\x1b[47;30m%c\x1b[0m", playar[c % 2].XO);
                else
                {
                    printf("\x1b[47;30m*\x1b[0m");
                    set_cursor_pos(40,4);
                    printf("\x1b[31mThis cell is already full!\x1b[0m");
                    check2 = false;
                    set_cursor_pos(51 - saved_game.game_size / 2 + j * 2 + 1, i * 2 + 5);
                }
            }
            else
                cout << color(i, j) << saved_game.map[i][j] << "\x1b[0m";
            if(j != saved_game.game_size - 1)
                cout << "\x1b[31m" << "|" << "\x1b[0m";
        }
        set_cursor_pos(51 - saved_game.game_size / 2 - 1, i * 2 + 6);
        if(i != saved_game.game_size - 1)
            for(int j = 0; j <= saved_game.game_size * 2; j++)
                cout << "\x1b[31m" << "-" << "\x1b[0m";
    }
    cout << endl;
}

void solo_game()
{
    system("cls");
    set_cursor_pos(35,7);
    cout << " Please enter your name:";
    cin >> playar[0].name;
    set_cursor_pos(35,8);
    cout << " Please choose your character:";
    set_cursor_pos(35,9);
    printf("\x1b[33m1.X\t2.O\x1b[0m");
    char input;
    do
    {
        input = getch();
        switch(input)
        {
        case '1':
        {
            playar[0].XO = 'X';
            playar[1].XO = 'O';
        }
        break;
        case '2':
        {
            playar[0].XO = 'O';
            playar[1].XO = 'X';
        }
        break;
        default:
            choose_option_error();
        }
    }
    while(input != '1' && input != '2');
    set_cursor_pos(35,10);
    cout << " Please choose the computer level:";
    set_cursor_pos(35,11);
    printf("\x1b[33m1.Easy\t2.Medium\t3.Hard\x1b[0m");
    do
    {
        input = getch();
        switch(input)
        {
        case '1':
            saved_game.AILevel = 1;
            break;
        case '2':
            saved_game.AILevel = 2;
            break;
        case '3':
            saved_game.AILevel = 3;
            break;
        default:
            choose_option_error();
        }
    }
    while(input != '1' && input != '2' && input != '3');
    playar[1].name = "Computer";
    int gamesize = 3;
    saved_game.game_size = gamesize;
    saved_game.map = new char*[gamesize];
    for(int i = 0; i < gamesize; i++)
        saved_game.map[i] = new char[gamesize];
    for(int i = 0; i < gamesize; i++)
        for(int j = 0; j < gamesize; j++)
            saved_game.map[i][j] = ' ';
    playar[0].map = new int*[gamesize];
    for(int i = 0; i < gamesize; i++)
        playar[0].map[i] = new int[gamesize];
    for(int i = 0; i < gamesize; i++)
        for(int j = 0; j < gamesize; j++)
            playar[0].map[i][j] = 0;
    playar[1].map = new int*[gamesize];
    for(int i = 0; i < gamesize; i++)
        playar[1].map[i] = new int[gamesize];
    for(int i = 0; i < gamesize; i++)
        for(int j = 0; j < gamesize; j++)
            playar[1].map[i][j] = 0;
    saved_game.ps[0] = playar[0];
    saved_game.ps[1] = playar[1];
    PlaySound(0, 0, 0);
}

void dou_game()
{
    system("cls");
    saved_game.AILevel = 0;
    set_cursor_pos(35,7);
    cout << "\x1b[32m" << "Playar 1!" << "\x1b[0m" << " Please enter your name:";
    cin >> playar[0].name;
    set_cursor_pos(35,8);
    cout << "\x1b[32m" << "Playar 1!" << "\x1b[0m" << " Please choose your character:";
    set_cursor_pos(35,9);
    printf("\x1b[33m1.X\t2.O\x1b[0m");
    char input;
    do
    {
        input = getch();
        switch(input)
        {
        case '1':
        {
            playar[0].XO = 'X';
            playar[1].XO = 'O';
        }
        break;
        case '2':
        {
            playar[0].XO = 'O';
            playar[1].XO = 'X';
        }
        break;
        default:
            choose_option_error();
        }
    }
    while(input != '1' && input != '2');
    set_cursor_pos(35,10);
    cout << "\x1b[32m" << "Playar 2!" << "\x1b[0m" << " Please enter your name:";
    cin >> playar[1].name;
    int gamesize;
    while(true)
    {
        set_cursor_pos(35,11);
        printf("Please enter the game size \x1b[33mbetween 3 and 10:\x1b[0m");
        scanf("%d", &gamesize);
        if(gamesize >= 3 && gamesize <= 10)
        {
            saved_game.game_size = gamesize;
            break;
        }
        else
            choose_option_error();
    }
    saved_game.map = new char*[gamesize];
    for(int i = 0; i < gamesize; i++)
        saved_game.map[i] = new char[gamesize];
    for(int i = 0; i < gamesize; i++)
        for(int j = 0; j < gamesize; j++)
            saved_game.map[i][j] = ' ';
    playar[0].map = new int*[gamesize];
    for(int i = 0; i < gamesize; i++)
        playar[0].map[i] = new int[gamesize];
    for(int i = 0; i < gamesize; i++)
        for(int j = 0; j < gamesize; j++)
            playar[0].map[i][j] = 0;
    playar[1].map = new int*[gamesize];
    for(int i = 0; i < gamesize; i++)
        playar[1].map[i] = new int[gamesize];
    for(int i = 0; i < gamesize; i++)
        for(int j = 0; j < gamesize; j++)
            playar[1].map[i][j] = 0;
    saved_game.ps[0] = playar[0];
    saved_game.ps[1] = playar[1];
    PlaySound(0, 0, 0);
}

int new_game()
{
    saved_game.movments = 0;
    system("cls");
    set_cursor_pos(41,5);
    printf("\x1b[36mChoose the game mode\x1b[0m");
    set_cursor_pos(45,7);
    printf("\x1b[32m1.Solo Game\x1b[0m\n");
    set_cursor_pos(45,8);
    printf("\x1b[32m2.Dou Game\x1b[0m\n");
    set_cursor_pos(45,9);
    printf("\x1b[32m3.First page\x1b[0m\n");
    char input;
    do
    {
        input = getch();
        switch(input)
        {
        case '1':
        {
            solo_game();
            saved_game.game_mode = 1;
        }
        break;
        case '2':
        {
            dou_game();
            saved_game.game_mode = 2;
        }
        break;
        case '3':
        {
            return 3;
        }
        break;
        default:
            choose_option_error();
        }
    }
    while(input != '1' && input != '2');
    return 4;
}

bool check_continue()
{
    ifstream infile;
    infile.open("saved_game.txt");
    string temp;
    infile >> temp;
    if(temp == "continue")
        return true;
    infile.close();
    return false;
}

int first_page()
{
    PlaySound(TEXT("start.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    system("cls");
    bool checkcontinue = check_continue();
    int i = 0;
    if(checkcontinue)
    {
        i = 1;
        set_cursor_pos(43,6);
        cout << "\x1b[32m1.Continue Game\x1b[0m";
    }
    MoveWindow(console,350,300,750,500,TRUE);
    set_cursor_pos(41,5);
    printf("\x1b[5;31mWELCOME TO XO GAME!\x1b[0m");
    set_cursor_pos(45,7);
    cout << "\x1b[32m" << i + 1 << ".New Game\x1b[0m";
    set_cursor_pos(43,8);
    cout << "\x1b[32m" << i + 2 << ".Game History\x1b[0m";
    set_cursor_pos(47,9);
    cout << "\x1b[32m" << i + 3 << ".Exit\x1b[0m\n";
    char input;
    do
    {
        input = getch();
        switch(input - '0' - i)
        {
        case 0:
        {
            if(i == 0)
                choose_option_error();
            else
                return 0;
        }
        break;
        case 1:
            return new_game();
            break;
        case 2:
        {
            game_history();
            return 2;
        }
        break;
        case 3:
            return -1;
            break;
        default:
            choose_option_error();
        }
    }
    while(input != '1' && input != '2' && input != '3');
    return 1;
}

void set_xo(int i, int j, int c)
{
    playar[c % 2].map[i][j] = 1;
    saved_game.map[i][j] = playar[c % 2].XO;
    saved_game.movments++;
    PlaySound(TEXT("set.wav"), NULL, SND_FILENAME | SND_ASYNC);
}

bool check_win(int **map)
{
    int i = 0, j = 0;
    for(i = 0; i < saved_game.game_size; i++)
    {
        for(j = 0; j < saved_game.game_size; j++)
            if(map[i][j] != 1)
                break;
        if(j == saved_game.game_size)
            return true;
    }
    for(i = 0; i < saved_game.game_size; i++)
    {
        for(j = 0; j < saved_game.game_size; j++)
            if(map[j][i] != 1)
                break;
        if(j == saved_game.game_size)
            return true;
    }
    for(i = 0 , j = 0; j < saved_game.game_size; i++ , j++)
        if(map[i][j] != 1)
            break;
    if(i == saved_game.game_size)
        return true;
    for(i = 0 , j = saved_game.game_size - 1; i < saved_game.game_size; i++ , j--)
        if(map[i][j] != 1)
            break;
    if(i == saved_game.game_size)
        return true;
    return false;
}

bool check_equal()
{
    int i = 0, j = 0;
    for(i = 0; i < saved_game.game_size; i++)
        for(j = 0; j < saved_game.game_size; j++)
            if(saved_game.map[i][j] == ' ')
                return false;
    return true;
}

int check_end_game()
{
    if(check_win(playar[0].map))
        return 1;
    if(check_win(playar[1].map))
        return 2;
    if(check_equal())
        return 0;
    return -1;
}

void equal_page()
{
    system("cls");
    set_cursor_pos(50,7);
    cout << "\x1b[5;36;1;3mEQUAL!\x1b[0m\n" ;
    PlaySound(TEXT("equal.wav"), NULL, SND_FILENAME | SND_ASYNC);
}

void congratulations_page(playars p)
{
    PlaySound(TEXT("win.wav"), NULL, SND_FILENAME | SND_ASYNC);
    system("cls");
    set_cursor_pos(40,5);
    cout << "\x1b[1;35;5mCongratulations " << p.name << "!\x1b[0m\n";
    for(int i = 0; i < 2; i++)
    {
        system("color 94");
        sleep(1);
        system("color D0");
        sleep(1);
    }
    system("color 0F");
    set_cursor_pos(40,5);
    cout << "\x1b[1;35;5mCongratulations " << p.name << "!\x1b[0m\n";
}

void save_history()
{
    fstream tempfile;
    fstream gamehistory;
    gamehistory.open("game_history.txt");
    tempfile.open("temp.txt");
    tempfile <<  playar[0].name << endl << playar[1].name << endl;
    if(check_end_game() == 0)
        tempfile << "EQUAL" << endl;
    else
        tempfile << playar[check_end_game() - 1].name << endl;
    string line;
    while(gamehistory >> line)
        tempfile << line << endl;
    tempfile.close();
    gamehistory.close();
    gamehistory.open("game_history.txt");
    tempfile.open("temp.txt");
    while(tempfile >> line)
        gamehistory << line << endl;
    tempfile.close();
    gamehistory.close();
}

void end_game()
{
    system("cls");
    save_history();
    ofstream outfile;
    outfile.open("saved_game.txt");
    outfile << "end" << endl;
}

void load_continue()
{
    ifstream infile;
    infile.open("saved_game.txt");
    string c;
    infile >> c;
    infile >> saved_game.game_mode;
    infile >> saved_game.game_size;
    infile >> saved_game.AILevel;
    infile >> saved_game.movments;
    infile >> saved_game.i;
    infile >> saved_game.j;
    infile >> saved_game.k;
    saved_game.map = new char*[saved_game.game_size];
    for(int i = 0; i < saved_game.game_size; i++)
        saved_game.map[i] = new char[saved_game.game_size];
    for(int i = 0; i < saved_game.game_size; i++)
        for(int j = 0; j < saved_game.game_size; j++)
            saved_game.map[i][j] = ' ';
    playar[0].map = new int*[saved_game.game_size];
    for(int i = 0; i < saved_game.game_size; i++)
        playar[0].map[i] = new int[saved_game.game_size];
    for(int i = 0; i < saved_game.game_size; i++)
        for(int j = 0; j < saved_game.game_size; j++)
            playar[0].map[i][j] = 0;
    playar[1].map = new int*[saved_game.game_size];
    for(int i = 0; i < saved_game.game_size; i++)
        playar[1].map[i] = new int[saved_game.game_size];
    for(int i = 0; i < saved_game.game_size; i++)
        for(int j = 0; j < saved_game.game_size; j++)
            playar[1].map[i][j] = 0;
    string temp;
    getline(infile, temp);
    for(int i = 0; i < saved_game.game_size; i++)
        for(int j = 0; j < saved_game.game_size; j++)
        {
            string t;
            getline(infile, t);
            saved_game.map[i][j] = t[0];
        }
    saved_game.ps[0] = playar[0];
    saved_game.ps[1] = playar[1];
    infile >> saved_game.ps[0].name;
    infile >> saved_game.ps[0].XO;
    for(int i = 0; i < saved_game.game_size; i++)
        for(int j = 0; j < saved_game.game_size; j++)
            infile >> saved_game.ps[0].map[i][j];
    infile >> saved_game.ps[1].name;
    infile >> saved_game.ps[1].XO;
    for(int i = 0; i < saved_game.game_size; i++)
        for(int j = 0; j < saved_game.game_size; j++)
            infile >> saved_game.ps[1].map[i][j];
    infile.close();
    playar[0] = saved_game.ps[0];
    playar[1] = saved_game.ps[1];
}

void save()
{
    ofstream outfile;
    outfile.open("saved_game.txt");
    outfile << "continue" << endl;
    outfile << saved_game.game_mode << endl;
    outfile << saved_game.game_size << endl;
    outfile << saved_game.AILevel << endl;
    outfile << saved_game.movments << endl;
    outfile << saved_game.i << endl;
    outfile << saved_game.j << endl;
    outfile << saved_game.k << endl;
    for(int i = 0; i < saved_game.game_size; i++)
        for(int j = 0; j < saved_game.game_size; j++)
            outfile << saved_game.map[i][j] << endl;
    outfile << saved_game.ps[0].name << endl;
    outfile << saved_game.ps[0].XO << endl;
    for(int i = 0; i < saved_game.game_size; i++)
        for(int j = 0; j < saved_game.game_size; j++)
            outfile << saved_game.ps[0].map[i][j] << endl;
    outfile << saved_game.ps[1].name << endl;
    outfile << saved_game.ps[1].XO << endl;
    for(int i = 0; i < saved_game.game_size; i++)
        for(int j = 0; j < saved_game.game_size; j++)
            outfile << saved_game.ps[1].map[i][j] << endl;
    outfile.close();
}

int empty_cell_num()
{
    int n = 0;
    for(int i = 0; i < saved_game.game_size; i++)
        for(int j = 0; j < saved_game.game_size; j++)
            if(saved_game.map[i][j] == ' ')
                n++;
    return n;
}

int** empty_cells(int n)
{
    int **nums = new int*[n];
    for(int i = 0; i < n; i++)
        nums[i] = new int[2];
    int k = 0;
    for(int i = 0; i < saved_game.game_size; i++)
        for(int j = 0; j < saved_game.game_size; j++)
            if(saved_game.map[i][j] == ' ')
            {
                nums[k][0] = i;
                nums[k][1] = j;
                k++;
            }
    return nums;
}

void easy_AI()
{
    int n = empty_cell_num();
    int **emptycells = empty_cells(n);
    int i, j;
    int temp = rand() % n;
    i = emptycells[temp][0];
    j = emptycells[temp][1];
    set_xo(i, j, 1);
}

bool check_around(int i, int j)
{
    if(i == 0 && j == 0)
    {
        if(playar[0].map[0][1] == 1 || playar[0].map[1][0] == 1 || playar[0].map[1][1] == 1)
            return true;
        else
            return false;
    }
    if(i == saved_game.game_size - 1 && j == 0)
    {
        if(playar[0].map[i][1] == 1 || playar[0].map[i - 1][0] == 1 || playar[0].map[i - 1][1] == 1)
            return true;
        else
            return false;
    }
    if(i == 0 && j == saved_game.game_size - 1)
    {
        if(playar[0].map[0][j - 1] == 1 || playar[0].map[1][j] == 1 || playar[0].map[1][j] == 1)
            return true;
        else
            return false;
    }
    if(i == saved_game.game_size - 1 && j == saved_game.game_size - 1)
    {
        if(playar[0].map[i - 1][j - 1] == 1 || playar[0].map[i - 1][j] == 1 || playar[0].map[i][j - 1] == 1)
            return true;
        else
            return false;
    }
    if(i == 0)
    {
        for(int a = i; a <= i + 1; a++)
            for(int b = j - 1; b <= j + 1; b++)
                if(playar[0].map[a][b] == 1)
                    return true;
        return false;
    }
    if(i == saved_game.game_size - 1)
    {
        for(int a = i - 1; a <= i; a++)
            for(int b = j - 1; b <= j + 1; b++)
                if(playar[0].map[a][b] == 1)
                    return true;
        return false;
    }
    if(j == 0)
    {
        for(int a = i; a <= i + 1; a++)
            for(int b = j; b <= j + 1; b++)
                if(playar[0].map[a][b] == 1)
                    return true;
        return false;
    }
    if(j == saved_game.game_size - 1)
    {
        for(int a = i; a <= i + 1; a++)
            for(int b = j - 1; b <= j; b++)
                if(playar[0].map[a][b] == 1)
                    return true;
        return false;
    }
    for(int a = i - 1; a <= i + 1; a++)
        for(int b = j - 1; b <= j + 1; b++)
            if(playar[0].map[a][b] == 1)
                return true;
    return false;
}

int CellsAroundPlayer_num()
{
    int n = 0;
    for(int i = 0; i < saved_game.game_size; i++)
        for(int j = 0; j < saved_game.game_size; j++)
            if(playar[0].map[i][j] != 1 && playar[1].map[i][j] != 1 && check_around(i, j))
                n++;
    return n;
}

int** CellsAroundPlayer(int n)
{
    int **nums = new int*[n];
    for(int i = 0; i < n; i++)
        nums[i] = new int[2];
    int k = 0;
    for(int i = 0; i < saved_game.game_size; i++)
        for(int j = 0; j < saved_game.game_size; j++)
            if(playar[0].map[i][j] != 1 && playar[1].map[i][j] != 1 && check_around(i, j))
            {
                nums[k][0] = i;
                nums[k][1] = j;
                k++;
            }
    return nums;
}

void medium_AI()
{
    int n = CellsAroundPlayer_num();
    int **cells = CellsAroundPlayer(n);
    int i, j;
    int temp = rand() % n;
    i = cells[temp][0];
    j = cells[temp][1];
    set_xo(i, j, 1);
}

void game_over_page()
{
    PlaySound(TEXT("gameover.wav"), NULL, SND_FILENAME | SND_ASYNC);
    system("cls");
    set_cursor_pos(50,8);
    cout << "\x1b[1;31;5mGAME OVER" << "!\x1b[0m\n";
    for(int i = 0; i < 2; i++)
    {
        system("color 1F");
        sleep(1);
        system("color 0F");
        sleep(1);
    }
    system("color 0F");
    set_cursor_pos(50,8);
    cout << "\x1b[1;31;5mGAME OVER" << "!\x1b[0m\n";
}

void check_first_player_move()
{
    if(playar[0].map[0][0] == 1)
        set_xo(2,2,1);
    else if(playar[0].map[2][2] == 1)
        set_xo(0,0,1);
    else if(playar[0].map[0][2] == 1)
        set_xo(2,0,1);
    else if(playar[0].map[2][0] == 1)
        set_xo(0,2,1);
    else
    {
        int num[4][2] = {{0,0},{2,0},{0,2},{2,2}};
        int temp = rand() % 4;
        int i = num[temp][0];
        int j = num[temp][1];
        set_xo(i, j, 1);
    }
}

bool check_player_win_chance()
{
    int i = 0, j = 0, k = 0;
    for(i = 0; i < 3; i++)
    {
        k = 0;
        for(j = 0; j < 3; j++)
            if(playar[0].map[i][j] == 1)
                k++;
        if(k == 2)
            for(j = 0; j < 3; j++)
                if(playar[0].map[i][j] == 0 && playar[1].map[i][j] == 0)
                {
                    set_xo(i, j, 1);
                    return true;
                }
    }
    for(i = 0; i < 3; i++)
    {
        k = 0;
        for(j = 0; j < 3; j++)
            if(playar[0].map[j][i] == 1)
                k++;
        if(k == 2)
            for(j = 0; j < 3; j++)
                if(playar[0].map[j][i] == 0 && playar[1].map[j][i] == 0)
                {
                    set_xo(j, i, 1);
                    return true;
                }
    }
    return false;
}

bool check_player_technique()
{
    if(playar[0].map[0][0] == 1 && saved_game.map[2][2] == ' ')
    {
        set_xo(2,2,1);
        return true;
    }
    else if(playar[0].map[2][2] == 1 && saved_game.map[0][0] == ' ')
    {
        set_xo(0,0,1);
        return true;
    }
    else if(playar[0].map[0][2] == 1 && saved_game.map[2][0] == ' ')
    {
        set_xo(2,0,1);
        return true;
    }
    else if(playar[0].map[2][0] == 1 && saved_game.map[0][2] == ' ')
    {
        set_xo(0,2,1);
        return true;
    }
    return false;
}

bool check_computer_win_chance()
{
    int i = 0, j = 0, k = 0;
    for(i = 0; i < 3; i++)
    {
        k = 0;
        for(j = 0; j < 3; j++)
            if(playar[1].map[i][j] == 1)
                k++;
        if(k == 2)
            for(j = 0; j < 3; j++)
                if(saved_game.map[i][j] == ' ')
                {
                    set_xo(i, j, 1);
                    return true;
                }
    }
    for(i = 0; i < 3; i++)
    {
        k = 0;
        for(j = 0; j < 3; j++)
            if(playar[1].map[j][i] == 1)
                k++;
        if(k == 2)
            for(j = 0; j < 3; j++)
                if(saved_game.map[j][i] == ' ')
                {
                    set_xo(j, i, 1);
                    return true;
                }
    }
    return false;
}

bool check_computer_technique()
{
    if(playar[1].map[0][0] == 1 && saved_game.map[2][2] == ' ')
    {
        set_xo(2,2,1);
        return true;
    }
    if(playar[1].map[2][2] == 1 && saved_game.map[0][0] == ' ')
    {
        set_xo(0,0,1);
        return true;
    }
    if(playar[1].map[0][2] == 1 && saved_game.map[2][0] == ' ')
    {
        set_xo(2,0,1);
        return true;
    }
    if(playar[1].map[2][0] == 1 && saved_game.map[0][2] == ' ')
    {
        set_xo(0,2,1);
        return true;
    }
    if(saved_game.map[2][2] == ' ')
    {
        set_xo(2,2,1);
        return true;
    }
    if(saved_game.map[0][0] == ' ')
    {
        set_xo(0,0,1);
        return true;
    }
    if(saved_game.map[0][2] == ' ')
    {
        set_xo(0,2,1);
        return true;
    }
    if(saved_game.map[2][0] == ' ')
    {
        set_xo(2,0,1);
        return true;
    }
    return false;
}

void hard_AI()
{
    if(saved_game.movments == 1)
        check_first_player_move();
    else
        if(check_computer_win_chance())
        {
        }
        else if(check_player_win_chance())
        {
        }
        else if(check_player_technique())
        {
        }
        else if(check_computer_technique())
        {
        }
        else
            medium_AI();
}

void Medium_AI()
{
    if(!check_player_win_chance())
        medium_AI();
}

int main()
{
    while(true)
    {
        bool check2;
        bool check3 = false;
        int check = first_page();
        while (check == 2 || check == 3)
        {
            check = first_page();
        }
        if(check == -1)
            break;
        else
        {
            int i = 0, j = 0, k = 0;
            if(check == 0)
            {
                load_continue();
                i = saved_game.i;
                j = saved_game.j;
                k = saved_game.k;
            }
            do
            {
                reload_page(i, j, k, check2);
                save();
                if(saved_game.game_mode == 1 && k % 2 == 1)
                {
                    switch (saved_game.AILevel)
                    {
                    case 1:
                        easy_AI();
                        break;

                    case 2:
                        Medium_AI();
                        break;
                    case 3:
                        hard_AI();
                        break;
                    }
                    k++;
                }
                else
                {
                    char input = getch();
                    switch(input)
                    {
                    case ' ':
                    {
                        if(check2)
                        {
                            set_xo(i, j, k);
                            k++;
                        }
                    }
                    break;
                    case 'w':
                        if(i > 0)
                            i--;
                        break;
                    case 's':
                        if(i < saved_game.game_size - 1)
                            i++;
                        break;
                    case 'a':
                        if(j > 0)
                            j--;
                        break;
                    case 'd':
                        if(j < saved_game.game_size - 1)
                            j++;
                        break;
                    case '1':
                        check3 = true;
                        break;
                    default:
                        choose_option_error();
                    }
                    if(check3)
                        break;
                }
            }while(check_end_game() == -1);
            if(check3)
                continue;
            if(check_end_game() == 0)
            {
                reload_page(i, j, k, check2);
                set_cursor_pos(0,0);
                cout << "Press any key to end game\n";
                getch();
                equal_page();
            }
            else if(playar[check_end_game() - 1].name == "Computer")
            {
                reload_page(i, j, k, check2);
                set_cursor_pos(0,0);
                cout << "Press any key to end game\n";
                getch();
                game_over_page();
            }
            else
            {
                reload_page(i, j, k, check2);
                set_cursor_pos(0,0);
                cout << "Press any key to end game\n";
                getch();
                congratulations_page(playar[check_end_game() - 1]);
            }
            set_cursor_pos(0,0);
            cout << "Press any key to return to the first page\n";
            getch();
            end_game();
        }
    }
    return 0;
}
