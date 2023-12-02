/*
* Author: Andrea Spina
* FirstB: 01/04/2023
* Lenguage: c++
* Descrition: This program creates an MQTT client that allows you to add management of Shelly devices present on the MQTT broker.
* It supports the creation and deletion of devices, as well as the creation, modification, and deletion of 1-to-1 routines.
* With this program, you can easily manage your Shelly devices from a single interface and automate their behavior with routines.
*/
#ifndef PLUG_H
#define PLUG_H



#include "MqttClient.h"
//Global define
#include "Define.h"

#include <string>
#include <iostream>

using namespace std;



/*-------------API----------------*/
//https://shelly-api-docs.shelly.cloud/gen1/#shelly-plug-plugs-overview
//shellies / <model>-<deviceid> / relay / 0							to report status : on, off or overpower
//shellies / <model>-<deviceid> / relay / 0 / power					to report instantaneous power consumption rate in Watts
//shellies / <model>-<deviceid> / relay / 0 / energy				to report amount of energy consumed in Watt - minute
//shellies / <model>-<deviceid> / relay / 0 / overpower_value		reports the value in Watts, on which an overpower condition is detected
//shellies / <model>-<deviceid> / relay / 0 / command				accepts on, off or toggle and applies accordingly
//shellies / shellyplug - s - <deviceid> / temperature				reports internal device temperature in °C
//shellies / shellyplug - s - <deviceid> / temperature_f			reports internal device temperature in °F
//shellies / shellyplug - s - <deviceid> / overtemperature			reports 1 when device has overheated, normally 0
/*--------------------------------*/

class Plug : public MqttClient {
private:
	
	MqttClient* m_client;

	static const char* TOPIC_PLUG;
	static const char* TOPIC_POWER;
	static const char* TOPIC_ENERGY;
	static const char* TOPIC_OVERPOWER_VALUE;
	static const char* TOPIC_COMMAND;
	static const char* TOPIC_TEMPERATURE;
	static const char* TOPIC_TEMPERATURE_F;
	static const char* TOPIC_OVERTEMPERATURE;

	static const char* MESSAGE_ON;
	static const char* MESSAGE_OFF;

	void subscribe_l(const char* topic);
	void publish_l(const char* topic, string message);
	void unsubscribe_l(const char* topic);

public:

	Plug(MqttClient* idc, string ido);
	~Plug();

	string IDO;	

	void set_on();
	void set_off();

	void get_plug();
	void get_power();
	void get_energy();
	void get_overpower_value();
	void get_temperature();
	void get_temperature_f();
	void get_overtemperature();

	void Get_All_Message();

	void Check();

	string messaggio_plug;
	string messaggio_power;
	string messaggio_energy;
	string messaggio_overpower_value;
	string messaggio_temperature;
	string messaggio_temperature_f;
	string messaggio_overtemperature;

	static int numObjects; // Numero di istanze create
	static Plug* objects[MAXNUMPLUG]; // Array degli oggetti creati

};
#endif