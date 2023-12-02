#include "Application.h"

#include <iostream>
#include <ctime>
#include <chrono>
#include <thread>

int main() {
    try {
        cout << "\x1b[1m";
        Application* client = new Application();
        cout << "\x1b";


        
    }
    catch (exception& e) {
        cerr << "An exception occurred: " << e.what() << endl;
    }
    return 0;
}