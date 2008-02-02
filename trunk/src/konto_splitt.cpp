#include "konto_splitt.h"
#include <QString>

Konto_Splitt::Konto_Splitt(){

}


Konto_Splitt::Konto_Splitt(quint32 kategorie, QString verwendung, float betrag){
	Kategorie = kategorie;
	Verwendung = verwendung;
	Betrag = betrag;
}


quint32 Konto_Splitt::changeKategorie(quint32 kategorie){
	Kategorie = kategorie;
	return Ok;
}


quint32 Konto_Splitt::getKategorie(){
	return Kategorie;
}


QString Konto_Splitt::getVerwendung(){
	return Verwendung;
}


float Konto_Splitt::getBetrag(){
	return Betrag;
}


quint32 Konto_Splitt::changeBetrag(float betrag){
	// ToDo: betrag ueberpruefen
	Betrag = betrag;
	return Ok;
}


quint32 Konto_Splitt::changeVerwendung(QString verwendung){
	Verwendung = verwendung;
	return Ok;
}
