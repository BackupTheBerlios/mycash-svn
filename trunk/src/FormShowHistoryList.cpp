#include "FormShowHistoryList.h"

#include <QWidget>
#include <QTableWidget>

FormShowHistoryList::FormShowHistoryList( QWidget *parent):QWidget( parent )
/******************************************************************************
* Konstruktor fuer FormShowHistoryList
*******************************************************************************/
{
	setupUi( this );
}


void FormShowHistoryList::update(Konto::VectorHistoryList& list) //SLOT
/******************************************************************************
* Methode aktualisiert die Tabelle
*******************************************************************************/
{
	Konto::VectorHistoryList::iterator it;

	//Tabelle leeren
	tableList -> clear();
	
	quint32 row = 0;
	for(it = list.begin(); it != list.end(); it++){
		QTableWidgetItem *itemDate = new QTableWidgetItem( it -> Datum );
		QTableWidgetItem *itemVerwendung = new QTableWidgetItem( it -> Verwendung );
		QTableWidgetItem *itemTransfer = new QTableWidgetItem( QString::number( it -> Transfer ) );
		QTableWidgetItem *itemBetrag = new QTableWidgetItem( QString::number( it -> Betrag ) );
		
		tableList -> setItem(row, 0, itemDate);
		tableList -> setItem(row, 1, itemVerwendung);
		tableList -> setItem(row, 2, itemTransfer);
		tableList -> setItem(row, 3, itemBetrag);

		row++; 
	}
}
