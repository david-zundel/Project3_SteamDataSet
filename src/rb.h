//
// Created by Zachary on 2025-04-16.
//


#ifndef RB_H
#define RB_H

#include <vector>
#include <string>
#include <map>
#include "Game.h"

using namespace std;

// Node structure for the Red-Black Tree
struct Node {
    Game game;
    int data;
    string color;
    Node *left, *right, *parent;

    Node(Game g) {
        game = g;
        data = stoi(g.getAppID());
        color = "RED";
        left = nullptr;
        right = nullptr;
        parent = nullptr;


    }
};

class rb {
private:
    Node* root;
    Node* leaf;

    void rotateLeft(Node* n);
    void rotateRight(Node* n);
    void fixTree(Node* n);
    vector<Game> searchHelper(Node* node);

public:
    rb();

    void insert(const Game& game);

    int getNumNodes(Node* node) const;
    int getHeight(Node* node) const;

    vector<Game> rbGetAllGames();

    Game* rbSearchByName(rb& tree, const string& term) const;
    vector<Game> rbSearchByGenre(rb& tree, const string& genre) const;
    vector<Game> rbSearchByReleaseDate(rb& tree, const string& date) const;
    vector<Game> rbSearchByRequiredAge(rb& tree, int maxAge) const;
    vector<Game> rbSearchByEstimatedOwners(rb& tree, const string& minOwners) const;


    void printTable() const;
    vector<Game> rbSortByPriceAsc(rb& tree) const;
    vector<Game> rbSortByPriceDesc(rb& tree) const;
    vector<Game> rbSortByReleaseDate(rb& tree) const;

};

#endif
