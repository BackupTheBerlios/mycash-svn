#ifndef _CLASS_ENTRY_KATEGORIE_H_
#define _CLASS_ENTRY_KATEGORIE_H_

#include <QVector>
#include <QString>
#include <QMap>

class EntryKategorie{
	public:
		enum SymKategorie{ALLE = 0};
		enum returnvalues{Ok = 1, WrongFile = 2, NotFound = 3};

		struct KategorieItems{
			QString Kategorie;
			QString Beschreibung;
			quint32 Typ;
		};

		typedef QMap<quint32, KategorieItems> MapKategorie;

	public:
		EntryKategorie();
		EntryKategorie(QString file);
		~EntryKategorie();

		quint32 load();
		quint32 load(QString file);
		quint32 save();
		
		quint32 setFile(QString file);
		quint32 getFreeNumber();

		quint32 addKategorie(QString kategorie, QString beschreibung, quint32 typ);
		quint32 deleteKategorie(quint32 nummer);
		
		QString getKategorie(quint32 nummer);
		QString getBeschreibung(quint32 nummer);
		quint32 getTyp(quint32 nummer);

		quint32 changeKategorie(quint32 nummer, QString newKategorie);
		quint32 changeBeschreibung(quint32 nummer, QString newBeschreibung);
		quint32 changeTyp(quint32 nummer, quint32 newTyp);

	private:
		bool isChanged;
		QString Filename;
		MapKategorie Kategorien;
		
};
#endif
