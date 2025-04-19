//
// Created by Jeremiah on 2025-04-16.
//

#include "Hash.h"
#include <functional>
#include <algorithm>
#include <iostream>
#include <cctype>

Hash::Hash(int size)
    : capacity(size), table(size)
{}

size_t Hash::hashFunction(const std::string& key) const {
    return std::hash<std::string>{}(key) % capacity;
}

void Hash::insert(const Game& game) {
    auto& chain = table[hashFunction(game.getName())];
    for (auto& g : chain) {
        if (g.getName() == game.getName()) {
            g = game;
            return;
        }
    }
    chain.push_back(game);
}

bool Hash::remove(const std::string& name) {
    auto& chain = table[hashFunction(name)];
    for (auto it = chain.begin(); it != chain.end(); ++it) {
        if (it->getName() == name) {
            chain.erase(it);
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
    for (int i = 0; i < capacity; ++i) {
        if (!table[i].empty()) {
            std::cout << "Bucket " << i << ":\n";
            for (auto const& g : table[i]) std::cout << "  " << g << "\n";
        }
    }
}

std::vector<Game> Hash::getAllGames() const {
    std::vector<Game> result;
    for (auto const& chain : table) for (auto const& g : chain) result.push_back(g);
    return result;
}

