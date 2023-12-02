#include "BulbDuo.h"

//---------------------------------------------------------------------

const char* BulbDuo::TOPIC_COMMAND = "shellies/%s/light/0/command";
const char* BulbDuo::TOPIC_SET = "shellies/%s/light/0/set";
const char* BulbDuo::TOPIC_STATUS = "shellies/%s/light/0/status";
const char* BulbDuo::TOPIC_LIGHT = "shellies/%s/light/0/";
const char* BulbDuo::TOPIC_POWER = "shellies/%s/light/0/power";
const char* BulbDuo::TOPIC_ENERGY = "shellies/%s/light/0/energy";

const char* BulbDuo::MESSAGE_ON = "on";
const char* BulbDuo::MESSAGE_OFF = "off";

int BulbDuo::numObjects = 0;
BulbDuo* BulbDuo::objects[MAXNUMLAMP];

//---------------------------------------------------------------------

BulbDuo::BulbDuo(MqttClient* client, string id) :
	m_client(client)
{
	if (numObjects < MAXNUMPLUG) {
		IDO = id;
		objects[numObjects++] = this;
		Get_All_Message();
	}
}

BulbDuo::~BulbDuo(){
	const char* topic[] = { TOPIC_STATUS,TOPIC_POWER,TOPIC_ENERGY};

	size_t size = sizeof(topic) / sizeof(topic[0]);

	for (size_t i = 0; i < size; ++i) {
		unsubscribe_l(topic[i]);
	}
}

//---------------------------------------------------------------------

void BulbDuo::subscribe_l(const char* topic)
{
	char buffer_s[100];
	snprintf(buffer_s, 100, topic, IDO.c_str());
	m_client->iscriviti(buffer_s);
}

void BulbDuo::publish_l(const char* topic,string message)
{
	char buffer_p[400];
	snprintf(buffer_p, 400, topic, IDO.c_str());
	m_client->publish(buffer_p,(message.c_str()));
}

void BulbDuo::unsubscribe_l(const char* topic) {
	char buffer_s[100];
	snprintf(buffer_s, 100, topic, IDO.c_str());
	m_client->disiscriviti(buffer_s);
}

//---------------------------------------------------------------------

void BulbDuo::set_on() 
{
	publish_l(TOPIC_COMMAND, MESSAGE_ON);
}

void BulbDuo::set_off() 
{
	publish_l(TOPIC_COMMAND, MESSAGE_OFF);
}

void BulbDuo::set_status(string state) 
{
	publish_l(TOPIC_COMMAND, state);
}

void BulbDuo::get_status()
{
	subscribe_l(TOPIC_STATUS);
}

void BulbDuo::get_power() 
{
	subscribe_l(TOPIC_POWER);
}

void BulbDuo::get_energy()
{
	subscribe_l(TOPIC_ENERGY);
}

void BulbDuo::Get_All_Message() 
{
	get_status();
	get_power();
	get_energy();
}

//---------------------------------------------------------------------

void BulbDuo::Check() 
{
	if (m_client->MSG[0].find(IDO) == string::npos) {
		// il messaggio non interessa all'oggetto
		return;
	}

	const string& topic = m_client->MSG[0];
	const string& msg = m_client->MSG[1];

	if (topic.find("power") != string::npos) {
		//cout << "Report power in watt: " << msg << endl;
		messaggio_power = msg;
	}
	else if (topic.find("energy") != string::npos) {
		//cout << "Report energy in watt-minutes: " << msg << endl;
		messaggio_energy = msg;
	}
	else if (topic.find("status") != string::npos) {
		//cout << "Report status: " << msg << endl;
		messaggio_status = msg;
	}
	else if (msg.find(MESSAGE_ON) != string::npos) {
		//cout << "Lampadina accesa: " << endl;
		messaggio_light = msg;
	}
	else if (msg.find(MESSAGE_OFF) != string::npos) {
		//cout << "Lampadina spenta: " << endl;
		messaggio_light = msg;
	}
	else {
		// messaggio sconosciuto
		cout << "Unknown message received: " << topic << " - " << msg << endl;
	}
	
}