/*****************************************
 *
 *            Fortune Teller
 *          By James C. Craven
 * 
*****************************************/
// EDIT 10th November, 2025: this was written sometime in early 2023. It is untouched aside from
// this comment.
#include <iostream>
#include <ctime>
#include <string>
#include <math.h>
using namespace std;

int main () {
    int month, birth_year, dieroll, s_base, s_exponent, s_temp, seed, fortune;
    enum season {winter, spring, summer, fall}; // for determining user's current season of the year
    season season_identifier;
    double t, years, year_as_percent, x, s_res;
    bool db_skipped;
    string input1;
    char choice;

    cout << "This fortune teller program magically reads fate, using your astro-temporal coordinates!" << endl;
    cout << "To begin, we must find your magnetic signature in relation to the poles." << endl;
    cout << "Please type n if you are in the northern hemisphere, s if you are in the southern hemisphere." << endl << ">";
    getline(std::cin, input1);
    
    //executes code as normal unless the second character in the input is a.
    if((input1.size() == 1) || (input1.size() <= 2) && (input1.at(1) != 'a')) {

    //ensures that either 1 or 0 is entered for hemisphere
    if (!((input1.at(0) == 'n') || (input1.at(0) == 's'))) {
        cout << "Fatal Error: input must be 'n' or 's'." << endl;
        return 0;
    }

    //allows a debug menu that will pick the season instead of basing it on the system clock
    if ((input1.size() >= 2) && (input1.at(1) == 'm')) {
        cout << "-----------------------------------------------" << endl;
        cout << "You have entered debug mode. Would you like to set the season manually? y/n" << endl << ">";
        cin >> choice;
        if (choice == 'y') {
            cout << "Select desired season:" << endl << "a) winter" << endl << "b) spring" << endl << "c) summer" << endl << "d) fall" << endl << ">";
            cin >> choice;
            switch (choice) {
                case 'a':
                    season_identifier = winter;
                break;
                case 'b':
                    season_identifier = spring;
                break;
                case 'c':
                    season_identifier = summer;
                break;
                case 'd':
                    season_identifier = fall;
                break;
            }
            db_skipped = true;
        } else {
            cout << "Succesfully skipped season override." << endl << endl;
        }
        cout << "-----------------------------------------------" << endl;
    }

    //continues default user input
    cout << endl << "Next your horoscope must be considered. Enter your birth month followed by your birth year." << endl << ">";
    cin >> month >> birth_year;
    cout << endl <<"Finally, choose a number 1-6. It is recommended that you roll a die." << endl << ">";
    cin >> dieroll;
    if (!((dieroll > 0) && (dieroll < 7))) {
        cout << "Fatal Error: The number must be an integer between 1 and 6." << endl;
        return 0;
    }

    // Takes the time since 1970 and divides it into years.
    t = time (0);
    years = t / 31556952;
    /* The value of the divisor was determined by multiplying 60s by 60min by 24h by
     365.2425 days to get the closest possible approximation*/
    year_as_percent = modf(years, &x); //takes the decimal component of the year to determine how far into the year it is
    
    

    //Determines current season.
    if (db_skipped == false) {
        switch (input1.at(0)) {
            case 'n':
                if ((year_as_percent >= 0.22) && (year_as_percent <= 0.47)) {
                    season_identifier = spring;
                } else if ((year_as_percent > 0.47) && (year_as_percent <= 0.73)) {
                    season_identifier = summer;
                } else if ((year_as_percent > 00.73) && (year_as_percent <= 0.98)) {
                    season_identifier = fall;
                } else if (((year_as_percent > 0.98) && (year_as_percent < 1)) || ((year_as_percent < 0.22) && (year_as_percent > 0))) {
                    season_identifier = winter;
                } else {
                    cout << "Fatal Error: Time from system is incorrectly interpreted." << endl;
                    return 0;
                }
            break;
            case 's':
                if ((year_as_percent >= 0.22) && (year_as_percent <= 0.47)) {
                    season_identifier = fall;
                } else if ((year_as_percent > 0.47) && (year_as_percent <= 0.73)) {
                    season_identifier = winter;
                } else if ((year_as_percent > 00.73) && (year_as_percent <= 0.98)) {
                    season_identifier = spring;
                } else if (((year_as_percent > 0.98) && (year_as_percent < 1)) || ((year_as_percent < 0.22) && (year_as_percent > 0))) {
                    season_identifier = summer;
                } else {
                    cout << "Fatal Error: Time from system is incorrectly interpreted." << endl;
                    return 0;
                }
            break;
            default:
                cout << "Fatal Error: Something messed up in the switch statement on line 83... how did you manage that???" << endl;
                return 0;
        }
    }

    //creates a value to seed the random function
    /*The value is calculated thus: 
    ((birth year - birth month) ^ ((the season of the year + 1) * the golden ratio)) * the roll of a die*/
    s_base = (birth_year - month);
    s_exponent = (season_identifier + 1) * 2.61803398875;
    s_res = (pow(s_base, s_exponent));
    s_temp = s_res;
    seed = s_temp * dieroll;

    srand(seed);

    fortune = ((rand()) % 120) + 1;
    
    cout << "  ____" << endl;
    cout << " /\\' .\\    _____" << endl;
    cout << "/: \\___\\  / .  /\\" <<endl;
    cout << "\\' / . / /____/..\\" << endl;
    cout << " \\/___/  \\'  '\\  /" << endl;
    cout << "          \\'__'\\/" << endl << endl;
    switch (fortune) {
        case 1 ... 10:
            cout << "Your character seems suspicious to others. Try smiling more. With excessive eye contact." << endl;
        break;
        case 11 ... 20:
            cout << "You owe me a soda." << endl;
        break;
        case 41 ... 50:
            cout << "You will overcome your fears." << endl;
        break;
        case 51 ... 60:
            cout << "You will see a cow today." << endl;
        break;
        case 69:
            cout << "https://youtu.be/dQw4w9WgXcQ" << endl;
        break;
        case 71 ... 80:
            cout << "Your next test, be it academic, medical, or otherwise will be positive, for better or for worse." << endl;
        break;
        case 91 ... 100:
            cout << "You will trip and fall today." << endl;
        break;
        case 101 ... 110:
            cout << "Beware the ides of the coming month." << endl;
        break;
        case 111 ... 120:
            cout << "Try a new hobby. (Might I suggest a sandbox videogame? Like Terraria or Satisfactory, for instance.)" << endl;
        break;
        default:
            cout << "Your future is blurry. You may try again." << endl;
    }}

    //executes only if the very first user input is the character 'a'.
    else {
        cout << "Warning, this will print all possible fortunes. Do you wish to proceed? y/n" << endl;
        cin >> choice;
        if (choice != 'y') {
            cout << "Print all terminated." << endl;
        } else {
            //Full disclosure, I didn't copy this from somewhere, I just took a year of JavaScript and figured this would be the easiest way to do this.
            for (int i = 1; i < 121; i++) {
                if (((i % 10) == 0) || (i == 69)) {
                    cout << "For fortune = " << i << ", the fortune is: " << endl;
                    switch (i) {
                        case 1 ... 10:
                            cout << "Your character seems suspicious to others. Try smiling more. With excessive eye contact." << endl;
                        break;
                        case 11 ... 20:
                            cout << "You owe me a soda." << endl;
                        break;
                        case 41 ... 50:
                            cout << "You will overcome your fears." << endl;
                        break;
                        case 51 ... 60:
                            cout << "You will see a cow today." << endl;
                        break;
                        case 69:
                            cout << "https://youtu.be/dQw4w9WgXcQ" << endl;
                        break;
                        case 71 ... 80:
                            cout << "Your next test, be it academic, medical, or otherwise will be positive, for better or for worse." << endl;
                        break;
                        case 91 ... 100:
                            cout << "You will trip and fall today." << endl;
                        break;
                        case 101 ... 110:
                            cout << "Beware the ides of the coming month." << endl;
                        break;
                        case 111 ... 120:
                            cout << "Try a new hobby. (Might I suggest a sandbox videogame? Like Terraria or Satisfactory, for instance.)" << endl;
                        break;
                        default:
                            cout << "Your future is blurry. You may try again." << endl;
                    }
                    cout << endl;               
                }
            }
        }
    }
    cout << endl << endl << endl;


    //displays all variables after runtime for bugtesting. m gives menu and end report, r gives report only
    if ((input1.size() >= 2) && ((input1.at(1) == 'm') || (input1.at(1) == 'r'))) {
        cout << "Time Calculations" << endl << "t: " << t << endl << "years:" << years << endl << "year_as_percent: " << year_as_percent << endl << "Season: "<< season_identifier << endl << endl;
        cout << "User Input " << endl << "input1: " << input1 << endl << "month: " << month << endl << "birth_year: " << birth_year << endl << "dieroll: " << dieroll << endl << endl;
        cout << "Seed Calculation" << endl << "s_base: " << s_base << endl << "s_exponent: " << s_exponent << endl << "s_res: " << s_res << endl << "s_temp: " << s_temp << endl << "seed: " << seed << endl;
        cout << "Fortune:" << fortune << endl << endl;
        cout << "Size of input string: " << input1.size() << endl;
    }

    cout << endl << endl << endl;
    cout << "Fortune fortold. Farewell." << endl << endl;

    return 0;
}   
