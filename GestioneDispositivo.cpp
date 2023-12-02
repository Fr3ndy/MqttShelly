#include "GestioneDispositivo.h"

GestioneDispositivi::GestioneDispositivi(MqttClient* idc) : m_client(idc) {
	// Iscrizione ai topic "Create" e "Delete" tramite il client MQTT
	m_client->iscriviti(Create.c_str());
	m_client->iscriviti(Delete.c_str());

	//crea un oggetto di classe generale per le publish di funzione
	DispositivoDefault* obj = new DispositivoDefault(m_client);
	

	// Creazione della cartella per i file se non esiste già
	fs::path folderPath = fs::current_path() / folderName;
	if (!fs::exists(folderPath))
	{
		if (!fs::create_directory(folderPath)) {
			cout << g_ErrStart << "Errore nella creazione della cartella(Il programma potrebbe non funzionare correttamente): " << folderName << g_ErrEnd << endl;
		}
	}

	// Lettura dei nomi dei dispositivi dal file e creazione degli oggetti corrispondenti
	ifstream file(CompletePath);
	if (file.good()) {
		string nomi;
		if (file.is_open()) {
			string nome;
			while (getline(file, nome)) {		
				if (create(nome)) {
					names.push_back(nome);
				}
			}
			file.close();
		}
		else {
			cout << g_ErrStart << "Errore: impossibile aprire il file " << g_ErrEnd << endl;
		}
	}
	else {
		// Se il file non esiste, viene creato
		ofstream file(CompletePath);
		if (!file) {
			cout << g_ErrStart << "Errore: il file non può essere creato" << g_ErrEnd << endl;
			return;
		}
		file.close();
	}
	update(); // Esegue l'aggiornamento dei dispositivi
}

bool GestioneDispositivi::create(string msg) {
	try {
		// Verifica se il nome del dispositivo esiste già nella lista
		bool nameExists = false;
		for (int i = 0; i < names.size(); i++) {
			//cout << names[i] << endl;
			if (names[i] == msg) {
				nameExists = true;
				break;
			}
		}

		if (!nameExists) {
			// Crea l'oggetto corrispondente al tipo di dispositivo specificato nel messaggio
			if (msg.find(Bulbs) != string::npos) {
				cout << "Creazione Lampadina id: " << msg << endl;
				BulbDuo* obj = new BulbDuo(m_client, msg);
				//controllo se raggiunto numero massimo dispositivi

				if (obj->IDO == msg) {
					write(msg);
				}
				else {
					delete obj;
					throw runtime_error("Numero masssimo dei dispositivi Lampadina raggiunto, dispositivo non creato");
				}
			}
			else if (msg.find(Plugs) != string::npos) {
				cout << "Creazione Plug id: " << msg << endl;
				Plug* obj = new Plug(m_client, msg);
				//cout << obj->IDO << "  " << msg << endl;
				//controllo se raggiunto numero massimo dispositivi
				if (obj->IDO == msg) {
					//cout << obj->IDO << "jhdihi" << endl;
					write(msg);
				}
				else {
					delete obj;
					throw runtime_error("Numero masssimo dei dispositivi Plug raggiunto, dispositivo non creato");
				}
			}
			else if (msg.find(HETs) != string::npos) {
				cout << "Creazione Sensore HT id: " << msg << endl;
				SensoreHT* obj = new SensoreHT(m_client, msg);
				//controllo se raggiunto numero massimo dispositivi
				if (obj->IDO == msg) {
					write(msg);
				}
				else {
					delete obj;
					throw runtime_error("Numero masssimo dei dispositivi SesnoreHT raggiunto, dispositivo non creato");
				}
			}
			else if (msg.find(DWs) != string::npos) {
				cout << "Creazione Sesore DW id: " << msg << endl;
				SensoreDW2* obj = new SensoreDW2(m_client, msg);
				//controllo se raggiunto numero massimo dispositivi
				if (obj->IDO == msg) {
					write(msg);
				}
				else {
					delete obj;
					throw runtime_error("Numero masssimo dei dispositivi SensoreDW raggiunto, dispositivo non creato");
				}
			}
			else {
				throw runtime_error("Tipo di dispositivo non accettato");
			}
		}
		else {
			throw runtime_error("Dispositivo esistente");
		}
		return true;
	}
	catch (exception& e) {
		cout << g_ErrStart << "Errore: " << e.what() << g_ErrEnd << endl;
		return false;
	}
}

void GestioneDispositivi::write(string msg) {
	// Apre il file in modalità lettura per verificare se il messaggio è già presente
	ifstream file(CompletePath);
	string line;
	bool found_string = false;
	while (getline(file, line)) {
		if (line == msg) {
			found_string = true;
			break; // Esci dal ciclo se la stringa è stata trovata
		}
	}
	file.close();

	// Se il messaggio non è ancora presente nel file, scrivilo
	if (!found_string) {
		names.push_back(msg); // Aggiungi il nome del dispositivo alla lista dei nomi
		ofstream out(CompletePath, ios::app);
		if (out.is_open()) {
			out << msg << "\n";
			out.close();
		}
	}
}

void GestioneDispositivi::erase(string msg) {
	// Rimuovi il nome del dispositivo dalla lista dei nomi
	auto it = find(names.begin(), names.end(), msg);
	if (it != names.end()) {
		names.erase(it);
	}

	// Apri il file di input e crea un file temporaneo per la scrittura
	ifstream infile(CompletePath);
	ofstream outfile(TempPath);
	string line;
	bool found = false;
	while (getline(infile, line)) {
		if (line == msg) {
			found = true;

			// Elimina l'oggetto corrispondente al dispositivo
			if (msg.find(Bulbs) != string::npos) {
				for (int i = 0; i < BulbDuo::numObjects; i++) {
					if (BulbDuo::objects[i]->IDO == msg) {
						delete BulbDuo::objects[i];
						BulbDuo::objects[i] = nullptr;
						BulbDuo::numObjects--;
						break;
					}
				}
			}
			if (msg.find(Plugs) != string::npos) {
				for (int i = 0; i < Plug::numObjects; i++) {
					if (Plug::objects[i]->IDO == msg) {
						delete Plug::objects[i];
						Plug::objects[i] = nullptr;
						Plug::numObjects--;
						break;
					}
				}
			}
			if (msg.find(HETs) != string::npos) {
				for (int i = 0; i < SensoreHT::numObjects; i++) {
					if (SensoreHT::objects[i]->IDO == msg) {
						delete SensoreHT::objects[i];
						SensoreHT::objects[i] = nullptr;
						SensoreHT::numObjects--;
						break;
					}
				}
			}
			if (msg.find(DWs) != string::npos) {
				for (int i = 0; i < SensoreDW2::numObjects; i++) {
					if (SensoreDW2::objects[i]->IDO == msg) {
						delete SensoreDW2::objects[i];
						SensoreDW2::objects[i] = nullptr;
						SensoreDW2::numObjects--;
						break;
					}
				}
			}
			continue;
		}
		outfile << line << endl; // Scrivi nel file temporaneo tutte le righe tranne quella del dispositivo da eliminare
	}
	infile.close();
	outfile.close();

	 //Sostituisci il file originale con il file temporaneo
	if (found) {
		if (remove(CompletePath.c_str()) != 0) {
			cout << g_ErrStart << "Errore nella rimozione del file originale. Verifica i permessi." << g_ErrEnd << endl;
		}
		else if (rename(TempPath.c_str(), CompletePath.c_str()) != 0) {
			cout << g_ErrStart << "Errore nel rinominare il file temporaneo. Verifica i permessi." << g_ErrEnd << endl;
		}
		else {
			cout << g_ErrStart << "Dispositivo rimosso. File sostituito correttamente." << g_ErrEnd << endl;
		}
	}
	else {
		cout << g_ErrStart << "Dispositivo non trovato." << g_ErrEnd << endl;
		remove(TempPath.c_str());
	}
}

void GestioneDispositivi::update() {
	string topic = m_client->MSG[0];
	string msg = m_client->MSG[1];

	// Effettua l'aggiornamento dei dispositivi in base al topic ricevuto
	if (topic.find(Bulbs) != string::npos) {
		for (int i = 0; i < BulbDuo::numObjects; i++) {
			if (topic.find(BulbDuo::objects[i]->IDO) != string::npos) {
				BulbDuo::objects[i]->Check();
				
			}
		}
	}
	if (topic.find(Plugs) != string::npos) {
		for (int i = 0; i < Plug::numObjects; i++) {
			if (topic.find(Plug::objects[i]->IDO) != string::npos) {
				Plug::objects[i]->Check();
				
			}
		}
	}
	if (topic.find(HETs) != string::npos) {
		for (int i = 0; i < SensoreHT::numObjects; i++) {
			if (topic.find(SensoreHT::objects[i]->IDO) != string::npos) {
				SensoreHT::objects[i]->Check();
				
			}
		}
	}
	if (topic.find(DWs) != string::npos) {
		for (int i = 0; i < SensoreDW2::numObjects; i++) {
			if (topic.find(SensoreDW2::objects[i]->IDO) != string::npos) {
				SensoreDW2::objects[i]->Check();
				
			}
		}
	}

	// Effettua l'operazione di creazione o eliminazione del dispositivo
	if (topic.find(Create) != string::npos) {
		create(msg);
	}
	else if (topic.find(Delete) != string::npos) {
		erase(msg);
	}
}
