/*
	Ueberlegung, ob isChanged sinnvoll, da eventuell nicht alle aenderungen erfasst werden können....
*/

#ifndef _konto_h_
#define _konto_h_

#include "KontoEntry.h"
#include <QString>
#include <QMap>

class Konto{
	enum Status{
		Ok = 1,
		WrongFile = 2,
		CantWrite = 3,
		NotFound = 4
	};
	enum Entry{
		newEntry = 0
	};

	enum Spalten{
		SpalteDatum = 1, SpalteVerwendung, SpalteAnzahl, SpalteSplittKategorie, SpalteSplittVerwendung, SpalteSplittBetrag
	};

	struct Verlauf{

	};

	typedef QMap<quint32, KontoEntry> MapKontoEntry;

	public:
		Konto();
		~Konto();
		quint32 loadFile(QString filename);
		quint32 saveFile();
		QString getKontoFile();
		quint32 setKontoFile(QString KontoFile);
		quint32 setKontoName(QString KontoName);
		quint32 setKontoBezeichnung(QString KontoBezeichnung);
		quint32 setBLZ(QString blz);
		quint32 setBankName(QString bankname);
		quint32 setKontoTyp(quint32 kontotyp);

		quint32 addEntry(quint32 nummer, KontoEntry *entry);
		quint32 addEntry(KontoEntry *entry);
		quint32 deleteEntry(quint32 nummer);
		quint32 deleteSplitt(quint32 nummer, quint32 splittnummer);

		quint32 changeDatum(QString datum, quint32 entry);
		quint32 changeDatum(QDate datum, quint32 entry);
		quint32	changeVerwendung(QString verwendung, quint32 entry);
		quint32 changeVerwendung(QString verwendung, quint32 entry, quint32 nummer);
		quint32 changeKategorie(quint32 kategorie, quint32 entry, quint32 nummer);
		quint32 changeBetrag(float betrag, quint32 entry, quint32 nummer);

		float getBetragKategorie(quint32 kategorie);
		float getBetragKategorieIntervall(quint32 kategorie, QDate von, QDate bis);
		
		operator bool();
		
		void showKontoData_deb();
		quint32 printEntry_deb();
	
	private:
		quint32 getFreeNumber();
		
		QString KontoName;
		QString KontoBezeichnung;
		QString KontoFile;	//Datei in der Kontodaten gespeichert
		QString BLZ;	//Bankleitzahl
		QString BankName;	//Bankname
		quint32 KontoTyp;

		bool isChanged;
		
		MapKontoEntry Eintraege;

		enum Zeilenspeicher{
			Kontoinfos = 1
		};
		
};

#endif
