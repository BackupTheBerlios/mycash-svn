#ifndef MAINWINDOW1_H
#define MAINWINDOW1_H

#include <QObject>
#include <QDialog>
#include <QVector>
#include <QString>
#include <QWidget>
#include <QMainWindow>
#include <QMap>
#include <QtXml>

#include "konto.h"
#include "ui_mainwindow1.h"
#include "neueskontodialog.h"
#include "tabkontomain.h"
#include "tabmain.h"

class MainWindow1 : public QMainWindow, public Ui::MainWindow1{
	Q_OBJECT
	
	typedef QMap<Konto *, TabKontoMain *> MapKontoWidget;

	public:
		MainWindow1(QWidget *parent = 0);
		~MainWindow1();

		bool loadFile();
		bool load(QString filename);

	protected:
		void closeEvent(QCloseEvent *event);

	public slots:
		void projectChanged();
		void showNeuesKontoDialog();
		bool addKonto(Konto *konto);
		bool addKonto(Konto *konto, bool show);
		bool newFile();
		bool load();
		bool save();
		bool saveAs();
		bool addExistKonto();
		bool closeTab(TabKontoMain *tab);
		bool deleteKonto(Konto *konto);

	private:
		//QDomElement getFilesXml(QDomDocument& doc);
		//bool parseFilesXml(QDomElement& element);
		bool okToContinue();
		bool saveFile(QString filename);
		bool clear();
		void setFileName(QString filename);
		void showMain(bool show);
		void unloadKonten();
		QString getFileName();
		QString correctFilename(QString filename, QString endung);
		bool existFilename(QString filename);
		void showMessageboxAlreadyExist();
		//bool loadFilePlain();
		//bool loadFileXml();

		MapKontoWidget connections;	//Verknüfungstabelle der Konten mit TabWidget
		//bool IsChanged;
		QString File;	//Projektdatei
		NeuesKontoDialog *neuesKontoDialog;	//Speicher des Formular NeuesKontoDialog
		TabMain *tabMain;

};

#endif
