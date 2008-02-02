#ifndef _KONTO_TYP_H_
#define _KONTO_TYP_H_

#include "configs.h"
#include <QString>
#include <QVector>
#include <QMap>
#include <QtGlobal>

/*
	struct KTypS
	Speichert die Bezeichnung und die Optionen einer zugehörigen Typenummer
	QString Bezeichnung:	Bezeichnung
	quint32 Qptions:		Optionen des Kontotyps
*/
struct KTypS{
	QString Bezeichnung;
	quint32 Options;
};

class Konto_Typ{
	typedef QMap<quint32, KTypS> MapTyp;
	
	public:
	/*
		enum Status
		Rueckgabewerte von Funktionen
	*/
	enum Status{
				NotFound = 0
				};
	
	/*
		enum Options
		Optionen der Kontentypen
	*/
	enum Options{
				Error = 0x0,
				Soll = 0x1,
				Haben = 0x2,
				NoSigned = 0x4
				};
	
	/*
		Konto_Typ()
		Konstrukter
	*/	
	Konto_Typ();

	/*
		~Konto_Typ()
		Destrukter
	*/
	~Konto_Typ();
	
	/*
		void newTyp(QString Bezeichnung, quint32 Option)
		Methode legt einen neuen Kontotyp an
		QString Bezeichung:	Bezeichnung des Kontotyps
		quint32 Option:		Optionen des zuerstellenden Kontotyps
	*/
	void newTyp(QString Bezeichnung, quint32 Option);
	
	/*
		quint32 getTypNummer(QString Bezeichnung)
		Methode gibt die Typennummer zu einer Bezeichnung zurueck
	*/
	quint32 getTypNummer(QString Bezeichnung);

	/*
		QString getTypBezeichnung(quint32 Nummer)
		Methode liefert die Bezeichung zu einen Kontotypen zurueck
	*/
	QString getTypBezeichnung(quint32 Nummer);

	/*
		quint32 getTypOptions(quint32 Nummer)
		Methode gibt die Optionen einer Kontotypbezeichnung zurueck
	*/
	quint32 getTypOptions(QString Bezeichnung);
	
	/*
		quint32 getTypOptions(QString Bezeichnung)
		Methode liefert die Optionen zu einer Kontotypnummer zurueck
	*/
	quint32 getTypOptions(quint32 Nummer);


	/*
		void saveTyp()
		Methode speichert die Kontotypen
	*/
	void saveTyp();
	

	/*
		QVector<QString> listTypen()
		Methode gibt alle Kontotypen zurueck
	*/
	QVector<QString> listTypen();
	
	private:
	/*
		uint32 getFreeNumber()
		Funktion gibt die naechste freie Nummer zurueck; freie Nummern zwischen zwei Typen werden ignoriert
	*/
	quint32 getFreeNumber();
	

	//std::vector<QString> typen;
	
	/*
		QMap<quint32, KTypS> typen
		Speicherung der Kontotypen mit deren Bezeichung und Optionen
		quint32:	Typ
		KTypS:		Struct mit Bezeichnung und Option
	*/
	MapTyp typen;
	
	/*
		bool isChanged
		Variable speichert das Stattfinden einer Veränderung
	*/
	bool isChanged;
};

#endif
