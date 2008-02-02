#include "EntryKategorie.h"
#include "configs.h"
#include <QString>
#include <QStringList>
#include <QMap>
#include <QTextStream>
#include <QFile>


EntryKategorie::EntryKategorie(){

}


EntryKategorie::EntryKategorie(QString file){
	if(setFile(file) == Ok){
		load();
	}
}


EntryKategorie::~EntryKategorie(){
	save();
}


quint32 EntryKategorie::load(){
	#ifdef DEBUG
		QTextStream console( stdout );
	#endif
	QFile file( Filename );
	if(!file.open(QIODevice::ReadOnly)){
		#ifdef DEBUG
			console << "\t" << "EntryKategorie::load(): " << "Dateiname (" << Filename << ") ungueltig" << "\n\r";
		#endif
		return WrongFile;
	}

	QTextStream in( &Filename );
	in.setCodec("UTF-8");
	
	QString line;
	while( (line = in.readLine()) != "" ){
		QStringList list;
		list = line.split("|");
		if( list.size() != 4){
			#ifdef DEBUG
				console << "\t" << "EntryKategorie::load(): " << "Zeile '" << line << "' ungueltig" << "\n\r";
			#endif
		}
		bool ok;
		KategorieItems tempStruct;
		tempStruct.Kategorie = list.at(1);
		tempStruct.Beschreibung = list.at(2);
		tempStruct.Typ = list.at(3).toUInt(&ok, 10);

		Kategorien[ list.at(0).toUInt(&ok, 10) ] = tempStruct;
	}
	
	#ifdef DEBUG
		console << "\t" << "EntryKategorie::load(); " << Kategorien.size() << " Eintraege erfolgreich eingelesen." << "\n\r";
	#endif

	return Ok;
}


quint32 EntryKategorie::load(QString file){
	if(setFile(file) == Ok){
		load();
	}else{
		return WrongFile;
	}

	return Ok;
}


quint32 EntryKategorie::save(){
	#ifdef DEBUG
		QTextStream console(stdout);
		quint32 anzahl = 0;
	#endif

	MapKategorie::iterator it;
	QFile file( Filename );
	if(!file.open(QIODevice::WriteOnly)){
		#ifdef DEBUG
			console << "\t" << "EntryKategorie::save(): " << "Ungueltiger Dateiname (" << Filename << ")" << "\n\r";
		#endif
		return WrongFile;
	}

	QTextStream out( &file );
	out.setCodec("UTF-8");

	for(it = Kategorien.begin(); it != Kategorien.end(); it++){
		out << it.key() << "|" << it.value().Kategorie << "|" << it.value().Beschreibung << "|" << it.value().Typ << "\n\r";
		#ifdef DEBUG
			anzahl++;
		#endif
	}
	out.flush();
	#ifdef DEBUG
		console << "\t" << "EntryKategorie::save(): " << anzahl << " Eintraege gespeichert" << "\n\r";
	#endif	

	return Ok;
}

		
quint32 EntryKategorie::setFile(QString file){
	Filename = file;
	return Ok;
}


quint32 EntryKategorie::getFreeNumber(){
	MapKategorie::iterator it = Kategorien.end();
	if( it == Kategorien.begin() ){
		return 1;
	}

	it--;

	return it.key() + 1;
}


quint32 EntryKategorie::addKategorie(QString kategorie, QString beschreibung, quint32 typ){
	KategorieItems tempStruct;
	tempStruct.Kategorie = kategorie;
	tempStruct.Beschreibung = beschreibung;
	tempStruct.Typ = typ;
	
	Kategorien[ getFreeNumber() ] = tempStruct;
	
	return Ok;
}


quint32 EntryKategorie::deleteKategorie(quint32 nummer){
	MapKategorie::iterator it;
	for(it = Kategorien.begin(); it != Kategorien.end(); it++){
		if( it.key() == nummer ){
			Kategorien.erase( it );
			return Ok;
		}
	}

	return NotFound;
}

		
QString EntryKategorie::getKategorie(quint32 nummer){
	MapKategorie::iterator it;
	for(it = Kategorien.begin(); it != Kategorien.end(); it++){
		if( it.key() == nummer){
			return it.value().Kategorie;
		}
	}

	return "";
}


QString EntryKategorie::getBeschreibung(quint32 nummer){
	MapKategorie::iterator it;
	for(it = Kategorien.begin(); it != Kategorien.end(); it++){
		if( it.key() == nummer){
			return it.value().Beschreibung;
		}
	}

	return "";
}


quint32 EntryKategorie::getTyp(quint32 nummer){
	MapKategorie::iterator it;
	for(it = Kategorien.begin(); it != Kategorien.end(); it++){
		if( it.key() == nummer ){
			return it.value().Typ;
		}
	}

	return 0;
}


quint32 EntryKategorie::changeKategorie(quint32 nummer, QString newKategorie){
	MapKategorie::iterator it;
	for(it = Kategorien.begin(); it != Kategorien.end(); it++){
		if( it.key() == nummer ){
			it.value().Kategorie = newKategorie;
			break;
		}
	}

	if( it == Kategorien.end() ){
		return NotFound;
	}

	return Ok;
}


quint32 EntryKategorie::changeBeschreibung(quint32 nummer, QString newBeschreibung){
	MapKategorie::iterator it;
	for(it = Kategorien.begin(); it != Kategorien.end(); it++){
		if( it.key() == nummer ){
			it.value().Beschreibung = newBeschreibung;
			break;
		}
	}

	if( it == Kategorien.end() ){
		return NotFound;
	}

	return Ok;
}


quint32 EntryKategorie::changeTyp(quint32 nummer, quint32 newTyp){
	MapKategorie::iterator it;
	for(it = Kategorien.begin(); it != Kategorien.end(); it++){
		if( it.key() == nummer ){
			it.value().Typ = newTyp;
			break;
		}
	}
	
	if( it == Kategorien.end() ){
		return NotFound;
	}
	
	return Ok;
}
