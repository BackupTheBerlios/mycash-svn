#include "konto_splitt.h"
#include <QString>

#include <QtXml> //contains #include <QDomDocument>, #include <QDomElement>, #include <QDomText>

KontoSplitt::KontoSplitt()
/******************************************************************************
* leerer Konstruktor
*******************************************************************************/
{

}


KontoSplitt::KontoSplitt(const QDomElement &element)
/******************************************************************************
* Konstruktor zum laden von XML-Baeumen
*******************************************************************************/
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


KontoSplitt::KontoSplitt(quint32 kategorie, QString verwendung, float betrag, bool steuer)
/******************************************************************************
* Konstruktor mit allen Werten.
* steuer(relevanz) mit false vorinitialisiert
*******************************************************************************/
{
	Kategorie = kategorie;
	Verwendung = verwendung;
	Betrag = betrag;
	SteuerRelevant = steuer;
}


KontoSplitt::operator bool()
/******************************************************************************
* Ueberladener Bool-Operator
* Wenn Verwendung != "" && Kategorie != 0 -> true, sonst false
*******************************************************************************/
{
	if( Verwendung != "" && Kategorie != 0 ){
		return true;
	}else{
		return false;
	}
}


quint32 KontoSplitt::changeKategorie(quint32 kategorie)
/******************************************************************************
* Methode aendert die Kategorie
*******************************************************************************/
{
	Kategorie = kategorie;
	return Ok;
}


quint32 KontoSplitt::getKategorie()
/******************************************************************************
* Methode gibt die Kategorie zurueck
*******************************************************************************/
{
	return Kategorie;
}


QString KontoSplitt::getVerwendung()
/******************************************************************************
* Methode gibt die Verwendung zurueck
*******************************************************************************/
{
	return Verwendung;
}


float KontoSplitt::getBetrag()
/******************************************************************************
* Methode gibt den Betrag zurueck
*******************************************************************************/
{
	return Betrag;
}


bool KontoSplitt::getSteuerrelevanz()
/******************************************************************************
* Methode gibt die Steuerrelevanz zurueck
*******************************************************************************/
{
 return SteuerRelevant;
}


quint32 KontoSplitt::changeBetrag(float betrag)
/******************************************************************************
* Methode aendert den Betrag
*******************************************************************************/
{
	// ToDo: betrag ueberpruefen
	Betrag = betrag;
	return Ok;
}


quint32 KontoSplitt::changeVerwendung(QString verwendung)
/******************************************************************************
* Methode aendert die Verwendung
*******************************************************************************/
{
	Verwendung = verwendung;
	return Ok;
}


quint32 KontoSplitt::changeSteuerrelevanz(bool steuer)
/******************************************************************************
* Methode aendert die Steuerrelevanz
*******************************************************************************/
{
	SteuerRelevant = steuer;
	return Ok;
}


QDomElement KontoSplitt::getXmlElement(QDomDocument &doc)
/******************************************************************************
* Methode gibt ein XML-Node zurueck
*******************************************************************************/
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
