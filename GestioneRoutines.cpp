#include "GestioneRoutines.h"

// Costruttore che si iscrive ai topic MQTT
GestioneRoutines::GestioneRoutines(MqttClient* idc) : m_client(idc) {
	// Iscrizione ai topic MQTT per creare, aggiornare ed eliminare le routine
	m_client->iscriviti(Create.c_str());
	m_client->iscriviti(State.c_str());
	m_client->iscriviti(Delete.c_str());

	// Definizione del nome della cartella
	

	// Definizione del percorso completo della cartella
	fs::path folderPath = fs::current_path() / folderName;

	// Verifica se la cartella esiste
	if (!fs::exists(folderPath))
	{
		// Se la cartella non esiste, la creiamo
		if (!fs::create_directory(folderPath)){
			cout << g_ErrStart << "Errore nella creazione della cartella Routines(Il programma potrebbe non funzionare correttamente): " << folderName << g_ErrEnd << endl;
		}
	}
	

	// Ciclo su tutti i file presenti nella cartella con estensione .json
	for (const auto& entry : fs::directory_iterator(folderName)) {
		if (entry.is_regular_file() && entry.path().extension() == estensione) {

			// Apertura del file JSON
			ifstream input_file(entry.path());

			// Parsing del file JSON
			try {
				json j;
				input_file >> j;

				// Conversione del JSON in stringa
				string content_str = j.dump();

				// Stampa del nome del file trovato
				cout << "Trovata Routines: " << entry.path().filename() << "" << endl;

				// Chiamata alla funzione "create" con la stringa contenente il JSON
				create(content_str);

			}
			catch (json::parse_error& e) {
				cout << g_ErrStart << "json Errato, non creato, Codice errore:" << e.what() << g_ErrEnd << endl;
			}

		}
	}
}

// Metodo per creare una nuova routine
void GestioneRoutines::create(string msg) {
	// Parse del messaggio JSON
	
	try {
		JsonManager j(msg);

		//// Estrazione degli attributi della routine dal messaggio JSON
		//string name = j.getName();
		//string status = j.getStatus();
		//string only_on_change = j.getOnlyOnChange();
		//string condiz_id = j.getConditionId();
		//string condiz_msg = j.getConditionMsg();
		//string condiz_operator = j.getConditionOperator();
		//string condiz_cmd = j.getConditionCommand();
		//string action_id = j.getActionsId();
		//string action_cmd = j.getActionsCommand();
		//vector<string> action_func_param = j.getActionsParameters();

		// Creazione di un nuovo oggetto Routines con gli attributi estratti
		Routines* obj = new Routines(
			j.getName(),
			j.getStatus(),
			j.getOnlyOnChange(),
			j.getConditionId(),
			j.getConditionMsg(),
			j.getConditionOperator(),
			j.getConditionCommand(),
			j.getActionsId(),
			j.getActionsCommand(),
			j.getActionsParameters());
	


		// Verifica se il nome della routine esiste già
		string nameR = obj->l_name;
		bool nameExists = false;
		for (int i = 0; i < names.size(); i++) {
			if (names[i] == nameR) {
				nameExists = true;
				break;
			}
		}

		// Se il nome della routine non esiste, lo aggiunge alla lista dei nomi e inizializza la routine
		if (!nameExists) {
		
			obj->inizializza();
			//obj->controllo();
			if (obj->error) {
				delete obj;
			}
			else {
				names.push_back(nameR);
				objects.push_back(obj);

				json k = json::parse(msg);

				// Salvare il file con il nome del campo "name" nella cartella "routines"
			
				ofstream file(folderName + j.getName() + estensione);
				file << k.dump(4); // 4 è il numero di spazi per l'indentazione
				file.close();
			}
		}
		// Se il nome della routine esiste già, elimina la nuova routine e stampa un messaggio di errore
		else {
			cout << g_ErrStart << "Nome routines gia' esistente, non possono esserci 2 nomi uguali. Routine non creata" << g_ErrEnd << endl;
			delete obj;
		}

		// Eliminazione dell'oggetto JsonManager
		j.~JsonManager();
	

		}
	catch (exception& e) {
		cout << g_ErrStart << "Json non valido, Codice Errore: " << e.what() << g_ErrEnd << endl;
		return;
	}
}

// Metodo per aggiornare lo stato di una routine
void GestioneRoutines::status(string msg) {
	// Parse del messaggio JSON
	try {
		json j = json::parse(msg);

		// Estrazione del nome e dello stato della routine dal messaggio JSON
		string name = j["Name"];
		string status = j["Status"];

		// Verifica se il nome della routine esiste
		bool nameExists = false;
		for (int i = 0; i < names.size(); i++) {
			if (names[i] == name) {
				nameExists = true;
				break;
			}
		}

		// Se il nome della routine esiste, aggiorna il suo stato e stampa un messaggio di successo
		if (nameExists) {
			for (auto& oggetto : objects) {
				if (oggetto->l_name == name) {
					oggetto->l_status = status;

					try
					{
						const string filename = folderName + name + estensione;

						// Apri il file JSON in modalità di lettura
						ifstream input_file(filename);
						if (!input_file.is_open())
						{
							cerr << g_ErrStart << "Errore: Impossibile aprire il file " << filename << g_ErrEnd << endl;
							break;
						}

						// Leggi il contenuto del file JSON in un oggetto JSON
						json data;
						input_file >> data;
						input_file.close();

						// Modifica l'oggetto JSON come necessario
						data["Status"] = status;

						// Apri il file JSON in modalità di scrittura
						ofstream output_file(filename);
						if (!output_file.is_open())
						{
							cerr << g_ErrStart << "Errore: Impossibile creare o aprire il file " << filename << g_ErrEnd << endl;
							break;
						}

						// Scrivi l'oggetto JSON modificato nel file
						output_file << setw(4) << data << endl;
						output_file.close();

						cout << "Routines modificata correttamente" << endl;
					}
					catch (exception& e)
					{
						cerr << g_ErrStart << "Errore: " << e.what() << g_ErrEnd << endl;
						break;
					}
					
					cout << "Stato di " << name << " cambiato in " << status << endl;
				}
			}
		}
		// Se il nome della routine non esiste, stampa un messaggio di errore
		else {
			cout << g_ErrStart << "Routine inesistente" << g_ErrEnd << endl;
		}
	}
	catch (exception& e) {
		cout << g_ErrStart << "Status non modificato, Errore codice: " << e.what() << g_ErrEnd << endl;
	}
}

// Metodo per eliminare una routine
void GestioneRoutines::erase(string msg) {
	
	string name = msg;

	// Verifica se il nome della routine esiste
	bool nameExists = false;
	for (auto it = names.begin(); it != names.end(); ++it) {
		if (*it == name) {
			nameExists = true;
			names.erase(it);
			break;
		}
	}

	// Se il nome della routine esiste, elimina la routine e stampa un messaggio di successo
	if (nameExists) {
		for (auto it = objects.begin(); it != objects.end(); ) {
			if ((*it)->l_name == name) {
				delete* it;
				it = objects.erase(it);

				string completepath = folderName + name + estensione;
				int r = remove(completepath.c_str());

				if (r == 0) {
					cout << "File eliminato con successo." << endl;
				}
				else {
					cout << g_ErrStart << "Errore durante l'eliminazione del file." << g_ErrEnd << endl;
				}
				cout << "Routines " << name << " Eliminata con successo " << endl;
			}
			else {
				++it;
			}
		}
	}
	// Se il nome della routine non esiste, stampa un messaggio di errore
	else {
		cout << g_ErrStart << "Routine inesistente" << g_ErrEnd << endl;
	}
}

// Metodo richiamato da mqtt client ogni volta che arriva un messaggio
void GestioneRoutines::update() {

	string topic = m_client->MSG[0];
	string msg = m_client->MSG[1];

	// Chiama il metodo appropriato in base al topic MQTT
	if (topic.find(Create) != string::npos) {
		create(msg);
		
	}
	else if (topic.find(State) != string::npos) {
		status(msg);
		
	}
	else if (topic.find(Delete) != string::npos) {
		erase(msg);
		
	}



	/*Questo codice serve per controllare se ci sono errori del comand, 
	le condizioni che provocano errori vengono eliminati alla creazione*/
	// Verifica ogni routine per le condizioni e esegue l'azione se necessario

	for (auto it = objects.begin(); it != objects.end(); ) {
		auto oggetto = *it; // Prende l'oggetto puntato dall'iteratore

		string condz = oggetto->l_condiz_id;
		string msg_condiz = oggetto->l_condiz_msg;

		// se trova l'id della condizione nel topic allora verifica
		if (topic.find(condz) != string::npos) {

			// scorrere tutti i dispositivi presenti nella mappa topic_msg
			for (auto it_esterna = topic_msg.begin(); it_esterna != topic_msg.end(); ++it_esterna) {

				// se il nome del dispositivo è presente nel topic
				if (topic.find(it_esterna->first) != string::npos) {

					// scorrere tutte le coppie di chiave-valore nella mappa interna per il dispositivo corrente
					for (auto it_interna = it_esterna->second.begin(); it_interna != it_esterna->second.end(); ++it_interna) {

						// se la chiave nella mappa interna è presente alla fine del topic
						if (topic.substr(topic.length() - it_interna->first.length()) == it_interna->first) {
							// se il valore nella mappa interna corrisponde alla condizione cercata

							if (it_interna->second == msg_condiz) {

								oggetto->controllo(); // Esegue il controllo sull'oggetto
								
							}
						}
					}
				}
			}
		}

		if (oggetto->error) { // Verifica se l'oggetto ha un errore
			erase(oggetto->l_name);
		}
		else {
			++it; // Avanza l'iteratore all'oggetto successivo nel vettore
		}
	}
}