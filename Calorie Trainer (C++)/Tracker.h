#ifndef TRACKER_H
#define TRACKER_H

#include "Food.h"
#include "Customer.h"
#include "io.h"
#include <fstream>

using namespace std;

class Food; //shares a friend function with Food class.
class Customer; //friend function of Customer class.

class Tracker: private io {
private:
    struct Daily {
        int day;
        string month;
        int year;
        double calories;
        double protein;
        double carbs;
        double fat;
    };
    struct date {
        int day;
        string month;
        int year;
    } dateToday;

    int numberOfDates;
    string fileName;
    Daily* daily;
public:
    //constructor
    Tracker(const Customer& C);

    //destructor
    ~Tracker();

    //receives the index or number of the user from Customer class and returns the name of an allocated file.
    string getFileName(int num) const;

    //reads the file allocated for the current user into a dynamically allocated array of structs.
    void readFile();

    //writes the dynamic array onto the file allocated for the current user.
    void writeFile() const;

    //appends a single struct into the allocated file.
    void appendFile(const Daily& temp) const;

    //sets the values of dateToday struct member.
    void setDateToday();

    //returns the index of the last struct element of dynamic array.
    int getCurrentdayIndex() const;

    //displays consumed nutritional values of all past days including current day.
    void displayHistory() const;

    //displays remaining nutritional values the user has to consume in order to meet his or her goal.
    void displayIntakeGoal(const Customer& C) const;

    //this function is used to select an appropriate field width for all other usages of setw(int n) functions.
    width widthCalculator(const Daily& temp) const;

    //this friend functions updates the struct array by adding values of selected food to the current day consumption, then updates the file as well.
    friend void eatFood(const Food& F, Tracker& T);
};

#endif
