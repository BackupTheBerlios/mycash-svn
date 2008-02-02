#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QDialog> //new
#include <QMessageBox>
//#include "mainwindow.h"
#include "configs.h"
#include "class_konto_typ.h"

//Konto_Typ GLOBAL::*MCKT; //MyCash-KontoTyp

#include "ui_mainwindow1.h"
#include "mainwindow1.h"


int main(int argc, char *argv[]){
	
	//Load Qt
	QApplication app(argc,argv);
	
	QTranslator appTranslator;
	appTranslator.load((QString)argv[0] + "_" + QLocale::system().name(), qApp->applicationDirPath());
	app.installTranslator(&appTranslator);

	MainWindow1 ui;
	ui.show();
	
	//Delete Kontotypen
	
	//Beende Qt
	return app.exec();
}
