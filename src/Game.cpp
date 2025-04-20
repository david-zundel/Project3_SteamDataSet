#include "Game.h"
#include <iostream>
#include <string>
using namespace std;

//constructor
Game::Game(const string& name, double price, const string& genre, const string& release_date, 
           int required_age, const string& estimated_owners, const string& appID) {
    this->name = name;
    this->price = price;
    this->genre = genre;
    this->release_date = release_date;
    this->required_age = required_age;
    this->estimated_owners = estimated_owners;
    this->appID = appID;
}

// getters
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

int Game::getRequiredAge() const {
    return required_age;
}

string Game::getEstimatedOwners() const {
    return estimated_owners;
}

string Game::getAppID() const {
    return appID;
}

// Setters
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

void Game::setRequiredAge(int required_age) {
    this->required_age = required_age;
}

void Game::setEstimatedOwners(const string& estimated_owners) {
    this->estimated_owners = estimated_owners;
}

// func to get it to string
string Game::toString() const {
    return "Name: " + name + ", Price: $" + to_string(price) + ", Genre: " + genre + 
           ", Release Date: " + release_date + ", Required Age: " + to_string(required_age) + 
           ", Estimated Owners: " + estimated_owners;
}

// overload << operator
ostream& operator<<(ostream& os, const Game& game) {
    os << game.toString();
    return os;
}