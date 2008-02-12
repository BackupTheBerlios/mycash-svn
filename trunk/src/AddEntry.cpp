#include <QWidget>
#include <QCompleter>

#include "AddEntry.h"

FormAddEntry::FormAddEntry ( QWidget *parent = 0 ) : QWidget ( parent )
/******************************************************************************
* Konstruktor
*******************************************************************************/
{
	setupUi ( this );

	//Verbindungen erstellen
	connect ( pushAdd,
			  SIGNAL ( clicked() ),
			  this,
			  SLOT ( clickAdd() )
			);

	connect ( pushMore,
			  SIGNAL ( clicked() ),
			  this,
			  SLOT ( clickMore() )
			);

	//Datum einstellen
	date -> setDate( QDate::currentDate() );

	//Autovervollstaendigung aktivieren
	/*QStringList list;
	list << "Hallo" << "Walter" << "Duden" << "Arschloch";
	QCompleter *comp = new QCompleter(list, this);
	comp -> setCompletionMode( QCompleter::PopupCompletion );
	comp -> setModelSorting( QCompleter::CaseInsensitivelySortedModel );
	comboTransfer -> setCompleter( comp );
	*/
}

void FormAddEntry::clickAdd()
/******************************************************************************
* Methode wird ausgefuehrt, wenn pushAdd geklickt wird
*******************************************************************************/
{
	KontoEntry *tempEntry = new KontoEntry();
	emit add ( tempEntry );
	delete tempEntry;
}


void FormAddEntry::clickMore()
/******************************************************************************
* Methode wird ausgefuehrt, wenn pushMore geklickt wird
*******************************************************************************/
{
	//Ueberpruefe, ob letzte Spalte mit Kategorie und Betrag ausgefuellt -> neue Zeile hinzufuegen
}
