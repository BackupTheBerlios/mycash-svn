//$Date$

#include <QtGui>
#include <QTextStream>
#include <QObject>
#include <QMap>
#include <QWidget>
#include <QTabWidget>
#include <QFile>
#include <QMessageBox>

#include "mainwindow1.h"
#include "configs.h"
#include "neueskontodialog.h"
#include "konto.h"

#include <QMainWindow>
#include <QWidget>

MainWindow1::MainWindow1(QWidget *parent) : QMainWindow(parent){
	setupUi(this);

	connect(actionQt,SIGNAL( triggered() ), qApp, SLOT( aboutQt() ));
	connect(actionNeu, SIGNAL( triggered() ), this, SLOT( newFile() ));
	connect(buttonNeuKonto, SIGNAL( clicked() ), this, SLOT( showNeuesKontoDialog() ));
	connect(actionOeffnen, SIGNAL( triggered() ), this, SLOT( loadFile() ));

	// Initialisiere Formulare
	neuesKontoDialog = 0;
}


MainWindow1::~MainWindow1(){
	//delete Formulare
	if(neuesKontoDialog){
		delete neuesKontoDialog; 
	}
}


void MainWindow1::projectChanged(){
	setWindowModified(true);
}


bool MainWindow1::addKonto(Konto *konto){
	QWidget *tempWidget = new QWidget();
	connect(konto, SIGNAL( doChange() ), this, SLOT( projectChanged() ));
	konto -> setChanged();
	connections[konto] = tempWidget;
	tabWidgetMain -> addTab(tempWidget, konto -> getKontoName());
	
	tabWidgetMain -> setCurrentWidget( tempWidget );
	return true;
}


void MainWindow1::closeEvent(QCloseEvent *event){
	if( connections.size() == 0){
		event -> accept();
	}else{
		if( okToContinue() ){
			event -> accept();
		}else{
			event -> ignore();
		}
	}
}


bool MainWindow1::newFile(){
	if( okToContinue() ){
		return clear();	
	}
	return false;
}


bool MainWindow1::clear(){
	File = QString();
	setWindowModified(false);

	connections.clear();

	return true;
}


bool MainWindow1::load(){
	#ifdef DEBUG
		QTextStream console(stdout);
	#endif
	
	QFile file(File);
	if(! file.open(QIODevice::ReadOnly) ){
		#ifdef DEBUG
			console << "MainWindow::load() :" << "Datei '" << File << "' kann nicht geoeffnet werden." << "\n\r";
		#endif
		return false;
	}

	QTextStream in( &file );
	in.setCodec("UTF-8");
	
	QString line;

	while( (line = in.readLine()) != "" ){
		Konto *tempKonto = new Konto(line);
		addKonto( tempKonto );
	}

	#ifdef DEBUG
		console.flush();
	#endif
	
	return true;
}


bool MainWindow1::load(QString filename){
	File = filename;
	return load();
}


bool MainWindow1::loadFile(){
	if( isWindowModified() ){

	}
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


bool MainWindow1::saveFile(QString filename){
	#ifdef DEBUG
		QTextStream console(stdout);
	#endif	


	QFile file(filename);
	if(! file.open(QIODevice::WriteOnly) ){
		#ifdef DEBUG
			console << "MainWindow::saveFile(): " << "Datei '" << filename << "' kann nicht geoeffnet werden" << "\n\r";
		#endif
		return false;
	}

	QTextStream out( &file );
	out.setCodec("UTF-8");
	
	MapKontoWidget::iterator it;
	for(it = connections.begin(); it != connections.end(); it++){
		if(! it.key() -> isNotChanged()){
			
			if(it.key() -> getKontoFile() == ""){

				QString kontofilename = QFileDialog::getSaveFileName(this, tr("Save Konto: %1").arg( it.key() -> getKontoName() ), ".",
					tr("%1 konten (*.mck)").arg(AppName));
				if(! kontofilename.isEmpty() ){
					it.key() -> setKontoFile( kontofilename );
					it.key() -> saveFile();
				}

			}else{
				it.key() -> saveFile();
			}

		}

		out << it.key() -> getKontoFile() << "\n\r";
	}

	file.close();

	#ifdef DEBUG
		console.flush();
	#endif

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
}

