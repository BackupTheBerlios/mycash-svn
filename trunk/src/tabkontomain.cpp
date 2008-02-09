#include <QWidget>
#include <QTextStream>
#include <QMessageBox>

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

	connect( buttonSettings,
			SIGNAL ( clicked() ),
			this,
			SLOT (showKontoSettingsDialog() )
			);

	//Formulurvariablen initialisieren
	AddEntryPointer = 0;
	KontoSettingsPointer = 0;

}


TabKontoMain::~TabKontoMain()
/******************************************************************************
* Destruktor entlaed benutzte Formulare
*******************************************************************************/
{
	if( AddEntryPointer){
		delete AddEntryPointer;
	}
	if(KontoSettingsPointer){
		delete KontoSettingsPointer;
	}
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


void TabKontoMain::clickSettings() //SLOT
/******************************************************************************
* Methode wird aufgerufen, wenn buttonSettings das Signal clicked gibt
*******************************************************************************/
{
	showKontoSettingsDialog();
}


void TabKontoMain::showKontoSettingsDialog()
/******************************************************************************
* Methode zeigt das KontoSettingsDialog an
*******************************************************************************/
{
	if ( KontoSettingsPointer == 0 ){
		KontoSettingsPointer = new KontoSettings(this);

		connect( this,
				SIGNAL ( updateSettingsDialog(Konto::KontoSettings) ),
				KontoSettingsPointer,
				SLOT ( update(Konto::KontoSettings) )
				);

		connect( KontoSettingsPointer,
				SIGNAL( updateKonto(const Konto::KontoSettings&) ),
				this,
				SLOT( fromKontoSettings( const Konto::KontoSettings&) )
				);
	}

	emit updateSettingsDialog( KontoPointer -> getKontoSettings() );

	KontoSettingsPointer -> setWindowFlags(Qt::Dialog);
	KontoSettingsPointer -> setEnabled (true);
	KontoSettingsPointer -> show();
}


void TabKontoMain::fromKontoSettings(const Konto::KontoSettings& settings) // SLOT
/******************************************************************************
* Methode aktualiesert die Kontoeinstellungen
* erhaelt das Signal und Daten von KontoSettings
*******************************************************************************/
{
	KontoPointer -> setKontoFile( settings.FileName);
	KontoPointer -> setKontoName( settings.KontoName );
	KontoPointer -> setKontoBeschreibung( settings.KontoBeschreibung );
	KontoPointer -> setBLZ( settings.BLZ );
	KontoPointer -> setBankName( settings.BankName );
	KontoPointer -> setKontoTyp( settings.KontoTyp );
	KontoPointer -> setLimitNegativ( settings.Limit );
	KontoPointer -> setCanBeNegativ( (int)settings.canBeNegativ ); 
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


