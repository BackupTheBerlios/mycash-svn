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
		KontoSplitt(quint32 kategorie, QString verwendung, float betrag);
		quint32 changeKategorie(quint32 kategorie);
		quint32 getKategorie();
		QString getVerwendung();
		quint32 changeVerwendung(QString verwendung);
		float getBetrag();
		quint32 changeBetrag(float betrag);

		operator bool();

		QDomElement getXmlElement(QDomDocument &doc);
		
	private:
		quint32 Kategorie;
		QString Verwendung;
		float Betrag;
};

#endif
