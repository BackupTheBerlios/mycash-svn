#include <QWidget>
#include <QTextStream>

#include "tabkontomain.h"
#include "konto.h"

TabKontoMain::TabKontoMain ( QWidget *parent = 0, Konto * connectedKonto = 0 ) : QWidget ( parent )
		/******************************************************************************
		* Konstruktor mit parent und Konto
		*******************************************************************************/
{
	setupUi ( this );
	KontoPointer = connectedKonto;

	/*connect ( buttonClose,
	    SIGNAL ( clicked() ),
	    this,
	    SLOT ( clickClose() )
	  ); */

	/*connect ( buttonDelete,
	    SIGNAL ( clicked() ),
	    this,
	    SLOT ( clickDelete() )
	  );*/

	connect ( buttonAddEntry,
			  SIGNAL ( clicked() ),
			  this,
			  SLOT( showAddEntryDialog() )
			);
	//Formulurvariablen initialisieren
	AddEntryPointer = 0;

}


void TabKontoMain::clickClose() //SLOT
/******************************************************************************
* Methode sendet das Signal zum Beenden diesen Tabs
*******************************************************************************/
{
	emit close ( this );
}


void TabKontoMain::clickDelete() //SLOT
/******************************************************************************
* Methode sendet das Signal zum loeschen dieses Kontos
*******************************************************************************/
{
	if ( KontoPointer != 0 ) {
		emit deleteKonto ( KontoPointer );
	}
}


void TabKontoMain::showAddEntryDialog() // SLOT
/******************************************************************************
* Methode laed den AddEntryDialog
*******************************************************************************/
{
	if( AddEntryPointer == 0 ){
		AddEntryPointer = new AddEntry(this);
	}
	//AddEntryPointer -> activateWindow();
	AddEntryPointer -> setWindowFlags(Qt::Dialog);
	AddEntryPointer -> setEnabled( true );
	AddEntryPointer -> show();
}


