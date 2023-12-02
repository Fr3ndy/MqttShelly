#include "Plug.h"

//---------------------------------------------------------------------

const char* Plug::TOPIC_PLUG = "shellies/%s/relay/0";
const char* Plug::TOPIC_POWER = "shellies/%s/relay/0/power";
const char* Plug::TOPIC_ENERGY = "shellies/%s/relay/0/energy";
const char* Plug::TOPIC_OVERPOWER_VALUE = "shellies/%s/relay/0/overpower_value";
const char* Plug::TOPIC_COMMAND = "shellies/%s/relay/0/command";

const char* Plug::TOPIC_TEMPERATURE = "shellies/%s/temperature";
const char* Plug::TOPIC_TEMPERATURE_F = "shellies/%s/temperature_f";
const char* Plug::TOPIC_OVERTEMPERATURE = "shellies/%s/overtemperature";
		   
const char* Plug::MESSAGE_ON = "on";
const char* Plug::MESSAGE_OFF = "off";

int Plug::numObjects = 0;
Plug* Plug::objects[MAXNUMPLUG];

//---------------------------------------------------------------------

Plug::Plug(MqttClient* client, string id) :
	m_client(client)
{
	
	if (numObjects < MAXNUMPLUG) {
		IDO = id;
		objects[numObjects++] = this;
		Get_All_Message();
	}
}

Plug::~Plug() {
	const char* topic[] = { TOPIC_TEMPERATURE,TOPIC_POWER,TOPIC_ENERGY,TOPIC_OVERPOWER_VALUE,TOPIC_OVERTEMPERATURE,TOPIC_PLUG,TOPIC_TEMPERATURE_F};

	size_t size = sizeof(topic) / sizeof(topic[0]);

	for (size_t i = 0; i < size; ++i) {
		unsubscribe_l(topic[i]);
	}
}

//---------------------------------------------------------------------

void Plug::subscribe_l(const char* topic)
{
	char buffer_s[100];
	snprintf(buffer_s, 100, topic, IDO.c_str());
	m_client->iscriviti(buffer_s);
}

void Plug::publish_l(const char* topic, string message)
{
	char buffer_p[400];
	snprintf(buffer_p, 400, topic, IDO.c_str());
	m_client->publish(buffer_p, (message.c_str()));
}

void Plug::unsubscribe_l(const char* topic) {
	char buffer_s[100];
	snprintf(buffer_s, 100, topic, IDO.c_str());
	m_client->disiscriviti(buffer_s);
}

//---------------------------------------------------------------------

void Plug::set_on() 
{

	publish_l(TOPIC_COMMAND, MESSAGE_ON);
}

void Plug::set_off() 
{
	publish_l(TOPIC_COMMAND, MESSAGE_OFF);
}

void Plug::get_plug() 
{
	subscribe_l(TOPIC_PLUG);
}

void Plug::get_power() 
{
	subscribe_l(TOPIC_POWER);
}

void Plug::get_energy() 
{
	subscribe_l(TOPIC_ENERGY);
}

void Plug::get_overpower_value() 
{
	subscribe_l(TOPIC_OVERPOWER_VALUE);
}

void Plug::get_temperature() 
{
	subscribe_l(TOPIC_TEMPERATURE);
}

void Plug::get_temperature_f() 
{
	subscribe_l(TOPIC_TEMPERATURE_F);
}

void Plug::get_overtemperature()
{
	subscribe_l(TOPIC_OVERTEMPERATURE);
}

void Plug::Get_All_Message() {
	get_plug();
	get_power();
	get_energy();
	get_overpower_value();
	get_temperature();
	get_temperature_f();
	get_overtemperature();
}

//---------------------------------------------------------------------

void Plug::Check() {
	
	if (m_client->MSG[0].find(IDO) == string::npos) {
		// message not related to this instance
		return;
	}

	//cout << "sono su plug" << endl;
	const string& topic = m_client->MSG[0];
	const string& msg = m_client->MSG[1];

	if (topic.find("power") != string::npos) {
		//cout << "State power:" << msg << endl;
		messaggio_power = msg;
	}
	else if (topic.find("energy") != string::npos) {
		//cout << "Report energy:" << msg << endl;
		messaggio_energy = msg;
	}
	else if (topic.find("overpower_value") != string::npos) {
		//cout << "Report overpower:" << msg << endl;
		messaggio_overpower_value = msg;
	}
	else if (topic.find("temperature") != string::npos) {
		//cout << "Report temperature C:" << msg << endl;
		messaggio_temperature = msg;
	}
	else if (topic.find("temperature_f") != string::npos) {
		//cout << "Report temperature F:" << msg << endl;
		messaggio_temperature_f = msg;
	}
	else if (topic.find("overtemperature") != string::npos) {
		//cout << "Report Overtemperature:" << msg << endl;
		messaggio_overtemperature = msg;
	}
	else if (msg.find(MESSAGE_ON) != string::npos) {
		//cout << "Plug " << IDO << " acceso" << endl;
		messaggio_plug = msg;
	}
	else if (msg.find(MESSAGE_OFF) != string::npos) {
		//cout << "Plug " << IDO << " spento" << endl;
		messaggio_plug = msg;
	}
	else {
		// message not recognized
		cout << "Unknown message received: " << topic << " - " << msg << endl;
	}
}