/*
 * Project 1: Craps
 * Author: [Your Name]
 * Version 1: Basic game with rounds, wagers, and session statistics
 */

//System Libraries
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>
using namespace std;

//Execution Begins Here!
int main() {
    //Declare and initialize variables
    int startB = 500;
    int mode = 0;
    int maxRnd = 10;
    int rnd = 0;
    int played = 0;
    int bank = 0;
    int wins = 0;
    int losses = 0;
    int rolls = 0;
    int sumRoll = 0;
    int sumSq = 0;
    int wager = 0;
    int dice1 = 0;
    int dice2 = 0;
    int total = 0;
    int point = 0;
    int result = 0;

    float winPct = 0.0f;
    float avgRol = 0.0f;
    float var = 0.0f;
    float stdDev = 0.0f;

    char again = 'y';
    bool playOn = true;
    string player;

    //Read the starting bankroll from the startup file.
    //The program uses $500 if the file is missing or has an invalid value.
    ifstream inFile("startup.txt");

    if (inFile >> startB) {
        if (startB <= 0) {
            cout << "The startup amount must be positive. Using $500.\n";
            startB = 500;
        }
    } else {
        cout << "Could not read startup.txt. Using $500.\n";
        startB = 500;
    }

    inFile.close();

    //Let the player choose how many rounds to play.
    cout << "CRAPS - PASS LINE\n";
    cout << "1. Short session: 5 rounds\n";
    cout << "2. Standard session: 10 rounds\n";
    cout << "3. Long session: 20 rounds\n";
    cout << "Choose a session: ";

    if (!(cin >> mode)) {
        cin.clear();
        cin.ignore(10000, '\n');
        mode = 0;
    }

    //Validate the session choice.
    while (mode < 1 || mode > 3) {
        cout << "Enter 1, 2, or 3: ";

        if (!(cin >> mode)) {
            cin.clear();
            cin.ignore(10000, '\n');
            mode = 0;
        }
    }

    //Use a switch to set the number of rounds.
    switch (mode) {
        case 1:
            maxRnd = 5;
            break;
        case 2:
            maxRnd = 10;
            break;
        case 3:
            maxRnd = 20;
            break;
    }

    cout << "Enter your first name: ";
    cin >> player;

    //Seed the random-number generator once before rolling.
    srand(static_cast<unsigned int>(time(0)));

    //A new session starts with the amount read from startup.txt.
    do {
        bank = startB;
        wins = 0;
        losses = 0;
        rolls = 0;
        sumRoll = 0;
        sumSq = 0;
        played = 0;

        cout << "\nWelcome, " << player << "!\n";
        cout << "Starting bankroll: $" << bank << "\n";

        //Play rounds until the session limit or the bankroll runs out.
        for (rnd = 1; rnd <= maxRnd && bank > 0; rnd++) {
            played++;
            wager = 0;

            //Check that the wager is positive and affordable.
            do {
                cout << "\nRound " << rnd << " of " << maxRnd << "\n";
                cout << "Your bankroll is $" << bank << ". Enter wager: ";

                if (!(cin >> wager)) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    wager = 0;
                }

                if (wager < 1 || wager > bank) {
                    cout << "Wager must be from $1 to $" << bank << ".\n";
                }
            } while (wager < 1 || wager > bank);

            //Roll two dice for the come-out roll.
            dice1 = rand() % 6 + 1;
            dice2 = rand() % 6 + 1;
            total = dice1 + dice2;
            rolls++;
            sumRoll += total;
            sumSq += total * total;

            cout << "You rolled " << dice1 << " + " << dice2
                 << " = " << total << "\n";

            //A 7 or 11 wins on the come-out roll.
            if (total == 7 || total == 11) {
                cout << "Come-out win!\n";
                bank += wager;
                wins++;
                result = 1;
            }
            //A 2, 3, or 12 loses on the come-out roll.
            else if (total == 2 || total == 3 || total == 12) {
                cout << "Come-out loss.\n";
                bank -= wager;
                losses++;
                result = -1;
            }
            //Any other total becomes the point.
            else {
                point = total;
                result = 0;
                cout << "The point is " << point << ". Keep rolling.\n";

                //During the point phase, the point wins and 7 loses.
                while (result == 0) {
                    dice1 = rand() % 6 + 1;
                    dice2 = rand() % 6 + 1;
                    total = dice1 + dice2;
                    rolls++;
                    sumRoll += total;
                    sumSq += total * total;

                    cout << "You rolled " << dice1 << " + " << dice2
                         << " = " << total << "\n";

                    if (total == point) {
                        cout << "You rolled the point. You win!\n";
                        bank += wager;
                        wins++;
                        result = 1;
                    } else if (total == 7) {
                        cout << "You rolled a 7. You lose.\n";
                        bank -= wager;
                        losses++;
                        result = -1;
                    } else {
                        cout << "Neither the point nor 7. Roll again.\n";
                    }
                }
            }

            cout << "Bankroll after round: $" << bank << "\n";
        }

        //Calculate and display session statistics.
        if (wins + losses > 0) {
            winPct = static_cast<float>(wins) * 100.0f
                   / static_cast<float>(wins + losses);
        }

        if (rolls > 0) {
            avgRol = static_cast<float>(sumRoll)
                   / static_cast<float>(rolls);

            var = static_cast<float>(sumSq)
                / static_cast<float>(rolls) - avgRol * avgRol;

            if (var < 0.0f) {
                var = 0.0f;
            }

            stdDev = static_cast<float>(sqrt(var));
        }

        cout << "\nSESSION SUMMARY\n";
        cout << fixed << setprecision(2);
        cout << left << setw(22) << "Rounds played:" << right << played << "\n";
        cout << left << setw(22) << "Wins:" << right << wins << "\n";
        cout << left << setw(22) << "Losses:" << right << losses << "\n";
        cout << left << setw(22) << "Win percentage:" << right
             << winPct << "%\n";
        cout << left << setw(22) << "Average dice total:" << right
             << avgRol << "\n";
        cout << left << setw(22) << "Dice-total deviation:" << right
             << stdDev << "\n";
        cout << left << setw(22) << "Ending bankroll:" << right
             << "$" << bank << "\n";

        //Append the session results to a text file.
        ofstream outFile("results.txt", ios::app);

        if (outFile) {
            outFile << fixed << setprecision(2);
            outFile << "Player: " << player << "\n";
            outFile << "Rounds played: " << played << "\n";
            outFile << "Wins: " << wins << "\n";
            outFile << "Losses: " << losses << "\n";
            outFile << "Win percentage: " << winPct << "%\n";
            outFile << "Average dice total: " << avgRol << "\n";
            outFile << "Ending bankroll: $" << bank << "\n";
            outFile << "------------------------------\n";
            outFile.close();
            cout << "\nSession saved to results.txt.\n";
        } else {
            cout << "\nCould not open results.txt for writing.\n";
        }

        //Ask whether to play another session.
        do {
            cout << "\nPlay another session? (y/n): ";
            cin >> again;

            if (again != 'y' && again != 'Y'
                    && again != 'n' && again != 'N') {
                cout << "Please enter y or n.\n";
            }
        } while (again != 'y' && again != 'Y'
                 && again != 'n' && again != 'N');

        if (again == 'y' || again == 'Y') {
            playOn = true;
        } else {
            playOn = false;
        }

    } while (playOn);

    cout << "\nThanks for playing, " << player << "!\n";
    return 0;
}