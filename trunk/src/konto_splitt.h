/**
* @file konto_splitt.h
*
* @brief Datei entält die Klassendefinition von KontoSplitt
*/
#ifndef _class_konto_splitt_h
#define _class_konto_splitt_h

#include <QString>
#include <QDomElement>
#include <QDomDocument>

/**
* @class KontoSplitt
*
* @brief Klasse enthält alle Daten einer Unterbuchung.
*
* Als Unterbuchungen sind zum Beispiel Waren/Objekte einer Buchung mit unterschiedlicher Kategorie oder steuerrelevanz zu sehen.
*/
class KontoSplitt{
	/**
	* @brief Enumeration mit (Status-)Rückgabewerten der Methoden dieser Klasse
	*/
	enum returns{Ok=1, NotFound=4, NotChanged=5, NotValid=6};
	
	public:
		KontoSplitt();
		KontoSplitt(const QDomElement &element);
		KontoSplitt(quint32 kategorie, QString verwendung, float betrag, bool steuer = false);
		quint32 getKategorie();
		QString getVerwendung();
		float getBetrag();
		bool getSteuerrelevanz();
		quint32 changeKategorie(quint32 kategorie);
		quint32 changeVerwendung(QString verwendung);
		quint32 changeBetrag(float betrag);
		quint32 changeSteuerrelevanz(bool steuer);

		operator bool();

		QDomElement getXmlElement(QDomDocument &doc);
		
	private:
		quint32 Kategorie;
		QString Verwendung;
		float Betrag;
		bool SteuerRelevant;
};

#endif
