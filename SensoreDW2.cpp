#include "SensoreDW2.h"

//---------------------------------------------------------------------

const char* SensoreDW2::TOPIC_STATE = "shellies/%s/sensor/state";
const char* SensoreDW2::TOPIC_TILT = "shellies/%s/sensor/tilt";
const char* SensoreDW2::TOPIC_VIBRATION = "shellies/%s/sensor/vibration";
const char* SensoreDW2::TOPIC_LUX = "shellies/%s/sensor/lux";
const char* SensoreDW2::TOPIC_BATTERY = "shellies/%s/sensor/battery";
const char* SensoreDW2::TOPIC_TEMPERATURE = "shellies/%s/sensor/temperature";
const char* SensoreDW2::TOPIC_ERROR = "shellies/%s/sensor/error";
const char* SensoreDW2::TOPIC_ACT_REASONS = "shellies/%s/sensor/act_reasons";

int SensoreDW2::numObjects = 0;
SensoreDW2* SensoreDW2::objects[MAXNUMDW];

//---------------------------------------------------------------------

SensoreDW2::SensoreDW2(MqttClient* client, string id) :
    m_client(client)
{
    if (numObjects < MAXNUMPLUG) {
        IDO = id;
        objects[numObjects++] = this;
        Get_All_Message();
    }
}

SensoreDW2::~SensoreDW2(){
    const char* topic[] = { TOPIC_TEMPERATURE,TOPIC_STATE,TOPIC_TILT,TOPIC_VIBRATION,TOPIC_LUX,TOPIC_BATTERY,TOPIC_ERROR,TOPIC_ACT_REASONS };

    size_t size = sizeof(topic) / sizeof(topic[0]);

    for (size_t i = 0; i < size; ++i) {
        unsubscribe_l(topic[i]);
    }
}

//---------------------------------------------------------------------

void SensoreDW2::subscribe_l(const char* topic)
{
    char buffer_s[100];
    snprintf(buffer_s, 100, topic, IDO.c_str());
    m_client->iscriviti(buffer_s);
}

void SensoreDW2::unsubscribe_l(const char* topic) 
{
    char buffer_s[100];
    snprintf(buffer_s, 100, topic, IDO.c_str());
    m_client->disiscriviti(buffer_s);
}

//---------------------------------------------------------------------

void SensoreDW2::get_state()
{
    subscribe_l(TOPIC_STATE);
}

void SensoreDW2::get_tilt()
{
    subscribe_l(TOPIC_TILT);
}

void SensoreDW2::get_vibration()
{
    subscribe_l(TOPIC_VIBRATION);
}

void SensoreDW2::get_lux()
{
    subscribe_l(TOPIC_LUX);
}

void SensoreDW2::get_battery()
{
    subscribe_l(TOPIC_BATTERY);
}

void SensoreDW2::get_temperature()
{
    subscribe_l(TOPIC_TEMPERATURE);
}

void SensoreDW2::get_error()
{
    subscribe_l(TOPIC_ERROR);
}

void SensoreDW2::get_act_reasons()
{
    subscribe_l(TOPIC_ACT_REASONS);
}

void SensoreDW2::Get_All_Message() {
    get_state();
    get_tilt();
    get_vibration();
    get_lux();
    get_battery();
    get_temperature();
    get_error();
    get_act_reasons();
}

//---------------------------------------------------------------------

void SensoreDW2::Check()
{
    if (m_client->MSG[0].find(IDO) == string::npos) {
        // message not related to this instance
        return;
    }

    const string& topic = m_client->MSG[0];
    const string& msg = m_client->MSG[1];

    if (topic.find("state") != string::npos) {
        if (msg.find("open") != string::npos) {
            //cout << "Doorwindows " << IDO << " is " << msg << endl;
            messaggio_open = msg;
        }
        else {
            //cout << "Doorwindows " << IDO << " e' " << msg << endl;
            messaggio_open = msg;
        }
    }
    else if (topic.find("tilt") != string::npos) {
        //cout << "Report tilt: " << msg << endl;
        messaggio_tilt = msg;
    }
    else if (topic.find("vibration") != string::npos) {
        //cout << "Report vibration: " << msg << endl;
        messaggio_vibration = msg;
    }
    else if (topic.find("lux") != string::npos) {
        //cout << "Report lux: " << msg << endl;
        messaggio_lux = msg;
    }
    else if (topic.find("battery") != string::npos) {
        //cout << "Report battery: " << msg << endl;
        messaggio_battery = msg;
    }
    else if (topic.find("temperature") != string::npos) {
        //cout << "Report temperature: " << msg << endl;
        messaggio_temperature = msg;
    }
    else if (topic.find("error") != string::npos) {
        //cout << "Report error: " << msg << endl;
        messaggio_error = msg;
    }
    else if (topic.find("act_reasons") != string::npos) {
        //cout << "Report act reasons: " << msg << endl;
        messaggio_act_reason = msg;
    }
    else {
        // message not recognized
        cout << "Unknown message received: " << topic << " - " << msg << endl;
    }
}