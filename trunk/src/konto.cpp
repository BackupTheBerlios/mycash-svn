#include "konto.h"
#include "configs.h"
#include <iostream>
#include <string>
#include <QString>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QObject>
#include <QDataStream>
#include <QTextStream>
#include <QMessageBox>

Konto::Konto(){ //Konstruktor fuer neues Konto
	KontoFile = "";
	KontoName = "";
	KontoBeschreibung = "";
	setNotChanged();
}


Konto::Konto(QString filename){
	KontoFile = filename;
	KontoName = "";
	KontoBeschreibung = "";
	loadFile(filename);
	setNotChanged();
}


Konto::Konto(QString kontoname, QString kontobeschreibung, QString blz, QString bankname, quint32 kontotyp){
	KontoFile = "";
	KontoName = kontoname;
	KontoBeschreibung = kontobeschreibung;
	BLZ = blz;
	BankName = bankname;
	KontoTyp = kontotyp;
	setNotChanged();	
}


Konto::~Konto(){
	//saveFile();
}


void Konto::setChanged(){
	isChanged = true;
	emit doChange();
}


void Konto::setNotChanged(){
	isChanged = false;
}


QString Konto::getKontoName(){
	return KontoName;
}


quint32 Konto::loadFile(QString filename){
	#ifdef DEBUG
		QTextStream console(stdout);
	#endif

	setNotChanged();

	//Oeffne Datei
	QFile file(filename);
	if(!file.open(QIODevice::ReadOnly)){
		#ifdef DEBUG
			console << "\tKonto::loadFile()\t" << "Datei mit Kontoinformationen (" << filename << ") konnte nicht geoeffnet werden.\n\r";
		#endif
		return WrongFile;
	}

	QTextStream in( &file );
	in.setCodec("UTF-8");

	int zeilennr = 0;
	QString line;
	
	#ifdef DEBUG
		console << "\n\n" << "========== Einlesen der Kontoinformationen ==========" << "\r\n";
	#endif
	
	while( (line=in.readLine()) != "" ){
		zeilennr++;

		if(zeilennr == Kontoinfos){
			QStringList list;
			//bool ok;

			list = line.split("|");
			KontoName = list.at(0);
			KontoBeschreibung = list.at(1);
			BLZ = list.at(2);
			BankName = list.at(3);
			KontoTyp = list.at(4).toInt();
		}else{
			KontoEntry tempEntry /*= new KontoEntry()*/;
			Konto_Splitt tempSplitt;
			quint32 zaehler = 0;
			QStringList list;
			QStringList::Iterator it;
			quint32 anzahl;	

			#ifdef DEBUG
				console << "Eintrag :" << zeilennr - 1 << "\n\r";
				console << "Line: '" << line << "'\r\n";
			#endif

			list = line.split("|");
			for(it = list.begin(); it != list.end(); it++){
				zaehler++;
				if(zaehler == SpalteDatum){
					QDate tempDate;
					tempDate.fromString(*it, "yyyyMMdd");
					//tempEntry.changeDatum(tempDate);
					tempEntry.changeDatum(*it);

					#ifdef DEBUG
						console << "\t" << "Datum: " << *it << "\n\r"; 
					#endif
				}
				if(zaehler == SpalteVerwendung){
					tempEntry.changeVerwendung(*it);

					#ifdef DEBUG
						console << "\t" << "Verwendung: " << *it << "\n\r";
					#endif
				}
				if(zaehler == SpalteAnzahl){
					bool ok;
					anzahl = it -> toUInt(&ok,10);

					#ifdef DEBUG
						console << "\t" << "Anzahl :" << *it << "\n\r";
					#endif
				}
				if( (zaehler == SpalteSplittKategorie) || (zaehler == SpalteSplittVerwendung) || (zaehler == SpalteSplittBetrag)){
					if(zaehler == SpalteSplittKategorie){
						bool ok;
						tempSplitt.changeKategorie( it -> toUInt(&ok,10) );
					}
					if(zaehler == SpalteSplittVerwendung){
						tempSplitt.changeVerwendung( *it );
					}
					if(zaehler == SpalteSplittBetrag){
						bool ok;
						tempSplitt.changeBetrag( it -> toFloat(&ok) );
						tempEntry.addSplitt(&tempSplitt);
						zaehler = SpalteSplittKategorie - 1;	// wegen inkrementierung am Anfang Wert um 1 geringer
					}
				}
			}
			tempEntry.list_Entry_deb();
			addEntry(&tempEntry);
		}
	}// Ende While
	
	KontoFile = filename;

	#ifdef DEBUG
		console << "\tKonto::loadFile()\t" << "Aus Datei mit Kontoinformationen (" << filename << ") wurde erfolgreich " <<
				Eintraege.size() << " Eintraege geladen.\n\r";
		console.flush();
	#endif
	
	#ifdef DEBUG
		printEntry_deb();
	#endif

	return Ok;
}


QString Konto::getKontoFile(){
	return KontoFile;
}


Konto::operator bool(){
	if(KontoFile.isEmpty()){
		return false;
	}else{
		return true;
	}
}


quint32 Konto::saveFile(){
	#ifdef DEBUG
		QTextStream console(stdout);
	#endif

	if( !isChanged ){	// Ueberprüfe ob Aenderung stattgefunden
		return Ok;
	}

	if(KontoFile == ""){	// Ueberprüfe ob Datei angegeben
		#ifdef DEBUG
			console << "\tKonto::saveFile()\t" << "Keine Datei zum Speichern angegeben\n\r";
		#endif
		return WrongFile;
	}
	QFile file(KontoFile);	// Lege Filehandler an
	if(!file.open(QIODevice::WriteOnly)){
		#ifdef DEBUG
			console << "\tKonto::saveFile()\t" << "Datei (" << KontoFile << ") konnte nicht geschrieben werden\n\r";
		#endif
		return WrongFile;
	}

	QTextStream out(&file);
	out.setCodec("UTF-8");
	
	out << KontoName << "|" << KontoBeschreibung << "|" << BLZ << "|" << BankName << "|" << KontoTyp << "\r\n";

	#ifdef DEBUG
		console << "Anzahl: " << Eintraege.size() << "\r\n";
	#endif

	MapKontoEntry::iterator it;
	for(it = Eintraege.begin(); it != Eintraege.end(); it++){
		out << it.value().getDatum() << "|";
		out << it.value().getVerwendung() << "|";
		
		QVector<Konto_Splitt> splitt = it.value().getSplittdaten();
		out << splitt.size() << "|";
		
		QVector<Konto_Splitt>::iterator it1;
		for(it1 = splitt.begin(); it1 !=  splitt.end(); it1++){
			out << it1 -> getKategorie() << "|";
			out << it1 -> getVerwendung() << "|";
			out << it1 -> getBetrag() << "|"; 
		}		

		out << "\r\n";
		out.flush();
	}

	#ifdef DEBUG
		console << "\tKonto::saveFile()\t" << "Datei (" << KontoFile << ") wurde erfolgreich geschrieben\n\r";
	#endif

	setNotChanged();
	return Ok;	
}


quint32 Konto::setKontoName(QString KName){
	KontoName = KName;
	setChanged();
	return Ok;
}


quint32 Konto::setKontoBezeichnung(QString KBez){
	KontoBeschreibung = KBez;
	setChanged();
	return Ok;
}


quint32 Konto::setKontoFile(QString KFile){
	KontoFile = KFile;
	setChanged();
	return Ok;
}


quint32 Konto::setBLZ(QString blz){
	BLZ = blz;
	setChanged();
	return Ok;
}


quint32 Konto::setBankName(QString bankname){
	BankName = bankname;
	setChanged();
	return Ok;
}


quint32 Konto::setKontoTyp(quint32 kontotyp){
	KontoTyp = kontotyp;
	setChanged();
	return Ok;
}


quint32 Konto::addEntry(quint32 nummer, KontoEntry *entry){
	if(nummer == newEntry){
		Eintraege[ getFreeNumber() ] = *entry;
	}
	setChanged();
	return Ok;
}


quint32 Konto::addEntry(KontoEntry *entry){
	setChanged();
	return addEntry(newEntry, entry);
}


quint32 Konto::deleteEntry(quint32 nummer){
	MapKontoEntry::iterator it;
	for(it = Eintraege.begin(); it != Eintraege.end(); it++){
		if( it.key() == nummer){
			Eintraege.erase(it);
			setChanged();
			return Ok;
		}
	}
	return NotFound;
}


quint32 Konto::deleteSplitt(quint32 nummer, quint32 splittnummer){
	MapKontoEntry::iterator it;
	for(it = Eintraege.begin(); it != Eintraege.end(); it++){
		if( it.key() == nummer ){
			if( it.value().deleteSplitt(splittnummer) ){
				setChanged();
			}
			return Ok;
		}
	}
	return NotFound;
}


quint32 Konto::changeDatum(QString datum, quint32 entry){
	MapKontoEntry::iterator it;
	for(it = Eintraege.begin(); it != Eintraege.end(); it++){
		if(it.key() == entry){
			if( it.value().changeDatum(datum) ){
				setChanged();
				return Ok;
			}
			break;
		}
	}
	return NotFound;
}


quint32 Konto::changeDatum(QDate datum, quint32 entry){
	MapKontoEntry::iterator it;
	for(it = Eintraege.begin(); it != Eintraege.end(); it++){
		if(it.key() == entry){
			if( it.value().changeDatum(datum) ){
				setChanged();
				return Ok;
			}
			break;
		}
	}
	return NotFound;
}


quint32	Konto::changeVerwendung(QString verwendung, quint32 entry){
	MapKontoEntry::iterator it;
	for(it = Eintraege.begin(); it != Eintraege.end(); it++){
		if(it.key() == entry){
			if( it.value().changeVerwendung(verwendung) ){
				setChanged();
				return Ok;
			}
			break;
		}
	}
	return NotFound;
}


quint32 Konto::changeVerwendung(QString verwendung, quint32 entry, quint32 nummer){
	MapKontoEntry::iterator it;
	for(it = Eintraege.begin(); it != Eintraege.end(); it++){
		if(it.key() == entry){
			if( it.value().changeVerwendung(verwendung, nummer) ){
				setChanged();
				return Ok;
			}
			break;
		}
	}
	return NotFound;
}


quint32 Konto::changeKategorie(quint32 kategorie, quint32 entry, quint32 nummer){
	MapKontoEntry::iterator it;
	for(it = Eintraege.begin(); it != Eintraege.end(); it++){
		if(it.key() == entry){
			if( it.value().changeKategorie(kategorie, nummer) ){
				setChanged();
				return Ok;
			}
			break;
		}
	}
	return NotFound;
}


quint32 Konto::changeBetrag(float betrag, quint32 entry, quint32 nummer){
	MapKontoEntry::iterator it;
	for(it = Eintraege.begin(); it != Eintraege.end(); it++){
		if(it.key() == entry){
			if( it.value().changeBetrag(betrag, nummer) ){
				setChanged();
				return Ok;
			}
			break;
		}
	}
	return NotFound;
}


quint32 Konto::getFreeNumber(){
	#ifdef DEBUG
		QTextStream console(stdout);
	#endif

	MapKontoEntry::iterator it;
	it = Eintraege.end();
	if( it == Eintraege.begin() ){
		return 1;
	}
	it--;
	
	#ifdef DEBUG
		console.flush();
	#endif

	return it.key() + 1;
}


float Konto::getBetragKategorie(quint32 kategorie){
	float summe = 0.0;
	MapKontoEntry::iterator it;
	for(it = Eintraege.begin(); it != Eintraege.end(); it++){
		summe += it.value().getBetragKategorie(kategorie);
	}
	return summe;
}


float Konto::getBetragKategorieIntervall(quint32 kategorie, QDate von, QDate bis){
	float summe = 0.0;
	MapKontoEntry::iterator it;
	for(it = Eintraege.begin(); it != Eintraege.begin(); it++){
		summe += it.value().getBetragKategorieIntervall(kategorie, von, bis);
	}
	return summe;
}


bool Konto::isNotChanged(){
	return !isChanged;
}


quint32 Konto::printEntry_deb(){
	QTextStream console(stdout);
	
	MapKontoEntry::iterator it;

	console << "\n\n========== UEBERSICHT DER KONTOEINTRAEGE ==========" << "\n\r";

	for(it = Eintraege.begin(); it != Eintraege.end(); it++){
		console << it.key() << ". Eintrag";
		console << "\t" << "Datum: " << it.value().getDatum();
		console << "\t" << "Verwendung: " << it.value().getVerwendung();
		console << "\n\r";

		QVector<Konto_Splitt> splitt = it.value().getSplittdaten();
		QVector<Konto_Splitt>::iterator it1;
		for(it1 = splitt.begin(); it1 != splitt.end(); it1++){
			console << "\t\t" << it1 -> getKategorie() << "\t" << it1 -> getVerwendung() << "\t" << it -> getBetrag() << "\n\r";
		}
		console << "\n";
	}
	console.flush();
	return Ok;
}


void Konto::showKontoData_deb(){
	QTextStream console(stdout);
	
	#ifdef DEBUG
		console << "\tKonto::showKontoData_deb()\t" << "Debuginfo wird angezeigt...\n\r";
		console.flush();
	#endif

	QString str;
	QMessageBox::information(0, "Kontoinfos",
						"KontoFile: '" + KontoFile + "'\nKontoName: '" + KontoName + "'\nKontoBezeichnung: '" + KontoBeschreibung +
						"'\nBLZ: '" + BLZ + "'\nBankName: '" + BankName + "'\nKontoTyp: '" + str.setNum(KontoTyp) + "'",
						QMessageBox::Yes | QMessageBox::Default);
}
