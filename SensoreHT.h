/*
* Author: Andrea Spina
* FirstB: 01/04/2023
* Lenguage: c++
* Descrition: This program creates an MQTT client that allows you to add management of Shelly devices present on the MQTT broker.
* It supports the creation and deletion of devices, as well as the creation, modification, and deletion of 1-to-1 routines.
* With this program, you can easily manage your Shelly devices from a single interface and automate their behavior with routines.
*/
#ifndef SENSOREHT_H
#define SENSOREHT_H

#include "MqttClient.h"
//Global define
#include "Define.h"

#include <string>
#include <iostream>





using namespace std;

/*-------------API----------------*/
//https://shelly-api-docs.shelly.cloud/gen1/#shelly-h-amp-t-settings
//shellies / shellyht - <deviceid> / sensor / temperature:		in °C or °F depending on configuration
//shellies / shellyht - <deviceid> / sensor / humidity			RH in %
//shellies / shellyht - <deviceid> / sensor / battery :			battery level in %
//shellies / shellyht - <deviceid> / sensor / error :			if different from 0, there is an error with the sensor
//shellies / shellyht - <deviceid> / sensor / act_reasons :		list of reasons which woke up the device : battery, button, periodic, poweron, sensor, alarm
//shellies / shellyht - <deviceid> / sensor / ext_power :		true, if the device is usb powered
/*--------------------------------*/

class SensoreHT : public MqttClient{
private:
	
	MqttClient* m_client;				

	static const char* TOPIC_TEMPERATURE;
	static const char* TOPIC_HUMIDITY;
	static const char* TOPIC_BATTERY;
	static const char* TOPIC_ERROR;
	static const char* TOPIC_ACT_REASONS;
	static const char* TOPIC_EXT_POWER;

	void subscribe_l(const char* topic);
	void unsubscribe_l(const char* topic);
	
public:

	SensoreHT(MqttClient* idc, string ido);
	~SensoreHT();

	string IDO;

	void get_temperature();
	void get_humidity();
	void get_battery();
	void get_error();
	void get_act_reason();
	void get_ext_power();
	
	void Get_All_Message();

	void Check();

	string messaggio_temperature;
	string messaggio_humidity;
	string messaggio_battery;
	string messaggio_error;
	string messaggio_act_reason;
	string messaggio_ext_power;

	static int numObjects; // Numero di istanze create
	static SensoreHT* objects[MAXNUMHT]; // Array degli oggetti creati
};
#endif