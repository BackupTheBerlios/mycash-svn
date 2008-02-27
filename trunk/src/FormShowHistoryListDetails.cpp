#include "FormShowHistoryListDetails.h"
#include "konto_splitt.h"
#include "ListDetailsDelegate.h"
#include <QMessageBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QDate>

FormShowHistoryListDetails::FormShowHistoryListDetails ( QWidget *parent ) : QWidget ( parent )
		/******************************************************************************
		* Konstruktor
		********************************************************************************/
{
	setupUi ( this );
	connect ( //
		pushDelete,
		SIGNAL ( clicked() ),
		this,
		SLOT ( getDelete() )
	);
	//tableSplitt -> setItemDelegate( new ListDetailsDelegate(-1, 3, -1) );
	dateEdit -> setDisplayFormat( tr("yyyy-MM-dd") );
}

void FormShowHistoryListDetails::getDelete()
/******************************************************************************
* Methode ruft ein Dialog auf, ob Eintrag geloescht werden soll
*******************************************************************************/
{
	int mboxvalue = QMessageBox::warning (
						this,
						"Delete Entry",
						"Are you sure that you want to delete this entry?",
						QMessageBox::Ok,
						QMessageBox::No | QMessageBox::Default | QMessageBox::Escape
					);

	if ( mboxvalue == QMessageBox::Ok ) {
		emit doDelete ( EntryNumber );
		clearAndClose();
	}
}


void FormShowHistoryListDetails::clearAndClose ( void )
/******************************************************************************
* Methode loescht die Eingabefelder und beendet das Fenster
*******************************************************************************/
{
	clear();
	close();
}


void FormShowHistoryListDetails::clear ( void )
/******************************************************************************
* Methode loescht die Eingabefelder
*******************************************************************************/
{
	dateEdit -> setDate( QDate::currentDate() );
	lineUsed -> setText("");
	comboTransfer -> clear();
	linePay -> setText("");
	
	//Tabelle löschen
	quint32 row = tableSplitt -> rowCount();
	while( row > 0 ){
		tableSplitt -> removeRow( row - 1 );
		row--;
	}
}

void FormShowHistoryListDetails::getData ( const Konto::HistoryListDetails& details )
/******************************************************************************
* Methode nimmt Daten entgegen
*******************************************************************************/
{
	clear();

	if ( details.Entry == 0 ) {
		QMessageBox::warning (
			this,
			tr ( "Foult Entry" ),
			tr ( "You have a foult Entry selected" )
		);
		close();
	}else{
		//Werte eintragen
		//lineDate -> setText( details.Datum );
		dateEdit -> setDate( QDate::fromString(details.Datum, "yyyyMMdd") );
		lineUsed -> setText( details.Verwendung );
		linePay -> setText( QString::number(details.Betrag));
		Konto::VectorSplitt::Iterator it;
		quint32 row = 0;
		Konto::VectorSplitt tempV = details.Splitts;
		for(it = tempV.begin(); it != tempV.end(); it++){
			QTableWidgetItem *ItemVerwendung	=	new QTableWidgetItem( it -> getVerwendung() );
			QTableWidgetItem *ItemKategorie		= 	new QTableWidgetItem( it -> getKategorie() );
			QTableWidgetItem *ItemBetrag		=	new QTableWidgetItem( QString::number(it -> getBetrag()) );
			QTableWidgetItem *ItemSteuer		= 	new QTableWidgetItem( QString::number( (it -> getSteuerrelevanz())?1:0 ) );

			tableSplitt -> insertRow(row);
			tableSplitt -> setItem(row, 0, ItemVerwendung);
			tableSplitt -> setItem(row, 1, ItemKategorie);
			tableSplitt -> setItem(row, 2, ItemBetrag);
			tableSplitt -> setItem(row, 3, ItemSteuer);
			row++; 
		}
	}
}
