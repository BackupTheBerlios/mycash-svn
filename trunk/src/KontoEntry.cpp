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
	//ToDo: Werte vorinitialiseren!!!
	
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
		}else if( nodeText == "Transfer" ){
			Transfer = node.toElement().text().toUInt();
		}else if( nodeText == "Shop" ){
			Shop = node.toElement().text().toUInt();
		}else{
			// ITEM invalid
		}

		node = node.nextSibling();
	}
}


KontoEntry::~KontoEntry()
/******************************************************************************
* Destruktor
*******************************************************************************/
{

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


QString KontoEntry::getDatum()
/******************************************************************************
* Methode gibt das Datum als String zurueck
*******************************************************************************/
{
	return Datum.toString("yyyyMMdd");
}


float KontoEntry::getBetrag()
/******************************************************************************
* Methode gibt den Gesamtbetrag zurueck
*******************************************************************************/
{
	float betrag = 0.0;
	MapSplitt::iterator it;
	for(it = Splittdaten.begin(); it != Splittdaten.end(); it++){
		betrag += it.value().getBetrag();
	}
	return betrag;
}


QVector<quint32> KontoEntry::getKategorien(quint32 sort)
/******************************************************************************
* Methode gibt die Kategorien eines Eintrages zurueck
*******************************************************************************/
{
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


QString KontoEntry::getVerwendung()
/******************************************************************************
* Methode gibt die Verwendung zurueck
*******************************************************************************/
{
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


quint32 KontoEntry::changeVerwendung(QString verwendung)
/******************************************************************************
* Methode aendert die Verwendung
*******************************************************************************/
{
	// ToDo: Überprüfung Verwendung
	Verwendung = verwendung;
	return Ok;
}


quint32 KontoEntry::changeDatum(QDate datum)
/******************************************************************************
* Methode aendert das Datum
* Eingabe des Datums als QDate
*******************************************************************************/
{
	Datum = datum;
	return Ok;
}


quint32 KontoEntry::changeDatum(QString datum)
/******************************************************************************
* Methode aendert das Datum
* Eingabe als String im Format "yyyyMMdd"
*******************************************************************************/
{
	QString jahr, monat, tag;
	jahr = datum.mid(0,4);
	monat = datum.mid(4,2);
	tag = datum.mid(6,2);
	bool ok;
	Datum.setDate(jahr.toUInt(&ok, 10), monat.toUInt(&ok, 10), tag.toUInt(&ok, 10));
	return Ok;
}


quint32 KontoEntry::changeSteuerrelevanz(bool steuer, quint32 nummer)
/******************************************************************************
* Methode aendert die Steuerrelevanz eines Eintrages
*******************************************************************************/
{
	MapSplitt::iterator it;
	for(it = Splittdaten.begin(); it != Splittdaten.end(); it++){
		if( it.key() == nummer ){
			return it.value().changeSteuerrelevanz( steuer );
		}
	}
	return NotFound;
}


quint32 KontoEntry::getFreeNumber() //private
/******************************************************************************
* Methode gibt eine freie Nummer zurueck
*******************************************************************************/
{
	MapSplitt::iterator it;
	it = Splittdaten.end();
	if(it == Splittdaten.begin()){	// logisch richtig???
		return 1;
	}
	it--;
	return it.key() + 1; 
}


quint32 KontoEntry::changeBetrag(float betrag, quint32 nummer)
/******************************************************************************
* Methode aendert den Betrag des Splitteintreages nummer in betrag
*******************************************************************************/
{
	MapSplitt::iterator it;
	for(it = Splittdaten.begin(); it != Splittdaten.end(); it++){
		if( it.key() == nummer){
			return it.value().changeBetrag(betrag);
		}
	}
	return NotFound;
}


quint32 KontoEntry::changeVerwendung(QString verwendung, quint32 nummer)
/******************************************************************************
* Methode aendert die Verwendung des Splitteintrages nummer in verwendung
*******************************************************************************/
{
	MapSplitt::iterator it;
	for(it = Splittdaten.begin(); it != Splittdaten.end(); it++){
		if(it.key() == nummer){
			return it.value().changeVerwendung(verwendung);
		}
	}
	return NotFound;
}


quint32 KontoEntry::changeKategorie(quint32 kategorie, quint32 nummer)
/******************************************************************************
* Methode aendert die Kategorie des Splitteintrages nummer in kategorie
*******************************************************************************/
{
	MapSplitt::iterator it;
	for(it = Splittdaten.begin(); it != Splittdaten.end(); it++){
		if(it.key() == nummer){
		//if(it.value().changeKategorie())
			return it.value().changeKategorie(kategorie);
		}
	}
	return 0;
}


quint32 KontoEntry::changeShop(quint32 shop)
/******************************************************************************
* Methode aendert den Shop
*******************************************************************************/
{
	Shop = shop;
	return Ok;
}


QVector<KontoSplitt> KontoEntry::getSplittdaten()
/******************************************************************************
* Methode gibt ein Vector mit den Splittdaten zurueck
*******************************************************************************/
{
	QVector<KontoSplitt> returnvalue;
	MapSplitt::iterator it;
	for(it = Splittdaten.begin(); it != Splittdaten.end(); it++){
		returnvalue.push_back( it.value() );
	}	
	return returnvalue;
}


quint32 KontoEntry::addSplitt(KontoSplitt *splitt)
/******************************************************************************
* Methode fuegt ein Splitteintrag hinzu
*******************************************************************************/
{
	Splittdaten[ getFreeNumber() ] = *splitt;
	return Ok;
}


quint32 KontoEntry::deleteSplitt(quint32 splittnummer)
/******************************************************************************
* Methode loescht Splitteintrag mit Nummer splittnummer
*******************************************************************************/
{
	MapSplitt::iterator it;
	for(it = Splittdaten.begin(); it != Splittdaten.end(); it++){
		if( it.key() == splittnummer ){
			Splittdaten.erase( it );
			return Ok;
		}
	}
	return NotFound;
}


float KontoEntry::getBetragKategorieIntervall(quint32 kategorie, QDate von, QDate bis)
/******************************************************************************
* Methode gibt den Gesamtbetrag der Splitteintraege einer Kategorie in einen Zeitintervall zurueck
*******************************************************************************/
{
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


float KontoEntry::getBetragKategorie(quint32 kategorie)
/******************************************************************************
* Methode gibt den Gesamtbetrag der Splitteintraege einer Kategorie zurueck
*******************************************************************************/
{
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


quint32 KontoEntry::getShop()
/******************************************************************************
* Methode gibt eine Shop-ID zurueck
*******************************************************************************/
{
	return Shop;
}


QDomElement KontoEntry::getXmlElement(QDomDocument &doc)
/******************************************************************************
* Methode gibt ein QDomElement mit den Eintraegen zurueck
*******************************************************************************/
{
	QDomElement elementEntry = doc.createElement("Eintraege"); //Hauptzweig
	QDomElement elementDatum = doc.createElement("Datum");
	QDomElement elementVerwendung = doc.createElement("Verwendung");
	QDomElement elementTransfer = doc.createElement("Transfer");
	
	QDomText textDatum = doc.createTextNode( getDatum() );
	QDomText textVerwendung = doc.createTextNode( getVerwendung() );
	QDomText textTransfer = doc.createTextNode ( QString::number( getTransfer() ));

	elementDatum.appendChild(textDatum);
	elementVerwendung.appendChild(textVerwendung);
	elementTransfer.appendChild( textTransfer );

	elementEntry.appendChild(elementDatum);
	elementEntry.appendChild(elementVerwendung);
	elementEntry.appendChild(elementTransfer);

	MapSplitt::iterator it;
	for(it = Splittdaten.begin(); it != Splittdaten.end(); it++){
		elementEntry.appendChild( it.value().getXmlElement(doc) );
	}
	
	return elementEntry;
}


quint32 KontoEntry::list_Entry_deb() //DEBUG
/******************************************************************************
* Methode listet alle Informationen auf der Konsole aus
*******************************************************************************/
{
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
