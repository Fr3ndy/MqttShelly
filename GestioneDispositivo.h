/*
* Author: Andrea Spina
* FirstB: 01/04/2023
* Lenguage: c++
* Descrition: This program creates an MQTT client that allows you to add management of Shelly devices present on the MQTT broker.
* It supports the creation and deletion of devices, as well as the creation, modification, and deletion of 1-to-1 routines.
* With this program, you can easily manage your Shelly devices from a single interface and automate their behavior with routines.
*/
#ifndef CREAZIONEDISPOSITIVO_H
#define CREAZIONEDISPOSITIVO_H

//Global define
#include "Define.h"

#include "Observer.h"
#include "MqttClient.h"

#include "BulbDuo.h"
#include "Plug.h"
#include "SensoreHT.h"
#include "SensoreDW2.h"

#include "DispositivoDefault.h"

#include <string>
#include <memory>
#include <fstream>
#include <iostream>
#include <memory>
#include <filesystem>



using namespace std;
namespace fs = filesystem;

class GestioneDispositivi : public Observer, public MqttClient {
public:
	MqttClient* m_client;

	GestioneDispositivi(MqttClient* idc);



private:

	void update() override;

	bool create(string msg);
	void erase(string msg);

	void write(string msg);
	vector<string> names;


	const string Bulbs = "ShellyBulbDuo-";
	const string Plugs = "shellyplug-s-";
	const string HETs = "shellyht-";
	const string DWs = "shellydw2-";

	const string Create = "Shelly/CreateDevice/";
	const string Delete = "Shelly/DeleteDevice/";

	const string folderName = "Devices";
	const string CompletePath = "./Devices/Dispositivi.txt";

	const string TempPath = "./Devices/temp.txt";
};

#endif
