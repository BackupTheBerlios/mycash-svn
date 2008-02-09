#include <QtGui>
#include <QMessageBox>
#include <QString>
#include <QTextStream>
#include <QLayout>
#include "neueskontodialog.h"
#include "konto.h"

NeuesKontoDialog::NeuesKontoDialog ( QWidget *parent ) : QDialog ( parent )
		/******************************************************************************
		* Konstruktor fuer NeuesKontoDialog
		* Connections erzeugen, Eingabevalidatoren bestimmen, Groesse festlegen
		*******************************************************************************/
{
	setupUi ( this );
	connect ( pushAdd, SIGNAL ( clicked() ), this, SLOT ( clickAdd() ) );

	QRegExp regExp ( "[1-9][0-9]{0,7}" );
	lineBLZ -> setValidator ( new  QRegExpValidator ( regExp, this ) );

	//QLayout::setSizeConstraint(QLayout::SetFixedSize);
	setFixedSize ( width(), height() );
}


void NeuesKontoDialog::clickAdd()
/******************************************************************************
* Methode bereitet Eingabefelder fuer weitergabe an MainWindow1 auf
*******************************************************************************/
{
	//ToDo: Fehleranalyse ... Einagbewerte checken...
	Konto *tempKonto = new Konto ( lineKontoName -> text(),
								   lineKontoBeschreibung -> text(),
								   lineBLZ -> text(),
								   lineBankName -> text(),
								   0, //KontoTyp
								   spinboxLimit -> value(),
								   checkBeUnderLimit -> checkState()
								 );
	emit add ( tempKonto );
	// tempKonto wird in ~MainWindow1 geloescht
	clearLines();
	close();
}


void NeuesKontoDialog::clearLines()
/******************************************************************************
* Methode loescht alle Eingabezeilen
*******************************************************************************/
{
	lineKontoName -> setText ( "" );
	lineKontoBeschreibung -> setText ( "" );
	lineBLZ -> setText ( "" );
	lineBankName -> setText ( "" );
	spinboxLimit -> setValue(0.0);
	checkBeUnderLimit -> setCheckState( Qt::Unchecked );
}
