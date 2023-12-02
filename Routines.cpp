#include "Routines.h"

Routines::Routines(string name, string status,string change, string cid, string cmsg, string co, string cc, string ai, string ac, vector<string> ap)
	: l_name(name), l_status(status), l_only_on_change(change), l_condiz_id(cid), l_condiz_msg(cmsg), l_condiz_operator(co), l_condiz_cmd(cc), l_action_id(ai), l_action_cmd(ac), l_action_func_param(ap)
{
	cout << "Creazione routines: " << l_name << ", Status: " << l_status << endl;
}

Routines::~Routines() {
	cout << "Routines " << l_name << " distrutta" << endl;
}
//----------------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------------
void Routines::inizializza() {
	try {
		// Controllo dell'id che deve rispettare le condizioni
		if (l_condiz_id.find(BULBDUO) != string::npos) {
			for (int i = 0; i < BulbDuo::numObjects; i++) {
				if (l_condiz_id.find(BulbDuo::objects[i]->IDO) != string::npos) {
					TipoOggCondiz = BULBDUO;
					OggCondiz = BulbDuo::objects[i];
					break;
				}
			}
		}
		else if (l_condiz_id.find(PLUG) != string::npos) {
			for (int i = 0; i < Plug::numObjects; i++) {
				if (l_condiz_id.find(Plug::objects[i]->IDO) != string::npos) {
					//cout << Plug::objects[i]->IDO;
					TipoOggCondiz = PLUG;
					OggCondiz = Plug::objects[i];
					break;
				}
			}
		}
		else if (l_condiz_id.find(SENSOREDW) != string::npos) {
			for (int i = 0; i < SensoreDW2::numObjects; i++) {
				if (l_condiz_id.find(SensoreDW2::objects[i]->IDO) != string::npos) {
					TipoOggCondiz = SENSOREDW;
					OggCondiz = SensoreDW2::objects[i];
					break;
				}
			}
		}
		else if (l_condiz_id.find(SENSOREHT) != string::npos) {
			for (int i = 0; i < SensoreHT::numObjects; i++) {
				if (l_condiz_id.find(SensoreHT::objects[i]->IDO) != string::npos) {
					TipoOggCondiz = SENSOREHT;
					OggCondiz = SensoreHT::objects[i];
					break;

				}
			}
		}
		else {
			throw runtime_error("ID condizione, non valido");
		}
	}
	catch (exception& e) {
		cout << g_ErrStart << "Errore: " << e.what() << g_ErrEnd << endl;
		error = true;
	}
	//----------------------------------------------------------------------------------------
	// Controllo dell'id che deve eseguire codice nel caso funzioni
	try {
		if (l_action_id	== "NULL") {
			//esegui funzioni se trova nulll
			//su cmd ci saranno le funioni
			TipoOggComando = FUNZIONE;
			if (l_action_cmd.find("WriteCSV") != string::npos) {
				string c = "Funzioni/";
				string c1 = l_action_func_param[0];
				string c2 = ".csv";
				string c3 = c + c1 + c2;

				ifstream file_exists(c3);
				if (!file_exists.good()) {
					ofstream file(c3);
				}
				file_exists.close();
			}
		}
		else if (l_action_id.find(BULBDUO) != string::npos) {
			for (int i = 0; i < BulbDuo::numObjects; i++) {
				if (l_action_id.find(BulbDuo::objects[i]->IDO) != string::npos) {
					TipoOggComando = BULBDUO;
					OggComando = BulbDuo::objects[i];
					break;
				}
			}
		}
		else if (l_action_id.find(PLUG) != string::npos) {
			for (int i = 0; i < Plug::numObjects; i++) {
				if (l_action_id.find(Plug::objects[i]->IDO) != string::npos) {
					TipoOggComando = PLUG;
					OggComando = Plug::objects[i];
					break;
					
				}
			}
		}
		else if (l_action_id.find(SENSOREDW) != string::npos) {
			throw runtime_error("ID comando, Il sensore porta non può eseguire comandi");
		}
		else if (l_action_id.find(SENSOREHT) != string::npos) {
			throw runtime_error("ID comando, Il sesnore Umidità e temperatura non può eseguire comandi");
		}
		else {
			throw runtime_error("ID comando, non valido");
		}
	}
	catch (exception& e) {
		cout << g_ErrStart << "Errore: " << e.what() << g_ErrEnd << endl;
		error = true;
	}

}

void Routines::CondizBulbDuo() 
{
	try {
		BulbDuo* lamp = static_cast<BulbDuo*>(OggCondiz);

		// Mappa di funzioni per eseguire il comando
		unordered_map<string, function<string(BulbDuo&)>> messg = {
			{ "messaggio_light", &BulbDuo::messaggio_light },
			{ "messaggio_status", &BulbDuo::messaggio_status },
			{ "messaggio_power", &BulbDuo::messaggio_power },
			{ "messaggio_energy", &BulbDuo::messaggio_energy }
		};

		auto it = messg.find(l_condiz_msg);
		if (it != messg.end()) {
			variabile = it->second(*lamp);
			if (!variabile.empty()) {
				if (l_condiz_msg == "messaggio_power" ||
					l_condiz_msg == "messaggio_energy") {
					//gestione operatori < > <= >= = !=

					double variabile_numerica_return;
					double variabile_numerica_condiz;

					try {
						variabile_numerica_return = stod(variabile);
						variabile_numerica_condiz = stod(l_condiz_cmd);
					}
					catch (const invalid_argument) {
						cout << g_ErrStart << "Errore: la stringa di return o il valore input non e' un double valido." << g_ErrEnd << endl;
						error = true;
					}

					bool condizione;
					if (l_condiz_operator == "=") {
						condizione = (variabile_numerica_return == variabile_numerica_condiz);
					}
					else if (l_condiz_operator == "<") {
						condizione = variabile_numerica_return < variabile_numerica_condiz;
					}
					else if (l_condiz_operator == "<=") {
						condizione = variabile_numerica_return <= variabile_numerica_condiz;
					}
					else if (l_condiz_operator == ">") {
						condizione = variabile_numerica_return > variabile_numerica_condiz;
					}
					else if (l_condiz_operator == ">=") {
						condizione = variabile_numerica_return >= variabile_numerica_condiz;
					}
					else if (l_condiz_operator == "!=") {
						condizione = variabile_numerica_return != variabile_numerica_condiz;
					}
					else {
						throw runtime_error("Operatore errato");
					}

					if (condizione) {
						cout << GetTime() << " Condizione rispettata" << endl;
						condiz = 1;
					}
					else {
						cout << GetTime() << " Condizione NON rispettata" << endl;
						condiz = 0;
					}
				}
				else {
					bool condizione;
					if (l_condiz_operator == "=") {
						condizione = variabile == l_condiz_cmd;
					}
					else if (l_condiz_operator == "!=") {
						condizione = variabile != l_condiz_cmd;
					}
					else {
						throw runtime_error("Errore operatore, questo tipo di messaggio supporta solo = o !=");
					}

					// Print the result
					if (condizione) {
						cout << "Routines " << l_name << " condizione rispettata" << endl;
						condiz = 1;
					}
					else {
						//cout << "Routines " << name_ << " condizione non rispettata" << endl;
						condiz = 0;
					}
				}
			}
		}
		else {
			throw runtime_error("Messaggio condizione Bulb non esistente");
			
		}
	}catch (exception& e) {
		cout << g_ErrStart << "Errore: " << e.what() << g_ErrEnd << endl;
		error = true;
	}

}

void Routines::CondizPlug()
{
	try {
		
		Plug* plug = static_cast<Plug*>(OggCondiz);

		// Mappa di funzioni per eseguire il comando
		unordered_map<string, function<string(Plug&)>> messg = {
			{ "messaggio_plug", &Plug::messaggio_plug },
			{ "messaggio_power", &Plug::messaggio_power },
			{ "messaggio_energy", &Plug::messaggio_energy },
			{ "messaggio_overpower_value", &Plug::messaggio_overpower_value },
			{ "messaggio_temperature", &Plug::messaggio_temperature },
			{ "messaggio_temperature_f", &Plug::messaggio_temperature_f },
			{ "messaggio_overtemperature", &Plug::messaggio_overtemperature }
		};
		auto it = messg.find(l_condiz_msg);
		if (it != messg.end()) {
			variabile = it->second(*plug);

			if (!variabile.empty()) {
				//cout << l_condiz_msg << endl;

				if (l_condiz_msg == "messaggio_power" ||
					l_condiz_msg == "messaggio_energy" ||
					l_condiz_msg == "messaggio_overpower_value" ||
					l_condiz_msg == "messaggio_temperature" ||
					l_condiz_msg == "messaggio_temperature_f") {
					//gestione operatori < > <= >= = !=

					double variabile_numerica_return;
					double variabile_numerica_condiz;

					try {
						variabile_numerica_return = stod(variabile);
						variabile_numerica_condiz = stod(l_condiz_cmd);

						//cout << variabile << " - " << l_condiz_cmd << endl;
					}
					catch (const invalid_argument) {
						cout << g_ErrStart << "Errore: la stringa di return o il valore input non e' un double valido." << g_ErrEnd << endl;
						error = true;
					}

					bool condizione;

					if (l_condiz_operator == "=") {
						condizione = (variabile_numerica_return == variabile_numerica_condiz);
					}
					else if (l_condiz_operator == "<") {
						condizione = variabile_numerica_return < variabile_numerica_condiz;
					}
					else if (l_condiz_operator == "<=") {
						condizione = variabile_numerica_return <= variabile_numerica_condiz;
					}
					else if (l_condiz_operator == ">") {
						condizione = variabile_numerica_return > variabile_numerica_condiz;
					}
					else if (l_condiz_operator == ">=") {
						condizione = variabile_numerica_return >= variabile_numerica_condiz;
					}
					else if (l_condiz_operator == "!=") {
						condizione = variabile_numerica_return != variabile_numerica_condiz;
					}
					else {
						throw runtime_error("Operatore errato");
					}


					if (condizione) {
						cout << GetTime() << " Condizione rispettata" << endl;
						condiz = 1;
					}
					else {
						cout << GetTime() << " Condizione NON rispettata" << endl;
						condiz = 0;
					}
				}
				else {
					bool condizione;
					if (l_condiz_operator == "=") {
						condizione = variabile == l_condiz_cmd;
					}
					else if (l_condiz_operator == "!=") {
						condizione = variabile != l_condiz_cmd;
					}
					else {
						throw runtime_error("Errore operatore, questo tipo di messaggio supporta solo = o !=");
					}

					// Print the result
					if (condizione) {
						cout << GetTime() << " Condizione rispettata" << endl;
						condiz = 1;
					}
					else {
						cout << GetTime() << " Condizione NON rispettata" << endl;
						condiz = 0;
					}
				}
			}
			else {
				//------------------------------------------
				cout << "Inizializzazione..." << endl;
			}
		}
		else {
			throw runtime_error("Messaggio condizione Plug non esistente");
		}
	}catch (exception& e) {
		cout << g_ErrStart << "Errore: " << e.what() << g_ErrEnd << endl;
		error = true;
	}
}

void Routines::CondizSensoreDW()
{
	try {
		SensoreDW2* dw = static_cast<SensoreDW2*>(OggCondiz);

		// Mappa di funzioni per eseguire il comando
		unordered_map<string, function<string(SensoreDW2&)>> messg = {
			{ "messaggio_open", &SensoreDW2::messaggio_open },
			{ "messaggio_tilt", &SensoreDW2::messaggio_tilt },
			{ "messaggio_lux", &SensoreDW2::messaggio_lux },
			{ "messaggio_battery", &SensoreDW2::messaggio_battery },
			{ "messaggio_temperature", &SensoreDW2::messaggio_temperature },
			{ "messaggio_error", &SensoreDW2::messaggio_error },
			{ "messaggio_act_reason", &SensoreDW2::messaggio_act_reason }
		};
		//cout << cmsg_ << endl;
		auto it = messg.find(l_condiz_msg);

		if (it != messg.end()) {
			variabile = it->second(*dw);
			if (!variabile.empty()) {
				if (l_condiz_msg == "messaggio_tilt" ||
					l_condiz_msg == "messaggio_vibration" ||
					l_condiz_msg == "messaggio_lux" ||
					l_condiz_msg == "messaggio_battery" ||
					l_condiz_msg == "messaggio_temperature") {
					//gestione operatori < > <= >= = !=

					double variabile_numerica_return;
					double variabile_numerica_condiz;

					try {
						variabile_numerica_return = stod(variabile);
						variabile_numerica_condiz = stod(l_condiz_cmd);
					}
					catch (const invalid_argument) {
						cout << g_ErrStart << "Errore: la stringa di return o il valore input non e' un double valido." << g_ErrEnd << endl;
						error = true;
					}



					bool condizione;
					if (l_condiz_operator == "=") {
						condizione = (variabile_numerica_return == variabile_numerica_condiz);
					}
					else if (l_condiz_operator == "<") {
						condizione = variabile_numerica_return < variabile_numerica_condiz;
					}
					else if (l_condiz_operator == "<=") {
						condizione = variabile_numerica_return <= variabile_numerica_condiz;
					}
					else if (l_condiz_operator == ">") {
						condizione = variabile_numerica_return > variabile_numerica_condiz;
					}
					else if (l_condiz_operator == ">=") {
						condizione = variabile_numerica_return >= variabile_numerica_condiz;
					}
					else if (l_condiz_operator == "!=") {
						condizione = variabile_numerica_return != variabile_numerica_condiz;
					}
					else {
						throw runtime_error("Operatore errato");
					}
					if (condizione) {
						cout << GetTime() << " Condizione rispettata" << endl;
						condiz = 1;
					}
					else {
						cout << GetTime() << " Condizione NON rispettata" << endl;
						condiz = 0;
					}
				}
				else {
					bool condizione;
					if (l_condiz_operator == "=") {
						condizione = variabile == l_condiz_cmd;
					}
					else if (l_condiz_operator == "!=") {
						condizione = variabile != l_condiz_cmd;
					}
					else {
						throw runtime_error("Errore operatore, questo tipo di messaggio supporta solo = o !=");
					}

					// Print the result
					if (condizione) {
						cout << "Routines " << l_name << " condizione rispettata" << endl;
						condiz = 1;
					}
					else {
						//cout << "Routines " << name_ << " condizione non rispettata" << endl;
						condiz = 0;
					}
				}
			}
		}
		else {
			throw runtime_error("Messaggio condizione SensoreDW non esistente");
		}
	}
	catch (exception& e) {
		cout << g_ErrStart << "Errore: " << e.what() << g_ErrEnd << endl;
		error = true;
	}
}

void Routines::CondizSensoreHT()
{
	try {
		
		SensoreHT* ht = static_cast<SensoreHT*>(OggCondiz);
		

		// Mappa di funzioni per eseguire il comando
		unordered_map<string, function<string(SensoreHT&)>> messg = {
			{ "messaggio_temperature", &SensoreHT::messaggio_temperature },
			{ "messaggio_humidity", &SensoreHT::messaggio_humidity },
			{ "messaggio_battery", &SensoreHT::messaggio_battery },
			{ "messaggio_act_reason", &SensoreHT::messaggio_act_reason },
			{ "messaggio_ext_power", &SensoreHT::messaggio_ext_power },
			{ "messaggio_error", &SensoreHT::messaggio_error }
		};
		//cout << cmsg_ << endl;
		auto it = messg.find(l_condiz_msg);

		if (it != messg.end()) {
			variabile = it->second(*ht);
			if (!variabile.empty()) {
				if (l_condiz_msg == "messaggio_temperature" ||
					l_condiz_msg == "messaggio_humidity" ||
					l_condiz_msg == "messaggio_battery" ||
					l_condiz_msg == "messaggio_error") {
					//gestione operatori < > <= >= = !=

					double variabile_numerica_return;
					double variabile_numerica_condiz;

					try {
						variabile_numerica_return = stod(variabile);
						variabile_numerica_condiz = stod(l_condiz_cmd);
					}
					catch (const invalid_argument) {
						cout << g_ErrStart << "Errore: la stringa di return o il valore input non e' un double valido." << g_ErrEnd << endl;
						error = true;
					}

					bool condizione;
					if (l_condiz_operator == "=") {
						condizione = (variabile_numerica_return == variabile_numerica_condiz);
					}
					else if (l_condiz_operator == "<") {
						condizione = variabile_numerica_return < variabile_numerica_condiz;
					}
					else if (l_condiz_operator == "<=") {
						condizione = variabile_numerica_return <= variabile_numerica_condiz;
					}
					else if (l_condiz_operator == ">") {
						condizione = variabile_numerica_return > variabile_numerica_condiz;
					}
					else if (l_condiz_operator == ">=") {
						condizione = variabile_numerica_return >= variabile_numerica_condiz;
					}
					else if (l_condiz_operator == "!=") {
						condizione = variabile_numerica_return != variabile_numerica_condiz;
					}
					else {
						throw runtime_error("Operatore errato");
					}
					if (condizione) {
						cout << GetTime() << " Condizione rispettata" << endl;
						condiz = 1;
					}
					else {
						//cout << GetTime() << " Condizione NON rispettata" << endl;
						condiz = 0;
					}
				}
				else {

					bool condizione;
					if (l_condiz_operator == "=") {
						condizione = variabile == l_condiz_cmd;
					}
					else if (l_condiz_operator == "!=") {
						condizione = variabile != l_condiz_cmd;
					}
					else {
						throw runtime_error("Errore operatore, questo tipo di messaggio supporta solo = o !=");
					}

					// Print the result
					if (condizione) {
						cout << GetTime() << " Condizione rispettata" << endl;
						condiz = 1;
					}
					else {
						cout << GetTime() << " Condizione NON rispettata" << endl;
						condiz = 0;
					}
				}
			}
		}
		else {
			throw runtime_error("Messaggio condizione SensoreHT non esistente");
		}
	}
	catch (exception& e) {
		cout << g_ErrStart << "Errore: " << e.what() << g_ErrEnd << endl;
		error = true;
	}
}

void Routines::ComandoBulbDuo()
{
	try {
		BulbDuo* lamp = static_cast<BulbDuo*>(OggComando);

		// Mappa di funzioni per eseguire il comando
		unordered_map<string, function<void(BulbDuo&)>> func_map = {
			{ "on", &BulbDuo::set_on },
			{ "off", &BulbDuo::set_off }
		};


		auto it = func_map.find(l_action_cmd);

		if (it != func_map.end()) {
			it->second(*lamp);
			cout << GetTime() << " Comando eseguito" << endl;
		}
		else {
			throw runtime_error("Comando Bulb non esistente");
		}
	}
	catch (exception& e) {
		cout << g_ErrStart << "Errore: " << e.what() << g_ErrEnd << endl;
		error = true;
	}
}

void Routines::ComandoPlug()
{
	try {
		Plug* plug = static_cast<Plug*>(OggComando);

		// Mappa di funzioni per eseguire il comando
		unordered_map<string, function<void(Plug&)>> func_map = {
			{ "on", &Plug::set_on },
			{ "off", &Plug::set_off }
		};


		auto it = func_map.find(l_action_cmd);

		if (it != func_map.end()) {
			it->second(*plug);
			cout << GetTime() << " Comando eseguito" << endl;
		}
		else {
			throw runtime_error("Comando Plug non esistente");
		}
	}
	catch (exception& e) {
		cout << g_ErrStart << "Errore: " << e.what() << g_ErrEnd << endl;
		error = true;
	}
}

void Routines::ComandoFunzione() {
	try {

		FunzioniRoutines F;
		
		// Mappa di funzioni per eseguire il comando
		unordered_map<string, function<void(FunzioniRoutines&,string&,string&)>> func_map = {
			{ "WriteCSV", &FunzioniRoutines::WriteCSV },
			{ "PublishMQTT", &FunzioniRoutines::PublishMQTT }
		};

		string param1 = l_action_func_param[0];
		

		auto it = func_map.find(l_action_cmd);

		if (it != func_map.end()) {

			if (l_action_cmd == "WriteCSV") {
				if (l_action_func_param.size() >= 1) {
					if (!l_action_func_param[0].empty()) {
						it->second(F, l_action_func_param[0], variabile);
					}
					else {
						throw runtime_error("Parametri funzione vuoti, mi aspetto (Nome File)");
					}
				}
				else {
					throw runtime_error("Non trovo i parametri funzione(Nome File)");
				}
				
			}
			else if (l_action_cmd == "PublishMQTT") {
				if (l_action_func_param.size() >= 2) {
					if (!l_action_func_param[0].empty() || !l_action_func_param[1].empty()) {
						it->second(F, l_action_func_param[0], l_action_func_param[1]);
					}
					else {
						throw runtime_error("Parametri funzione vuoti, mi aspetto (Topic e Messaggio)");
					}
				}
				else {
					throw runtime_error("Non trovo i parametri funzione(Topic e Messaggio)");
				}
			}
			
			cout << GetTime() << " Comando eseguito" << endl;
		}
		else {
			throw runtime_error("Comando funzione non trovato");
		}
	}
	catch (exception& e) {
		cout << g_ErrStart << "Errore: " << e.what() << g_ErrEnd << endl;
		error = true;
	}
}
//----------------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------------
void Routines::controllo(){
	
	if (!error) {
		if (l_status == "enabled") {

			
			cout << endl << "----------------------------------------------------------------------" << endl;


			cout << "Controllo la routine \" " << l_name << " \"" << endl;
			cout << "Se " << l_condiz_id << " soddisfa (" << l_condiz_msg << l_condiz_operator << l_condiz_cmd << ") allora " << l_action_id << " -> " << l_action_cmd << endl;

			if (TipoOggCondiz == BULBDUO) {
				CondizBulbDuo();
			}
			else if (TipoOggCondiz == PLUG) {
				CondizPlug();
			}
			else if (TipoOggCondiz == SENSOREDW) {
				CondizSensoreDW();
			}
			else if (TipoOggCondiz == SENSOREHT) {
				CondizSensoreHT();
			}

			
			if (l_only_on_change == "true") {
				if (variabile == last_message) {
					//messaggio uguale al precedente
					cout << "Messaggio uguale al precedente, comando non eseguito, " << endl << "modifica il parametro only on message per eseguire ad ogni chiamata" << endl;
					condiz = 0;
				}
				else {
					last_message = variabile;
				}
			}
			

			if (condiz == 1) {

				if (TipoOggComando == BULBDUO) {
					ComandoBulbDuo();
				}
				else if (TipoOggComando == PLUG) {
					ComandoPlug();
				}
				else if (TipoOggComando == FUNZIONE) {
					ComandoFunzione();
				}
			}

			cout << "----------------------------------------------------------------------" << endl;
		}
	}
	else {
		cout << "Errore da gestire" << endl;
	}
}

string Routines::GetTime() {
	// Ottieni il timestamp corrente
	auto now = chrono::system_clock::now();
	auto now_ms = chrono::time_point_cast<chrono::milliseconds>(now);
	auto value = now_ms.time_since_epoch();
	long long timestamp = value.count();

	// Converti il timestamp in un orario leggibile
	time_t now_time = static_cast<time_t>(timestamp / 1000); // divide per 1000 per ottenere i secondi
	tm now_tm;
	localtime_s(&now_tm, &now_time);
	char time_str[100];
	strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", &now_tm); // formatta l'orario come stringa
	string str;
	str.assign(time_str);

	return str;
}

