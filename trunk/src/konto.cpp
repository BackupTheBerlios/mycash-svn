#include "konto.h"
#include "configs.h"

//#include <iostream>
#include <string>
#include <QString>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QObject>
#include <QDataStream>
#include <QTextStream>
#include <QMessageBox>

#include <QtXml> //constains #include <QDomDocument>, #include <QDomText>

Konto::Konto()  //Konstruktor fuer neues Konto
/******************************************************************************
* leerer Konstruktor
*******************************************************************************/
{
	KontoFile = "";
	KontoName = "";
	KontoBeschreibung = "";
	setNotChanged();
}


Konto::Konto ( QString& filename )
/******************************************************************************
* Konstruktor mit DateiNamen, Datei wird geladen
*******************************************************************************/
{
	KontoFile = filename;
	KontoName = "";
	KontoBeschreibung = "";
	loadFileXML ( KontoFile );
	setNotChanged();
}


Konto::Konto ( QString& kontoname, QString& kontobeschreibung, QString& blz, QString& bankname, quint32& kontotyp )
/******************************************************************************
* Kontruktor mit KontoName, KontoBeschreibung, BLZ, BankName und Kontotyp
*******************************************************************************/
{
	KontoFile = "";
	KontoName = kontoname;
	KontoBeschreibung = kontobeschreibung;
	BLZ = blz;
	BankName = bankname;
	KontoTyp = kontotyp;
	setNotChanged();
	setLimitNegativ ( 0.00 );
	setCanBeNegativ ( true );
}

Konto::Konto ( QString kontoname, QString kontobeschreibung, QString blz, QString bankname, quint32 kontotyp, float limit, bool underLimit )
/******************************************************************************
* Kontstruktor mit KontoName, KontoBeschreibung, BLZ, BankName, KontoTyp, Limit, LimitFix
*******************************************************************************/
{	
	KontoFile = "";
	KontoName = kontoname;
	KontoBeschreibung = kontobeschreibung;
	BLZ = blz;
	BankName = bankname;
	KontoTyp = kontotyp;
	setNotChanged();
	setLimitNegativ ( limit );
	setCanBeNegativ ( underLimit );
}


Konto::~Konto()
/******************************************************************************
* Dekonstruktor
*******************************************************************************/
{
	//saveFile();
}


void Konto::setChanged()
/******************************************************************************
* Methode setzt Konto als geaendert
*******************************************************************************/
{
	isChanged = true;
	emit doChange();
}


quint32 Konto::setLimitNegativ ( float limit )
/******************************************************************************
* Methode setzt den Limit
*******************************************************************************/
{
	LimitNegativ = limit;
	return Ok;
}


quint32 Konto::setCanBeNegativ ( bool beNegativ )
/******************************************************************************
* Methode setzt den Limit fix
********************************************************************************/
{
	canBeNegativ = beNegativ;
	return true;
}


float Konto::getLimitNegativ()
/******************************************************************************
* Methode gibt das Limit zurueck
*******************************************************************************/
{
	return LimitNegativ;
}


bool Konto::getCanBeNegativ()
/******************************************************************************
* Methode gibt zurueck, ob Limit unterschritten werden kann
*******************************************************************************/
{
	return canBeNegativ;
}


void Konto::setNotChanged()
/******************************************************************************
* Methode setzt Konto als nicht geaendert
*******************************************************************************/
{
	isChanged = false;
}


QString Konto::getKontoName()
/******************************************************************************
* Methode gibt KontoNamen zurueck
*******************************************************************************/
{
	return KontoName;
}


quint32 Konto::loadFile ( QString filename )
/******************************************************************************
* Methode laed eine Kontodatei(filename)
*******************************************************************************/
{
#ifdef DEBUG
	QTextStream console ( stdout );
#endif

	setNotChanged();

	//Oeffne Datei
	QFile file ( filename );

	if ( !file.open ( QIODevice::ReadOnly ) ) {
#ifdef DEBUG
		console << "\tKonto::loadFile()\t" << "Datei mit Kontoinformationen (" << filename <<
		") konnte nicht geoeffnet werden.\n\r";
#endif
		return WrongFile;
	}

	QTextStream in ( &file );

	in.setCodec ( "UTF-8" );

	int zeilennr = 0;
	QString line;

#ifdef DEBUG
	console << "\n\n" << "========== Einlesen der Kontoinformationen ==========" << "\r\n";
#endif

	while ( ( line = in.readLine() ) != "" ) {
		zeilennr++;

		if ( zeilennr == Kontoinfos ) {
			QStringList list;
			//bool ok;

			list = line.split ( "|" );
			KontoName = list.at ( 0 );
			KontoBeschreibung = list.at ( 1 );
			BLZ = list.at ( 2 );
			BankName = list.at ( 3 );
			KontoTyp = list.at ( 4 ).toInt();

		} else {
			KontoEntry tempEntry /*= new KontoEntry()*/;
			KontoSplitt tempSplitt;
			quint32 zaehler = 0;
			QStringList list;
			QStringList::Iterator it;
			quint32 anzahl;

#ifdef DEBUG
			console << "Eintrag :" << zeilennr - 1 << "\n\r";
			console << "Line: '" << line << "'\r\n";
#endif

			list = line.split ( "|" );

			for ( it = list.begin(); it != list.end(); it++ ) {
				zaehler++;

				if ( zaehler == SpalteDatum ) {
					QDate tempDate;
					tempDate.fromString ( *it, "yyyyMMdd" );
					//tempEntry.changeDatum(tempDate);
					tempEntry.changeDatum ( *it );

#ifdef DEBUG
					console << "\t" << "Datum: " << *it << "\n\r";
#endif
				}

				if ( zaehler == SpalteVerwendung ) {
					tempEntry.changeVerwendung ( *it );

#ifdef DEBUG
					console << "\t" << "Verwendung: " << *it << "\n\r";
#endif
				}

				if ( zaehler == SpalteAnzahl ) {
					bool ok;
					anzahl = it -> toUInt ( &ok, 10 );

#ifdef DEBUG
					console << "\t" << "Anzahl :" << *it << "\n\r";
#endif
				}

				if ( ( zaehler == SpalteSplittKategorie ) || ( zaehler == SpalteSplittVerwendung ) || ( zaehler == SpalteSplittBetrag ) ) {
					if ( zaehler == SpalteSplittKategorie ) {
						bool ok;
						tempSplitt.changeKategorie ( it -> toUInt ( &ok, 10 ) );
					}

					if ( zaehler == SpalteSplittVerwendung ) {
						tempSplitt.changeVerwendung ( *it );
					}

					if ( zaehler == SpalteSplittBetrag ) {
						bool ok;
						tempSplitt.changeBetrag ( it -> toFloat ( &ok ) );
						tempEntry.addSplitt ( &tempSplitt );
						zaehler = SpalteSplittKategorie - 1; // wegen inkrementierung am Anfang Wert um 1 geringer
					}
				}
			}

			tempEntry.list_Entry_deb();

			addEntry ( &tempEntry );
		}
	}// Ende While

	KontoFile = filename;

#ifdef DEBUG
	console << "\tKonto::loadFile()\t" << "Aus Datei mit Kontoinformationen (" << filename <<
	") wurde erfolgreich " << Eintraege.size() << " Eintraege geladen.\n\r";

	console.flush();

#endif

#ifdef DEBUG
	printEntry_deb();

#endif

	return Ok;
}


quint32 Konto::loadFileXML ( QString filename )
/******************************************************************************
* Methode laed eine Kontodatei im XML-Format
*******************************************************************************/
{
	//Filehandler
	QFile file ( filename );

	//Fehlervariablen fuer Parser
	QString errorStr;
	int errorLine;
	int errorColumn;

	//DOM Document
	QDomDocument doc;

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

	if ( root.tagName() != "Konto" ) {
		QMessageBox::warning ( this,
							   tr ( "%1" ).arg ( AppName ),
							   tr ( "This is't a correct KontoFile" )
							 );
		return false;
	}

	//DOM Node
	QDomNode node = root.firstChild();

	while ( !node.isNull() ) {
		QString nodeText = node.toElement().tagName();

		if ( nodeText == "KontoName" ) {
			KontoName = node.toElement().text();
			//QMessageBox::information(this,"XML load", "Kontoname: '" + node/*.firstChild()*/.toElement().text() + "'");

		} else if ( nodeText == "KontoBeschreibung" ) {
			KontoBeschreibung = node.toElement().text();

		} else if ( nodeText == "BLZ" ) {
			BLZ = node.toElement().text();

		} else if ( nodeText == "BankName" ) {
			BankName = node.toElement().text();

		} else if ( nodeText == "KontoTyp" ) {
			KontoTyp = node.toElement().text().toUInt ( 0 );

		} else if ( nodeText == "Eintraege" ) {
			KontoEntry eintrag ( node.toElement() );
			//Abfrage ob erfolgreich, dann in Eintraege

			if ( eintrag ) {
				addEntry ( &eintrag );
			}

		} else if ( nodeText == "RepeatEintraege" ) {

		} else {
			QMessageBox::information ( this,
									   tr ( "%1" ).arg ( AppName ),
									   tr ( "XML Item '%1' ivalid. Item is ignored." ).arg ( nodeText )
									 );
		}

		node = node.nextSibling();
	}

	return true;
}


QString Konto::getKontoFile()
/******************************************************************************
* Methode gibt die aktuelle Kontodatei zurueck
*******************************************************************************/
{
	return KontoFile;
}


Konto::operator bool()
/******************************************************************************
* Ueberladener Bool-Operator
*******************************************************************************/
{
	if ( KontoName.isEmpty() ) {
		return false;

	} else {
		return true;
	}
}


quint32 Konto::saveFile()
/******************************************************************************
* Methode speichert ein Konto
*******************************************************************************/
{
#ifdef DEBUG
	QTextStream console ( stdout );
#endif

	if ( !isChanged ) { // Ueberprüfe ob Aenderung stattgefunden
		return Ok;
	}

	if ( KontoFile == "" ) { // Ueberprüfe ob Datei angegeben
#ifdef DEBUG
		console << "\tKonto::saveFile()\t" << "Keine Kontodatei zum Speichern angegeben\n\r";
#endif
		return WrongFile;
	}

	QFile file ( KontoFile ); // Lege Filehandler an

	if ( !file.open ( QIODevice::WriteOnly ) ) {
#ifdef DEBUG
		console << "\tKonto::saveFile()\t" << "Kontodatei (" << KontoFile << ") konnte nicht geschrieben werden\n\r";
#endif
		return WrongFile;
	}

	QTextStream out ( &file );

	out.setCodec ( "UTF-8" );

	out << KontoName << "|" << KontoBeschreibung << "|" << BLZ << "|" << BankName << "|" << KontoTyp << "\r\n";

#ifdef DEBUG
	console << "Anzahl: " << Eintraege.size() << "\r\n";
#endif

	MapKontoEntry::iterator it;

	for ( it = Eintraege.begin(); it != Eintraege.end(); it++ ) {
		out << it.value().getDatum() << "|";
		out << it.value().getVerwendung() << "|";

		QVector<KontoSplitt> splitt = it.value().getSplittdaten();
		out << splitt.size() << "|";

		QVector<KontoSplitt>::iterator it1;

		for ( it1 = splitt.begin(); it1 !=  splitt.end(); it1++ ) {
			out << it1 -> getKategorie() << "|";
			out << it1 -> getVerwendung() << "|";
			out << it1 -> getBetrag() << "|";
		}

		out << "\r\n";

		out.flush();
	}

#ifdef DEBUG
	console << "\tKonto::saveFile()\t" << "Kontodatei (" << KontoFile << ") wurde erfolgreich geschrieben\n\r";

	console.flush();

#endif

	setNotChanged();

	return Ok;
}


quint32 Konto::saveFileXML()
/******************************************************************************
* Methode speichert ein Konto im XML-Format
*******************************************************************************/
{
	sort();

	const int Indent = 4; //Einrueckung

#ifdef DEBUG
	QTextStream console ( stdout );
#endif

	if ( !isChanged ) { // Ueberprüfe ob Aenderung stattgefunden
		return Ok;
	}

	if ( KontoFile == "" ) { // Ueberprüfe ob Datei angegeben
#ifdef DEBUG
		console << "\tKonto::saveFile()\t" << "Keine Kontodatei zum Speichern angegeben\n\r";
#endif
		return WrongFile;
	}

	QFile file ( KontoFile ); // Lege Filehandler an

	if ( !file.open ( QIODevice::WriteOnly ) ) {
#ifdef DEBUG
		console << "\tKonto::saveFile()\t" << "Kontodatei (" << KontoFile
		<< ") konnte nicht geschrieben werden\n\r";
#endif
		return WrongFile;
	}

	QTextStream out ( &file );
	out.setCodec ( "UTF-8" );

	//XML - Document erzeugen
	QDomDocument doc;

	//Elemente erzeugen
	QDomElement elementKonto = doc.createElement ( "Konto" );
	QDomElement elementName = doc.createElement ( "KontoName" );
	QDomElement elementBeschreibung = doc.createElement ( "KontoBeschreibung" );
	QDomElement elementBlz = doc.createElement ( "BLZ" );
	QDomElement elementBankName = doc.createElement ( "BankName" );
	QDomElement elementKontoTyp = doc.createElement ( "KontoTyp" );

	//Textelemente erzeugen
	QDomText textName = doc.createTextNode ( KontoName );
	QDomText textBeschreibung = doc.createTextNode ( KontoBeschreibung );
	QDomText textBlz = doc.createTextNode ( BLZ );
	QDomText textBankName = doc.createTextNode ( BankName );
	QDomText textKontoTyp = doc.createTextNode ( QString().setNum ( KontoTyp ) );

	//Textelemente mit Unterelementen verbinden
	elementName.appendChild ( textName );
	elementBeschreibung.appendChild ( textBeschreibung );
	elementBlz.appendChild ( textBlz );
	elementBankName.appendChild ( textBankName );
	elementKontoTyp.appendChild ( textKontoTyp );

	//Unterelemente mit Hauptelement verbinden
	elementKonto.appendChild ( elementName );
	elementKonto.appendChild ( elementBeschreibung );
	elementKonto.appendChild ( elementBlz );
	elementKonto.appendChild ( elementBankName );
	elementKonto.appendChild ( elementKontoTyp );

	for ( MapKontoEntry::iterator it = Eintraege.begin(); it != Eintraege.end(); it++ ) {
		//einzelne Eintraege mit Hauptelement verbinden
		elementKonto.appendChild ( it.value().getXmlElement ( doc ) );
	}

	for ( MapRepeatEntry::iterator it = RepeatEintraege.begin(); it != RepeatEintraege.end(); it++ ) {
		elementKonto.appendChild ( it.value().getXmlElement ( doc ) );
	}

	//Baum ans Dokument binden
	doc.appendChild ( elementKonto );

	//Version bestimmen und anhaengen
	QDomNode xmlNode = doc.createProcessingInstruction ( "xml", "version=\"1.0\" encoding=\"UTF-8\"" );

	doc.insertBefore ( xmlNode, doc.firstChild() );

	doc.save ( out, Indent );

	return true;
}


quint32 Konto::setKontoName ( QString KName )
/******************************************************************************
* Methode setzt den KontoNamen
*******************************************************************************/
{
	KontoName = KName;
	setChanged();
	return Ok;
}


quint32 Konto::setKontoBeschreibung ( QString KBez )
/******************************************************************************
* Methode setzt die KontoBeschreibung
*******************************************************************************/
{
	KontoBeschreibung = KBez;
	setChanged();
	return Ok;
}


quint32 Konto::setKontoFile ( QString KFile )
/******************************************************************************
* Methode setzt die Kontodatei
*******************************************************************************/
{
	KontoFile = KFile;
	setChanged();
	return Ok;
}


quint32 Konto::setBLZ ( QString blz )
/******************************************************************************
* Methode setzt die BLZ
*******************************************************************************/
{
	BLZ = blz;
	setChanged();
	return Ok;
}


quint32 Konto::setBankName ( QString bankname )
/******************************************************************************
* Methode setzt den Banknamen
*******************************************************************************/
{
	BankName = bankname;
	setChanged();
	return Ok;
}


quint32 Konto::setKontoTyp ( quint32 kontotyp )
/******************************************************************************
* Methode setzt den KontoTyp
*******************************************************************************/
{
	KontoTyp = kontotyp;
	setChanged();
	return Ok;
}


quint32 Konto::addEntry ( quint32 nummer, KontoEntry *entry )
/******************************************************************************
* Methode fuegt an Stelle nummer einen neuen Eintrag(entry) ein
*******************************************************************************/
{
	if ( nummer == newEntry ) {
		Eintraege[ getFreeNumber() ] = *entry;

	} else {
		Eintraege[ nummer ] = *entry;
	}

	setChanged();

	return Ok;
}


quint32 Konto::addEntry ( KontoEntry *entry ) //???
/******************************************************************************
* Methode fuegt einen Eintrag hinzu - Überladene Funktion für neuen Eintrag
*******************************************************************************/
{
	setChanged();
	return addEntry ( newEntry, entry );
}


quint32 Konto::deleteEntry ( quint32 entry )
/******************************************************************************
* Methode löschen einen Eintrag(entry)
*******************************************************************************/
{
	MapKontoEntry::iterator it;

	for ( it = Eintraege.begin(); it != Eintraege.end(); it++ ) {
		if ( it.key() == entry ) {
			Eintraege.erase ( it );
			setChanged();
			return Ok;
		}
	}

	return NotFound;
}


quint32 Konto::deleteSplitt ( quint32 entry, quint32 splittnummer )
/******************************************************************************
* Methode loescht einen Splitteintrag(splittnummer) eines Eintrages(entry)
*******************************************************************************/
{
	MapKontoEntry::iterator it;

	for ( it = Eintraege.begin(); it != Eintraege.end(); it++ ) {
		if ( it.key() == entry ) {
			if ( it.value().deleteSplitt ( splittnummer ) ) {
				setChanged();
			}

			return Ok;
		}
	}

	return NotFound;
}


quint32 Konto::changeDatum ( QString datum, quint32 entry )
/******************************************************************************
* Methode aendert das Datum eines Eintrages(entry) - Datum als String
*******************************************************************************/
{
	MapKontoEntry::iterator it;

	for ( it = Eintraege.begin(); it != Eintraege.end(); it++ ) {
		if ( it.key() == entry ) {
			if ( it.value().changeDatum ( datum ) ) {
				setChanged();
				return Ok;
			}

			break;
		}
	}

	return NotFound;
}


quint32 Konto::changeDatum ( QDate datum, quint32 entry )
/******************************************************************************
* Methode aendert das Datum eines Eintrages(entry) - Datum als Klasse
*******************************************************************************/
{
	MapKontoEntry::iterator it;

	for ( it = Eintraege.begin(); it != Eintraege.end(); it++ ) {
		if ( it.key() == entry ) {
			if ( it.value().changeDatum ( datum ) ) {
				setChanged();
				return Ok;
			}

			break;
		}
	}

	return NotFound;
}


quint32 Konto::changeVerwendung ( QString verwendung, quint32 entry )
/******************************************************************************
* Methode aendert die Verwendung eines Eintrages(entry)
*******************************************************************************/
{
	MapKontoEntry::iterator it;

	for ( it = Eintraege.begin(); it != Eintraege.end(); it++ ) {
		if ( it.key() == entry ) {
			if ( it.value().changeVerwendung ( verwendung ) ) {
				setChanged();
				return Ok;
			}

			break;
		}
	}

	return NotFound;
}


quint32 Konto::changeVerwendung ( QString verwendung, quint32 entry, quint32 nummer )
/******************************************************************************
*  Methode aendert die Verwendung eines Postens(nummer) eines Eintrages(entry)
*******************************************************************************/
{
	MapKontoEntry::iterator it;

	for ( it = Eintraege.begin(); it != Eintraege.end(); it++ ) {
		if ( it.key() == entry ) {
			if ( it.value().changeVerwendung ( verwendung, nummer ) ) {
				setChanged();
				return Ok;
			}

			break;
		}
	}

	return NotFound;
}


quint32 Konto::changeKategorie ( quint32 kategorie, quint32 entry, quint32 nummer )
/******************************************************************************
*  Methode aendert die Kategorie eines Postens(nummer) eines Eintrages(entry)
*******************************************************************************/
{
	MapKontoEntry::iterator it;

	for ( it = Eintraege.begin(); it != Eintraege.end(); it++ ) {
		if ( it.key() == entry ) {
			if ( it.value().changeKategorie ( kategorie, nummer ) ) {
				setChanged();
				return Ok;
			}

			break;
		}
	}

	return NotFound;
}


quint32 Konto::changeBetrag ( float betrag, quint32 entry, quint32 nummer )
/******************************************************************************
* Methode aendert den Betrag eines Postens(nummer) eines Eintrages(entry)
*******************************************************************************/
{
	MapKontoEntry::iterator it;

	for ( it = Eintraege.begin(); it != Eintraege.end(); it++ ) {
		if ( it.key() == entry ) {
			if ( it.value().changeBetrag ( betrag, nummer ) ) {
				setChanged();
				return Ok;
			}

			break;
		}
	}

	return NotFound;
}


quint32 Konto::getFreeNumber()
/******************************************************************************
* Methode gibt eine freie Number der Eintraege zurueck, Luecken werden ignoriert
*******************************************************************************/
{
#ifdef DEBUG
	QTextStream console ( stdout );
#endif

	MapKontoEntry::iterator it;
	it = Eintraege.end();

	if ( it == Eintraege.begin() ) {
		return 1;
	}

	it--;

#ifdef DEBUG
	console.flush();
#endif

	return it.key() + 1;
}


float Konto::getBetragKategorie ( quint32 kategorie )
/******************************************************************************
* Methode gibt den Gesamtbetrag einer Kategorie zurueck
* Parameter fuer alles  EntryKategorie::ALLE
*******************************************************************************/
{
	float summe = 0.0;
	MapKontoEntry::iterator it;

	for ( it = Eintraege.begin(); it != Eintraege.end(); it++ ) {
		summe += it.value().getBetragKategorie ( kategorie );
	}

	return summe;
}


Konto::VectorHistoryList Konto::getHistoryList()
/******************************************************************************
*
*******************************************************************************/
{
	VectorHistoryList result;
	
	MapKontoEntry::iterator it;
	for(it = Eintraege.begin(); it != Eintraege.end(); it++){
		HistoryList temp;
		temp.Datum = it -> getDatum();
		temp.Verwendung = it -> getVerwendung();
		temp.Transfer = it -> getTransfer();
		temp.Betrag = it -> getBetrag();
		
		result.push_back( temp );
	}
	return result;
}


float Konto::getBetragKategorieIntervall ( quint32 kategorie, QDate von, QDate bis )
/******************************************************************************
* Methode gibt den Gesamtbetrag der Eintraege einer Kategorie in einen bestimmten Zeitraum zurueck
* Parameter fuer alles  EntryKategorie::ALLE
*******************************************************************************/
{
	float summe = 0.0;
	MapKontoEntry::iterator it;

	for ( it = Eintraege.begin(); it != Eintraege.begin(); it++ ) {
		summe += it.value().getBetragKategorieIntervall ( kategorie, von, bis );
	}

	return summe;
}


bool Konto::isNotChanged()
/******************************************************************************
* Methode gibt zurueck, ob Konto nicht geaendert wurde
*******************************************************************************/
{
	return !isChanged;
}


quint32 Konto::printEntry_deb() //DEBUG-Funktion
/******************************************************************************
* Methode listet die Kontoeintraege in der Konsole auf
*******************************************************************************/
{
	QTextStream console ( stdout );

	MapKontoEntry::iterator it;

	console << "\n\n========== UEBERSICHT DER KONTOEINTRAEGE ==========" << "\n\r";

	for ( it = Eintraege.begin(); it != Eintraege.end(); it++ ) {
		console << it.key() << ". Eintrag";
		console << "\t" << "Datum: " << it.value().getDatum();
		console << "\t" << "Verwendung: " << it.value().getVerwendung();
		console << "\n\r";

		QVector<KontoSplitt> splitt = it.value().getSplittdaten();
		QVector<KontoSplitt>::iterator it1;

		for ( it1 = splitt.begin(); it1 != splitt.end(); it1++ ) {
			console << "\t\t" << it1 -> getKategorie() << "\t" << it1 -> getVerwendung() << "\t" << it -> getBetrag() << "\n\r";
		}

		console << "\n";
	}

	console.flush();

	return Ok;
}


Konto::KontoSettings Konto::getFormKontoSettings()
/******************************************************************************
* Methode gibt ein Struct mit Kontoinformationen zurueck
*******************************************************************************/
{
	KontoSettings temp;

	temp.KontoName = KontoName;
	temp.KontoBeschreibung = KontoBeschreibung;
	temp.BLZ = BLZ;
	temp.BankName = BankName;
	temp.KontoTyp = KontoTyp;
	temp.FileName = KontoFile;
	temp.Limit = LimitNegativ;
	temp.canBeNegativ = canBeNegativ;

	return temp;
}


Konto::HistoryListDetails Konto::getEntryDetails(quint32 entrynummer)
/******************************************************************************
* Methode gibt ein Struct mit KontoEintragsInformationen zurueck
*******************************************************************************/
{
	HistoryListDetails temp;
	if( Eintraege.contains( entrynummer ) ){
		temp.Entry = entrynummer;
		temp.Datum = Eintraege[ entrynummer ].getDatum();
		temp.Shop = Eintraege[ entrynummer ].getShop();
		temp.Verwendung = Eintraege[ entrynummer ].getVerwendung();
		temp.Transfer = Eintraege[ entrynummer ].getTransfer();
		temp.Betrag = Eintraege[ entrynummer ].getBetrag();
		temp.Splitts = Eintraege[ entrynummer ].getSplittdaten();
	}else{
		temp.Entry = 0;
		temp.Datum = "";
		temp.Shop = 0;
		temp.Verwendung = "";
		temp.Transfer = 0;
		temp.Betrag = 0.0;
		VectorSplitt tempS;
		temp.Splitts = tempS;
	}
	return temp;
}


void Konto::sort(){
	//qSort( Eintraege.begin(), Eintraege.end() );
	bool sorted = false;
	while( sorted == false ){
		sorted = true;

		for(MapKontoEntry::iterator it = Eintraege.begin(); it != Eintraege.end(); it++){
			MapKontoEntry::iterator it1 = (it + 1);
			if(it1 == Eintraege.end() ){
				break;
			}
			QString date1 = it.value().getDatum();
			QString date2 = it1.value().getDatum();
			if( date1 > date2 ){
				sorted = false;
				KontoEntry temp = it1.value();
				it1.value() = it.value();
				it.value() = temp;
				break;
			}
		}

	}
}


void Konto::printKontoInfos() //DEBUG-Funktion
/******************************************************************************
* Methode gibt Kontoinformationen auf der Konsole aus
*******************************************************************************/
{
	QTextStream console ( stdout );
	console << "Kontoinfo:" << "\n\r" << "==========" << "\n\r";
	console << "KontoName: '" << KontoName << "'\n\r";
	console << "KontoBeschreibung: '" << KontoBeschreibung << "'\n\r";
	console << "BLZ: '" << BLZ << "'\n\r";
	console << "BankName: '" << BankName << "'\n\r" << "\n\r";
	console << "KontoTyp: " << KontoTyp << "\n\r";
	console << "KontoFile: '" << KontoFile << "'\n\r";
	console.flush();
}


void Konto::showKontoData_deb() //DEBUG-Funktion
/******************************************************************************
* Methode gibt eine MessageBox mit Kontoinformationen aus
*******************************************************************************/
{
#ifdef DEBUG
	QTextStream console ( stdout );
	console << "\tKonto::showKontoData_deb()\t" << "Debuginfo wird angezeigt...\n\r";
	console.flush();
#endif

	QString str;
	QMessageBox::information ( 0,
							   "Kontoinfos",
							   "KontoFile: '" + KontoFile + "'\nKontoName: '" + KontoName + "'\nKontoBeschreibung: '" +
							   KontoBeschreibung + "'\nBLZ: '" + BLZ + "'\nBankName: '" + BankName + "'\nKontoTyp: '" +
							   str.setNum ( KontoTyp ) + "'",
							   QMessageBox::Yes | QMessageBox::Default
							 );
}
