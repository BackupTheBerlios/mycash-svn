/**
* @file FormShowHistoryListDetails.cpp
*
* @brief Datei enthält die Definitionen der Klasse FormShowHistoryListDetails
*/
#include "FormShowHistoryListDetails.h"
#include "konto_splitt.h"
#include "ListDetailsDelegate.h"
#include <QMessageBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QDate>

/**
* @brief Konstruktor des Formulars
*
* @param *parent Adresse der Elternklasse
*/
FormShowHistoryListDetails::FormShowHistoryListDetails ( QWidget *parent ) : QWidget ( parent )
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


/**
* @brief Methode löscht Eingabefelder und beendet das Fenster
*/
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
			
			//QTableWidgetItem *ItemSteuer		= 	new QTableWidgetItem( QString::number( (it -> getSteuerrelevanz())?1:0 ) );
			QTableWidgetItem *item = new QTableWidgetItem(/* tr( "Checkbox" ) */);
			//item -> setFlags( Qt::ItemIsUserCheckable );
			item -> setCheckState( (it -> getSteuerrelevanz())?Qt::Checked:Qt::Unchecked );


			tableSplitt -> insertRow(row);
			tableSplitt -> setItem(row, VERWENDUNG, ItemVerwendung);
			tableSplitt -> setItem(row, KATEGORIE, ItemKategorie);
			tableSplitt -> setItem(row, BETRAG, ItemBetrag);
			tableSplitt -> setItem(row, STEUER, /*ItemSteuer*/item);
			row++; 
		}
	}
}

/**
* @brief Methode wird ausgeführt, wenn ButtonOk geklickt wird
*/
void FormShowHistoryListDetails::buttonOkClicked( void )
{
	//ToDo:  Ueberpruefen, ob alle wichtigen Felder ausgefüllt

	Konto::HistoryListDetails tempStruct;
	tempStruct.Entry = EntryNumber;
	tempStruct.Datum = (dateEdit -> date()).toString("yyyyMMdd");
	tempStruct.Shop = 0; //An ComboBox anpassen
	tempStruct.Verwendung = lineUsed -> text();
	tempStruct.Transfer = 0; // An ComboBox anpassen
	tempStruct.Betrag = (linePay -> text()).toFloat(0);

	Konto::VectorSplitt splitts;
	quint32 anzahlRow = tableSplitt -> rowCount();
	for(quint32 i = 0; i < anzahlRow; i++){
		QString Verwendung = (tableSplitt -> item(i, VERWENDUNG)) -> text();
		quint32 Kategorie = 0; //ComboBox auswerten
		float Betrag = ((tableSplitt -> item(i, BETRAG)) -> text()).toFloat(0);
		bool Steuer = ((tableSplitt -> item(i, STEUER)) -> checkState() == Qt::Checked)?true:false;
		
		if( Verwendung != "" && Betrag != 0.0 ){
			KontoSplitt tempSplitt(Kategorie, Verwendung, Betrag, Steuer);
			splitts.push_back( tempSplitt );
		}
	}
	tempStruct.Splitts = splitts;
	
	emit sendData( tempStruct );
}
