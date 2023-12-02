/*
* Author: Andrea Spina
* FirstB: 01/04/2023
* Lenguage: c++
* Descrition: This program creates an MQTT client that allows you to add management of Shelly devices present on the MQTT broker.
* It supports the creation and deletion of devices, as well as the creation, modification, and deletion of 1-to-1 routines.
* With this program, you can easily manage your Shelly devices from a single interface and automate their behavior with routines.
*/
#include <iostream>
#include <string.h>

//Global define
#include "Define.h"

//mqttclient
#include "MqttClient.h"

//creazione dispositivi
#include "GestioneDispositivo.h"

//routines
#include "GestioneRoutines.h"

using namespace std;

//instanziamo la classe figlia di mqttclient
class Application{
public:

	Application();
	~Application();

};



