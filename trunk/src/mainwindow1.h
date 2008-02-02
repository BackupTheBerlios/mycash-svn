#ifndef MAINWINDOW1_H
#define MAINWINDOW1_H

#include <QObject>
#include <QDialog>
#include <QVector>
#include <QString>
#include <QWidget>
#include <QMainWindow>
#include <QMap>

#include "konto.h"
#include "ui_mainwindow1.h"
#include "neueskontodialog.h"

class MainWindow1 : public QMainWindow, public Ui::MainWindow1{
	Q_OBJECT
	
	typedef QMap<Konto *, QWidget *> MapKontoWidget;

	public:
		MainWindow1(QWidget *parent = 0);
		~MainWindow1();

		bool save();
		bool saveAs();
		bool loadFile();
		bool load(QString filename);

	protected:
		void closeEvent(QCloseEvent *event);

	public slots:
		void projectChanged();
		void showNeuesKontoDialog();
		bool addKonto(Konto *konto);
		bool newFile();
		bool load();

	private:
		bool okToContinue();
		bool saveFile(QString filename);
		bool clear();
		void setFileName(QString filename);
		void unloadKonten();
		QString getFileName();
		QString correctFilename(QString filename, QString endung);

		MapKontoWidget connections;	//Verknüfungstabelle der Konten mit TabWidget
		//bool IsChanged;
		QString File;	//Projektdatei
		NeuesKontoDialog *neuesKontoDialog;	//Speicher des Formular NeuesKontoDialog

};

#endif
