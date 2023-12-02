/*
* Author: Andrea Spina
* FirstB: 01/04/2023
* Lenguage: c++
* Descrition: This program creates an MQTT client that allows you to add management of Shelly devices present on the MQTT broker.
* It supports the creation and deletion of devices, as well as the creation, modification, and deletion of 1-to-1 routines.
* With this program, you can easily manage your Shelly devices from a single interface and automate their behavior with routines.
*/

#ifndef ROUTINES_H
#define ROUTINES_H

//Global define
#include "Define.h"

#include "BulbDuo.h"
#include "Plug.h"
#include "SensoreDW2.h"
#include "SensoreHT.h"

#include "JsonManagerRoutines.h"
#include "FunzioniRoutines.h"

#include <string>
#include <iostream>
#include <map>
#include <functional>
#include <unordered_map>
#include <fstream>



using namespace std;

class Routines{
public:
	Routines(string name, string status,string change, string cid, string cmsg, string co, string cc, string ai, string ac, vector<string> ap);
	~Routines();

	void controllo();
	void inizializza();

	string l_name;
	string l_status;

	string l_condiz_id;
	string l_condiz_cmd;
	string l_condiz_msg;

	bool error = false;

private:

	const string BULBDUO = "ShellyBulbDuo-";
	const string PLUG = "shellyplug-s-";
	const string SENSOREHT = "shellyht-";
	const string SENSOREDW = "shellydw2-";
	const string FUNZIONE = "Funzione";
		
	void CondizBulbDuo();
	void ComandoBulbDuo();

	void CondizPlug();
	void ComandoPlug();
	
	void CondizSensoreDW();
	
	void CondizSensoreHT();

	void ComandoFunzione();



	string GetTime();

	string variabile;
	string last_message = "a";
	
	
	string l_condiz_operator;
	string l_only_on_change;
	string l_action_id;
	string l_action_cmd;
	vector<string> l_action_func_param;
	string TipoOggCondiz;
	string TipoOggComando;
	void* OggCondiz = nullptr;
	void* OggComando = nullptr;

	int condiz = 0;

	int j = 0;
};

#endif // ROUTINES_H