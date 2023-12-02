#include "FunzioniRoutines.h"

namespace fs = filesystem;

FunzioniRoutines::FunzioniRoutines() {

}

FunzioniRoutines::~FunzioniRoutines() {

}

void FunzioniRoutines::WriteCSV(string name, string variabile) {

	// Definizione del nome della cartella
	string folderName = "Funzioni";

	// Definizione del percorso completo della cartella
	fs::path folderPath = fs::current_path() / folderName;

	// Verifica se la cartella esiste
	if (!fs::exists(folderPath))
	{
		// Se la cartella non esiste, la creiamo
		if (!fs::create_directory(folderPath)) {
			cout << g_ErrStart << "Errore nella creazione della cartella Funzioni(Il programma potrebbe non funzionare correttamente): " << folderName << g_ErrEnd << endl;
		}
	}



	//cout << "sono su fwritecsv"  << endl;
	string c = "Funzioni/";
	string c1 = name;
	string c2 = ".csv";
	string c3 = c + c1 + c2;


	//cout << c3 << endl;
	ofstream outfile(c3, ios_base::app); // apri il file in modalità append
	if (outfile.is_open()) {

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

		//---------------------------------------------------------

		cout << time_str << "," << " Valore: " << variabile << endl;
		outfile << time_str << "," << " Valore: " << variabile << endl;

		outfile.close();
	}
	else {
		//errore di scrittura
		cout << g_ErrStart << "Errore di scrittura nel file csv" << g_ErrEnd << endl;
	}
}

void FunzioniRoutines::PublishMQTT(string topic, string msg)
{
	DispositivoDefault::ogg[0]->publica(topic, msg);
}
