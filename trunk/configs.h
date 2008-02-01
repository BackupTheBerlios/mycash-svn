#ifndef _CONFIGS_H_
#define _CONFIGS_H_

#include <QString>
#include <QDataStream>
#include "class_konto_typ.h"

//const bool DEBUG = true;
#define DEBUG

const QString AppName = "MyCash";

//QT-Einstellungen
const int QDataStreamVersion = QDataStream::Qt_4_3;
//const char [] QTextStreamCodec = "UTF-8";

//Config-Dateien
const QString KTypFile = "./configs/KontoTyp.mcc";
const QString KatFile = "./configs/Kategorie.mck";

//Globale Variablen
//extern Konto_Typ MCKT;

#endif

