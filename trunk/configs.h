#ifndef _CONFIGS_H_
#define _CONFIGS_H_

#include <QString>
#include <QDataStream>
#include <QTextCodec>
#include "class_konto_typ.h"

//const bool DEBUG = true;
#define DEBUG

const QString AppName = "MyCash";
const QString WindowTitle = "MyCash[*]";
const QString END_PROJECT = "*.mc";
const QString END_KONTO = "*.mck";

//QT-Einstellungen
const int QDataStreamVersion = QDataStream::Qt_4_3;
//QTextCodec *QTextStreamCodec = QTextCodec::codecForName("UTF-8");

//Config-Dateien
const QString KTypFile = "./configs/KontoTyp.mckt";
const QString KatFile = "./configs/Kategorie.mckk";

//Globale Variablen
//extern Konto_Typ MCKT;

#endif

