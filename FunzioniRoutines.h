/*
* Author: Andrea Spina
* FirstB: 01/04/2023
* Lenguage: c++
* Descrition: This program creates an MQTT client that allows you to add management of Shelly devices present on the MQTT broker.
* It supports the creation and deletion of devices, as well as the creation, modification, and deletion of 1-to-1 routines.
* With this program, you can easily manage your Shelly devices from a single interface and automate their behavior with routines.
*/
#ifndef FUNZIONIRUTINES_H
#define FUNZIONIRUTINES_H

using namespace std;


//Global define
#include "Define.h"
#include "DispositivoDefault.h"

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <ctime>
#include <chrono>
#include <filesystem>



class FunzioniRoutines {
public:
    FunzioniRoutines(); // costruttore
    ~FunzioniRoutines(); // distruttore

    void WriteCSV(string name,string variabile);
    void PublishMQTT(string topic, string msg);
  

};

#endif // FUNZIONIRUTINES_H