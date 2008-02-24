#include "FormShowHistoryList.h"
#include "FormShowHistoryListDetails.h"

#include <QWidget>
#include <QTableWidget>
#include <QVector>
#include <QTableWidgetSelectionRange>
#include <QMessageBox>

#include "konto.h"

FormShowHistoryList::FormShowHistoryList ( QWidget *parent ) : QWidget ( parent )
		/******************************************************************************
		* Konstruktor fuer FormShowHistoryList
		*******************************************************************************/
{
	setupUi ( this );
	tableList -> setSelectionMode( QAbstractItemView::SingleSelection );

	connect ( // Druecken auf eine Zelle
		tableList,
		SIGNAL ( cellActivated ( int, int ) ),
		this,
		SLOT ( selectRow ( int, int ) )
	);
	connect(
		tableList,
		SIGNAL( cellPressed(int, int) ),
		this,
		SLOT( selectRow( int, int ) )
	);
	connect(
		tableList,
		SIGNAL( cellDoubleClicked(int, int) ),
		this,
		SLOT( showDetails(int, int) )
	);

	//Item-Settings
	tableList -> setEditTriggers ( QAbstractItemView::NoEditTriggers ); // Tabelle schreibschuetzen

	//Formulare initialisieren
	PointerFormDetails = 0;
}


FormShowHistoryList::~FormShowHistoryList()
/******************************************************************************
* Dekonstruktor
*******************************************************************************/
{
	if( PointerFormDetails ){
		delete PointerFormDetails;
	}
}


void FormShowHistoryList::showDetails(int row, int column)
/******************************************************************************
* Methode zeigt Details eines Eintrages an
*******************************************************************************/
{
	column = column; //wegen GCC
	if(! PointerFormDetails){
		PointerFormDetails = new FormShowHistoryListDetails(this);
		connect(
			this,
			SIGNAL( DoUpdateDetails(Konto::HistoryListDetails) ),
			PointerFormDetails,
			SLOT( getData(Konto::HistoryListDetails) )
		);
		
		PointerFormDetails -> setWindowFlags ( Qt::Dialog );
	}
	emit GetUpdateDetails( row + 1);
	PointerFormDetails -> show();
}


/*void FormShowHistoryList::debugSel(){
	QList<QTableWidgetSelectionRange> liste = tableList -> selectedRanges();
	QMessageBox::warning (
		this,
		"Range",
		"Top: " +   QString::number ( liste.first().topRow() ) +
		"\nLeft :" +  QString::number ( liste.first().leftColumn() ) +
		"\nBottom: " +  QString::number ( liste.first().bottomRow() ) +
		"\nRight: " +  QString::number ( liste.first().rightColumn() )
	);
}*/


void FormShowHistoryList::updateTable ( const Konto::VectorHistoryList& liste ) //SLOT
/******************************************************************************
* Methode aktualisiert die Tabelle
*******************************************************************************/
{
	Konto::VectorHistoryList::iterator it;
	Konto::VectorHistoryList list = liste;
	
	//Tabelle leeren
	quint32 row = tableList -> rowCount(); 
	while(row > 0){
		tableList -> removeRow( row - 1 );
		row--;
	}


	//durch while-Schleife -> row := 0:
	for ( it = list.begin(); it != list.end(); it++ ) {
		tableList -> insertRow ( row );
		QTableWidgetItem *itemDate = new QTableWidgetItem ( it -> Datum );
		QTableWidgetItem *itemVerwendung = new QTableWidgetItem ( it -> Verwendung );
		QTableWidgetItem *itemTransfer = new QTableWidgetItem ( QString::number ( it -> Transfer ) );
		QTableWidgetItem *itemBetrag = new QTableWidgetItem ( QString::number ( it -> Betrag ) );

		tableList -> setItem ( row, 0, itemDate );
		tableList -> setItem ( row, 1, itemVerwendung );
		tableList -> setItem ( row, 2, itemTransfer );
		tableList -> setItem ( row, 3, itemBetrag );

		row++;
	}
}


void FormShowHistoryList::selectRow ( int row, int column )
/******************************************************************************
* Methode markiert die Zeile in der eine Zelle angeklickt wurde
*******************************************************************************/
{
	column = column; //gegen wwarnung von gcc; eventuell pragma Anweisung
	tableList -> setRangeSelected (
		/*
			top := row
			left := column
			bottom := height(Range) + top - 1
			right := width(Range) + left - 1
		*/
		QTableWidgetSelectionRange (
			row, //top
			0, //left
			row, //bottom
			tableList -> columnCount() - 1 // right
		),
		true
	);
}


void FormShowHistoryList::deleteEntry(const quint32& entry)
/******************************************************************************
* Methode leitet den zu loeschenden Eintrag weiter
*******************************************************************************/
{
	emit SigDeleteEntry( entry );
}

void FormShowHistoryList::updateDetails(const Konto::HistoryListDetails& details)
/******************************************************************************
* Methode leitet die Werte fuer die Deatails weiter
*******************************************************************************/
{
	emit DoUpdateDetails( details );
}
