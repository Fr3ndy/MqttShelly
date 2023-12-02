/*
* Author: Andrea Spina
* FirstB: 01/04/2023
* Lenguage: c++
* Descrition: This program creates an MQTT client that allows you to add management of Shelly devices present on the MQTT broker.
* It supports the creation and deletion of devices, as well as the creation, modification, and deletion of 1-to-1 routines.
* With this program, you can easily manage your Shelly devices from a single interface and automate their behavior with routines.
*/
#ifndef DISPOSITIVODEFAULT_H
#define DISPOSITIVODEFAULT_H

#include "MqttClient.h"

//Global define
#include "Define.h"

#include <string>
#include <iostream>

using namespace std; 

class DispositivoDefault : public MqttClient {
private:
	MqttClient* m_client;
public:

	DispositivoDefault(MqttClient* client);
	static DispositivoDefault* ogg[1];

	void publica(string topic, string msg);
};

#endif