#include "Dispositivo.h"

Dispositivo::Dispositivo(std::string nome) : nome_(nome), valore_(0) {}

void Dispositivo::aggiungiCondizione(Condizione c) {
    condizioni_.push_back(c);
}

void Dispositivo::setValore(int valore) {
    valore_ = valore;
    for (auto c : condizioni_) {
        if (c.isSoddisfatta(valore_)) {
            c.eseguiOperazione();
        }
    }
}