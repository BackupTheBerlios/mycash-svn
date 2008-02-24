#include <QWidget>
#include <QTextStream>
#include <QMessageBox>
#include <QVector>

#include "tabkontomain.h"
#include "konto.h"
#include "FormShowHistoryList.h"

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

	connect (
		buttonFormAddEntry,
		SIGNAL ( clicked() ),
		this,
		SLOT ( showFormAddEntryDialog() )
	);

	connect (
		buttonSettings,
		SIGNAL ( clicked() ),
		this,
		SLOT ( clickSettings() )
	);

	connect (
		buttonShowHistoryList,
		SIGNAL ( clicked() ),
		this,
		SLOT ( clickHistoryList() )
	);

	//Formulurvariablen initialisieren
	FormAddEntryPointer = 0;
	FormKontoSettingsPointer = 0;
	FormShowHistoryListPointer = 0;

}


TabKontoMain::~TabKontoMain()
/******************************************************************************
* Destruktor entlaed benutzte Formulare
*******************************************************************************/
{
	if ( FormAddEntryPointer ) {
		delete FormAddEntryPointer;
	}

	if ( FormKontoSettingsPointer ) {
		delete FormKontoSettingsPointer;
	}

	if ( FormShowHistoryListPointer ) {
		delete FormShowHistoryListPointer;
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
* Methode wird aufgerufen, wenn buttonSettings das Signal clicked() gibt
*******************************************************************************/
{
	showFormKontoSettingsDialog();
}


void TabKontoMain::clickHistoryList()
/******************************************************************************
* Methode wird aufgerufen, wenn buttonShowHistoryList das Signal clicked() gibt
*******************************************************************************/
{
	showHistoryListDialog();
}


void TabKontoMain::showFormKontoSettingsDialog()
/******************************************************************************
* Methode zeigt das FormKontoSettingsDialog an
*******************************************************************************/
{
	if ( FormKontoSettingsPointer == 0 ) {
		FormKontoSettingsPointer = new FormKontoSettings ( this );

		connect ( this,
				  SIGNAL ( updateSettingsDialog ( Konto::KontoSettings ) ),
				  FormKontoSettingsPointer,
				  SLOT ( update ( Konto::KontoSettings ) )
				);

		connect ( FormKontoSettingsPointer,
				  SIGNAL ( updateKonto ( const Konto::KontoSettings& ) ),
				  this,
				  SLOT ( fromFormKontoSettings ( const Konto::KontoSettings& ) )
				);
	}

	emit updateSettingsDialog ( KontoPointer -> getFormKontoSettings() );

	FormKontoSettingsPointer -> setWindowFlags ( Qt::Dialog );
	FormKontoSettingsPointer -> setEnabled ( true );
	FormKontoSettingsPointer -> show();
}


void TabKontoMain::fromFormKontoSettings ( const Konto::KontoSettings& settings ) // SLOT
/******************************************************************************
* Methode aktualiesert die Kontoeinstellungen
* erhaelt das Signal und Daten von FormKontoSettings
*******************************************************************************/
{
	KontoPointer -> setKontoFile ( settings.FileName );
	KontoPointer -> setKontoName ( settings.KontoName );
	KontoPointer -> setKontoBeschreibung ( settings.KontoBeschreibung );
	KontoPointer -> setBLZ ( settings.BLZ );
	KontoPointer -> setBankName ( settings.BankName );
	KontoPointer -> setKontoTyp ( settings.KontoTyp );
	KontoPointer -> setLimitNegativ ( settings.Limit );
	KontoPointer -> setCanBeNegativ ( ( int ) settings.canBeNegativ );
}


void TabKontoMain::showFormAddEntryDialog() // SLOT
/******************************************************************************
* Methode laed den FormAddEntry Dialog
*******************************************************************************/
{
	if ( FormAddEntryPointer == 0 ) {
		FormAddEntryPointer = new FormAddEntry ( this );
	}

	//FormAddEntryPointer -> activateWindow();
	FormAddEntryPointer -> setWindowFlags ( Qt::Dialog );

	FormAddEntryPointer -> setEnabled ( true );

	FormAddEntryPointer -> show();
}


void TabKontoMain::showHistoryListDialog()
/******************************************************************************
* Methode laed den FormShowHistoryList Dialog
*******************************************************************************/
{
	if ( FormShowHistoryListPointer == 0 ) {
		FormShowHistoryListPointer = new FormShowHistoryList ( this );
		connect (
			this,
			SIGNAL ( updateHistoryListDialog ( Konto::VectorHistoryList ) ),
			FormShowHistoryListPointer,
			SLOT ( updateTable ( Konto::VectorHistoryList ) )
		);
		connect (
			FormShowHistoryListPointer,
			SIGNAL ( SigDeleteEntry ( quint32 ) ),
			this,
			SLOT ( deleteEntry ( quint32 ) )
		);
		connect(
			FormShowHistoryListPointer,
			SIGNAL( GetUpdateDetails(quint32) ),
			this,
			SLOT( updateEntryDetails(quint32) )
		);
		connect(
			this,
			SIGNAL( updateHistoryListDetailsDialog( Konto::HistoryListDetails ) ),
			FormShowHistoryListPointer,
			SLOT( updateDetails( Konto::HistoryListDetails ) )
		);

		FormShowHistoryListPointer -> setWindowFlags ( Qt::Dialog );
		FormShowHistoryListPointer -> setEnabled ( true );
	}

	updateTableHistory();

	FormShowHistoryListPointer -> show();
}


void TabKontoMain::deleteEntry ( quint32 entrynummer )
/******************************************************************************
* Methode loescht ein Kontoeintrag
*******************************************************************************/
{
	KontoPointer -> deleteEntry ( entrynummer );
	updateTableHistory();
}


void TabKontoMain::updateEntryDetails ( quint32 entrynummer )
/******************************************************************************
* Methode gibt detaillierte Informationen zu einen Eintrag zurueck
*******************************************************************************/
{
	emit updateHistoryListDetailsDialog ( KontoPointer -> getEntryDetails ( entrynummer ) );
}


void TabKontoMain::updateTableHistory( void )
/******************************************************************************
* Methode aktualisiert die HistoryListTable
*******************************************************************************/
{
	emit updateHistoryListDialog ( KontoPointer -> getHistoryList() );
}
