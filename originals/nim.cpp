// This was originally written in early 2023. Lines with a comment saying "EDIT" are newly added or changed.
// The edited lines were changed to remove compilation errors and bugs caused by undefined behaviour.
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>

//These lines of code check to see what OS is used. I needed this for the sleep function. Comments further down.
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
# define Sleep(ms) sleep((ms) / 1000) // EDIT
#endif

using namespace std;

int main () {
    string player_name;
    char choice;
    int heapValue = 23; //total objects
    int valueRemoved;
    string err[2];
    string obj[2]; //used to make statements grammatical.
    int computer_move;
    enum winStatus {computer, player, none}; // EDIT
    winStatus winner = none;

    srand(time(NULL));

    cout << "Welcome to nim. Please enter your name." << endl << "> ";
    getline(std::cin, player_name);
    while (!((choice == 'y') || (choice == 'n'))) {
        cout << "Display game rules? y/n" << endl << "> ";
        cin >> choice;
    }

    if (choice == 'y') {
        cout << "The game begins with a pool of 23 objects. Players take turns removing one to three items at a time." << endl;
        cout << "On each turn a player must take at least one object. Whoever is forced to remove the last object loses." << endl;
        cout << "Press c to continue." << endl << "> ";
        while (choice != 'c') {
            cin >> choice;
        }
    }

    while (heapValue > 0) {
        Sleep(1000);
           
            //grammar edit
            if (heapValue == 1) {
            obj[0] = "object";
            } else {
            obj[0] = "objects";
            }

        //begin player turn.
        cout << endl << endl << player_name << "'s turn:" << endl << heapValue << " " << obj[0] << " remaining." << endl << "Objects to remove:" << endl << "> ";
        cin >> valueRemoved;

        //Prevents removing more than the value that exists.
        while ((heapValue - valueRemoved) < 0) {
            cout << "Invalid input. The amount removed cannot be more than the amount currently in the heap." << endl << "> ";
            cin >> valueRemoved;
        }

        //Prevents removing more or less than the allowed values
        while ((valueRemoved > 3) || (valueRemoved < 1)) {
            cout << "Invalid input. The amount removed must be either 1, 2, or 3. ";
            if (valueRemoved > 3) {
                err[0] = "greater";
                err[1] = "3";
            } else {
                err[0] = "less";
                err[1] = "1";
            }
            cout << valueRemoved << " is " << err[0] << " than " << err[1] << "." << endl << "> ";
            cin >> valueRemoved;
        }

        heapValue -= valueRemoved;

            //grammar edit
            if (heapValue == 1) {
                obj[0] = "object";
            } else {
                obj[0] = "objects";
            }

        cout << heapValue << ' ' << obj[0] << " remaning." << endl;
        if (heapValue == 0) {
            winner = computer;
        }
        Sleep(1000); //these are used to make the program feel more natural. Without them the speed at which the computer "chooses" its play is jarring

        //begin computer turn.
        if (winner != computer) {
            cout << endl << endl << "Computer's turn." << endl;
            computer_move = (rand() % 3) + 1;

            //Prevents the computer removing more objects than exist in the heap
            if ((heapValue - computer_move) < 0) {
                computer_move = heapValue;
            }

            heapValue -= computer_move;
            if (heapValue == 0) {
                winner = player;
            }

                //grammar edits
                if(heapValue == 1) {
                    obj[0] = "object";
                } else {
                    obj[0] = "objects";
                }

                if (computer_move == 1) {
                    obj[1] = "object";
                } else {
                    obj[1] = "objects";
                }

            Sleep(1500); 
            cout << "The computer has chosen to remove " << computer_move << ' ' << obj[1] << ". " << heapValue << ' ' << obj[0] << " remaining." << endl;
        }
    }
    Sleep(1000);
    cout << endl << endl;
    if (winner == computer) {
        cout << player_name << " has been beaten by the computer." << endl;
    } else {
        cout << player_name << " has won!" << endl;
    }
    Sleep(2000);
 
    return 0;
}
