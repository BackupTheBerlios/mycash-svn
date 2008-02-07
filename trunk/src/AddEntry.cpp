#include <QWidget>

#include "AddEntry.h"

AddEntry::AddEntry ( QWidget *parent = 0 ) : QWidget ( parent )
/******************************************************************************
* Konstruktor
*******************************************************************************/
{
	setupUi ( this );

	connect ( pushAdd,
			  SIGNAL ( click() ),
			  this,
			  SLOT ( clickAdd() )
			);

	connect ( pushMore,
			  SIGNAL ( click() ),
			  this,
			  SLOT ( clickMore() )
			);
}

void AddEntry::clickAdd()
/******************************************************************************
* Methode wird ausgefuehrt, wenn pushAdd geklickt wird
*******************************************************************************/
{
	KontoEntry *tempEntry = new KontoEntry();
	emit add ( tempEntry );
	delete tempEntry;
}


void AddEntry::clickMore()
/******************************************************************************
* Methode wird ausgefuehrt, wenn pushMore geklickt wird
*******************************************************************************/
{

}
