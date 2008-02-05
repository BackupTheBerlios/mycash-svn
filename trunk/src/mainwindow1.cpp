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
#include "tabkontomain.h"

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

	connect ( actionQt,
			  SIGNAL ( triggered() ),
			  qApp,
			  SLOT ( aboutQt() )
			);
	connect ( actionNeu,
			  SIGNAL ( triggered() ),
			  this,
			  SLOT ( newFile() )
			);
	connect ( buttonNeuKonto,
			  SIGNAL ( clicked() ),
			  this,
			  SLOT ( showNeuesKontoDialog() )
			);
	connect ( actionOeffnen,
			  SIGNAL ( triggered() ),
			  this,
			  SLOT ( load() )
			);
	connect ( actionSave,
			  SIGNAL ( triggered() ),
			  this,
			  SLOT ( save() )
			);
	connect ( actionSaveAs,
			  SIGNAL ( triggered() ),
			  this,
			  SLOT ( saveAs() )
			);
	connect ( buttonAddKonto,
			  SIGNAL ( clicked() ),
			  this,
			  SLOT ( addExistKonto() )
			);

	// Initialisiere Variablen der Formulare
	neuesKontoDialog = 0;
}


MainWindow1::~MainWindow1()
/******************************************************************************
* Dekonstruktur der GUI MainWindow1
* entlaedt die Formulare
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


bool MainWindow1::addKonto ( Konto *konto ) // SLOT
/******************************************************************************
* Methode uebernimmt eine Klasse, fuegt sie in die Verknüpfungstabelle ein,
* erzeugt ein TabWidget Eintrag
*******************************************************************************/
{
	TabKontoMain *tempWidget = new TabKontoMain(this, konto);
	connect ( konto, SIGNAL ( doChange() ), this, SLOT ( projectChanged() ) );
	konto -> setChanged(); // Wegen Signal an MainWindow
	connections[konto] = tempWidget;
	tabWidgetMain -> addTab ( tempWidget, konto -> getKontoName() );

	//setze Ansicht auf neu hinzugefuegtes TabWidget
	tabWidgetMain -> setCurrentWidget ( tempWidget );
	return true;
}


bool MainWindow1::addExistKonto()
{
	QString filename;
	filename = QFileDialog::getOpenFileName ( this,
			   tr ( "Open Kontofile" ),
			   ".",
			   tr ( "%1 konten (%2)" ).arg ( AppName ).arg ( END_KONTO )
											);

	if ( filename.isEmpty() ) {
		return false; // Dialog ohne Auswahl beendet
	}

	Konto *tempKonto = new Konto ( filename );

	addKonto ( tempKonto );

	return true;
}


void MainWindow1::closeEvent ( QCloseEvent *event )
/******************************************************************************
* Methode ueberschreibt closeEvent
* wenn MainWindow geaendert wurde: Abfrage ob Speichern -> Reaktion darauf
*******************************************************************************/
{
	// !Keine Abfrage der Groesse von der Verknuepfungstabelle, da Programmeinstellungen geändert worden koennten
	if ( okToContinue() ) {
		event -> accept();

	} else {
		event -> ignore();
	}
}


bool MainWindow1::newFile() // SLOT
/******************************************************************************
* Methode fragt bei geaenderten Projekt ab, ob ein Neues Projekt erstellt werden soll
* wenn ja: altes Projekt wird zurueck gesetzt
*******************************************************************************/
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
			setFileName ( filename );
			loadFile();
			return true;
		}

		return false;

	} else {
		return false;
	}
}


bool MainWindow1::load ( QString filename )
/******************************************************************************
* Ueberladene Methode von load, wenn Dateiname schon bekannt
*******************************************************************************/
{
	setFileName ( filename );
	return loadFile();
}


bool MainWindow1::loadFile()
/******************************************************************************
* Methode, die Datei ausliesst und Konten hinzufuegt
*******************************************************************************/
{
#ifdef DEBUG
	QTextStream console ( stdout );
#endif

	// erstelle ein Dateihandler auf die Projektdatei
	QFile file ( getFileName() );

	if ( ! file.open ( QIODevice::ReadOnly ) ) {
#ifdef DEBUG
		console << "MainWindow::load() :" << "Projektdatei '" << getFileName() << "' kann nicht geoeffnet werden." << "\n\r";
#endif
		return false;
	}

	// erstelle ein TextStream auf die Projektdatei
	QTextStream in ( &file );

	in.setCodec ( "UTF-8" );

	QString line; // Speicher fuer die einzulesende Zeile

	while ( ( line = in.readLine() ) != "" ) {
		//ToDo: testen ob Dateiname(line) gueltig!
		Konto *tempKonto = new Konto ( line );
		addKonto ( tempKonto );
	}

#ifdef DEBUG
	console << "MainWindow::load() :" << "Projektdatei '" << getFileName() << "' wurde erfolgreich eingelesen." << "\n\r";

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

		if ( kontofile.isEmpty() ) { //Wenn kein kontofile angegeben
			//Rufe ein FileDialog auf
			QString kontofilename = QFileDialog::getSaveFileName ( this,
									tr ( "Save Konto: %1" ).arg ( it.key() -> getKontoName() ),
									".",
									tr ( "%1 konten (%2)" ).arg ( AppName ).arg ( END_KONTO )
																 );

			if ( kontofilename.isEmpty() ) { //Filedialog abgebrochen
#ifdef DEBUG
				console << "MainWindow::saveFile(): " << "Dialog fuer Dateinamen ohne Auswahl geschlossen" << "\n\r";
				console.flush();
#endif
				return false;

			} else {
				// passe Dateinamen an
				kontofilename = correctFilename ( kontofilename, END_KONTO );
				it.key() -> setKontoFile ( kontofilename ); // setze den erhaltenen Dateinamen in dem Kontoobjekt
			}
		}
		it.key() -> setKontoFile("konto.xml");
		it.key() -> saveFileXML();

		out << it.key() -> getKontoFile() << "\n\r";
	}

	file.close();

#ifdef DEBUG
	console << "MainWindow::saveFile(): " << "Projekdatei '" << getFileName() << "' erfolgreich gespeichert." << "\n\r";
	console.flush();
#endif

	setWindowModified( false );

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

