#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Game.h" 

using namespace std;

void parseDataset(string filePath) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cout << "couldn't open file " << filePath << endl;
        return;
    }

    string line, header;
    getline(file, header); //header line
    int numGames = 0;
    while (getline(file, line)) {
        stringstream ss(line);
        string appID, name, releaseDate, estimatedOwners, peakCCU, requiredAgeStr, priceStr;
        getline(ss, appID, ',');  // app id

        // name w quotes
        getline(ss, name, '"'); //skip the opening quote
        getline(ss, name, '"'); //read the quoted name
        ss.ignore(1);           //skip the comma 
     

        // release date w quotes
        getline(ss, releaseDate, '"'); //skip the opening quote
        getline(ss, releaseDate, '"'); //read the quoted release date
        ss.ignore(1);                  //skip the comma after the closing quote
       

        getline(ss, estimatedOwners, ','); //estimated owners
        getline(ss, peakCCU, ',');         //peak ccu (we ignore this)
        getline(ss, requiredAgeStr, ',');  //required age
        getline(ss, priceStr, ',');        //price

        // convert required age to int
        int requiredAge;
        if (requiredAgeStr.empty()) {
            requiredAge = 0;
        } else {
            try {
                requiredAge = stoi(requiredAgeStr);
            } catch (const invalid_argument& e) {
                cout << "invalid required Age " << appID  << requiredAgeStr << endl;
                requiredAge = 0;
            }
        }

        // convert price to double
        double price;
        if (priceStr.empty()) {
            price = 0.0;
        } 
        else { 
          price = stod(priceStr);
        }

        // create a game object
        Game game(name, price, "", releaseDate, requiredAge, estimatedOwners);

        // print the game object
        numGames++;
        //cout << "AppID: " << appID << ", " << game << endl;
        
    }
    cout << numGames << " games parsed." << endl;

    file.close();
}
void displayMenu() {
    cout << "Welcome to the Game Dataset Analyzer!" << endl;
    cout << "--------------------------------------" << endl;
    cout << "Choose an option:" << endl;
    cout << "1. Search for a game by name or AppID" << endl;
    cout << "2. Find the most expensive game" << endl;
    cout << "3. Find the cheapest game" << endl;
    cout << "4. Filter games by required age" << endl;
    cout << "5. Filter games by estimated owners" << endl;
    cout << "6. Sort games by price" << endl;
    cout << "7. Sort games by release date" << endl;
    cout << "8. Display dataset statistics" << endl;
    cout << "9. Exit" << endl;
    cout << "--------------------------------------" << endl;
    cout << "Enter your choice: ";
}

int main() {
    string filePath = "dataset/games.csv";

    // parse the dataset
    parseDataset(filePath);
    displayMenu();

    return 0;
}