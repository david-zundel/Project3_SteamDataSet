//
// Created by Zachary on 2025-04-16.
//

// Helped by code from GeeksForGeeks: https://www.geeksforgeeks.org/introduction-to-red-black-tree/#1-insertion in my parts of the implementation of the Red Black Tree

#include "rb.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <map>

rb::rb() {
    leaf = nullptr;
    root = leaf;
}

void rb::insert(const Game& game) {
    Node* new_node = new Node(game);
    new_node->left = leaf;
    new_node->right = leaf;

    Node* parent = nullptr;
    Node* current = root;

    // BST insert
    while (current != leaf) {
        parent = current;
        if ((new_node->data) < (current->data)) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }
    new_node->parent = parent;

    if (parent == nullptr) {
        root = new_node;
    }
    else if (new_node->data < parent->data) {
        parent->left = new_node;
    }
    else {
        parent->right = new_node;
    }

    if (new_node->parent == nullptr) {
        new_node->color = "BLACK";
        return;
    }

    if (new_node->parent->parent == nullptr) {
        return;
    }
    fixTree(new_node);
}

// Right Rotation
void rb::rotateRight(Node* n) {
    Node* n2 = n->left;
    n->left = n2->right;
    if (n2->right != leaf) {
        n2->right->parent = n;
    }
    n2->parent = n->parent;
    if (n->parent == nullptr) {
        root = n2;
    }
    else if (n == n->parent->right) {
        n->parent->right = n2;
    }
    else {
        n->parent->left = n2;
    }
    n2->right = n;
    n->parent = n2;
}

// Left Rotation
void rb::rotateLeft(Node* n) {
    Node* n2 = n->right;
    n->right = n2->left;
    if (n2->left != leaf) {
        n2->left->parent = n;
    }
    n2->parent = n->parent;
    if (n->parent == nullptr) {
        root = n2;
    }
    else if (n == n->parent->left) {
        n->parent->left = n2;
    }
    else {
        n->parent->right = n2;
    }
    n2->left = n;
    n->parent = n2;
}

// Balances the tree and makes sure each node's colors are correct
void rb::fixTree(Node* n) {
    while ((n != root) && (n->parent->color == "RED")) {
        if (n->parent == n->parent->parent->left) {
            Node* uncle = n->parent->parent->right;

            if (uncle != nullptr && uncle->color == "RED") {
                n->parent->color = "BLACK";
                uncle->color = "BLACK";
                n->parent->parent->color = "RED";
                n = n->parent->parent;
            } else {
                if (n == n->parent->right) {
                    n = n->parent;
                    rotateLeft(n);
                }
                n->parent->color = "BLACK";
                n->parent->parent->color = "RED";
                rotateRight(n->parent->parent);
            }
        } else {
            Node* uncle = n->parent->parent->left;

            if (uncle != nullptr && uncle->color == "RED") {
                n->parent->color = "BLACK";
                uncle->color = "BLACK";
                n->parent->parent->color = "RED";
                n = n->parent->parent;
            } else {
                if (n == n->parent->left) {
                    n = n->parent;
                    rotateRight(n);
                }
                n->parent->color = "BLACK";
                n->parent->parent->color = "RED";
                rotateLeft(n->parent->parent);
            }
        }
    }
    root->color = "BLACK";
}



// Gets list of Games that will be used by search functions
vector<Game> rb::rbGetAllGames() {
    vector<Game> result;
    result = searchHelper(root);
    return result;
}

// Gets List of all Games
vector<Game> rb::searchHelper(Node* n) {
    if (n == nullptr || n == leaf) {
        return {};
    }

    vector<Game> leftGames = searchHelper(n->left);
    vector<Game> rightGames = searchHelper(n->right);

    vector<Game> result;
    result.push_back(n->game);
    result.insert(result.end(), leftGames.begin(), leftGames.end());
    result.insert(result.end(), rightGames.begin(), rightGames.end());

    return result;
}


// Searches for a game by Name
Game* rb::rbSearchByName(rb& tree, const string& term) const {
    auto gameList = tree.rbGetAllGames();
    for (int i = 0; i < gameList.size(); i++) {
        if (gameList[i].getName() == term) {
            return new Game(gameList[i]);
        }
    }
    return nullptr;
}

// Searches for a game by Genre
vector<Game> rb::rbSearchByGenre(rb& tree, const string& genre) const {
    vector<Game> result;
    auto gameList = tree.rbGetAllGames();
    for (const auto& g : gameList) {
        if (g.getGenre() == genre) {
            result.push_back(g);
        }
    }
    return result;
}

// Searches for a game by Release Date
vector<Game> rb::rbSearchByReleaseDate(rb& tree, const string& date) const {
    vector<Game> result;
    auto gameList = tree.rbGetAllGames();
    for (const auto& g : gameList) {
        if (g.getReleaseDate() == date) {
            result.push_back(g);
        }
    }
    return result;
}

// Searches for a game by Required Age
vector<Game> rb::rbSearchByRequiredAge(rb& tree, int maxAge) const {
    vector<Game> result;
    auto gameList = tree.rbGetAllGames();
    for (const auto& g : gameList) {
        if (g.getRequiredAge() <= maxAge) { 
            result.push_back(g);
        }
    }
    return result;
}

// Searches for a game by Required Age
vector<Game> rb::rbSearchByEstimatedOwners(rb& tree, const string& minOwners) const {
    vector<Game> result;
    auto gameList = tree.rbGetAllGames();
    for (const auto& g : gameList) {
        string numOwners = g.getEstimatedOwners();
        string num = "";
        for (char c : numOwners) {
            if (isdigit(static_cast<unsigned char>(c))) {
                num += c;
            } 
            if (c == '-') {
               break;
            }
        }
        if (stoi(num) > stoi(minOwners)) {
            result.push_back(g);
        }

    }
    return result;
}

// Prints information about the Dataset
void rb::printTable() const {
    cout << "\n--- Dataset Statistics ---\n";
    cout << "Total games (nodes): " << getNumNodes(root) << "\n";
    cout << "Total Levels: " << getHeight(root) << "\n";
}

// Gets number of Nodes in Tree
int rb::getNumNodes(Node* node) const {
    if (node == nullptr || node == leaf) {
        return 0;
    }
    return 1 + getNumNodes(node->left) + getNumNodes(node->right);
}

// Gets height of tree
int rb::getHeight(Node* node) const {
    if (node == nullptr || node == leaf) {
        return 0;
    }
    int leftHeight = getHeight(node->left);
    int rightHeight = getHeight(node->right);
    return 1 + max(leftHeight, rightHeight);

}

// Sorts by Price Ascending
vector<Game> rb::rbSortByPriceAsc(rb& tree) const {
    auto result = tree.rbGetAllGames();
    sort(result.begin(), result.end(), [](const Game& a, const Game& b) {
        return a.getPrice() < b.getPrice();
    });
    return result;
}

// Sorts by Price Descending
vector<Game> rb::rbSortByPriceDesc(rb& tree) const {
    auto result = tree.rbGetAllGames();
    sort(result.begin(), result.end(), [](const Game& a, const Game& b) {
        return a.getPrice() > b.getPrice();
    });
    return result;
}

// Sorts by Date
vector<Game> rb::rbSortByReleaseDate(rb& tree) const {
    vector<Game> allGames = tree.rbGetAllGames();

    map<string, int> monthMap = {
        {"Jan", 1}, {"Feb", 2}, {"Mar", 3}, {"Apr", 4}, {"May", 5}, {"Jun", 6},
        {"Jul", 7}, {"Aug", 8}, {"Sep", 9}, {"Oct", 10}, {"Nov", 11}, {"Dec", 12}
    };

    sort(allGames.begin(), allGames.end(), [&monthMap](const Game& a, const Game& b) {
        auto parseDate = [&monthMap](const string& date, int& year, int& month, int& day) {
            try {
                string monthStr = date.substr(0, 3);
                day = stoi(date.substr(4, date.find(',') - 4));
                year = stoi(date.substr(date.find(',') + 2));
                month = monthMap.at(monthStr);
            } catch (...) {
                year = month = day = 0;
            }
        };

        int y1, m1, d1, y2, m2, d2;
        parseDate(a.getReleaseDate(), y1, m1, d1);
        parseDate(b.getReleaseDate(), y2, m2, d2);

        if (y1 != y2) return y1 > y2;
        if (m1 != m2) return m1 > m2;
        return d1 > d2;
    });

    return allGames;
}
