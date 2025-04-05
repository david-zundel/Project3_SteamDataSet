#include <string>
#include <iostream>
#pragma once

using namespace std;

class Game {
private:
    string name;
    double price;
    string genre;
    string release_date;

public:
    // Constructor
    Game(const string& name = "", double price = 0.0, const string& genre = "", const string& release_date = "");

    // functions to get attributes
    string getName() const;
    double getPrice() const;
    string getGenre() const;
    string getReleaseDate() const;

    // functions to set attributes
    void setName(const string& name);
    void setPrice(double price);
    void setGenre(const string& genre);
    void setReleaseDate(const string& release_date);

 
    string toString() const;

    // Overload << operator for easy printing
    friend ostream& operator<<(ostream& os, const Game& game);
};

