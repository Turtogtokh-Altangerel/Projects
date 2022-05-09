#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "io.h"
#include <fstream>

using namespace std;

class Customer: private io {
    friend class Tracker;
private:
    struct Info{
        string name;
        double weight;
        double height;
        double age;
        char gender;
        double activityLevel;
        double bmr;
        double dailyCalorie;
        double weightLossCalorie;
        double weightGainCalorie;
        double dailyProtein;
    };
    Info* customers;
    int numberOfCustomers;
    int customerNum;
    string fileName;
public:
    //constructor
    Customer();

    //destructor
    ~Customer();

    //reads customer database into a dynamically allocated array of structs.
    void readFile();

    //writes array of structs into customer datatbase.
    void writeFile() const;

    //appends a new user into customer datatbase.
    void appendFile(const Info& temp) const;

    //returns the character for gender of the current user.
    char genderSelector() const;

    //overloaded function that returns either 'male' or 'female' string depending on the char received.
    string genderSelector(const char& ch) const;

    //returns the multiplier constant of activity measurement.
    double activitySelector() const;

    //overloaded function that returns a description for user's activity level depending on the value received.
    string activitySelector(const double& num) const;

    //receives user information and returns BMR value.
    double calculateBmr(const char& gender, const double& weight, const double& height, const double& age) const;

    //changes measurements of the user.
    void changeValues();

    //the index of the member struct in the array  is the customer No.
    void setCustomerNum(const int& i);

    //function for adding new customer.
    void addCustomer();

    //searches the records array of customers.
    int findCustomer() const;

    //displays the user's measurements.
    void displayCustomer() const;

    //this function is used to select an appropriate field width for all other usages of setw(int n) functions.
    width widthCalculator(const Info& temp) const;
};

#endif