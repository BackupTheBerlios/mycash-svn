#include <QtGui>
#include <QMessageBox>
#include <QString>
#include <QTextStream>
#include <QLayout>
#include "neueskontodialog.h"
#include "konto.h"

NeuesKontoDialog::NeuesKontoDialog(QWidget *parent) : QDialog(parent){
	setupUi(this);
	connect(pushAdd, SIGNAL( clicked() ), this, SLOT( clickAdd() ));

	QRegExp regExp("[1-9][0-9]{0,7}");
	lineBLZ -> setValidator(new  QRegExpValidator(regExp, this));

	//QLayout::setSizeConstraint(QLayout::SetFixedSize);
	setFixedSize(width(), height());
}


void NeuesKontoDialog::clickAdd(){
	//Fehleranalyse ... Einagbewerte checken...
	Konto *tempKonto = new Konto(lineKontoName -> text(), lineKontoBeschreibung -> text(), lineBLZ -> text(), lineBankName -> text(), 0);
	emit add(tempKonto);
	delete tempKonto;
	clearLines();
	close();
}


void NeuesKontoDialog::clearLines(){
	lineKontoName -> setText("");
	lineKontoBeschreibung -> setText("");
	lineBLZ -> setText("");
	lineBankName -> setText("");
}
