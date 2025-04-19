#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Game.h" 
#include "Hash.h"
#include <vector>
#include <algorithm>
#include <chrono>
#include <iomanip>

using namespace std;

void parseDataset(string filePath, Hash& hashTable) {
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
        hashTable.insert(game); // insert the game into the hash table
        numGames++;
        //cout << "AppID: " << appID << ", " << game << endl;
        
    }
    cout << numGames << " games parsed." << endl << endl;

    file.close();
}
void displayMenu() {
    cout << "Welcome to the Game Dataset Analyzer!" << endl;
    cout << "--------------------------------------" << endl;
    cout << "Choose an option:" << endl;
    cout << "1. Search for a game by name" << endl;
    cout << "2. Find the most expensive game" << endl;
    cout << "3. Filter games by required age" << endl;
    cout << "4. Filter games by estimated owners" << endl;
    cout << "5. Sort games by price" << endl;
    cout << "6. Sort games by release date" << endl;
    cout << "7. Display dataset statistics" << endl;
    cout << "8. Exit" << endl;
    cout << "--------------------------------------" << endl;
    cout << "\nEnter your choice: ";
}
void searchByName(Hash& hashTable) {
    string name; 
    cout << "\nEnter the name of the game: ";
    getline(cin, name); // Read the full game name, including spaces

    auto startHash = chrono::high_resolution_clock::now();
    Game* gameHash = hashTable.search(name);
    auto endHash = chrono::high_resolution_clock::now();
    auto durationHash = chrono::duration_cast<chrono::microseconds>(endHash - startHash);

    cout << "Hash Table Search Time: " << durationHash.count() << " microseconds" << endl;

    if (gameHash != nullptr) {
        cout << "Game found: " << endl;
        cout << "Name: " << gameHash->getName() << endl;
        cout << "Price: $" << gameHash->getPrice() << endl;
        cout << "Release Date: " << gameHash->getReleaseDate() << endl;
        cout << "Required Age: " << gameHash->getRequiredAge() << endl;
        cout << "Estimated Owners: " << gameHash->getEstimatedOwners() << endl;
    } else {
        cout << "Game not found." << endl;
    }

    // Add a blank line for better formatting
    cout << endl;
}
void findMostExpensiveGame(Hash& hashTable){
    auto startHash = chrono::high_resolution_clock::now();
    vector<Game> gamesHash = hashTable.sortByPriceDesc();
    auto endHash = chrono::high_resolution_clock::now();
    auto durationHash = chrono::duration_cast<chrono::microseconds>(endHash - startHash);
    cout << "Hash Table Sort Time: " << durationHash.count() << " microseconds" << endl;
    if (!gamesHash.empty()) {
        cout << "Most Expensive Game: " << endl;
        cout << "Name: " << gamesHash[0].getName() << endl;
        cout << "Price: $" << gamesHash[0].getPrice() << endl;
        cout << "Release Date: " << gamesHash[0].getReleaseDate() << endl;
        cout << "Required Age: " << gamesHash[0].getRequiredAge() << endl;
        cout << "Estimated Owners: " << gamesHash[0].getEstimatedOwners() << endl << endl;
        
    } else {
        cout << "No games found." << endl;
    }

}
void filterByRequiredAge(Hash& hashTable) {
    int maxAge;
    cout << "Enter the maximum required age: ";
    cin >> maxAge;

    auto startHash = chrono::high_resolution_clock::now();
    vector<Game> gamesHash = hashTable.searchByRequiredAge(maxAge);
    auto endHash = chrono::high_resolution_clock::now();
    auto durationHash = chrono::duration_cast<chrono::microseconds>(endHash - startHash);
    cout << "Hash Table Filter Time: " << durationHash.count() << " microseconds" << endl;

    if (!gamesHash.empty()) {
        cout << "Games suitable for age " << maxAge << " or older:\n" << endl;
        for (const auto& game : gamesHash) {
            cout << "Name: " << game.getName() << ", Required Age: " << game.getRequiredAge() << endl;
        }
    } else {
        cout << "No games found for the specified age." << endl;
    }
}
void filterByEstimatedOwners(Hash& hashTable) {
    int minOwners;
    cout << "\nEnter the minimum estimated owners: ";
    cin >> minOwners;

    auto startHash = chrono::high_resolution_clock::now();
    vector<Game> filteredGames = hashTable.searchByEstimatedOwners(minOwners);
    auto endHash = chrono::high_resolution_clock::now();
    auto durationHash = chrono::duration_cast<chrono::microseconds>(endHash - startHash);

    cout << "\nHash Table Filter Time: " << durationHash.count() << " microseconds\n" << endl;

    if (!filteredGames.empty()) {
        cout << "Games with at least " << minOwners << " estimated owners:\n" << endl;
        for (const auto& game : filteredGames) {
            cout << "Name: " << game.getName() << ", Estimated Owners: " << game.getEstimatedOwners() << endl;
        }
    } else {
        cout << "No games found for the specified estimated owners." << endl;
    }
}
void sortGamesByPrice(Hash& hashTable) {
    auto startHash = chrono::high_resolution_clock::now();
    vector<Game> sortedGames = hashTable.sortByPriceAsc();
    auto endHash = chrono::high_resolution_clock::now();
    auto durationHash = chrono::duration_cast<chrono::microseconds>(endHash - startHash);

    cout << "\nHash Table Sort Time: " << durationHash.count() << " microseconds\n" << endl;

    if (!sortedGames.empty()) {
        cout << "Games sorted by price (ascending):\n" << endl;

        // Calculate step size to evenly distribute 100 games
        int step = max(1, static_cast<int>(sortedGames.size() / 100));
        int count = 0;

        for (size_t i = 0; i < sortedGames.size() && count < 100; i += step) {
            cout << "Name: " << sortedGames[i].getName() << ", Price: $" << sortedGames[i].getPrice() << endl;
            count++;
        }
    } else {
        cout << "No games available to sort." << endl;
    }
}
void sortGamesByReleaseDate(Hash& hashTable) {
    auto startHash = chrono::high_resolution_clock::now();
    vector<Game> sortedGames = hashTable.sortByReleaseDate();
    auto endHash = chrono::high_resolution_clock::now();
    auto durationHash = chrono::duration_cast<chrono::microseconds>(endHash - startHash);

    cout << "\nHash Table Sort Time: " << durationHash.count() << " microseconds\n" << endl;

    if (!sortedGames.empty()) {
        cout << "Games sorted by release date (newest to oldest):\n" << endl;

        // Calculate step size to evenly distribute 100 games
        int step = max(1, static_cast<int>(sortedGames.size() / 100));
        int count = 0;

        for (size_t i = 0; i < sortedGames.size() && count < 100; i += step) {
            cout << "Name: " << sortedGames[i].getName() << ", Release Date: " << sortedGames[i].getReleaseDate() <<  "," << i/step << endl;
            count++;
        }
    } else {
        cout << "No games available to sort." << endl;
    }
}
void displayDatasetStatistics(Hash& hashTable) {
    auto startHash = chrono::high_resolution_clock::now();
    hashTable.printTable();
    auto endHash = chrono::high_resolution_clock::now();
    auto durationHash = chrono::duration_cast<chrono::microseconds>(endHash - startHash);

    std::cout << "\nHash Table Statistics Display Time: " << durationHash.count() << " microseconds\n" << std::endl;
}
int main() {
    string filePath = "dataset/games.csv";
    Hash hashTable(100); // Initialize hash table with a size of 100

    // Parse the dataset
    parseDataset(filePath, hashTable);

    while (true) {
        displayMenu();
        int choice;
        cin >> choice;

        // Clear the input buffer to avoid issues with getline
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 1) {
            searchByName(hashTable);
        } else if (choice == 2) {
            findMostExpensiveGame(hashTable);
        } else if (choice == 3) {
            filterByRequiredAge(hashTable);
        } else if (choice == 4) {
            filterByEstimatedOwners(hashTable);
        } else if (choice == 5) {
            sortGamesByPrice(hashTable);
        } else if (choice == 6) {
            sortGamesByReleaseDate(hashTable);
        } else if (choice == 7) {
            displayDatasetStatistics(hashTable);
        } else if (choice == 8) {
            cout << "Exiting..." << endl;
            break;
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }

        // Add a blank line for better formatting
        cout << endl;
    }

    return 0;
}