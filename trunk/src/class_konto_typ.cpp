#include "class_konto_typ.h"
#include "configs.h"
#include <vector>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QtGlobal>
#include <QMap>

const char seperator = '|';


/*
	Konto_Typ()
	Konstrukter
*/	
Konto_Typ::Konto_Typ(){
	#ifdef DEBUG	// Erzeuge fuer Debugfall ein Textstream auf Konsole
		QTextStream console(stdout);
	#endif

	QFile file(KTypFile);	//Lege ein Dateihandler mit Dateinamen KTypFile an
	if(!file.open(QIODevice::ReadOnly)){	
		#ifdef DEBUG
			console << "\tKonto_Typ::Konto_Typ()\t" << "Datei mit Kontotypen (" << KTypFile << ") konnte nicht geladen werden.\n\r";  
		#endif
		return;		//Beende Konstruktor, wenn Datei nicht geladen werden konnte
	}
	QTextStream in(&file); // Erzeuge TextStream
	in.setCodec("UTF-8");  // Kodierung: UTF-8
	
	typen.clear(); // Lösche alle KontoTypen
	
	while(!in.atEnd()){	// Lese solange Stream nicht am Ende
		QString tmp = in.readLine(); //Lese Zeile ein
		if(tmp.size() > 0){ //Überprüfe ob kein Nullstring
			QStringList list = tmp.split(seperator, QString::SkipEmptyParts); //Zerlege String
			if(list.size() == 3){ //Überprüfe, ob richtige Anzahl
				KTypS tempS; //temporärer Struct
				//Weise Struct Werte zu
				tempS.Bezeichnung = list.at(1);
				tempS.Options = list.at(2).toUInt();
				//Füge Kontotyp ein
				typen.insert(list.at(0).toUInt(),tempS);
			}	
		}
	}
	
	isChanged = false;
}


/*
	~Konto_Typ()
	Destrukter
*/
Konto_Typ::~Konto_Typ(){
	saveTyp();
}


/*
	void saveTyp()
	Methode speichert die Kontotypen
*/
void Konto_Typ::saveTyp(){
	#ifdef DEBUG	// Erzeuge fuer Debugfall ein Textstream auf Konsole
		QTextStream console(stdout);
	#endif

	// Ueberprüfe ob Änderungen vorgenommen
	if(isChanged == false){
		return;
	}
	
	QFile file(KTypFile);
	if(!file.open(QIODevice::WriteOnly)){
		#ifdef DEBUG
			console << "\tKonto_Typ::saveTyp()\t" << "Datei mit Kontotypen (" << KTypFile << ") konnte nicht beschrieben werden.\n\r"; 
		#endif
		return;
	}
	QTextStream out(&file);
	out.setCodec("UTF-8");
	
	for(MapTyp::iterator it = typen.begin(); it != typen.end(); it++){
		out << it.key()  << seperator <<  it.value().Bezeichnung << seperator << it.value().Options << "\r\n";
	}
	
	isChanged = false;
	return;
}


/*
	void newTyp(QString Bezeichnung, quint32 Option)
	Methode legt einen neuen Kontotyp an
	QString Bezeichung:	Bezeichnung des Kontotyps
	quint32 Option:		Optionen des zuerstellenden Kontotyps
*/
void Konto_Typ::newTyp(QString Bezeichnung, quint32 Option){
	#ifdef DEBUG	// Erzeuge fuer Debugfall ein Textstream auf Konsole
		QTextStream console(stdout);
	#endif

	// Wenn keine Bezeichnung eingegeben beende Funktion
	if(Bezeichnung == ""){
		#ifdef DEBUG	//Debugausgabe
			console << "\tKonto_Typ::newTyp()\t" << "Keine Bezeichnung fuer neuen Kontotyp eingegeben.\n\r";
		#endif
		return;
	}
	//Durchsuche ob schon vorhanden
	MapTyp::iterator it;
	for(it = typen.begin(); it != typen.end(); it++){
		if(it.value().Bezeichnung == Bezeichnung){
			#ifdef DEBUG	//Debugausgabe
				console <<	"\tKonto_Typ::newTyp()\t" << "Eintrag '" << Bezeichnung <<
							"' schon vorhanden. Kontotyp wird nicht angelegt.\n\r";
			#endif
			return;
		}
	}
	KTypS tempS;
	tempS.Bezeichnung = Bezeichnung;
	tempS.Options = Option;
	typen.insert(getFreeNumber(), tempS);

	isChanged = true;
	
	return;
}


/*
	uint32 getFreeNumber()
	Funktion gibt die naechste freie Nummer zurueck; freie Nummern zwischen zwei Typen werden ignoriert
*/
quint32 Konto_Typ::getFreeNumber(){
	if( typen.isEmpty() ){
		return 1;
	}else{
		MapTyp::iterator it = typen.end();
		it--;
		return (it.key() + 1);
	}
}


/*
	quint32 getTypNummer(QString Bezeichnung)
	Methode gibt die Typennummer zu einer Bezeichnung zurueck
*/
quint32 Konto_Typ::getTypNummer(QString TypBezeichnung){
	MapTyp::iterator it;
	for(it=typen.begin(); it != typen.end(); it++){
		if(it.value().Bezeichnung == TypBezeichnung){
			return it.key();
		}
	}
	return Konto_Typ::NotFound;
}


/*
	QString getTypBezeichnung(quint32 Nummer)
	Methode liefert die Bezeichung zu einen Kontotypen zurueck
*/
QString Konto_Typ::getTypBezeichnung(quint32 Nummer){
	KTypS tempS;
	tempS.Bezeichnung = ""; // Options wird nicht benötigt.
	return typen.value(Nummer, tempS).Bezeichnung;
}


/*
	QVector<QString> listTypen()
	Methode gibt alle Kontotypen zurueck
*/
QVector<QString> Konto_Typ::listTypen(){
	QVector<QString> kontotypen;
	MapTyp::iterator it;
	for(it=typen.begin(); it != typen.end(); it++){
		kontotypen.push_back(it.value().Bezeichnung);
	}
	return kontotypen;
}
