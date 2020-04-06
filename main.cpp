#include <bits/stdc++.h>
using namespace std;

const int encounterRate[10] = {1, 1, 1, 0, 0, 0, 0, 0, 0, 0};
const string pokemonList[3] = {"poA", "poB", "poC"};

int generateRand()
{
    srand(time(0));
    return rand();
}

void chooseStarterPokemon(int& pokemonNumber, int& playerPokemonLevel, string& playerPokemon)
{
    cout << "Choose your pokemon\n";
    for(int i = 0; i < sizeof(pokemonList)/sizeof(pokemonList[0]); i++)
    {
        cout << "Press " << i << " to choose " << pokemonList[i] << endl;
    }
    cin >> pokemonNumber;
    cout << "Your pokemon is level " << playerPokemonLevel << " " << pokemonList[pokemonNumber] << endl;
    playerPokemon = pokemonList[pokemonNumber];
}

void spawnPlayground(char board[][10], int& playerRowPosition, int& playerColumnPosition)
{
    for(int i = 0; i <= 10; i++)
    {
        for(int j = 0; j <=10; j++)
        {
            if(i < 5) board[i][j] = '_';
            else board[i][j] = '*';
        }
    }
    board[playerRowPosition][playerColumnPosition] = 'O';
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

void updatePlayGround(const char& moveDirection, char board[][10], int& playerRowPosition, int& playerColumnPosition)
{
    if(moveDirection == 'w')
        {
            board[playerRowPosition - 1][playerColumnPosition] = 'O';
            if(playerRowPosition < 5)  board[playerRowPosition][playerColumnPosition] = '_';
            else board[playerRowPosition][playerColumnPosition] = '*';
            playerRowPosition--;
        }
        if(moveDirection == 'a')
        {
            board[playerRowPosition][playerColumnPosition - 1] = 'O';
            if(playerRowPosition < 5)  board[playerRowPosition][playerColumnPosition] = '_';
            else board[playerRowPosition][playerColumnPosition] = '*';
            playerColumnPosition--;
        }
        if(moveDirection == 's')
        {
            board[playerRowPosition + 1][playerColumnPosition] = 'O';
            if(playerRowPosition < 5)  board[playerRowPosition][playerColumnPosition] = '_';
            else board[playerRowPosition][playerColumnPosition] = '*';
            playerRowPosition++;
        }
        if(moveDirection == 'd')
        {
            board[playerRowPosition][playerColumnPosition + 1] = 'O';
            if(playerRowPosition < 5)  board[playerRowPosition][playerColumnPosition] = '_';
            else board[playerRowPosition][playerColumnPosition] = '*';
            playerColumnPosition++;
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

void combatResult(int& playerPokemonLevel,const int& wildPokemonLevel, const string& playerPokemon,const string& wildPokemon, bool& gameStatus)
{
    if(playerPokemonLevel >= wildPokemonLevel)
    {
        cout << "Your " << playerPokemon << " used Tackle\n";
        cout << "Wild " << wildPokemon << " has faint\n";
        playerPokemonLevel++;
        cout << "Your " << playerPokemon << " is now level " << playerPokemonLevel << endl;
    }
    else
    {
        cout << "Wild " << wildPokemon << " used Tackle\n";
        cout << "Your " << playerPokemon << " has faint\n";
        gameStatus = false;
    }
}

void battle(string& wildPokemon, int& wildPokemonLevel, int& playerPokemonLevel, const string& playerPokemon, bool& gameStatus)
{
     int a = (generateRand() % 10);
            //cout << "Your number is " << a << endl;
            if(encounterRate[a] == 1)
            {
                char FoR;
                wildPokemon = pokemonList[a%3];
                cout << "You encounter a wild " << wildPokemon << endl;
                wildPokemonLevel = generateRand() % (playerPokemonLevel + a);

                do
                {
                    cout << "Press F to fight, Press R to run\n";
                    cin >> FoR;

                    if(FoR == 'f')
                    {
                        combatResult(playerPokemonLevel, wildPokemonLevel, playerPokemon, wildPokemon, gameStatus);
                    }
                    else if(FoR == 'r') cout << "You have ran away from " << wildPokemon << endl;
                    else cout << "Wrong button!!\n";
                }while(FoR != 'f' && FoR != 'r');
            }
}

int main()
{
    int BushRow, BushColumn, pokemonNumber;
    char board[10][10], moveDirection;
    int playerRowPosition = 3, playerColumnPosition = 4, playerPokemonLevel = 5, wildPokemonLevel;
    string playerPokemon, wildPokemon;
    bool gameStatus = true;

    //Pokemon starter pack
    chooseStarterPokemon(pokemonNumber, playerPokemonLevel, playerPokemon);
    //spawn playground;
    spawnPlayground(board, playerRowPosition, playerColumnPosition);
    //player move
    do
    {
        getMove(moveDirection);
        updatePlayGround(moveDirection, board, playerRowPosition, playerColumnPosition);

        if(playerRowPosition >= 5)
            //encouter pokemon
            battle(wildPokemon, wildPokemonLevel, playerPokemonLevel, playerPokemon, gameStatus);
    }while( gameStatus == true);
}

