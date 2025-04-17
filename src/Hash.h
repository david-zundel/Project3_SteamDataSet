// Created by Jeremiah on 2025-04-16.
//

#ifndef HASH_H
#define HASH_H


#include <vector>
#include <list>
#include <string>
#include "Game.h"

class Hash {
public:
    explicit Hash(int size = 101);

    // Insert or update a Game
    void insert(const Game& game);
    // Remove by name
    bool remove(const std::string& name);
    // Search by name
    Game* search(const std::string& name);
    std::vector<Game> searchByName(const std::string& term) const;

    // Search by other fields
    std::vector<Game> searchByGenre(const std::string& genre) const;
    std::vector<Game> searchByReleaseDate(const std::string& date) const;

    // Sorting
    std::vector<Game> sortByPriceAsc() const;
    std::vector<Game> sortByPriceDesc() const;
    std::vector<Game> sortByGenre() const;
    std::vector<Game> sortByName() const;
    std::vector<Game> sortByReleaseDate() const;

    // Debug printing
    void printTable() const;

private:
    // Helper to gather all games
    std::vector<Game> getAllGames() const;
    // Hash function
    size_t hashFunction(const std::string& key) const;

    int capacity;
    std::vector<std::list<Game>> table;
};

#endif //HASH_H
