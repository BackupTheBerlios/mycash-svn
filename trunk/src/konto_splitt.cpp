#include "konto_splitt.h"
#include <QString>

#include <QtXml> //contains #include <QDomDocument>, #include <QDomElement>, #include <QDomText>

KontoSplitt::KontoSplitt(){

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
		}else{
			// Item nicht gueltig
		}

		node = node.nextSibling();
	}
}


KontoSplitt::KontoSplitt(quint32 kategorie, QString verwendung, float betrag){
	Kategorie = kategorie;
	Verwendung = verwendung;
	Betrag = betrag;
}


KontoSplitt::operator bool()
/******************************************************************************
* UEberladener Bool-Operator
* Wenn Verwendung != "" && Kategorie != 0 -> true, sonst false
*******************************************************************************/
{
	if( Verwendung != "" && Kategorie != 0 ){
		return true;
	}else{
		return false;
	}
}


quint32 KontoSplitt::changeKategorie(quint32 kategorie){
	Kategorie = kategorie;
	return Ok;
}


quint32 KontoSplitt::getKategorie(){
	return Kategorie;
}


QString KontoSplitt::getVerwendung(){
	return Verwendung;
}


float KontoSplitt::getBetrag(){
	return Betrag;
}


quint32 KontoSplitt::changeBetrag(float betrag){
	// ToDo: betrag ueberpruefen
	Betrag = betrag;
	return Ok;
}


quint32 KontoSplitt::changeVerwendung(QString verwendung){
	Verwendung = verwendung;
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
