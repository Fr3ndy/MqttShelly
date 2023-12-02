/*
* Author: Andrea Spina
* FirstB: 01/04/2023
* Lenguage: c++
* Descrition: This program creates an MQTT client that allows you to add management of Shelly devices present on the MQTT broker.
* It supports the creation and deletion of devices, as well as the creation, modification, and deletion of 1-to-1 routines.
* With this program, you can easily manage your Shelly devices from a single interface and automate their behavior with routines.
*/
#ifndef GESTIONE_H
#define GESTIONE_H


//Global define
#include "Define.h"

#include "JsonManagerRoutines.h"
#include "Observer.h"
#include "MqttClient.h"

#include "Routines.h"

#include <string>
#include <memory>
#include <fstream>
#include <iostream>
#include <filesystem>


using namespace std;
using json = nlohmann::json;
namespace fs = filesystem;

class GestioneRoutines : public Observer, public MqttClient{
public:
	MqttClient* m_client;

	GestioneRoutines(MqttClient* idc);

	vector<Routines*> objects;
	vector<string> names;
	
private:

	void update() override;

	void create(string msg);
	void status(string msg);
	void erase(string msg);
	
	const string Create = "Shelly/CreateRoutine/";
	const string State = "Shelly/StateRoutine/";
	const string Delete = "Shelly/DeleteRoutine/";

    const string folderName = "Routines/";
    const string estensione = ".json";

    const map<string,map<string,string>> topic_msg{
        /*
        topic collegati alla variabile dei dispositivi
        i topic vuoti come /0 per on off vanno lasciati per ultimi
        */
        //bulbduo        
        {"ShellyBulbDuo", { 
            {"/0","messaggio_light"},
            {"energy","messaggio_energy"},
            {"power","messaggio_power"},
            {"status","messaggio_status"}
        }},
        //plug
        {"shellyplug", {
            {"overtemperature","messaggio_overtemperature"},
            {"temperature_f","messaggio_temperature_f"},
            {"temperature","messaggio_temperature"},
            {"overpower_value","messaggio_overpower_value"},
            {"energy","messaggio_energy"},
            {"power","messaggio_power"},
            {"/0","messaggio_plug"}
        }},
        //dw
        {"shellydw", {
            {"act_reasons","messaggio_act_reason"},
            {"error","messaggio_error"},
            {"temperature","messaggio_temperature"},
            {"battery","messaggio_battery"},
            {"lux","messaggio_lux"},
            {"vibration","messaggio_vibration"},
            {"tilt","messaggio_tilt"},
            {"state","messaggio_open"}
        }},
        //ht
        {"shellyht", {
            {"ext_power","messaggio_ext_power"},
            {"act_reasons","messaggio_act_reason"},
            {"error","messaggio_error"},
            {"battery","messaggio_battery"},
            {"humidity","messaggio_humidity"},
            {"temperature","messaggio_temperature"}
        }}
    };

};

#endif // ROUTINES_H