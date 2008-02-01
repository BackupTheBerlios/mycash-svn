#ifndef MAINWINDOW1_H
#define MAINWINDOW1_H

#include <QObject>
#include <QDialog>
#include <QVector>
#include <QString>

#include "konto.h"
#include "ui_mainwindow1.h"
#include "neueskontodialog.h"

class MainWindow1 : public QMainWindow, public Ui::MainWindow1{
	Q_OBJECT
	
	public:
		MainWindow1(QWidget *parent = 0);
		~MainWindow1();
		//void SetIsChanged();
		//void SetIsNotChanged();
		bool save();
		bool saveAs();

	protected:
		void closeEvent(QCloseEvent *event);

	public slots:
		void showNeuesKontoDialog();
		bool addKonto(Konto *konto);
		bool newFile();

	private:
		bool okToContinue();
		bool saveFile(QString file);
		bool clear();

		//bool IsChanged;
		QString File;
		NeuesKontoDialog *neuesKontoDialog;

};

#endif
