//$Date$

#include <QtGui>
#include <QTextStream>
#include <QObject>
#include <QMap>
#include <QWidget>
#include <QTabWidget>
#include <QFile>
#include <QMessageBox>
#include <QtXml>

#include "mainwindow1.h"
#include "configs.h"
#include "neueskontodialog.h"
#include "konto.h"
#include "tabkontomain.h"
#include "tabmain.h"

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

	connect (  //Menueintrag "about-Qt"
		actionQt,
		SIGNAL ( triggered() ),
		qApp,
		SLOT ( aboutQt() )
	);

	connect ( // Menueintrag "program-new"
		actionNew,
		SIGNAL ( triggered() ),
		this,
		SLOT ( newFile() )
	);

	connect ( //Menueintrag "project-create new account"
		actionNewAccount,
		SIGNAL ( triggered() ),
		this,
		SLOT ( showNeuesKontoDialog() )
	);

	connect ( // Menueintrag "program - open"
		actionOpen,
		SIGNAL ( triggered() ),
		this,
		SLOT ( load() )
	);

	connect ( // Menueintrag "program - save"
		actionSave,
		SIGNAL ( triggered() ),
		this,
		SLOT ( save() )
	);

	connect ( // Menueintrag "program - save as"
		actionSaveAs,
		SIGNAL ( triggered() ),
		this,
		SLOT ( saveAs() )
	);

	connect ( // Menueintrag "project - add exist account"
		actionAddAccount,
		SIGNAL ( triggered() ),
		this,
		SLOT ( addExistKonto() )
	);

	connect ( // Menueintrag "about - MyCash"
		actionMyCash,
		SIGNAL ( triggered() ),
		this,
		SLOT ( about() )
	);

	// Initialisiere Variablen der Formulare
	neuesKontoDialog = 0;
	tabMain = 0;

	// configure view
	qint32 pos;
	pos = tabWidgetMain -> currentIndex();
	tabWidgetMain -> removeTab ( pos );
	showMain ( false );
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

	//entlade Tabs

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


bool MainWindow1::addKonto ( Konto *konto, bool show )
/******************************************************************************
* Ueberladene Funktion, um das das hinzufuegen zum TabWidget zu steuern
*******************************************************************************/
{
	if ( show ) {
		return addKonto ( konto );

	} else {
		connectKonto ( konto );

		connections[konto] = 0;

		if ( connections.size() == 1 ) { // Wenn 1 -> einiger Eintrag, sonst schon da..
			showMain ( true );
		}
	}

	return true;
}


bool MainWindow1::addKonto ( Konto *konto ) // SLOT
/******************************************************************************
* Methode uebernimmt eine Klasse, fuegt sie in die Verknüpfungstabelle ein,
* erzeugt ein TabWidget Eintrag
*******************************************************************************/
{
	//QMessageBox::warning(this,"Sender",QString::number((uint)sender()) + "\n" + sender() -> objectName() );
	QString file = konto -> getKontoFile();

	if ( !file.isEmpty() && existFilename ( file ) ) {
		showMessageboxAlreadyExist();
		return false;
	}

	TabKontoMain *tempWidget = new TabKontoMain ( this, konto );

	connectKonto ( konto );

	konto -> setChanged(); // Wegen Signal an MainWindow

	connections[konto] = tempWidget;
	connect ( tempWidget,
			  SIGNAL ( close ( TabKontoMain * ) ),
			  this,
			  SLOT ( closeTab ( TabKontoMain * ) )
			);

	if ( connections.size() == 1 ) { // Wenn 1 -> einiger Eintrag, sonst schon da..
		showMain ( true );
	}

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

	if ( existFilename ( filename ) ) {
		showMessageboxAlreadyExist();
		return false;
	}

	Konto *tempKonto = new Konto ( filename );

	if ( ! *tempKonto ) {
		return false;
	}

	addKonto ( tempKonto );

	return true;
}

bool MainWindow1::closeTab ( TabKontoMain *tab ) //SLOT
/******************************************************************************
* Methode empfaengt den Wunsch zum schließen eines Tabs und entlaed den entsprechenden Tab
*******************************************************************************/
{
	qint32 pos = 0;
	pos = tabWidgetMain -> indexOf ( tab );

	if ( pos == -1 ) { // Tab existiert nicht in tabWidgetMain
		return false;
	}

	MapKontoWidget::iterator it;

	for ( it = connections.begin(); it != connections.end(); it++ ) {
		if ( it.value() == tab ) { //entsprechenden Eintrag in Verknuepfungstabelle gefunden
			tabWidgetMain -> removeTab ( pos );
			delete it.value();
			it.value() = 0;

			return true;
		}
	}

	// Ende erreicht -> Tab nicht gefunden
	return false;
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
			load ( filename );
			return true;
		}
	}

	return false;
}


bool MainWindow1::load ( QString filename )
/******************************************************************************
* Ueberladene Methode von load, wenn Dateiname schon bekannt
*******************************************************************************/
{
	setFileName ( filename );
	return loadProjectXml ( filename );
}


bool MainWindow1::loadProject()
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


bool MainWindow1::loadProjectXml ( QString& filename )
/******************************************************************************
* Methode laed eine XML-Datei mit Projektinformationen
*******************************************************************************/
{
	QFile file ( filename );

	//Fehlervariablen fuer Parser
	QString errorStr;
	int errorLine;
	int errorColumn;

	//DOM Document
	QDomDocument doc;

	// Dokument parsen

	if ( !doc.setContent ( &file, true, &errorStr, &errorLine, &errorColumn ) ) {
		QMessageBox::warning ( this,
							   tr ( "%1" ).arg ( AppName ),
							   tr ( "Parse error at line %1, column %2:\n%3" )
							   .arg ( errorLine )
							   .arg ( errorColumn )
							   .arg ( errorStr )
							 );
		return false;
	}

	//DOM Element
	QDomElement root = doc.documentElement();

	if ( root.tagName() != "Projekt" ) {
		QMessageBox::warning ( this,
							   tr ( "%1" ).arg ( AppName ),
							   tr ( "This is not a correct Projectfile" )
							 );
		return false;
	}

	//QDomNode - Laufvariable
	QDomNode node = root.firstChild();

	//gehe alle Zweige von 'Projekt' durch
	while ( !node.isNull() ) {
		QString nodeText = node.toElement().tagName();

		if ( nodeText == "Konten" ) {
			parseKontenXml ( node.toElement() );

		} else {
			// nicht gueltig
		}

		node = node.nextSibling();
	}

	return true;
}


QString MainWindow1::showGetFileNameDialog ( QString endung, QString name )
/******************************************************************************
* Methode zeigt ein FileNameDialog
*******************************************************************************/
{
	QString file;

	if ( endung == END_KONTO ) {
		file = QFileDialog::getSaveFileName ( this,
											  tr ( "Save Konto: %1" ).arg ( name ),
											  ".",
											  tr ( "%1 konten (%2)" ).arg ( AppName ).arg ( endung )
											);

	} else if ( endung == END_PROJECT ) {
		file = QFileDialog::getSaveFileName ( this,
											  tr ( "Save Project" ),
											  ".",
											  tr ( "%1 projects (%2)" ).arg ( AppName ).arg ( endung )
											);

	} else {
		return QString();
	}

	if ( file.isEmpty() ) {
		return file;
	}

	return correctFilename ( file, endung ); // korrigiere Dateinamen
}


bool MainWindow1::save() // SLOT
/******************************************************************************
* Funktion ruft speicher-Funktionen auf, Abhaengig der gesetzten Project-Datei
*******************************************************************************/
{
	if ( File.isEmpty() ) {
		return saveAs(); // bekomme von saveAs den Dateinamen und speichere Projekt

	} else {
		return saveProjectXml ( getFileName() ); // speichere Projekt unter dem angegebenen Dateinamen
	}
}


bool MainWindow1::saveAs()
/******************************************************************************
* Funktion ruft ein FileDialog auf -> ergibt Projektdatei
* Wenn Datei angegeben: return saveFile( )
* Wenn keine Datei angegeben: return false;
*******************************************************************************/
{
	QString filename = showGetFileNameDialog ( END_PROJECT, "" );

	if ( filename.isEmpty() ) {
		return false; // wenn keine Datei angegeben gib false zurueck
	}

	setFileName ( filename ); // setze Projektdateiname

	return saveProjectXml ( filename );
}


bool MainWindow1::saveProject ( QString filename )
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
			QString kontofilename = showGetFileNameDialog ( END_KONTO, it.key() -> getKontoFile() );

			if ( kontofilename.isEmpty() ) { //Filedialog abgebrochen
#ifdef DEBUG
				console << "MainWindow::saveFile(): " << "Dialog fuer Dateinamen ohne Auswahl geschlossen" << "\n\r";
				console.flush();
#endif
				return false;

			} else {
				it.key() -> setKontoFile ( kontofilename ); // setze den erhaltenen Dateinamen in dem Kontoobjekt
			}
		}

		it.key() -> saveFileXML();

		out << it.key() -> getKontoFile() << "\n\r";
	}

	file.close();

#ifdef DEBUG
	console << "MainWindow::saveFile(): " << "Projekdatei '" << getFileName() << "' erfolgreich gespeichert." << "\n\r";
	console.flush();
#endif

	setWindowModified ( false );

	return true;
}


bool MainWindow1::saveProjectXml ( QString filename )
/******************************************************************************
* Methode speichert das Project ab
*******************************************************************************/
{
	quint32 Indent = 4;

#ifdef DEBUG
	QTextStream console ( stdout );
#endif

	if ( ! isWindowModified() ) {
		return true;
	}

	if ( filename.isEmpty() ) {
#ifdef DEBUG
		console << "MainWindow1::saveFileXml(): " << "Keine Projektdatei angegeben." << "\n\r";
#endif
		return false;
	}

	QFile file ( filename );

	if ( !file.open ( QIODevice::WriteOnly ) ) {
#ifdef DEBUG
		console << "MainWindow1::saveFileXml(): " << "Projektdatei '" << filename << "' kann nicht geoeffnet werden." << "\n\r";
#endif
		return false;
	}

	// TextStream fuer XML - Datei
	QTextStream out ( &file );

	out.setCodec ( "UTF-8" );

	QDomDocument doc;

	QDomElement elementProject = doc.createElement ( "Projekt" );

	QDomNode xmlNode = doc.createProcessingInstruction ( "xml", "version=\"1.0\" encoding=\"UTF-8\"" );

	// An das Dokument den Zweig 'Konten' anhaengen
	elementProject.appendChild ( getKontenXml ( doc ) );

	//Fuege Zweig 'Konten' an das Dokument an
	doc.appendChild ( elementProject );

	// Fuege am Anfang XML-Header ein
	doc.insertBefore ( xmlNode, doc.firstChild() );

	// Speichere XML-Baum
	doc.save ( out, Indent );

	// Speichere einzelen Konten
	if ( saveKonten() ) {
		setWindowModified ( false );
		return true;
	}

	return false;
}

QDomElement MainWindow1::getKontenXml ( QDomDocument& doc )
/******************************************************************************
* Methode gint ein XML-Zweig mit den Dateinamen der Konten des Projekts
*******************************************************************************/
{
	QDomElement elementKonten = doc.createElement ( "Konten" );

	MapKontoWidget::iterator it;

	for ( it = connections.begin(); it != connections.end(); it++ ) {
		if ( ( it.key() -> getKontoFile() ).isEmpty() ) {
			QString file = showGetFileNameDialog ( END_KONTO, it.key() -> getKontoName() );

			if ( file.isEmpty() ) {
				return doc.createElement ( "" );
			}

			it.key() -> setKontoFile ( file );
		}

		QDomElement elementFile = doc.createElement ( "File" );

		QDomText textFile = doc.createTextNode ( it.key() -> getKontoFile() );

		elementFile.appendChild ( textFile );
		elementFile.setAttribute ( "open", ( it.value() != 0 ) ? "ja" : "nein" );

		elementKonten.appendChild ( elementFile );
	}

	return elementKonten;
}


bool MainWindow1::parseKontenXml ( const QDomElement& element )
/******************************************************************************
* Methode parset den Zweig 'Files'
*******************************************************************************/
{
	if ( element.tagName() != "Konten" ) {
		return false;
	}

	QDomNode node = element.firstChild();

	while ( !node.isNull() ) {
		QString nodeText = node.toElement().tagName();

		if ( nodeText == "File" ) {
			QString attribute = node.toElement().attribute ( "open", "nein" );
			QString filename = node.toElement().text();
			bool open;

			if ( attribute == "nein" ) {
				open = false;

			} else {
				open = true;
			}

			QFile file ( filename );

			Konto *konto = new Konto ( filename );

			if ( *konto ) {
				addKonto ( konto, open );
			}

		} else {
			// nicht gueltig
		}

		node = node.nextSibling();
	}

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


bool MainWindow1::connectKonto ( Konto *konto )
/******************************************************************************
* Methode verknuepft Signal doChange() von Konto mit projectChanged()
*******************************************************************************/
{
	return connect ( konto,
					 SIGNAL ( doChange() ),
					 this,
					 SLOT ( projectChanged() )
				   );

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
		connect (
			neuesKontoDialog,
			SIGNAL ( add ( Konto * ) ),
			this,
			SLOT ( addKonto ( Konto * ) )
		);
		// toDo: Titelleistenbuttons entfernen
	}

	neuesKontoDialog -> show();

	neuesKontoDialog -> activateWindow();
}


bool MainWindow1::existFilename ( QString filename )
/******************************************************************************
* Methode fragt alle Konten in der Verknuepfungstabelle nach den Dateinamen
* filename wird mit den Dateinamen verglichen
* wenn filename € Dateinamen -> true, sonst: false
*******************************************************************************/
{
	MapKontoWidget::iterator it;

	for ( it = connections.begin(); it != connections.end(); it++ ) {
		if ( it.key() -> getKontoFile() == filename ) {
			return true;
		}
	}

	return false;
}


void MainWindow1::showMessageboxAlreadyExist()
/******************************************************************************
* Methode zeigt eine MessageBox, dass ein Konto schon Element des Projects ist
*******************************************************************************/
{
	QMessageBox::information ( this,
							   tr ( "account can't add" ),
							   tr ( "Account can't add. Account is already in this project." ),
							   QMessageBox::Ok
							 );
}


bool MainWindow1::saveKonten()
/******************************************************************************
* Methode ruft die Speicherfunktionen der einezelnen Konten auf
*******************************************************************************/
{
#ifdef DEBUG
	// Stream auf console erstellen
	QTextStream console ( stdout );
#endif

	MapKontoWidget::iterator it;

	for ( it = connections.begin(); it != connections.end(); it++ ) {

		QString kontofile = it.key() -> getKontoFile();

		if ( kontofile.isEmpty() ) { //Wenn kein kontofile angegeben
			//Rufe ein FileDialog auf
			QString kontofilename = showGetFileNameDialog ( END_KONTO, it.key() -> getKontoFile() );

			if ( kontofilename.isEmpty() ) { //Filedialog abgebrochen
#ifdef DEBUG
				console << "MainWindow::saveKonten(): " << "Dialog fuer Dateinamen ohne Auswahl geschlossen (Konto '"
				<< it.key() -> getKontoName() << "')" << "\n\r";
				console.flush();
#endif
				return false;

			} else {
				it.key() -> setKontoFile ( kontofilename ); // setze den erhaltenen Dateinamen in dem Kontoobjekt
			}
		}

		// Konto Speichern
		it.key() -> saveFileXML();
	}

	return true;
}

bool MainWindow1::deleteKonto ( Konto *konto ) // SLOT
/******************************************************************************
* Methode loescht ein Konto aus dem Projekt
* Wenn ein entsprechender Tab noch geladen ist -> entladen und entfernen
* Konto entladen + aus Verknuepfungstabelle loeschen
*******************************************************************************/
{
	MapKontoWidget::iterator it;

	for ( it = connections.begin(); it != connections.end(); it++ ) {
		if ( it.key() == konto ) {
			if ( it.value() != 0 ) { //Tab geladen
				qint32 pos;
				pos = tabWidgetMain -> indexOf ( it.value() );

				if ( pos == -1 ) { //Tab nicht existent
					return false;
				}

				tabWidgetMain -> removeTab ( pos ); // entferne Tab aus QTabWidget

				delete it.value(); //entlade Tab
				delete it.key(); //entlade Konto
				connections.erase ( it ); // loesche Konto aus Verknuepfungstabelle

				if ( connections.size() == 0 ) { //Kein Konto im Project
					showMain ( false );
				}

				return true;
			}
		}
	}

	return false;
}


void MainWindow1::showMain ( bool show )
/******************************************************************************
* Methode aktiviert/deaktiviert das Hauptmenu
*******************************************************************************/
{
	if ( tabMain == 0 ) {
		tabMain = new TabMain ( this );
		tabWidgetMain -> addTab ( tabMain, tr ( "Mainmenu" ) );
	}

	qint32 pos;

	pos = tabWidgetMain -> indexOf ( tabMain );
	//Keine Abfrage, ob 'pos == -1' da an dieser Stelle geladen

	if ( show ) {
		tabWidgetMain -> setTabEnabled ( pos, true );
		tabWidgetMain -> setVisible ( true );

	} else {
		// Aendere Ansicht und so...
		tabWidgetMain -> setTabEnabled ( pos, false );
		tabWidgetMain -> setVisible ( false );
	}
}

void MainWindow1::about()
/******************************************************************************
* Methode zeigt das About-Fenster
*******************************************************************************/
{
	QMessageBox::about (
		this,
		tr ( "About %1" ).arg ( AppName ),
		tr ( "%1 is a financiall management tool.\n" ).arg ( AppName ) +
		tr ( "Written by Matthias Ende" )
	);
}

