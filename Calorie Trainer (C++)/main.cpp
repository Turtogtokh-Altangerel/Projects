#include "Food.h"
#include "Customer.h"
#include "Tracker.h"
#include "io.h"

//this function receives a selected food from the user, retrieves its nutritional values from Food class
//and adds to the daily array in Customer class so that a daily nutrional intake is tracked.
void eatFood(const Food& F, Tracker& T) { //receives food and tracker classes.
    int index = F.searchFood();
    cout << "Enter portion size (in grams): ";
    double portion = T.cinput(0) / 100;
    int today = T.getCurrentdayIndex();
    T.daily[today].calories = T.daily[today].calories + F.records[index].calories * portion;
    T.daily[today].carbs = T.daily[today].carbs + F.records[index].carbs * portion;
    T.daily[today].fat = T.daily[today].fat + F.records[index].fat * portion;
    T.daily[today].protein = T.daily[today].protein + F.records[index].protein * portion;
    T.writeFile();
    cout << endl << "You just ate " << portion*100 << " grams of " << F.records[index].name << endl;
}


int main()
{
    Customer C;
    Food F;
    io I;
    switch (I.selectUser()) {
    case 1:
        C.addCustomer(); //new user.
        break;
    case 2: 
        C.setCustomerNum(C.findCustomer()); //sets the customerNum by finding the existing user.
        break;
    }
    Tracker T(C); //Tracker class can be initialized after Customer class is created and initialized.
    while (true) { //the loop repeats until the user exits.
        switch (I.repeat()) {  //asks the user whether to continue.
        case true:
            switch (I.menu()) { //displays menu.
            case 1:
                F.addFood();
                break;
            case 2:
                eatFood(F, T);
                break;
            case 3:
                F.displayFood();
                break;
            case 4:
                C.displayCustomer();
                break;
            case 5:
                C.changeValues();
                break;
            case 6:
                T.displayHistory();
                break;
            case 7:
                T.displayIntakeGoal(C);
                break;
            case 8:
                F.displayAllFood();
                break;
            case 9:
                return 0;
            }
            break;
        case false:
            return 0;
        }
    }
}