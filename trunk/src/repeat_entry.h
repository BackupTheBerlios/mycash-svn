#ifndef REPEAT_ENTRY_H
#define REPEAT_ENTRY_H

#include <QString>
#include <QDate>
#include <QDomElement>
#include <QDomDocument>

#include "KontoEntry.h"

class RepeatEntry{
	enum RepeatType{EveryDay = 0, EveryWeek = 1, EveryMonth = 2, EveryYear = 3};
	enum RepeatTime{Infinit = 0xFFFFFFFF};

	public:
		RepeatEntry(KontoEntry entry, QDate startdate, quint32 repeat, quint32 repeatTyp, quint32 step, quint32 count);
		//operator bool();
		bool hasActualEntry();
		bool hasNextEntry();
		KontoEntry getEntry();
		KontoEntry getEntryParsed();
		/*QString getVerwendung();
		QString getVerwendungParse();
		KontoEntry getEntry(*/
		QDomElement getXmlElement(QDomDocument &doc);

	private:
		void calcNext();
		QString parse(QString text);


		QDate Next;
		//Repeat-Settings
		quint32 Count;
		quint32 Repeat;
		quint32 RepeatTyp;
		quint32 Step;

		KontoEntry Entry;
};

#endif
