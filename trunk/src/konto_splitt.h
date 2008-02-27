#ifndef _class_konto_splitt_h
#define _class_konto_splitt_h

#include <QString>
#include <QDomElement>
#include <QDomDocument>

class KontoSplitt{
	enum returns{Ok=1};
	
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
