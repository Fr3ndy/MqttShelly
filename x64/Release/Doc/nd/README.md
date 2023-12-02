# Mqtt shelly c++

Programma in c++ 

Data creazione: 01/04/2023

Ultima modifica: 16/05/2023

Cosa fà:

* Connessione al broker mqtt
* Creazione dispositivi shelly.
* Eliminazione dispositivi shelly. 
* Creazione routines.
* Modifica stato routines.
* Eliminazione routines

## Connessione al broker

Per connettersi al broker MQTT, il programma utilizza l'indirizzo completo (IP e porta) preso da un file di configurazione apposito. Tale file di configurazione si trova nella cartella /Mqtt del programma e contiene un file chiamato IpPort.txt, che a sua volta contiene solamente l'indirizzo IP del broker. Nel caso in cui si voglia modificare l'indirizzo IP del broker, è necessario assicurarsi che il programma non sia in esecuzione, modificare il file eliminando il precedente indirizzo IP e sostituendolo con il nuovo. Si tenga presente che, nel caso in cui il file venga eliminato o non esista all'interno della directory di esecuzione, il programma creerà un nuovo file con l'indirizzo predefinito localhost:1883.

Nel caso di errori il terminale mostra l'errore con un colore del carattere rosso.
## Creazione Shelly

La creazione del dispositivo Shelly all'interno del programma può essere effettuata in 2 diversi modi.
* **tramite publish al broker mqtt**
* **tramite la modifica del file dispositivi.txt**
In entrambi i modi di creazione bisogna conoscere il tipo e il suo id

il tipo è collegato al tipo del dispositivo, puoi trovare il tipo del dispositivo sulle api dei dispositivi [shelly](https://shelly-api-docs.shelly.cloud/gen1/).

nota che all'interno del programma i dispositivi compatibili sono:
* [Lampadina](https://shelly-api-docs.shelly.cloud/gen1/#shelly-duo) -> ShellyBulbDuo-
* [Sensore ht](https://shelly-api-docs.shelly.cloud/gen1/#shelly-h-amp-t-settings) -> shellyht-
* [Sensore Dw](https://shelly-api-docs.shelly.cloud/gen1/#shelly-door-window-1-2) -> shellydw-
* [Presa Plug](https://shelly-api-docs.shelly.cloud/gen1/#shelly-plug-plugs-overview) -> shellyplug-s-

l'id del dispositivo invece è quella serie di numeri e lettere che viene utilizzato per identificare il dispositivo. Puoi trovare l'id sulla pagina http del dispositivo su informazioni dispositivo. ATTENZIONE il l'identificativo dele avere le lettere maiuscole.

**Un esempio di dispositivo:**

shellydw2-D751AF

Indipendentemente dal metodo di creazione,si possono creare un massimo di 100 dispositivi per tipo.

### Creazione tramite publish mqtt

per creare un dispositivo sull' appplicativo dobbiamo primaditutto sapere l'indirizzo del broker dove è collegata.
il comando da eseguire tramite terminale mosquitto è il seguente.

```
 mosquitto_pub -h <INDIRIZZO_BROKER> -p <PORTA> -t Shelly/CreateDevices/ -m <ID_DISPOSITIVO> 
```
Il programma leggerà il topic creando il dispositivo all'interno dell'applicativo. Se il dispositivo risulta non valido non verrà creato, 
per visualizzare l'errore controlla le stampe dell'applicativo.

### Creazione tramite modifica del file

un altra opzione per aggiungere un dispositivo può essere effettuta modificando il file dispositivi.txt.
La modifica di questo file deve essere effettuata solo quando il programma non è in esecuzione, se si fà altrimenti il programma potrebbe non funzionare correttamente.

Per aggiungere o eliminare il dispositivo dobbiamo prendere il file e aggiungere o modificare i dispositivi, ogni linea è dedicata ad un solo dispositivo.


## Authors

Andrea Spina

 

 

