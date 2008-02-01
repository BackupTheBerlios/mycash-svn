#include <QtGui>
#include <QMessageBox>
#include <QString>
#include <QTextStream>
#include "neueskontodialog.h"

NeuesKontoDialog::NeuesKontoDialog(QWidget *parent) : QDialog(parent){
	setupUi(this);
	connect(pushAdd, SIGNAL( clicked() ), this, SLOT( clickAdd() ));

	QRegExp regExp("[1-9][0-9]{0,7}");
	lineBLZ -> setValidator(new  QRegExpValidator(regExp, this));
	
}


void NeuesKontoDialog::clickAdd(){
	//QWidget neu;
	QMessageBox::warning(0,"Titel","Message",QMessageBox::Ok);
	//QTextStream console(stdout);
	//console << "clickAdd()" << "\n\r";
}
