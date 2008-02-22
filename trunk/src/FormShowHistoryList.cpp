#include "FormShowHistoryList.h"

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
	tableList -> clear();

	//Header setzten
	tableList -> setHorizontalHeaderLabels ( QStringList() << tr ( "date" ) << tr ( "used for" ) << tr ( "transfer" ) <<  tr ( "pay" ) );

	// Tabelle schreibschuetzen
	tableList -> setEditTriggers ( QAbstractItemView::NoEditTriggers );


	quint32 row = 0;

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
