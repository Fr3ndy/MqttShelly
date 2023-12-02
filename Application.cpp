#include "Application.h"

#include <iostream>
#include <chrono>
#include <ctime>

// Costruttore
Application::Application() {	

	//creazione client di connessione mosquitto
	MqttClient* client = new MqttClient();

	//leggi il file di config ip/port broker
	client->leggifile();

	// instanzia digli oggetti di gestione vuoti
	GestioneRoutines* g = nullptr;
	GestioneDispositivi* c = nullptr;

	//variabile di controllo connessione locale
	int j = 0;

	while (true) {


		//controlla se il client è connesso
		if (client->connesso) {
			//client connesso
			//verifica se il client è connesso solo 1 volta
			if (j == 0) {
				//se il client è connesso inizializza le classi di gestione
				//e aggiungi il client all'ascoltatore du observer

				c = new GestioneDispositivi(client);
				MqttClient::subject.attach(c);

				//gestione dovrà essere l'ultima classe chiamata per gestione routines
				g = new GestioneRoutines(client);
				MqttClient::subject.attach(g);

				

				//client connesso, al prossimo ciclo non passare per l'if
				j = 1;
			}
		}
		else {
			//se il client non è connesso rimuovi gli oggetti dall'ascoltatore
			MqttClient::subject.detach(c);
			MqttClient::subject.detach(g);
			//connessione persa
			j = 0;

			//inserisci connesso a true, questo verrà sovrascritto dall'on connect del client
			
			client->connesso = true;
		}

		client->loop();
	}
}

Application::~Application() {

}
