#ifndef SUBJECT_H
#define SUBJECT_H

#include <vector>
#include "Observer.h"

class Subject {
public:
    void attach(Observer* observer);
    void detach(Observer* observer);
    void notify();
private:
    std::vector<Observer*> observers;
};
#endif