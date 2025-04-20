//
// Created by Jeremiah on 2025-04-16.
//

#include "Hash.h"
#include <functional>
#include <algorithm>
#include <iostream>
#include <cctype>
#include <iomanip>
#include <vector>
#include <string>
#include <map>
using namespace std;
Hash::Hash(int size)
    : capacity(size), table(size), totalGames(0)
{}

size_t Hash::hashFunction(const std::string& key) const {
    return std::hash<std::string>{}(key) % capacity;
}
void Hash::resize() {
    int newCapacity = capacity * 2; //double the number of buckets
    std::vector<std::list<Game>> newTable(newCapacity);

    //rehash all elements into the new table
    for (const auto& chain : table) {
        for (const auto& game : chain) {
            size_t newHash = std::hash<std::string>{}(game.getName()) % newCapacity;
            newTable[newHash].push_back(game);
        }
    }

    //replace the old table with the new one
    table = std::move(newTable);
    capacity = newCapacity;
}
void Hash::insert(const Game& game) {
    auto& chain = table[hashFunction(game.getName())];
    for (auto& g : chain) {
        if (g.getName() == game.getName()) {
            g = game; //update the existing game
            return;
        }
    }
    chain.push_back(game);
    totalGames++; //increment the total number of games

    //check the load factor and resize if necessary
    double loadFactor = static_cast<double>(totalGames) / capacity;
    if (loadFactor > 1.0) { //resize when load factor exceeds 1.0
        resize();
    }
}

bool Hash::remove(const std::string& name) {
    auto& chain = table[hashFunction(name)];
    for (auto it = chain.begin(); it != chain.end(); ++it) {
        if (it->getName() == name) {
            chain.erase(it);
            totalGames--; //decrement the total number of games
            return true;
        }
    }
    return false;
}

Game* Hash::search(const std::string& name) {
    auto& chain = table[hashFunction(name)];
    for (auto& g : chain) {
        if (g.getName() == name) {
            return &g;
        }
    }
    return nullptr;
}

std::vector<Game> Hash::searchByName(const std::string& term) const {
    std::vector<Game> result;
    for (auto const& chain : table) {
        for (auto const& g : chain) {
            if (g.getName().find(term) != std::string::npos) {
                result.push_back(g);
            }
        }
    }
    return result;
}

std::vector<Game> Hash::searchByGenre(const std::string& genre) const {
    std::vector<Game> result;
    for (auto const& chain : table) {
        for (auto const& g : chain) {
            if (g.getGenre() == genre) result.push_back(g);
        }
    }
    return result;
}

std::vector<Game> Hash::searchByReleaseDate(const std::string& date) const {
    std::vector<Game> result;
    for (auto const& chain : table) {
        for (auto const& g : chain) {
            if (g.getReleaseDate() == date) result.push_back(g);
        }
    }
    return result;
}

std::vector<Game> Hash::searchByRequiredAge(int maxAge) const {
    std::vector<Game> result;
    for (auto const& chain : table) {
        for (auto const& g : chain) {
            if (g.getRequiredAge() <= maxAge) result.push_back(g);
        }
    }
    return result;
}

std::vector<Game> Hash::searchByEstimatedOwners(int minOwners) const {
    std::vector<Game> result;
    for (auto const& chain : table) {
        for (auto const& g : chain) {
            const std::string& s = g.getEstimatedOwners();
            std::string num;
            for (char c : s) {
                if (std::isdigit(static_cast<unsigned char>(c))) num += c;
                else if (!num.empty()) break;
            }
            if (!num.empty()) {
                int val = std::stoi(num);
                if (val >= minOwners) result.push_back(g);
            }
        }
    }
    return result;
}

void Hash::printTable() const {
    int totalGames = 0;
    int nonEmptyBuckets = 0;
    int largestBucketSize = 0;

    for (int i = 0; i < capacity; ++i) {
        if (!table[i].empty()) {
            nonEmptyBuckets++;
            int bucketSize = table[i].size();
            totalGames += bucketSize;
            largestBucketSize = max(largestBucketSize, bucketSize);

            // Print the contents of the bucket
            // std::cout << "Bucket " << i << " (" << bucketSize << " items):\n";
            // for (auto const& g : table[i]) {
            //     std::cout << "  - " << g.getName() << "\n";
            // }
        }
    }

    // Print summary statistics
    std::cout << "\n--- Dataset Statistics ---\n";
    std::cout << "Total games: " << totalGames << "\n";
    std::cout << "Total buckets: " << capacity << "\n";
    std::cout << "Non-empty buckets: " << nonEmptyBuckets << "\n";
    std::cout << "Load factor: " << std::fixed << std::setprecision(2)
              << static_cast<double>(totalGames) / capacity << "\n";
    std::cout << "Largest bucket size: " << largestBucketSize << "\n";
}

std::vector<Game> Hash::getAllGames() const {
    std::vector<Game> result;
    for (auto const& chain : table) for (auto const& g : chain) result.push_back(g);
    return result;
}
vector<Game> Hash::sortByPriceAsc() const {
    vector<Game> allGames = getAllGames();
    sort(allGames.begin(), allGames.end(), [](const Game& a, const Game& b) {
        return a.getPrice() < b.getPrice(); 
    });
    return allGames;
}
vector<Game> Hash::sortByPriceDesc() const {
    vector<Game> allGames = getAllGames();
    sort(allGames.begin(), allGames.end(), [](const Game& a, const Game& b) {
        return a.getPrice() > b.getPrice();
    });
    return allGames;
}
vector<Game> Hash::sortByReleaseDate() const {
    vector<Game> allGames = getAllGames();

    // Map month names to numeric values
    map<string, int> monthMap = {
        {"Jan", 1}, {"Feb", 2}, {"Mar", 3}, {"Apr", 4}, {"May", 5}, {"Jun", 6},
        {"Jul", 7}, {"Aug", 8}, {"Sep", 9}, {"Oct", 10}, {"Nov", 11}, {"Dec", 12}
    };

    // Sort the games by release date
    sort(allGames.begin(), allGames.end(), [&monthMap](const Game& a, const Game& b) {
        const string& date1 = a.getReleaseDate();
        const string& date2 = b.getReleaseDate();

        // Parse year, month, and day from date1
        int year1 = 0, month1 = 0, day1 = 0;
        int year2 = 0, month2 = 0, day2 = 0;

        try {
            // Split date1 into components
            string monthStr1 = date1.substr(0, 3); // First 3 characters are the month
            day1 = stoi(date1.substr(4, date1.find(',') - 4)); // Extract day
            year1 = stoi(date1.substr(date1.find(',') + 2));   // Extract year
            month1 = monthMap[monthStr1]; // Convert month name to numeric value

            // Split date2 into components
            string monthStr2 = date2.substr(0, 3); // First 3 characters are the month
            day2 = stoi(date2.substr(4, date2.find(',') - 4)); // Extract day
            year2 = stoi(date2.substr(date2.find(',') + 2));   // Extract year
            month2 = monthMap[monthStr2]; // Convert month name to numeric value
        } catch (const invalid_argument& e) {
            //cerr << "Invalid date format: " << e.what() << endl;
            return false;
        }

        // Compare year first, then month, then day
        if (year1 != year2) return year1 > year2;
        if (month1 != month2) return month1 > month2;
        return day1 > day2;
    });

    return allGames;
}
