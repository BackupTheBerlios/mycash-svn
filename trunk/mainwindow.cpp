#include <QtGui>

#include <QVector>
#include <iostream>

#include "mainwindow.h"
#include "konto.h"
#include "configs.h"
#include "class_konto_typ.h"
#include "KontoEntry.h"
#include "konto_splitt.h"

extern Konto_Typ* MCKT;

MainWindow::MainWindow(){
	createActions();
	createMenus();
	deaktivateSave();
	
	curFile = "";
	//Lade Kontotypen
	MCKT = new Konto_Typ();
	
	
	MCKT -> newTyp("Tagesgeld", Konto_Typ::Haben);
	MCKT -> newTyp("Girokonto", Konto_Typ::Haben);
	
	MCKT -> saveTyp();
}

void MainWindow::createActions(){
	//newAction
	newAction = new QAction( tr("&New") , this);
	newAction -> setShortcut( tr("Ctrl+N") );
	newAction -> setStatusTip( tr("create a new project") );
	connect(newAction, SIGNAL( triggered() ), this, SLOT( newFile() ));
	
	//openAction
	openAction = new QAction( tr("&Open"), this );
	openAction -> setShortcut( tr("Ctrl+O") );
	openAction -> setStatusTip( tr("open a existing project") );
	connect(openAction, SIGNAL( triggered() ), this, SLOT( open() ));
	
	//saveAction
	saveAction = new QAction( tr("Save"), this);
	saveAction -> setShortcut( tr("Ctrl+S") );
	saveAction -> setStatusTip( tr("save project") );
	connect(saveAction, SIGNAL( triggered() ), this, SLOT( save() ));
	
	//saveAsAction
	saveAsAction = new QAction( tr("Save &As"), this);
	saveAsAction -> setStatusTip( tr("save a project as") );
	connect(saveAsAction, SIGNAL( triggered() ), this, SLOT( saveAs() ));
	
	//exitAction
	exitAction = new QAction( tr("&Exit"), this);
	exitAction -> setShortcut( tr("Alt+F4") );
	exitAction -> setStatusTip( tr("exit %1").arg(AppName) );
	connect(exitAction, SIGNAL( triggered() ), this, SLOT( exit() ));
	
	//aboutAction
	aboutAction = new QAction( tr("&About"), this );
	aboutAction -> setShortcut( tr("F1") );
	aboutAction -> setStatusTip( tr("about %1").arg("mycash") );
	connect(aboutAction, SIGNAL( triggered() ), this, SLOT( about() ));
	
	//aboutQtAction
	aboutQtAction = new QAction( tr("&About Qt") , this);
	aboutQtAction -> setStatusTip( tr("Show the Qt library's About box") );
	connect(aboutQtAction, SIGNAL( triggered() ), qApp, SLOT( aboutQt() ));
	
}

void MainWindow::closeEvent(QCloseEvent *event){
	if(okToContinue()){
		event -> accept();
	}else{
		event -> ignore();
	}
}

void MainWindow::createMenus(){
	//FileMenu
	fileMenu = menuBar() -> addMenu( tr("&Project") );
	fileMenu -> addAction(newAction);
	fileMenu -> addAction(openAction);
	fileMenu -> addAction(saveAction);
	fileMenu -> addAction(saveAsAction);
	fileMenu -> addAction(exitAction);
	
	//helpMenu
	helpMenu = menuBar() -> addMenu( tr("help") );
	helpMenu -> addAction(aboutAction);
	helpMenu -> addAction(aboutQtAction);
}

void MainWindow::newFile(){
	createNewKonto_deb("./debug/Testkonto1.mck");
	showKData_deb("./debug/Testkonto1.mck");
	
}

void MainWindow::open(){
	if(okToContinue()){
		QString filename = QFileDialog::getOpenFileName(
							this,
							tr("Open project"),
							".",
							tr("%1 projects (*.mc)").arg(AppName)
						);
		if(!filename.isEmpty()){
			loadFile(filename);
		}
	}
}

bool MainWindow::save(){
	if(!curFile.isEmpty()){
		return saveFile(curFile);
	}else{
		return saveAs();
	}
}

bool MainWindow::saveAs(){
	QString filename = QFileDialog::getSaveFileName(this, tr("Save Project"), ".", tr("%1 projects (*.mc)").arg(AppName));
	
	if(filename.isEmpty()){
		return false;
	}
	return saveFile(filename);
}

bool MainWindow::saveFile(const QString &filename){
	//Datei oeffnen
	QFile file(filename);
	if(! file.open(QIODevice::ReadOnly)){
		QMessageBox::warning(this,
						tr("save project", "messageboxtitle for load project error"),
						tr("File %1 can't save: %2","messageboxtext for save project error, arg1:filename,"
							"arg2:error").arg(file.fileName()).arg(file.errorString()),
						QMessageBox::Yes | QMessageBox::Default
		);
		return false;
	}
	QDataStream out(&file);
	out.setVersion(QDataStreamVersion);
	
	//Cursor ändern
	QApplication::setOverrideCursor(Qt::WaitCursor);
	
	QVector<Konto *>::iterator it;
	for(it = konten.begin(); it != konten.end(); it++){
		out << (*it) -> getKontoFile();
	}
	
	//Setze Cursor zurueck
	QApplication::restoreOverrideCursor();
	return true;
	
}

bool MainWindow::loadFile(const QString &filename){
	//Datei oeffnen
	QFile file(filename);
	if(! file.open(QIODevice::ReadOnly)){
		QMessageBox::warning(this,
						tr("load project", "messageboxtitle for load project error"),
						tr("File %1 can't load: %2","messageboxtext for load project error, arg1:filename,"
							"arg2:error").arg(file.fileName()).arg(file.errorString()),
						QMessageBox::Yes | QMessageBox::Default
		);
		return false;
	}
	deleteKonten();
	

	//QDataStream erstellen + Version einstellen
	QDataStream in(&file);
	in.setVersion(QDataStreamVersion);
	
	//Cursor ändern
	QApplication::setOverrideCursor(Qt::WaitCursor);
	
	//Datei einlesen
	//quint32 zeilennummer = 0;
	QString Zeile;

	while(! in.atEnd()){
		//zeilennummer ++;
		in >> Zeile;	
		//Lade Konto aus Datei 'Zeile'
		Konto *tmp = new Konto();
		if(!Zeile.isEmpty()){
			tmp -> loadFile(Zeile);
		}
		//Fuge Konto in die Liste ein
		if(*tmp){
			konten.push_back( tmp );
		}
	}
	
	//Setze Cursor zurueck
	QApplication::restoreOverrideCursor();
	return true;
}

void MainWindow::about(){
	QMessageBox::information(this,
					tr("%1").arg(AppName),
					tr(	"Version: %1\n"
						"programmed by %2").arg("0.1").arg("Matthias Ende"),
						QMessageBox::Ok | QMessageBox::Default
	);
}

bool MainWindow::okToContinue(){
	if(isWindowModified()){
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

void MainWindow::exit(){
	QWidget::close();
}

void MainWindow::deleteKonten(){
	QVector<Konto *>::iterator it;
	for(it=konten.begin(); it != konten.end(); it++){
		delete( *it );
	}
	konten.clear();
}

MainWindow::~MainWindow(){
	deleteKonten();
	delete(MCKT);
}

void MainWindow::aktivateSave(){
	saveAsAction -> setEnabled(true);
	saveAction -> setEnabled(true);
}

void MainWindow::deaktivateSave(){
	saveAsAction -> setEnabled(false);
	saveAction -> setEnabled(false);
}

void MainWindow::createNewKonto_deb(QString File){
	Konto *tmp = new Konto();
	tmp -> setKontoFile(File);
	tmp -> setKontoBezeichnung("Testkonto zum Testen");
	tmp -> setKontoName("Testkonto");
	tmp -> setBLZ("0123456789");
	tmp -> setBankName("TestBank");
	tmp -> setKontoTyp(23);

	QDate tempDate;
	tempDate.fromString("20080118", "yyyymmdd");

	KontoEntry tempEntry;
	//tempEntry.changeDatum(QDate::fromString("20080117", "yyyymmdd"));
	tempEntry.changeDatum("20070123");
	tempEntry.changeVerwendung("DebugEntry");
	Konto_Splitt tempSplitt;
	tempSplitt.changeKategorie(1);
	tempSplitt.changeVerwendung("erster Untereintrag");
	tempSplitt.changeBetrag(11.11);
	tempEntry.addSplitt(&tempSplitt);
	tempSplitt.changeKategorie(2);
	tempSplitt.changeVerwendung("zweiter Untereintrag");
	tempSplitt.changeBetrag(22.22);
	tempEntry.addSplitt(&tempSplitt);

	KontoEntry tempEntry2;
	tempEntry2.changeDatum("20060101");
	tempEntry2.changeVerwendung("2. DEBUG Entry");
	Konto_Splitt tempS;
	tempS.changeKategorie(3);
	tempS.changeVerwendung("erster Debug Untereintrag");
	tempS.changeBetrag(33.33);
	tempEntry2.addSplitt(&tempS);	

	tmp -> addEntry(&tempEntry);
	tmp -> addEntry(&tempEntry2);
	tmp -> saveFile();
	delete (tmp);
}

void MainWindow::showKData_deb(QString File){
	Konto *tmp = new Konto();
	tmp -> loadFile(File);
	tmp -> showKontoData_deb();
	delete(tmp);
}

