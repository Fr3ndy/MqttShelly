#include "MqttClient.h"

// Costruttore della classe MqttClient
MqttClient::MqttClient()
{
    mosqpp::lib_init();

}
// Distruttore della classe MqttClient
MqttClient::~MqttClient()
{
    mosqpp::lib_cleanup();
}
//----------------------------------------------------------------------------------------------------
// Metodo per connettersi al broker MQTT
//----------------------------------------------------------------------------------------------------
void MqttClient::connect_to_broker(const char* host, int port)
{
       
    int rc = MOSQ_ERR_SUCCESS;
    do {
        cout << "Tentativo di connessione al broker " << host << ":" << port << endl;
        rc = connect(host, port, 60);

        if (rc != MOSQ_ERR_SUCCESS) {
            cout << g_ErrStart << "Errore durante la connessione al broker MQTT ->" << host << ":" << port << " Cod= " << mosqpp::strerror(rc) << g_ErrEnd << endl;
            cout << "Tentativo di riconnessione in corso..." << endl;
            this_thread::sleep_for(std::chrono::seconds(3));
        }
    } while (rc != MOSQ_ERR_SUCCESS);

    cout << "Connessione al broker MQTT effettuata con successo -> " << host << ":" << port << endl;

}
//----------------------------------------------------------------------------------------------------
// Metodo per iscriversi a un topic
//----------------------------------------------------------------------------------------------------
void MqttClient::iscriviti(const char* topic_subscribe, int qos)
{
    int rc = subscribe(NULL, topic_subscribe, qos);
    if (rc != MOSQ_ERR_SUCCESS) {
        cout << g_ErrStart << "Errore di iscrizione al topic: " << mosqpp::strerror(rc) << g_ErrEnd << endl;
    }
    else {
        cout << "Iscritto al topic: " << topic_subscribe << endl;
    }
}
//----------------------------------------------------------------------------------------------------
// Metdo per disiscriversi da un topic
//----------------------------------------------------------------------------------------------------
void MqttClient::disiscriviti(const char* topic) {
    // Disiscrivi il client dal topic
    int result = unsubscribe(NULL, topic);
    if (result == MOSQ_ERR_SUCCESS) {
        cout << "Disiscrizione dal topic: "<< topic <<" avvenuta con successo" << std::endl;
    }
    else {
        cout << g_ErrStart << "Errore durante la disiscrizione dal topic: " << mosqpp::strerror(result) << g_ErrEnd << endl;
    }
}
//----------------------------------------------------------------------------------------------------
// Metodo per pubblicare un messaggio sul broker MQTT
//----------------------------------------------------------------------------------------------------
int MqttClient::publish(const char* topic, const char* message, int qos, bool retain)
{
    int mid;
    int len = static_cast<int>(strlen(message));
    int rc = mosqpp::mosquittopp::publish(&mid, topic, len, message, qos, retain);
    if (rc != MOSQ_ERR_SUCCESS) {
        cout << g_ErrStart << "Errore di pubblicazione del messaggio: " << mosqpp::strerror(rc) << g_ErrEnd << endl;
    }
    else {
        //cout << "------------------------------------" << endl;
        //cout << "Messaggio pubblicato sul topic \"" << topic << "\": " << message << endl;
        //cout << "------------------------------------" << endl;
    }
    return rc;
}
//----------------------------------------------------------------------------------------------------
// Metodo per gestire i messaggi in ingresso
//----------------------------------------------------------------------------------------------------
void MqttClient::on_message(const mosquitto_message* message)
{
    string payload = string((char*)message->payload, message->payloadlen);
    string topic(message->topic);

    MSG[0] = topic;
    MSG[1] = payload;

    //cout << payload << topic << endl;

    invia_messg();
}
//----------------------------------------------------------------------------------------------------
// Messaggi di errore alla disconnessione
//----------------------------------------------------------------------------------------------------
void MqttClient::on_disconnect(int rc) {
    if (rc == MOSQ_ERR_SUCCESS) {
        cout << g_ErrStart << "Disconnesso dal Broker " << mosqpp::strerror(rc) << g_ErrEnd << endl;
        connesso = false;
        leggifile();
    }
    else {
        cout << g_ErrStart << "Connessione Persa " << mosqpp::strerror(rc) << g_ErrEnd << endl;
        connesso = false;
        leggifile();
    }
}
//----------------------------------------------------------------------------------------------------
// Invio messaggi a subject
//----------------------------------------------------------------------------------------------------
Subject MqttClient::subject;
void MqttClient::invia_messg() {
    subject.notify();
}
//----------------------------------------------------------------------------------------------------
// Lettura file
//----------------------------------------------------------------------------------------------------
void MqttClient::leggifile() {

    // Definizione del nome della cartella
    string folderName = "Mqtt";

    // Definizione del percorso completo della cartella
    fs::path folderPath = fs::current_path() / folderName;

    // Verifica se la cartella esiste
    if (!fs::exists(folderPath))
    {
        // Se la cartella non esiste, la creiamo
        if (!fs::create_directory(folderPath)) {
            cout << g_ErrStart << "Errore nella creazione della cartella(Il programma potrebbe non funzionare correttamente): " << folderName << endl;
        }
    }

    // Apertura del file in lettura
    ifstream file("./Mqtt/IpPort.txt");

    // Verifica se l'apertura del file è andata a buon fine
    if (file.good()) {
        // Se il file è stato aperto correttamente
        if (file.is_open()) {
            // Lettura della prima riga del file
            string line;
            getline(file, line);

            // Verifica se il file è vuoto
            if (file.peek() != ifstream::traits_type::eof()) {
                // Se il file è vuoto, viene stampato un messaggio a video
                cout << "Attenzione il file contenente l'ip del broker e' vuoto" << endl << "e' stato creato un nuovo file di default" << endl;
                // Connessione al broker MQTT con i valori di default
                connect_to_broker("localhost", 1883);
            }
            else {
                // Se il file non è vuoto, viene estratto l'indirizzo IP e la porta dal file
                string host = line.substr(0, line.find(":"));
                const char* h = host.c_str();

                string port = line.substr(line.find(":") + 1);
                int p = stoi(port);
                // Connessione al broker MQTT con l'indirizzo IP e la porta estratti dal file
                connect_to_broker(h, p);
            }

            // Chiusura del file
            file.close();
        }
        else {
            // Se l'apertura del file non è andata a buon fine, viene stampato un messaggio di errore
            cout << g_ErrStart << "Errore: impossibile aprire il file " << endl;
        }
    }
    else {
        // Se il file non esiste, viene creato un nuovo file di default
        ofstream file("./Mqtt/IpPort.txt");
        file << "localhost:1883" << endl;

        // Verifica se la creazione del file è andata a buon fine
        if (!file) {
            // Se la creazione del file non è andata a buon fine, viene stampato un messaggio di errore
            cout << g_ErrStart << "Errore: il file non può essere creato" << endl;
            return;
        }
        else {
            // Se la creazione del file è andata a buon fine, viene stampato un messaggio a video
            cout << "Attenzione il file contenente l'ip del broker non e' presente" << endl << "e' stato creato un nuovo file di default" << endl;
            // Connessione al broker MQTT con i valori di default
            connect_to_broker("localhost", 1883);
        }
        // Chiusura del file
        file.close();
    }
}