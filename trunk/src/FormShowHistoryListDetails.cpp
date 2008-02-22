#include "FormShowHistoryListDetails.h"
#include <QMessageBox>

FormShowHistoryListDetails::FormShowHistoryListDetails(QWidget *parent):QWidget(parent)
/******************************************************************************
* Konstruktor
********************************************************************************/
{
	setupUi(this);
	connect( //
		pushDelete,
		SIGNAL( clicked() ),
		this,
		SLOT( getDelete() )
	);
}

void FormShowHistoryListDetails::getDelete()
/******************************************************************************
* Methode ruft ein Dialog auf, ob Eintrag geloescht werden soll
*******************************************************************************/
{
	int mboxvalue = QMessageBox::warning(
		this,
		"Delete Entry",
		"Are you sure that you want to delete this entry?",
		QMessageBox::Ok,
		QMessageBox::No | QMessageBox::Default | QMessageBox::Escape
	);
	if( mboxvalue == QMessageBox::Ok ){
		emit doDelete( EntryNumber );
		clearAndClose();
	}
}


void FormShowHistoryListDetails::clearAndClose()
/******************************************************************************
* Methode loescht die Eingabefelder und beendet das Fenster
*******************************************************************************/
{
	close();
}

void FormShowHistoryListDetails::getData()
/******************************************************************************
* Methode nimmt Daten entgegen
*******************************************************************************/
{

}
