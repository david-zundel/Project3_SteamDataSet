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
    int required_age;
    string estimated_owners;

public:
    // Constructor
    Game(const string& name = "", double price = 0.0, const string& genre = "", const string& release_date = "", 
        int required_age = 0, const string& estimated_owners = "");


    // functions to get attributes
    string getName() const;
    double getPrice() const;
    string getGenre() const;
    string getReleaseDate() const;
    int getRequiredAge() const;
    string getEstimatedOwners() const;

    // functions to set attributes
    void setName(const string& name);
    void setPrice(double price);
    void setGenre(const string& genre);
    void setReleaseDate(const string& release_date);
    void setRequiredAge(int required_age);
    void setEstimatedOwners(const string& estimated_owners);

 
    string toString() const;

    // overload the  << operator for printing
    friend ostream& operator<<(ostream& os, const Game& game);
};

