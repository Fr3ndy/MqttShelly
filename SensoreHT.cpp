#include "SensoreHT.h"

//---------------------------------------------------------------------

const char* SensoreHT::TOPIC_TEMPERATURE = "shellies/%s/sensor/temperature";
const char* SensoreHT::TOPIC_HUMIDITY = "shellies/%s/sensor/humidity";
const char* SensoreHT::TOPIC_BATTERY = "shellies/%s/sensor/battery";
const char* SensoreHT::TOPIC_ERROR = "shellies/%s/sensor/error";
const char* SensoreHT::TOPIC_ACT_REASONS = "shellies/%s/sensor/act_reasons";
const char* SensoreHT::TOPIC_EXT_POWER = "shellies/%s/sensor/ext_power";

int SensoreHT::numObjects = 0;
SensoreHT* SensoreHT::objects[MAXNUMHT];

//---------------------------------------------------------------------

SensoreHT::SensoreHT(MqttClient* client, string id) :
	m_client(client)
{
	if (numObjects < MAXNUMPLUG) {
		IDO = id;
		objects[numObjects++] = this;
		Get_All_Message();
	}
}
SensoreHT::~SensoreHT() {
	const char* topic[] = {TOPIC_TEMPERATURE,TOPIC_HUMIDITY,TOPIC_BATTERY,TOPIC_ERROR,TOPIC_ACT_REASONS,TOPIC_EXT_POWER};

	size_t size = sizeof(topic) / sizeof(topic[0]);

	for (size_t i = 0; i < size; ++i) {
		unsubscribe_l(topic[i]);
	}
}

//---------------------------------------------------------------------

void SensoreHT::subscribe_l(const char* topic)
{
	char buffer_s[100];
	snprintf(buffer_s, 100, topic, IDO.c_str());
	m_client->iscriviti(buffer_s);
}

void SensoreHT::unsubscribe_l(const char* topic) {
	char buffer_s[100];
	snprintf(buffer_s, 100, topic, IDO.c_str());
	m_client->disiscriviti(buffer_s);
}

//---------------------------------------------------------------------

void SensoreHT::get_temperature() 
{
	subscribe_l(TOPIC_TEMPERATURE);
}

void SensoreHT::get_humidity() 
{
	subscribe_l(TOPIC_HUMIDITY);
}

void SensoreHT::get_battery() 
{
	subscribe_l(TOPIC_BATTERY);
}

void SensoreHT::get_error() 
{
	subscribe_l(TOPIC_ERROR);
}

void SensoreHT::get_act_reason() 
{
	subscribe_l(TOPIC_ACT_REASONS);
}

void SensoreHT::get_ext_power() 
{
	subscribe_l(TOPIC_EXT_POWER);
}

void SensoreHT::Get_All_Message() {
	get_temperature();
	get_humidity();
	get_battery();
	get_error();
	get_act_reason();
	get_ext_power();
}

//---------------------------------------------------------------------

void SensoreHT::Check() {
	if (m_client->MSG[0].find(IDO) == string::npos) {
		// message not related to this instance
		return;
	}

	const string& topic = m_client->MSG[0];
	const string& msg = m_client->MSG[1];

	if (topic.find("temperature") != string::npos) {
		//cout << "Report temperatura : " << msg << endl;
		messaggio_temperature = msg;
	}
	else if (topic.find("humidity") != string::npos) {
		//cout << "Report umidita' : " << msg << endl;
		messaggio_humidity = msg;
	}
	else if (topic.find("battery") != string::npos) {
		//cout << "Report batteria : " << msg << endl;
		messaggio_battery = msg;
	}
	else if (topic.find("error") != string::npos) {
		//cout << "Report errore : " << msg << endl;
		messaggio_error = msg;
	}
	else if (topic.find("act_reasons") != string::npos) {
		//cout << "Report motivo messaggi : " << msg << endl;
		messaggio_act_reason = msg;
	}
	else if (topic.find("ext_power") != string::npos) {
		//cout << "Report se collegato esternamente : " << msg << endl;
		messaggio_ext_power = msg;
	}
	else {
		// message not recognized
		cout << "Unknown message received: " << topic << " - " << msg << endl;
	}
}