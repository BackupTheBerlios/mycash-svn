//$Date$

#include <QtGui>
#include <QTextStream>
#include <QObject>

#include "mainwindow1.h"
#include "configs.h"
#include "neueskontodialog.h"

MainWindow1::MainWindow1(QWidget *parent) : QMainWindow(parent){
	setupUi(this);

	connect(actionQt,SIGNAL( triggered() ), qApp, SLOT( aboutQt() ));
	connect(actionNeu, SIGNAL( triggered() ), this, SLOT( newFile() ));
	connect(buttonNeuKonto, SIGNAL( clicked() ), this, SLOT( showNeuesKontoDialog() ));

	// Initialisiere Formulare
	neuesKontoDialog = 0;
}


MainWindow1::~MainWindow1(){
	//delete Formulare
	if(neuesKontoDialog){
		delete neuesKontoDialog; 
	}
}


bool MainWindow1::addKonto(Konto *konto){
	Konto k = *konto;
	return true;
}


void MainWindow1::closeEvent(QCloseEvent *event){
	if( okToContinue() ){
		event -> accept();
	}else{
		event -> ignore();
	}
}


/*void MainWindow1::SetIsChanged(){
	IsChanged = true;
}


void MainWindow1::SetIsNotChanged(){
	IsChanged = false;
}*/


bool MainWindow1::newFile(){
	if( okToContinue() ){
		return clear();	
	}
	return false;
}


bool MainWindow1::clear(){
	File = QString();
	setWindowModified(false);

	return true;
}


bool MainWindow1::save(){
	if(! File.isEmpty() ){
		return saveFile(File);
	}else{
		return saveAs();
	}
}
bool MainWindow1::saveAs(){
	QString filename = QFileDialog::getSaveFileName(this, tr("Save Project"), ".", tr("%1 projects (*.mc)").arg(AppName));
	
	if(filename.isEmpty()){
		return false;
	}
	return saveFile(filename);
}


bool MainWindow1::saveFile(QString file){
	file = file;
	return true;
}


bool MainWindow1::okToContinue(){
	if( isWindowModified() ){
		int r = QMessageBox::warning(this,
								tr("%1").arg(AppName),
								tr(	"The project has been modified.\n"
									"Do you want to save your changes?"),
								QMessageBox::Yes | QMessageBox::Default,
								QMessageBox::No,
								QMessageBox::Cancel | QMessageBox::Escape
				);
		if(r == QMessageBox::Yes){
			return save();
		}else if(r == QMessageBox::Cancel){
			return false;
		}
	}
	return true;
}


void MainWindow1::showNeuesKontoDialog(){
	if(! neuesKontoDialog){
		neuesKontoDialog = new NeuesKontoDialog(this);
		//neuesKontoDialog -> setWindowFlags(Qt::FramelessWindowHint);
	}
	connect(neuesKontoDialog, SIGNAL( add(Konto *) ), this, SLOT( addKonto(Konto *) ));
	neuesKontoDialog -> show();
	neuesKontoDialog -> activateWindow();
	//QTextStream console(stdout);
	//console << "showNeuesKontoDialog";	

	//QMessageBox::warning(this,"Neues Konto","Dieser Dialog erscheint, da sie ein neues Konto erstellen möchten.", QMessageBox::Ok);
}

