/*
* Author: Andrea Spina
* FirstB: 01/04/2023
* Lenguage: c++
* Descrition: This program creates an MQTT client that allows you to add management of Shelly devices present on the MQTT broker.
* It supports the creation and deletion of devices, as well as the creation, modification, and deletion of 1-to-1 routines.
* With this program, you can easily manage your Shelly devices from a single interface and automate their behavior with routines.
*/
#ifndef BULBDUO_H
#define BULBDUO_H

//Mqtt
#include "MqttClient.h"

//Global define
#include "Define.h"


using namespace std;


/*-------------API----------------*/
//https://shelly-api-docs.shelly.cloud/gen1/#shelly-duo
//shellies / ShellyBulbDuo - <deviceid> / light / 0 / command		accepts on and off payloads
//shellies / ShellyBulbDuo - <deviceid> / light / 0					is used by the device to report its current on - off state
//shellies / ShellyBulbDuo - <deviceid> / light / 0 / set			For controlling other parameters of the light channel publish 
//shellies / ShellyBulbDuo - <deviceid> / light / 0 / status		Subscribers can use this to obtain the latest device state.
//shellies / ShellyBulbDuo - <deviceid> / light / 0 / power			reports instantaneous power consumption rate in Watts
//shellies / ShellyBulbDuo - <deviceid> / light / 0 / energy		reports amount of energy consumed in Watt - minute
/*--------------------------------*/

class BulbDuo : public MqttClient{
private:
	
	MqttClient* m_client;				

	static const char* TOPIC_COMMAND;
	static const char* TOPIC_SET;
	static const char* TOPIC_STATUS;
	static const char* TOPIC_LIGHT;
	static const char* TOPIC_POWER;
	static const char* TOPIC_ENERGY;

	static const char* MESSAGE_ON;
	static const char* MESSAGE_OFF;

	void subscribe_l(const char* topic);
	void publish_l(const char* topic, string message);
	void unsubscribe_l(const char* topic);

public:
	
	BulbDuo(MqttClient* idc,string ido);
	~BulbDuo();

	string IDO;

	void set_on();
	void set_off();
	void set_status(string state);

	void get_status();
	void get_power();
	void get_energy();

	void Get_All_Message();

	void Check();

	string messaggio_light;
	string messaggio_status;
	string messaggio_power;
	string messaggio_energy;

	static int numObjects; // Numero di istanze create
	static BulbDuo* objects[MAXNUMLAMP]; // Array degli oggetti creati

};
#endif