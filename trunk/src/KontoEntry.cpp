#include "KontoEntry.h"
#include <QVector>
#include <QDate>
#include <QString>
#include <QtAlgorithms>
#include <QTextStream>
#include <QtXml> //contain #include <QDomElement>, #include <QDomDocument>, #include <QDomText>
#include "EntryKategorie.h"

KontoEntry::KontoEntry(QDate datum, QString verwendung, quint32 transfer, MapSplitt *splittdaten)
/******************************************************************************
* Konstrukter mit Einzeleintraegen
*******************************************************************************/
{
	Datum = datum;
	Transfer = transfer;
	Verwendung = verwendung;
	Splittdaten = *splittdaten;
}


KontoEntry::KontoEntry()
/******************************************************************************
* Standardkonstruktor
*******************************************************************************/
{

}


KontoEntry::KontoEntry(const QDomElement & element)
/******************************************************************************
* Konstruktor mit QDomElement
*******************************************************************************/
{
	if( element.tagName() != "Eintraege" ){
		return;
	}
	
	QDomNode node = element.firstChild();
	while( !node.isNull() ){
		QString nodeText = node.toElement().tagName();
		if( nodeText == "Datum" ){
			Datum = QDate::fromString(node.toElement().text(), "yyyyMMdd");
		}else if( nodeText == "Verwendung" ){
			Verwendung = node.toElement().text();
		}else if( nodeText == "Splittdaten" ){
			KontoSplitt tempSplitt( node.toElement() );
			if( tempSplitt ){
				addSplitt( &tempSplitt ); 
			}
		}else{
			// ITEM invalid
		}

		node = node.nextSibling();
	}
}


KontoEntry::~KontoEntry(){

}


KontoEntry::operator bool()
/******************************************************************************
* Ueberladener Bool-Operator
*******************************************************************************/
{
	if( !Datum.isNull()  && Splittdaten.size() > 0 ){
		return true;
	}else{
		return false;
	}
}


QString KontoEntry::getDatum(){
	return Datum.toString("yyyyMMdd");
}


float KontoEntry::getBetrag(){
	float betrag = 0.0;
	MapSplitt::iterator it;
	for(it = Splittdaten.begin(); it != Splittdaten.end(); it++){
		betrag += it.value().getBetrag();
	}
	return betrag;
}


QVector<quint32> KontoEntry::getKategorien(quint32 sort){
	QVector<quint32> kategorien;
	MapSplitt::iterator it;
	for(it = Splittdaten.begin(); it != Splittdaten.end(); it++){
		kategorien.push_back( it.value().getKategorie() ); 
	}
	if(sort == ASC){
		qSort(kategorien.begin(), kategorien.end(), qLess<quint32>() );
	}
	if(sort == DES){
		qSort(kategorien.begin(), kategorien.end(), qGreater<quint32>() );
	}
	return kategorien;
}


QString KontoEntry::getVerwendung(){
	return Verwendung;
}


quint32 KontoEntry::changeTransfer(quint32 transfer)
/******************************************************************************
* Methode aendert die Art des Transfers
*******************************************************************************/
{
	Transfer = transfer;
	return true;
}


quint32 KontoEntry::changeVerwendung(QString verwendung){
	// ToDo: Überprüfung Verwendung
	Verwendung = verwendung;
	return Ok;
}


quint32 KontoEntry::changeDatum(QDate datum){
	//QTextStream console(stdout);
	Datum = datum;
	//console << "\t" << "KontoEntry::changeDatum()" << Datum.toString("yyyyMMdd") << "\r\n";
	return Ok;
}


quint32 KontoEntry::changeDatum(QString datum){
	QString jahr, monat, tag;
	jahr = datum.mid(0,4);
	monat = datum.mid(4,2);
	tag = datum.mid(6,2);
	bool ok;
	Datum.setDate(jahr.toUInt(&ok, 10), monat.toUInt(&ok, 10), tag.toUInt(&ok, 10));
	return Ok;
}


quint32 KontoEntry::getFreeNumber(){
	MapSplitt::iterator it;
	it = Splittdaten.end();
	if(it == Splittdaten.begin()){	// logisch richtig???
		return 1;
	}
	it--;
	return it.key() + 1; 
}


quint32 KontoEntry::changeBetrag(float betrag, quint32 nummer){
	MapSplitt::iterator it;
	for(it = Splittdaten.begin(); it != Splittdaten.end(); it++){
		if( it.key() == nummer){
			return it.value().changeBetrag(betrag);
		}
	}
	return NotFound;
}


quint32 KontoEntry::changeVerwendung(QString verwendung, quint32 nummer){
	MapSplitt::iterator it;
	for(it = Splittdaten.begin(); it != Splittdaten.end(); it++){
		if(it.key() == nummer){
			return it.value().changeVerwendung(verwendung);
		}
	}
	return NotFound;
}


quint32 KontoEntry::changeKategorie(quint32 kategorie, quint32 nummer){
	MapSplitt::iterator it;
	for(it = Splittdaten.begin(); it != Splittdaten.end(); it++){
		if(it.key() == nummer){
		//if(it.value().changeKategorie())
			return it.value().changeKategorie(kategorie);
		}
	}
	return 0;
}


QVector<KontoSplitt> KontoEntry::getSplittdaten(){
	QVector<KontoSplitt> returnvalue;
	MapSplitt::iterator it;
	for(it = Splittdaten.begin(); it != Splittdaten.end(); it++){
		returnvalue.push_back( it.value() );
	}	
	return returnvalue;
}


quint32 KontoEntry::addSplitt(KontoSplitt *splitt){
	Splittdaten[ getFreeNumber() ] = *splitt;
	return Ok;
}


quint32 KontoEntry::deleteSplitt(quint32 splittnummer){
	MapSplitt::iterator it;
	for(it = Splittdaten.begin(); it != Splittdaten.end(); it++){
		if( it.key() == splittnummer ){
			Splittdaten.erase( it );
			return Ok;
		}
	}
	return NotFound;
}


float KontoEntry::getBetragKategorieIntervall(quint32 kategorie, QDate von, QDate bis){
	float summe = 0.0;
	if((Datum >= von) && (Datum <= bis)){
		MapSplitt::iterator it;
		for(it = Splittdaten.end(); it != Splittdaten.end(); it++){
			if( (it.value().getKategorie() == kategorie) || (kategorie == EntryKategorie::ALLE) ){
				summe += it.value().getBetrag();
			}
		}
	}
	return summe;
}


float KontoEntry::getBetragKategorie(quint32 kategorie){
	float summe = 0.0;
	MapSplitt::iterator it;
	for(it = Splittdaten.begin(); it != Splittdaten.end(); it++){
		if( (it.value().getKategorie() == kategorie) || (kategorie == EntryKategorie::ALLE) ){
			summe += it.value().getBetrag();
		}
	}
	return summe;
}


quint32 KontoEntry::getTransfer()
/******************************************************************************
* Methode gibt die Transfermethode zurueck
*******************************************************************************/
{
	return Transfer;
}


QDomElement KontoEntry::getXmlElement(QDomDocument &doc)
/******************************************************************************
* Methode gibt ein QDomElement mit den Eintraegen zurueck
*******************************************************************************/
{
	QDomElement elementEntry = doc.createElement("Eintraege");
	QDomElement elementDatum = doc.createElement("Datum");
	QDomElement elementVerwendung = doc.createElement("Verwendung");
	
	QDomText textDatum = doc.createTextNode( getDatum() );
	QDomText textVerwendung = doc.createTextNode( getVerwendung() );

	elementDatum.appendChild(textDatum);
	elementVerwendung.appendChild(textVerwendung);

	elementEntry.appendChild(elementDatum);
	elementEntry.appendChild(elementVerwendung);

	MapSplitt::iterator it;
	for(it = Splittdaten.begin(); it != Splittdaten.end(); it++){
		elementEntry.appendChild( it.value().getXmlElement(doc) );
	}
	
	return elementEntry;
}


quint32 KontoEntry::list_Entry_deb(){
	QTextStream console(stdout);
	console << "\t" << "KontoEntry::list_Entry_deb(): ";
	console << "Datum: " << getDatum() << "\t" << "Verwendung: " << getVerwendung() << "\t";
	MapSplitt::iterator it;
	for(it = Splittdaten.begin(); it != Splittdaten.end(); it++){
		console << "SplittKat: " << it -> getKategorie() << "\t";
		console << "SplittVer: " << it -> getVerwendung() << "\t";
		console << "SplittBet: " << it -> getBetrag() << "\t";	
	}
	console << "\r\n";
	console.flush();
	return Ok;
}
