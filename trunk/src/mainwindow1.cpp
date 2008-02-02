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

MainWindow1::MainWindow1 ( QWidget *parent ) : QMainWindow ( parent )
		/******************************************************************************
		* Konstruktor fuer GUI MainWindow1
		* Verbindung der Signale und Slots
		* Initialisierung der Formularen
		*******************************************************************************/
{
	setupUi ( this );

	connect ( actionQt, SIGNAL ( triggered() ), qApp, SLOT ( aboutQt() ) );
	connect ( actionNeu, SIGNAL ( triggered() ), this, SLOT ( newFile() ) );
	connect ( buttonNeuKonto, SIGNAL ( clicked() ), this, SLOT ( showNeuesKontoDialog() ) );
	connect ( actionOeffnen, SIGNAL ( triggered() ), this, SLOT ( load() ) );

	// Initialisiere Variablen der Formulare
	neuesKontoDialog = 0;
}


MainWindow1::~MainWindow1()
/******************************************************************************
* Dekonstruktur der GUI MainWindow1
* entlade die Formulare
*******************************************************************************/
{
	//delete Formulare
	if ( neuesKontoDialog ) {
		delete neuesKontoDialog;
	}

	//loesche Eintraege aus der Verknuepfungstabelle
	unloadKonten();
}


void MainWindow1::projectChanged() // SLOT
/******************************************************************************
* Funktion, die das Fenster auf modifiziert setzt
*******************************************************************************/
{
	setWindowModified ( true );
}


bool MainWindow1::addKonto ( Konto *konto )
{
	QWidget *tempWidget = new QWidget();
	connect ( konto, SIGNAL ( doChange() ), this, SLOT ( projectChanged() ) );
	konto -> setChanged();
	connections[konto] = tempWidget;
	tabWidgetMain -> addTab ( tempWidget, konto -> getKontoName() );

	tabWidgetMain -> setCurrentWidget ( tempWidget );
	return true;
}


void MainWindow1::closeEvent ( QCloseEvent *event )
{
	if ( connections.size() == 0 ) {
		event -> accept();

	} else {
		if ( okToContinue() ) {
			event -> accept();

		} else {
			event -> ignore();
		}
	}
}


bool MainWindow1::newFile()
{
	if ( okToContinue() ) {
		return clear();
	}

	return false;
}


bool MainWindow1::clear()
/******************************************************************************
* Funktion setzt alles fuer ein neues Projekt zurueck
*******************************************************************************/
{
	setFileName ( QString() );
	setWindowModified ( false );

	//Lösche die Verknuepfungstabelle und entlade alle darin enthaltenen Konten
	unloadKonten();
	connections.clear();

	return true;
}


bool MainWindow1::load() // SLOT
/******************************************************************************
* Funktion fragt nach dem Dateinamen des Projekts, welches geladen werden soll
*******************************************************************************/
{
	if ( okToContinue() ) {
		clear();
		QString filename;
		filename = QFileDialog::getOpenFileName ( this,
				   tr ( "Open Project" ),
				   ".",
				   tr ( "%1 projects (%2)" ).arg ( AppName ).arg ( END_PROJECT ) );

		if ( filename != "" ) {
			setFileName( filename );
			loadFile();
			return true;
		}

		return false;

	} else {
		return false;
	}
}


bool MainWindow1::load ( QString filename )
{
	setFileName ( filename );
	return load();
}


bool MainWindow1::loadFile()
{
#ifdef DEBUG
	QTextStream console ( stdout );
#endif

	QFile file ( getFileName() );

	if ( ! file.open ( QIODevice::ReadOnly ) ) {
#ifdef DEBUG
		console << "MainWindow::load() :" << "Datei '" << File << "' kann nicht geoeffnet werden." << "\n\r";
#endif
		return false;
	}

	QTextStream in ( &file );

	in.setCodec ( "UTF-8" );

	QString line;

	while ( ( line = in.readLine() ) != "" ) {
		Konto *tempKonto = new Konto ( line );
		addKonto ( tempKonto );
	}

#ifdef DEBUG
	console.flush();

#endif

	return true;
}


bool MainWindow1::save() // SLOT
/******************************************************************************
* Funktion ruft speicher-Funktionen auf, Abhaengig der gesetzten Project-Datei
*******************************************************************************/
{
	if ( File.isEmpty() ) {
		return saveAs(); // bekomme von saveAs den Dateinamen und speichere Projekt

	} else {
		return saveFile ( getFileName() ); // speichere Projekt unter dem angegebenen Dateinamen
	}
}


bool MainWindow1::saveAs()
/******************************************************************************
* Funktion ruft ein FileDialog auf -> ergibt Projektdatei
* Wenn Datei angegeben: return saveFile( )
* Wenn keine Datei angegeben: return false;
*******************************************************************************/
{
	//QMessageBox::warning(this,"MainWindow::saveAs","1", QMessageBox::Ok);
	QString filename = QFileDialog::getSaveFileName ( this,
					   tr ( "Save Project" ), ".",
					   tr ( "%1 projects (%2)" ).arg ( AppName ).arg ( END_PROJECT )
													);

	if ( filename.isEmpty() ) {
		return false; // wenn keine Datei angegeben gib false zurueck
	}

	filename = correctFilename ( filename, END_PROJECT ); // korrigiere Dateinamen

	setFileName ( filename ); // setze Projektdateiname

	//QMessageBox::warning ( this, "MainWindow::saveAs", File, QMessageBox::Ok );

	return saveFile ( getFileName() );
}


bool MainWindow1::saveFile ( QString filename )
/******************************************************************************
*
*******************************************************************************/
{
#ifdef DEBUG
	// Stream auf console erstellen
	QTextStream console ( stdout );
#endif

	//Dateihandler erstellen
	QFile file ( filename );

	if ( ! file.open ( QIODevice::WriteOnly ) ) { // Ueberpruefung ob Datei geoeffnet werden kann
#ifdef DEBUG
		console << "MainWindow::saveFile(): " << "Datei '" << filename << "' kann nicht geoeffnet werden" << "\n\r";
#endif
		return false;
	}

	// Textstream auf Dateihandler erstellen, Codec anpassen
	QTextStream out ( &file );

	out.setCodec ( "UTF-8" );

	//Durchlaufe die Verknuepfungstabelle der Kontodaten
	MapKontoWidget::iterator it;

	for ( it = connections.begin(); it != connections.end(); it++ ) {

		QString kontofile = it.key() -> getKontoFile();
		//QMessageBox::warning ( this, "saveFile()", "KontoFileName: '" + kontofile + "'", QMessageBox::Ok );
		//QMessageBox::warning(this, "saveFile() - Kontodaten", "Kontofile: " + it.key() -> getKontoFile() + "\n\r" + "KontoName: " + it.key() -> getKontoName(), QMessageBox::Ok);

		if ( kontofile.isEmpty() ) { //Wenn kein kontofile angegeben
			//Rufe ein FileDialog auf
			QString kontofilename = QFileDialog::getSaveFileName ( this, tr ( "Save Konto: %1" ).arg ( it.key() -> getKontoName() ),
									".", tr ( "%1 konten (%2)" ).arg ( AppName ).arg ( END_KONTO ) );

			if ( kontofilename.isEmpty() ) { //Filedialog abgebrochen
#ifdef DEBUG
				console << "MainWindow::saveFile(): " << "Dialog fuer Dateinamen ohne Auswahl geschlossen" << "\n\r";
#endif
				return false;

			} else {
				// passe Dateinamen an
				kontofilename = correctFilename ( kontofilename, END_KONTO );
				it.key() -> setKontoFile ( kontofilename ); // setze den erhaltenen Dateinamen in dem Kontoobjekt
			}
		}

		it.key() -> saveFile();

		out << it.key() -> getKontoFile() << "\n\r";
	}

	file.close();

#ifdef DEBUG
	console.flush();
#endif

	return true;
}


void MainWindow1::setFileName ( QString filename )
/******************************************************************************
* Funktion setzt Projektdateinamen
*******************************************************************************/
{
	File = filename;
}


QString MainWindow1::getFileName()
/******************************************************************************
* Funktion gibt Projektdateinamen zurueck
*******************************************************************************/
{
	return File;
}


bool MainWindow1::okToContinue()
/******************************************************************************
* Funktion ruft Messagebox auf, Nutzeranfrage ob Aenderungen gespeichert werden
* sollen.
* Wenn Ja: speichern - Return true;
* Wenn Nein: nicht speichern - Return true
* Wenn Cancel: Reurn false
*******************************************************************************/
{
	if ( isWindowModified() ) {
		int r = QMessageBox::warning ( this,
									   tr ( "%1" ).arg ( AppName ),
									   tr ( "The project has been modified.\n"
											"Do you want to save your changes?" ),
									   QMessageBox::Yes | QMessageBox::Default,
									   QMessageBox::No,
									   QMessageBox::Cancel | QMessageBox::Escape
									 );

		if ( r == QMessageBox::Yes ) {
			return save();

		} else if ( r == QMessageBox::Cancel ) {
			return false;
		}
	}

	return true;
}


QString MainWindow1::correctFilename ( QString filename, QString endung )
/******************************************************************************
* Funktion ueberprüft einen filename ob dieser die endung beinhaltet
* Gibt angepassent filename zurueck
*******************************************************************************/
{
	QString file = filename;

	if ( endung.at ( 0 ) == '*' ) {
		endung = endung.right ( endung.size() - 1 );
	}

	if ( ! file.contains ( endung, Qt::CaseInsensitive ) ) {
		file.append ( endung );
	}

	return file;
}


void MainWindow1::unloadKonten()
/******************************************************************************
* Funktion beseitigt alle Instanzen der Konten aus der Verknuepfungstabelle
*******************************************************************************/
{
	for ( MapKontoWidget::iterator it = connections.begin(); it != connections.end(); it++ ) {
		delete it.key();
	}
}


void MainWindow1::showNeuesKontoDialog() //SLOT
/******************************************************************************
* Funktion ruft das Fenster der Klasse NeuesKontoDialog auf, zeigt es an und
* verbindet die Signale
*******************************************************************************/
{
	if ( ! neuesKontoDialog ) {
		neuesKontoDialog = new NeuesKontoDialog ( this );
		// toDo: Titelleistenbuttons entfernen
	}

	connect ( neuesKontoDialog, SIGNAL ( add ( Konto * ) ), this, SLOT ( addKonto ( Konto * ) ) );

	neuesKontoDialog -> show();
	neuesKontoDialog -> activateWindow();
}

