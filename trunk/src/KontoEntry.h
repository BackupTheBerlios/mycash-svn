/**
* @file KontoEntry.h
*
* @brief Datei beinhaltet die Klassendeklaration von KontoEntry
*/
#ifndef _CLASS_KONTOENTRY_H_
#define _CLASS_KONTOENTRY_H_

#include "konto_splitt.h"
#include <QDate>
#include <QVector>
#include <QString>
#include <QMap>
#include <QDomElement>
#include <QDomDocument>

/**
* @class KontoEntry
*
* @brief Klasse beinhaltet Daten einer Buchung
*/
class KontoEntry{	//zB ein Einkauf in einen Laden bzw. ein Einkaufszettel
	enum sort{NO=0, ASC=1, DES=2};
	enum returns{Ok=1, NotFound=4, NotChanged=5, NotValid=6}; 
	typedef QMap<quint32, KontoSplitt> MapSplitt;

	public:
		KontoEntry();
		KontoEntry(QDate datum, QString verwendung, quint32 transfer, MapSplitt *splittdaten);
		KontoEntry(const QDomElement& element);
		~KontoEntry();

		quint32 changeTransfer(quint32 transfer);
		quint32 changeDatum(QDate datum);
		quint32 changeDatum(QString datum); //yyyyMMdd
		quint32 changeBetrag(float betrag, quint32 nummer);
		quint32 changeVerwendung(QString verwendung, quint32 nummer);	// Splittdaten
		quint32 changeKategorie(quint32 kategorie, quint32 nummer);
		quint32 changeVerwendung(QString verwendung);	//Hauptverwendung
		quint32 changeSteuerrelevanz(bool steuer, quint32 nummer);
		quint32 changeShop(quint32 shop);

		quint32 addSplitt(KontoSplitt *splitt);
		quint32 deleteSplitt(quint32 splittnummer);

		operator bool();

		QString getVerwendung();
		float getBetrag();
		QString getDatum();
		QVector<quint32> getKategorien(quint32 sort = ASC);
		QVector<KontoSplitt> getSplittdaten();
		quint32 getTransfer();
		quint32 getShop();
		// --- get ..KontoSplitts-Werte..

		float getBetragKategorie(quint32 kategorie);
		float getBetragKategorieIntervall(quint32 kategorie, QDate von, QDate bis);

		QDomElement getXmlElement(QDomDocument &doc);

		quint32 list_Entry_deb();

	private:
		quint32 getFreeNumber();

		quint32 Transfer;
		QDate Datum;
		QString Verwendung;	// z.B "Einkauf bei xxx"
		quint32 Shop;
 		MapSplitt Splittdaten;	//Kategorien auf dem Einkaufszettel
};

#endif 
