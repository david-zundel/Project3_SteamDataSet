#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Game.h" 
#include "Hash.h"
#include "rb.cpp"
#include <vector>
#include <algorithm>
#include <chrono>
#include <iomanip>

using namespace std;

void parseDataset(string filePath, Hash& hashTable, rb& rbTree) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cout << "couldn't open file " << filePath << endl;
        return;
    }

    string line, header;
    getline(file, header); //header line
    int numGames = 0;
    long long totalInsertionTime = 0;
    long long totalInsertionTimeRB = 0;
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
        Game game(name, price, "", releaseDate, requiredAge, estimatedOwners, appID);

        auto startInsert = chrono::high_resolution_clock::now();
        hashTable.insert(game); // insert the game into the hash table
        auto endInsert = chrono::high_resolution_clock::now();
        totalInsertionTime += chrono::duration_cast<chrono::microseconds>(endInsert - startInsert).count();

        auto startRB = chrono::high_resolution_clock::now();
        rbTree.insert(game);
        auto endRB = chrono::high_resolution_clock::now();
        totalInsertionTimeRB += chrono::duration_cast<chrono::microseconds>(endRB - startRB).count();

        numGames++;
        //cout << "AppID: " << appID << ", " << game << endl;
        
    }
    cout << numGames << " games parsed." << endl << endl;
    cout << "Total Hash Table Insertion Time: " << totalInsertionTime << " microseconds (" 
    << totalInsertionTime / 1000.0 << " milliseconds)" << endl;
      cout << "Total Red-Black Tree Insertion Time: " << totalInsertionTimeRB << " microseconds (" 
         << totalInsertionTimeRB / 1000.0 << " milliseconds)" << endl;
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
// Gives user choice of the type of data structure they want to use
void tableChoice() {
    cout << "Choose the type of data structure that you want to use:" << endl;
    cout << "--------------------------------------" << endl;
    cout << "1. Hash Table" << endl;
    cout << "2. Red-Black Tree" << endl;
    cout << "3. Go Back" << endl;
    cout << "--------------------------------------" << endl;
    cout << "\nEnter your choice: ";
}
void searchByName(Hash& hashTable) {
    string name; 
    cout << "\nEnter the name of the game: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
    int limit = 100;
    int maxAge;
    cout << "Enter the maximum required age: ";
    cin >> maxAge;

    auto startHash = chrono::high_resolution_clock::now();
    vector<Game> gamesHash = hashTable.searchByRequiredAge(maxAge);
    auto endHash = chrono::high_resolution_clock::now();
    auto durationHash = chrono::duration_cast<chrono::microseconds>(endHash - startHash);
    cout << "Hash Table Filter Time: " << durationHash.count() << " microseconds" << endl;

    if (!gamesHash.empty()) {
        cout << "Games suitable for age " << maxAge << " or older (showing up to " << limit << " results):\n" << endl;

        //limit the number of displayed results to the specified limit
        for (size_t i = 0; i < gamesHash.size() && i < static_cast<size_t>(limit); ++i) {
            cout << "Name: " << gamesHash[i].getName() << ", Required Age: " << gamesHash[i].getRequiredAge() << endl;
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

        //calculate step size to evenly distribute 100 games
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

        //calculate step size to evenly distribute 100 games
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
void rbSearchByName(rb& rbTree) {
    string name; 
    cout << "\nEnter the name of the game: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, name); // Read the full game name, including spaces

    auto startRB = chrono::high_resolution_clock::now();
    Game* gameRB = rbTree.rbSearchByName(rbTree, name);
    auto endRB = chrono::high_resolution_clock::now();
    auto durationRB = chrono::duration_cast<chrono::microseconds>(endRB - startRB);

    cout << "Red Black Tree Search Time: " << durationRB.count() << " microseconds" << endl;

    if (gameRB != nullptr) {
        cout << "Game found: " << endl;
        cout << "Name: " << gameRB->getName() << endl;
        cout << "Price: $" << gameRB->getPrice() << endl;
        cout << "Release Date: " << gameRB->getReleaseDate() << endl;
        cout << "Required Age: " << gameRB->getRequiredAge() << endl;
        cout << "Estimated Owners: " << gameRB->getEstimatedOwners() << endl;
    } else {
        cout << "Game not found." << endl;
    }

    // Add a blank line for better formatting
    cout << endl;
}
void rbFindMostExpensiveGame(rb& rbTree){
    auto startRB = chrono::high_resolution_clock::now();
    vector<Game> gamesRB = rbTree.rbSortByPriceDesc(rbTree);
    auto endRB = chrono::high_resolution_clock::now();
    auto durationRB = chrono::duration_cast<chrono::microseconds>(endRB - startRB);
    cout << "Red Black Tree Sort Time: " << durationRB.count() << " microseconds" << endl;
    if (!gamesRB.empty()) {
        cout << "Most Expensive Game: " << endl;
        cout << "Name: " << gamesRB[0].getName() << endl;
        cout << "Price: $" << gamesRB[0].getPrice() << endl;
        cout << "Release Date: " << gamesRB[0].getReleaseDate() << endl;
        cout << "Required Age: " << gamesRB[0].getRequiredAge() << endl;
        cout << "Estimated Owners: " << gamesRB[0].getEstimatedOwners() << endl << endl;
        
    } else {
        cout << "No games found." << endl;
    }

}
void rbFilterByRequiredAge(rb& rbTree) {
    int maxAge;
    cout << "Enter the maximum required age: ";
    cin >> maxAge;

    auto startRB = chrono::high_resolution_clock::now();
    vector<Game> gamesRB = rbTree.rbSearchByRequiredAge(rbTree, maxAge);
    auto endRB = chrono::high_resolution_clock::now();
    auto durationRB = chrono::duration_cast<chrono::microseconds>(endRB - startRB);
    cout << "Red Black Tree Filter Time: " << durationRB.count() << " microseconds" << endl;

    if (!gamesRB.empty()) {
        cout << "Games suitable for age " << maxAge << " or older (showing up to 100 results):\n" << endl;

        //limit the number of displayed results to 100
        for (size_t i = 0; i < gamesRB.size() && i < 100; ++i) {
            cout << "Name: " << gamesRB[i].getName() << ", Required Age: " << gamesRB[i].getRequiredAge() << endl;
        }
    } else {
        cout << "No games found for the specified age." << endl;
    }
}
void rbFilterByEstimatedOwners(rb& rbTree) {
    string minOwners;
    cout << "\nEnter the minimum estimated owners: ";
    cin >> minOwners;

    auto startRB = chrono::high_resolution_clock::now();
    vector<Game> filteredGames = rbTree.rbSearchByEstimatedOwners(rbTree, minOwners);
    auto endRB = chrono::high_resolution_clock::now();
    auto durationRB = chrono::duration_cast<chrono::microseconds>(endRB - startRB);

    cout << "\nRed Black Tree Filter Time: " << durationRB.count() << " microseconds\n" << endl;

    if (!filteredGames.empty()) {
        cout << "Games with at least " << minOwners << " estimated owners:\n" << endl;
        for (const auto& game : filteredGames) {
            cout << "Name: " << game.getName() << ", Estimated Owners: " << game.getEstimatedOwners() << endl;
        }
    } else {
        cout << "No games found for the specified estimated owners." << endl;
    }
}
void rbSortGamesByPrice(rb& rbTree) {
    auto startRB = chrono::high_resolution_clock::now();
    vector<Game> sortedGames = rbTree.rbSortByPriceAsc(rbTree);
    auto endRB = chrono::high_resolution_clock::now();
    auto durationHash = chrono::duration_cast<chrono::microseconds>(endRB - startRB);

    cout << "\nRed Black Tree Sort Time: " << durationHash.count() << " microseconds\n" << endl;

    if (!sortedGames.empty()) {
        cout << "Games sorted by price (ascending):\n" << endl;

        //calculate step size to evenly distribute 100 games
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
void rbSortGamesByReleaseDate(rb& rbTree) {
    auto startRB = chrono::high_resolution_clock::now();
    vector<Game> sortedGames = rbTree.rbSortByReleaseDate(rbTree);
    auto endRB = chrono::high_resolution_clock::now();
    auto durationRB = chrono::duration_cast<chrono::microseconds>(endRB - startRB);

    cout << "\nRed Black Tree Sort Time: " << durationRB.count() << " microseconds\n" << endl;

    if (!sortedGames.empty()) {
        cout << "Games sorted by release date (newest to oldest):\n" << endl;

        //calculate step size to evenly distribute 100 games
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
void rbDisplayDatasetStatistics(rb& rbTree) {
    auto startRB = chrono::high_resolution_clock::now();
    rbTree.printTable();
    auto endRB = chrono::high_resolution_clock::now();
    auto durationRB = chrono::duration_cast<chrono::microseconds>(endRB - startRB);

    cout << "\nRed Black Tree Statistics Display Time: " << durationRB.count() << " microseconds\n" << endl;
}
int main() {
    string filePath = "dataset/games.csv";
    Hash hashTable(50000); //nitialize hash table with a size of 50000
    rb rbTree; //ionitializes rbTree

    //parse the dataset
    parseDataset(filePath, hashTable, rbTree);

    while (true) {
        displayMenu();
        int choice;
        cin >> choice;

        //clear the input buffer to avoid issues with getline
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 1) {
            tableChoice();
            int tChoice;
            cin >> tChoice;
            if (tChoice == 1) {
                searchByName(hashTable);
            }
            else if (tChoice == 2)
            {
                rbSearchByName(rbTree);
            }
            else if (tChoice == 3)
            {
                cout << "Exiting..." << endl;
            }
            else {
                cout << "Invalid choice. Please try again." << endl;
            }
        } else if (choice == 2) {
             tableChoice();
            int tChoice;
            cin >> tChoice;
            if (tChoice == 1) {
                findMostExpensiveGame(hashTable);
            }
            else if (tChoice == 2)
            {
                rbFindMostExpensiveGame(rbTree);
            }
            else if (tChoice == 3)
            {
                cout << "Exiting..." << endl;
            }
            else {
                cout << "Invalid choice. Please try again." << endl;
            }
        } else if (choice == 3) {
            tableChoice();
            int tChoice;
            cin >> tChoice;
            if (tChoice == 1) {
                filterByRequiredAge(hashTable);
            }
            else if (tChoice == 2)
            {
                rbFilterByRequiredAge(rbTree);
            }
            else if (tChoice == 3)
            {
                cout << "Exiting..." << endl;
            }
            else {
                cout << "Invalid choice. Please try again." << endl;
            }
        } else if (choice == 4) {
                         tableChoice();
            int tChoice;
            cin >> tChoice;
            if (tChoice == 1) {
                filterByEstimatedOwners(hashTable);
            }
            else if (tChoice == 2)
            {
                rbFilterByEstimatedOwners(rbTree);
            }
            else if (tChoice == 3)
            {
                cout << "Exiting..." << endl;
            }
            else {
                cout << "Invalid choice. Please try again." << endl;
            }
        } else if (choice == 5) {
            tableChoice();
            int tChoice;
            cin >> tChoice;
            if (tChoice == 1) {
                sortGamesByPrice(hashTable);
            }
            else if (tChoice == 2)
            {
                rbSortGamesByPrice(rbTree);
            }
            else if (tChoice == 3)
            {
                cout << "Exiting..." << endl;
            }
            else {
                cout << "Invalid choice. Please try again." << endl;
            }
        } else if (choice == 6) {
            tableChoice();
            int tChoice;
            cin >> tChoice;
            if (tChoice == 1) {
                sortGamesByReleaseDate(hashTable);
            }
            else if (tChoice == 2)
            {
                rbSortGamesByReleaseDate(rbTree);
            }
            else if (tChoice == 3)
            {
                cout << "Exiting..." << endl;
            }
            else {
                cout << "Invalid choice. Please try again." << endl;
            }
        } else if (choice == 7) {
            tableChoice();
            int tChoice;
            cin >> tChoice;
            if (tChoice == 1) {
                displayDatasetStatistics(hashTable);
            }
            else if (tChoice == 2)
            {
                rbDisplayDatasetStatistics(rbTree);
            }
            else if (tChoice == 3)
            {
                cout << "Exiting..." << endl;
            }
            else {
                cout << "Invalid choice. Please try again." << endl;
            }
        } else if (choice == 8) {
            cout << "Exiting..." << endl;
            break;
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }

        //added a blank line for better formatting
        cout << endl;
    }

    return 0;
}