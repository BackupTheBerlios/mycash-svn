#ifndef _class_konto_splitt_h
#define _class_konto_splitt_h

#include <QString>

class Konto_Splitt{
	enum returns{Ok=1};
	
	public:
		Konto_Splitt();
		Konto_Splitt(quint32 kategorie, QString verwendung, float betrag);
		quint32 changeKategorie(quint32 kategorie);
		quint32 getKategorie();
		QString getVerwendung();
		quint32 changeVerwendung(QString verwendung);
		float getBetrag();
		quint32 changeBetrag(float betrag);
		
	private:
		quint32 Kategorie;
		QString Verwendung;
		float Betrag;
};

#endif
