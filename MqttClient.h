/*
* Author: Andrea Spina
* FirstB: 01/04/2023
* Lenguage: c++
* Descrition: This program creates an MQTT client that allows you to add management of Shelly devices present on the MQTT broker.
* It supports the creation and deletion of devices, as well as the creation, modification, and deletion of 1-to-1 routines.
* With this program, you can easily manage your Shelly devices from a single interface and automate their behavior with routines.
*/
#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

//Global define
#include "Define.h"

#include "Subject.h"

#include <iostream>
#include <mosquittopp.h>
#include <mosquitto.h>
#include <string>
#include <map>

#include <filesystem>
#include <iostream>
#include <fstream>



using namespace std;
namespace fs = std::filesystem;


class MqttClient : public mosqpp::mosquittopp{
public:

    MqttClient();
    ~MqttClient();

    void leggifile();

    void connect_to_broker(const char* host, int port);
    void on_message(const mosquitto_message* message) override;
    int publish(const char* topic, const char* message, int qos = 0, bool retain = false);

    void iscriviti(const char* topic_subscribe, int qos = 0);
    void disiscriviti(const char* topic);

    void on_disconnect(int rc);
    
    string MSG[2];

    static Subject subject;
    static void invia_messg();

    bool connesso = false;
};
#endif