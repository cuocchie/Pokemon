#include <bits/stdc++.h>
using namespace std;

const int encounterRate[10] = {1, 1, 1, 0, 0, 0, 0, 0, 0, 0};
const string pokemonList[3] = {"Bulbasaur", "Charmander", "Squirtle"};
const int bushRow = 5;
const char moveUp = 'w', moveDown = 's', moveRight = 'd', moveLeft = 'a';

int generateRand()
{
    srand(time(0));
    return rand();
}

struct pokemon
{
    int number;
    string name;
    int level;

    pokemon(int _number, string _name, int _level)
    {
        number = _number;
        name = _name;
        level = _level;
    }

    pokemon(){};

    void chooseNumber()
    {
        int num;
        cin >> num;
        number = num;
    }
};

struct coordinate
{
   int rowPosition;
   int columnPosition;
   coordinate(){};
   coordinate(int _row, int _col)
   {
       rowPosition = _row;
       columnPosition = _col;
   }
};

void chooseStarterPokemon(pokemon& player)
{
    cout << "Choose your pokemon\n";
    for(unsigned int i = 0; i < sizeof(pokemonList)/sizeof(pokemonList[0]); i++)
    {
        cout << "Press " << i << " to choose " << pokemonList[i] << endl;
    }
    player.chooseNumber();
    player.level = 5;

    cout << "Your pokemon is level " << player.level << " " << pokemonList[player.number] << endl;
    player.name = pokemonList[player.number];
}

void spawnPlayground(char board[][10], coordinate& player)
{
    for(int i = 0; i <= 10; i++)
    {
        for(int j = 0; j <=10; j++)
        {
            if(i < 5) board[i][j] = '_';
            else board[i][j] = '*';
        }
    }
    board[player.rowPosition][player.columnPosition] = 'O';
    for(int i = 0; i < 10; i++)
        {
            for(int j = 0; j < 10; j++)
            {
                cout << board[i][j] <<setw(2);
            }
            cout << '\n';
        }
}

void getMove(char& moveDirection)
{
    cout << "Nhap Huong Di chuyen: ";
    cin >> moveDirection;
}

void updatePlayGround(const char& moveDirection, char board[][10], coordinate& player)
{
        if(moveDirection == moveUp || moveDirection == moveLeft || moveDirection == moveDown || moveDirection == moveRight)
        {

            if(player.rowPosition < bushRow)  board[player.rowPosition][player.columnPosition] = '_';
            else board[player.rowPosition][player.columnPosition] = '*';

            if(moveDirection == moveUp)
            {
               player.rowPosition--;
               board[player.rowPosition - 1][player.columnPosition] = 'O';
            }

            else if(moveDirection == moveLeft)
            {
                board[player.rowPosition][player.columnPosition - 1] = 'O';
                player.columnPosition--;
            }
            else if(moveDirection == moveDown)
            {
                board[player.rowPosition + 1][player.columnPosition] = 'O';
                player.rowPosition++;
            }
            else if(moveDirection == moveRight)
            {
                board[player.rowPosition][player.columnPosition + 1] = 'O';
                player.columnPosition++;
            }
        }

        for(int i = 0; i < 10; i++)
        {
            for(int j = 0; j < 10; j++)
            {
                cout << board[i][j] << setw(2);
            }
            cout << '\n';
        }
}

void combatResult(pokemon& player, pokemon& wild, bool& gameStatus)
{
    if(player.level >= wild.level)
    {
        cout << "Your " << player.name << " used Tackle\n";
        cout << "Wild " << wild.name << " has faint\n";
        player.level++;
        cout << "Your " << player.name << " is now level " << player.level << endl;
    }
    else
    {
        cout << "Wild " << wild.name << " used Tackle\n";
        cout << "Your " << player.name << " has faint\n";
        gameStatus = false;
    }
}

void battle(pokemon player, pokemon wild, bool& gameStatus)
{
     int a = (generateRand() % 10);
            //cout << "Your number is " << a << endl;
            if(encounterRate[a] == 1)
            {
                char FoR;
                wild.name = pokemonList[a%3];
                wild.level = generateRand() % (player.level + a);
                cout << "You encounter a wild " << wild.name << " level " << wild.level<< endl;
                do
                {
                    cout << "Press F to fight, Press R to run\n";
                    cin >> FoR;

                    if(FoR == 'f')
                    {
                        combatResult(player, wild, gameStatus);
                    }
                    else if(FoR == 'r') cout << "You have ran away from " << wild.name << endl;
                    else cout << "Wrong button!!\n";
                }while(FoR != 'f' && FoR != 'r');
            }
}



int main()
{
    char board[10][10], moveDirection;
    string playerPokemon, wildPokemon;
    bool gameStatus = true;
    pokemon player, wild;
    coordinate playerLocation(3, 4);

    //Pokemon starter pack
    chooseStarterPokemon(player);
    //spawn playground;
    spawnPlayground(board,playerLocation);
    //player move
    do
    {
        getMove(moveDirection);
        updatePlayGround(moveDirection, board, playerLocation);

        if(playerLocation.rowPosition >= bushRow)
            //encounter pokemon
            battle(player, wild, gameStatus);

    }while(gameStatus == true);
}

