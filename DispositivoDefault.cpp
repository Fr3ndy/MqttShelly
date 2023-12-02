#include "DispositivoDefault.h"

DispositivoDefault* DispositivoDefault::ogg[1];

DispositivoDefault::DispositivoDefault(MqttClient* client) :
	m_client(client)
{
	ogg[0] = this;
}

void DispositivoDefault::publica(string topic, string msg)
{
	m_client->publish(topic.c_str(),msg.c_str());
}
