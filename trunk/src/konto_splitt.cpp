#include "konto_splitt.h"
#include <QString>

#include <QtXml> //contains #include <QDomDocument>, #include <QDomElement>, #include <QDomText>

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


QDomElement Konto_Splitt::getXmlElement(QDomDocument &doc)
/******************************************************************************
* Methode gibt ein XML-Node zurueck
*******************************************************************************/
{
	QDomElement elementSplitt = doc.createElement("Splittdaten");
	QDomElement elementKategorie = doc.createElement("Kategorie");
	QDomElement elementVerwendung = doc.createElement("Verwendung");
	QDomElement elementBetrag = doc.createElement("Betrag");
	
	QDomText textKategorie = doc.createTextNode( QString().setNum(Kategorie) );
	QDomText textVerwendung = doc.createTextNode(Verwendung);
	QDomText textBetrag = doc.createTextNode( QString().setNum(Betrag, 'f', 2) );

	elementKategorie.appendChild(textKategorie);
	elementVerwendung.appendChild(textVerwendung);
	elementBetrag.appendChild(textBetrag);

	elementSplitt.appendChild(elementKategorie);
	elementSplitt.appendChild(elementVerwendung);
	elementSplitt.appendChild(elementBetrag);

	return elementSplitt;
}
