#ifndef _CLASS_KONTOENTRY_H_
#define _CLASS_KONTOENTRY_H_

#include "konto_splitt.h"
#include <QDate>
#include <QVector>
#include <QString>
#include <QMap>
#include <QDomElement>
#include <QDomDocument>

class KontoEntry{	//zB ein Einkauf in einen Laden bzw. ein Einkaufszettel
	enum sort{NO=0, ASC=1, DES=2};
	enum returns{Ok=1, NotFound=4}; 
	typedef QMap<quint32, Konto_Splitt> MapSplitt;

	public:
		KontoEntry();
		KontoEntry(QDate datum, QString verwendung, quint32 transfer, MapSplitt *splittdaten);
		~KontoEntry();

		quint32 changeTransfer(quint32 transfer);
		quint32 changeDatum(QDate datum);
		quint32 changeDatum(QString datum); //yyyyMMdd
		quint32 changeBetrag(float betrag, quint32 nummer);
		quint32 changeVerwendung(QString verwendung, quint32 nummer);	// Splittdaten
		quint32 changeKategorie(quint32 kategorie, quint32 nummer);
		quint32 changeVerwendung(QString verwendung);	//Hauptverwendung

		quint32 addSplitt(Konto_Splitt *splitt);
		quint32 deleteSplitt(quint32 splittnummer);

		QString getVerwendung();
		float getBetrag();
		QString getDatum();
		QVector<quint32> getKategorien(quint32 sort);
		QVector<Konto_Splitt> getSplittdaten();
		quint32 getTransfer();

		float getBetragKategorie(quint32 kategorie);
		float getBetragKategorieIntervall(quint32 kategorie, QDate von, QDate bis);

		QDomElement getXmlElement(QDomDocument &doc);

		quint32 list_Entry_deb();

	private:
		quint32 getFreeNumber();

		quint32 Transfer;
		QDate Datum;
		QString Verwendung;	// z.B "Einkauf bei xxx"
 		MapSplitt Splittdaten;	//Kategorien auf dem Einkaufszettel
};

#endif 
