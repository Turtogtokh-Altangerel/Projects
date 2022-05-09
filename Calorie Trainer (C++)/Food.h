#ifndef FOOD_H
#define FOOD_H

#include "Tracker.h"
#include "io.h"
#include <fstream>

using namespace std;

class Tracker; //shares a friend funcction with Tracker class.

class Food: private io {
private:
    struct nutrition {
        string name;
        double calories;
        double protein;
        double carbs;
        double fat;
    };
    int numberOfFood;
    nutrition* records;
    string fileName;
public:
    //constructor
    Food();

    //destructor
    ~Food();

    //reads food database into a dynamically allocated array of structs.
    void readFile();

    //appends a single struct into food datatbase.
    void appendFile(const nutrition& temp) const;

    //searches a food from the dynamic array and returns its index.
    int searchFood() const;

    //adds a food by directly appending the struct into food database, then reads back the database into newly allocated dynamic array.
    void addFood();

    //searches a food and displays its nutritional values.
    void displayFood() const;

    //displays all food with their nutritional values in a tabular format.
    void displayAllFood() const;

    //this function is used to select an appropriate field width for all other usages of setw(int n) functions.
    width widthCalculator(const nutrition& temp) const;

    //food class provides nutritional values of requested food to this friend function.
    friend void eatFood(const Food& F, Tracker& T);
};

#endif
