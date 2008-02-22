/*
 Ueberlegung, ob isChanged sinnvoll, da eventuell nicht alle aenderungen erfasst werden können....
*/

#ifndef _konto_h_
#define _konto_h_

#include "KontoEntry.h"
#include "repeat_entry.h"
#include <QString>
#include <QMap>
#include <QVector>
#include <QWidget>

class Konto : public QWidget
{
		Q_OBJECT

	public:
		enum Status {
			Ok = 1,
			WrongFile = 2,
			CantWrite = 3,
			NotFound = 4
		};
		enum Entry {
			newEntry = 0
		};

		enum Spalten {
			SpalteDatum = 1, SpalteVerwendung, SpalteAnzahl, SpalteSplittKategorie, SpalteSplittVerwendung, SpalteSplittBetrag
		};

		struct Verlauf {

		};

		struct KontoSettings {
			QString KontoName;
			QString KontoBeschreibung;
			QString BLZ;
			QString BankName;
			quint32 KontoTyp;
			QString FileName;
			float Limit;
			bool canBeNegativ;
		};

		struct HistoryList{
			QString Datum;
			QString Verwendung;
			quint32 Transfer;
			float Betrag;
		};

		struct HistoryListDetails{
			quint32 Entry;
			QString Datum;
			QString Verwendung;
			quint32 Transfer;
			float Betrag;
			QVector<KontoEntry> Entrys;	
		};

		typedef QMap<quint32, KontoEntry> MapKontoEntry;
		typedef QMap<quint32, RepeatEntry> MapRepeatEntry;
		typedef QVector<Konto::HistoryList> VectorHistoryList;

	public:
		Konto();
		Konto ( QString& filename );
		Konto ( QString& kontoname, QString& kontobeschreibung, QString& blz, QString& bankname, quint32& kontotyp );
		Konto ( QString kontoname, QString kontobeschreibung, QString blz, QString bankname, quint32 kontotyp, float limit, bool underLimit );
		~Konto();

		quint32 loadFile ( QString filename );
		quint32 loadFileXML ( QString filename );
		quint32 saveFile();
		quint32 saveFileXML();
		QString getKontoFile();
		QString getKontoName();
		float getLimitNegativ();
		bool getCanBeNegativ();

		Konto::KontoSettings getFormKontoSettings();

		quint32 setKontoFile ( QString KontoFile );
		quint32 setKontoName ( QString KontoName );
		quint32 setKontoBeschreibung ( QString KontoBezeichnung );
		quint32 setBLZ ( QString blz );
		quint32 setBankName ( QString bankname );
		quint32 setKontoTyp ( quint32 kontotyp );
		quint32 setLimitNegativ ( float limit );
		quint32 setCanBeNegativ ( bool beNegativ );

		quint32 addEntry ( quint32 nummer, KontoEntry *entry );
		quint32 addEntry ( KontoEntry *entry );
		quint32 deleteEntry ( quint32 nummer );
		quint32 deleteSplitt ( quint32 nummer, quint32 splittnummer );

		quint32 changeDatum ( QString datum, quint32 entry );
		quint32 changeDatum ( QDate datum, quint32 entry );
		quint32 changeVerwendung ( QString verwendung, quint32 entry );
		quint32 changeVerwendung ( QString verwendung, quint32 entry, quint32 nummer );
		quint32 changeKategorie ( quint32 kategorie, quint32 entry, quint32 nummer );
		quint32 changeBetrag ( float betrag, quint32 entry, quint32 nummer );

		float getBetragKategorie ( quint32 kategorie );
		float getBetragKategorieIntervall ( quint32 kategorie, QDate von, QDate bis );
		Konto::VectorHistoryList getHistoryList();
		//VectorHistoryList getHistoryList(QDate von, QDate bis);

		void setChanged();

		bool isNotChanged();

		operator bool();

		//DEBUG - FUNKTIONS
		void printKontoInfos();
		void showKontoData_deb();
		quint32 printEntry_deb();

	signals:
		void doChange();

	private:
		quint32 getFreeNumber();
		void setNotChanged();
		void sort();

		QString KontoName;
		QString KontoBeschreibung;
		QString KontoFile; //Datei in der Kontodaten gespeichert
		QString BLZ; //Bankleitzahl
		QString BankName; //Bankname
		quint32 KontoTyp;

		bool isChanged;
		float LimitNegativ;
		bool canBeNegativ;

		MapKontoEntry Eintraege;
		MapRepeatEntry RepeatEintraege;

		enum Zeilenspeicher {
			Kontoinfos = 1
		};

};

#endif
