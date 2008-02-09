#include "repeat_entry.h"

#include <QDate>
#include <QObject>
#include "KontoEntry.h"

RepeatEntry::RepeatEntry ( KontoEntry entry, QDate startdate, quint32 repeat, quint32 repeatTyp, quint32 step = 1, quint32 count = 0)
/******************************************************************************
* Konstruktor
*******************************************************************************/
{
	if ( startdate > QDate::currentDate() ) {
		return;
	}

	if ( count >= repeat ) {
		return;
	}

	Entry = entry;

	Next = startdate;
	Repeat = repeat;
	RepeatTyp = repeatTyp;
	Step = step;
	Count = count;
}


bool RepeatEntry::hasActualEntry()
/******************************************************************************
* Methode gibt zurueck, ob ein Eintrag aktuell ist
*******************************************************************************/
{
	QDate today = QDate::currentDate();

	if ( today >= Next ) {
		return true;

	} else {
		return false;
	}
}


bool RepeatEntry::hasNextEntry()
/******************************************************************************
* Methode gibt zurueck, ob Eintrag noch aktuell ist
*******************************************************************************/
{
	QDate today = QDate::currentDate();

	if ( today <= Next ) {
		return true;
	}

	if ( Repeat == Infinit ) {
		return true;
	}

	if ( Count + 1 <= Repeat ) {
		return true;
	}

	return false;
}


void RepeatEntry::calcNext()
/******************************************************************************
* Methode berechnet das naechste Datum
*******************************************************************************/
{
	switch ( RepeatTyp ) {

		case EveryDay:
			Next = Next.addDays ( Step );
			break;

		case EveryWeek:
			Next = Next.addDays ( Step * 7 );
			break;

		case EveryMonth:
			Next = Next.addMonths ( Step );
			break;

		case EveryYear:
			Next = Next.addYears ( Step );
			break;

		default:
			break;
	}
}


KontoEntry RepeatEntry::getEntry()
/******************************************************************************
* Methode gibt den Eintrag zurueck (inklusive Variablen)
*******************************************************************************/
{
	return Entry;
}


KontoEntry RepeatEntry::getEntryParsed()
/******************************************************************************
* Methode gibt den Eintrag geparsed zurueck (Variablen ersetzt)
*******************************************************************************/
{
	KontoEntry tempEntry = Entry;
	QString tempVerwendung = Entry.getVerwendung();
	tempVerwendung = parse( tempVerwendung );
	tempEntry.changeVerwendung(tempVerwendung);
	
	return tempEntry;
}


QString RepeatEntry::parse ( QString text )
/******************************************************************************
* Methode parsed den String text (erstetzt Variablen)
*******************************************************************************/
{
	text.replace ( QObject::tr ( "%DAY%" ),
				   QString::number ( Next.day() ),
				   Qt::CaseInsensitive
				 );
	text.replace ( QObject::tr ( "%MONTH%" ),
				   QString::number ( Next.month() ),
				   Qt::CaseInsensitive
				 );
	text.replace ( QObject::tr ( "%YEAR%" ),
				   QString::number( Next.year() ),
				   Qt::CaseInsensitive
				 );
	text.replace ( QObject::tr ( "%COUNT%" ),
				   QString::number( Count ),
				   Qt::CaseInsensitive
				 );
	text.replace ( QObject::tr ( "%REPEAT%" ),
				   QString::number( Repeat ),
				   Qt::CaseInsensitive
				 );

	return text;
}

