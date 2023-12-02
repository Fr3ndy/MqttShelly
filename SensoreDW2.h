/*
* Author: Andrea Spina
* FirstB: 01/04/2023
* Lenguage: c++
* Descrition: This program creates an MQTT client that allows you to add management of Shelly devices present on the MQTT broker.
* It supports the creation and deletion of devices, as well as the creation, modification, and deletion of 1-to-1 routines.
* With this program, you can easily manage your Shelly devices from a single interface and automate their behavior with routines.
*/
#ifndef SENSOREDW_H
#define SENSOREDW_H

#include "MqttClient.h"

//Global define
#include "Define.h"

#include <string>
#include <iostream>

using namespace std;



/*-------------API----------------*/
// https://shelly-api-docs.shelly.cloud/gen1/#shelly-door-window-1-2
//shellies / shellydw - <deviceid> / sensor / state			:door / window sensor state open or close
//shellies / shellydw - <deviceid> / sensor / tilt			: tilt in °, 0..180
//shellies / shellydw - <deviceid> / sensor / vibration		: 0 - no vibration, 1 - vibration detected
//shellies / shellydw - <deviceid> / sensor / lux			: luminance level in lux
//shellies / shellydw - <deviceid> / sensor / battery		: battery level in %
//shellies / shellydw - <deviceid> / sensor / temperature	: Door / Window 2 only Temperature, in C or F as per user configuration
//shellies / shellydw - <deviceid> / sensor / error			: if different from 0, there is an error with the sensor
//shellies / shellydw - <deviceid> / sensor / act_reasons	: list of reasons which woke up the device : battery, button, periodic, poweron, sensor, movement, temperature, light
/*--------------------------------*/

class SensoreDW2 : public MqttClient {
private:

	MqttClient* m_client;

	static const char* TOPIC_STATE;
	static const char* TOPIC_TILT;
	static const char* TOPIC_VIBRATION;
	static const char* TOPIC_LUX;
	static const char* TOPIC_BATTERY;
	static const char* TOPIC_TEMPERATURE;
	static const char* TOPIC_ERROR;
	static const char* TOPIC_ACT_REASONS;

	void subscribe_l(const char* topic);
	void unsubscribe_l(const char* topic);

public:

	SensoreDW2(MqttClient* idc, string ido);
	~SensoreDW2();

	string IDO;

	void get_state();
	void get_tilt();
	void get_vibration();
	void get_lux();
	void get_battery();
	void get_temperature();
	void get_error();
	void get_act_reasons();

	void Get_All_Message();
	
	void Check();

	string messaggio_open;
	string messaggio_tilt;
	string messaggio_vibration;
	string messaggio_lux;
	string messaggio_battery;
	string messaggio_temperature;
	string messaggio_error;
	string messaggio_act_reason;

	static int numObjects; // Numero di istanze create
	static SensoreDW2* objects[MAXNUMDW]; // Array degli oggetti creati
};
#endif