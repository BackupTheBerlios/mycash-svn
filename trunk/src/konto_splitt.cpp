/**
* @file konto_splitt.cpp
*
* @brief Datei enthält die Methodendefinitionen der Klasse KontoSplitt
*/
#include "konto_splitt.h"
#include <QString>

#include <QtXml> //contains #include <QDomDocument>, #include <QDomElement>, #include <QDomText>

/**
* @brief Überladener Konstruktor der Klasse
* Dieser erzeugt keine gültige Klasse.
*/
KontoSplitt::KontoSplitt()
{

}

/**
* @brief Überladener Konstruktor der Klasse zum Initialisieren über ein XML-Zweig
* @param element Ein XML-Zweig indem klassenrelevante Daten enthalten sind.
* Falsche Unterzweige werden ignoriert.
*/
KontoSplitt::KontoSplitt(const QDomElement &element)
{
	if( element.tagName() != "Splittdaten" ){ //Falscher Baum
		return;
	}

	//QDomNode
	QDomNode node = element.firstChild();
	while( !node.isNull() ){
		QString textNode = node.toElement().tagName();
		if(textNode == "Kategorie"){
			Kategorie = node.toElement().text().toUInt(0);
		}else if( textNode == "Verwendung"){
			Verwendung = node.toElement().text();
		}else if( textNode == "Betrag"){
			Betrag = node.toElement().text().toFloat(0);
		}else if( textNode == "Steuerrelevant" ){
			SteuerRelevant = (bool)node.toElement().text().toInt(0);
		}else{
			// Item nicht gueltig
		}

		node = node.nextSibling();
	}
}


/**
* @brief Überladener Konstruktor der Klasse
* @param kategorie ID der Kategorie
* @param verwendung Verwendung dieses Eintrages
* @param betrag Betrag dieses Eintrages
* @param steuer Steuerrelevanz dieses Eintrages. Vorinitialisiert mit false
*/
KontoSplitt::KontoSplitt(quint32 kategorie, QString verwendung, float betrag, bool steuer)
{
	Kategorie = kategorie;
	Verwendung = verwendung;
	Betrag = betrag;
	SteuerRelevant = steuer;
}


/**
* @brief Überladener bool-Operator
* @return Eintrag gültig
* Ein Eintrag gilt als gültig, wenn eine Verwendung zugeordnet wurde und die ID der Kategorie nicht 0 ist.
*/
KontoSplitt::operator bool()
{
	if( Verwendung != "" && Kategorie != 0 ){
		return true;
	}else{
		return false;
	}
}


/**
* @brief Methode ändert die Kategorie dieses Eintrages
* @param kategorie Neue Kategorie (ID)
* @return Ergebniss der Operation
*/
quint32 KontoSplitt::changeKategorie(quint32 kategorie)
{
	if(Kategorie != kategorie){
		Kategorie = kategorie;
		return Ok;
	}
	return NotChanged;
}


/**
* @brief Methode gibt die Kategorie zurück
* @return Kategorie (ID) des Eintrages
*/
quint32 KontoSplitt::getKategorie()
{
	return Kategorie;
}


/**
* @brief Methode gibt die Verwendung dieses Eintrages zurück
* @return Verwendung des Eintrages
*/
QString KontoSplitt::getVerwendung()
{
	return Verwendung;
}


/**
* @brief Methode gibt den Betrag dieses Eintrages zurück
* @return Betrag des Eintrages
*/
float KontoSplitt::getBetrag()
{
	return Betrag;
}


/**
* @brief Methode gibt die Steuerrelevanz dieses Eintrages zurück
* @return Eintrag steuerrelevant
*/
bool KontoSplitt::getSteuerrelevanz()
{
 return SteuerRelevant;
}


/**
* @brief Methode ändert den Betrag dieses Eintrages
* @param betrag Neuer Betrag
* @return Ergebniss der Operation
*/
quint32 KontoSplitt::changeBetrag(float betrag)
{
	if(Betrag != betrag){
		// ToDo: betrag ueberpruefen
		Betrag = betrag;
		return Ok;
	}
	return NotChanged;
}


/**
* @brief Methode ändert die Verwendung dieses Eintrages
* @param verwendung Neue Verwendung
* @return Ergebniss der Operation
*/
quint32 KontoSplitt::changeVerwendung(QString verwendung)
{
	if(Verwendung != verwendung){
		Verwendung = verwendung;
		return Ok;
	}
	return NotChanged;
}


/**
* @brief Methode ändert die Steuerrelevant dieses Eintrages
* @param steuer Neue Steuerrelevanz
* @return Ergebniss der Operation
*/
quint32 KontoSplitt::changeSteuerrelevanz(bool steuer)
{
	if(SteuerRelevant != steuer){
		SteuerRelevant = steuer;
		return Ok;
	}
	return NotChanged;
}


/**
* @brief Methode erstellt aus diesen Eintrag ein XML-Zweig
* @param doc Referenz zum Dokument
* @return Erstellter XML-Zweig
*/
QDomElement KontoSplitt::getXmlElement(QDomDocument &doc)
{
	QDomElement elementSplitt = doc.createElement("Splittdaten");
	QDomElement elementKategorie = doc.createElement("Kategorie");
	QDomElement elementVerwendung = doc.createElement("Verwendung");
	QDomElement elementBetrag = doc.createElement("Betrag");
	QDomElement elementSteuer = doc.createElement("Steuerrelevant");
	
	QDomText textKategorie = doc.createTextNode( QString().setNum(Kategorie) );
	QDomText textVerwendung = doc.createTextNode(Verwendung);
	QDomText textBetrag = doc.createTextNode( QString().setNum(Betrag, 'f', 2) );
	QDomText textSteuer = doc.createTextNode( QString().setNum( SteuerRelevant?1:0 ) );

	elementKategorie.appendChild( textKategorie );
	elementVerwendung.appendChild( textVerwendung );
	elementBetrag.appendChild( textBetrag );
	elementSteuer.appendChild( textSteuer );

	elementSplitt.appendChild( elementKategorie );
	elementSplitt.appendChild( elementVerwendung );
	elementSplitt.appendChild( elementBetrag );
	elementSplitt.appendChild( elementSteuer );

	return elementSplitt;
}
