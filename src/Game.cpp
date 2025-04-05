#include "Game.h"
#include <iostream>
#include <string>
using namespace std;

// Constructor
Game::Game(const string& name, double price, const string& genre, const string& release_date) {
    this->name = name;
    this->price = price;
    this->genre = genre;
    this->release_date = release_date;
}


string Game::getName() const {
    return name;
}

double Game::getPrice() const {
    return price;
}

string Game::getGenre() const {
    return genre;
}

string Game::getReleaseDate() const {
    return release_date;
}


void Game::setName(const string& name) {
    this->name = name;
}

void Game::setPrice(double price) {
    this->price = price;
}

void Game::setGenre(const string& genre) {
    this->genre = genre;
}

void Game::setReleaseDate(const string& release_date) {
    this->release_date = release_date;
}


string Game::toString() const {
    return "Name: " + name + ", Price: $" + to_string(price) + ", Genre: " + genre + ", Release Date: " + release_date;
}

// Overload << operator
ostream& operator<<(ostream& os, const Game& game) {
    os << game.toString();
    return os;
}