#include "Subject.h"
#include <string>
#include <iostream>

void Subject::attach(Observer* observer) {
    if (observer == nullptr) {
        // handle the error here
        std::cerr <<"\033[1;31m" << "Error: observer pointer is null." << "\033[" << std::endl;
        // You can also throw an error or return an error code here if needed
        return;
    }
    observers.push_back(observer);
}

void Subject::detach(Observer* observer) {
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

void Subject::notify() {
   // std::cout << "La grandezza del vettore è: " << observers.size() << std::endl;
    for (Observer* observer : observers) {
        try {
            observer->update();
        }
        catch (const std::exception& e) {
            // handle the error here
            std::cerr << "\033[1;31m" << "An error occurred while updating observer: " << e.what() << "\033[0m" << std::endl;
             //You can also throw an error or return an error code here if needed
        }
    }
}



