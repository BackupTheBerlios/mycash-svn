#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include "konto.h"
#include "class_konto_typ.h"
#include "konto_splitt.h"
#include "KontoEntry.h"

class QAction;
class QString;

class MainWindow : public QMainWindow{
	Q_OBJECT
	
	public:
		MainWindow();
		~MainWindow();
				
	protected:
		void closeEvent(QCloseEvent *event);
		
	private slots:
		void newFile();
		void open();
		bool save();
		bool saveAs();
		void exit();
		void about();
		
	private:
		void createActions();
		void createMenus();
		bool loadFile(const QString &filename);
		bool saveFile(const QString &filename);
		void aktivateSave();
		void deaktivateSave();
		bool okToContinue();
		void deleteKonten();
		
		//Debug
		void createNewKonto_deb(QString File);
		void showKData_deb(QString File);
		
		//Variablen
		QString curFile;
		QVector<Konto *> konten;
		
		//Menus
		QMenu *fileMenu;
		QMenu *helpMenu;
		
		//Actions
		QAction *newAction;
		QAction *openAction;
		QAction *saveAsAction;
		QAction *saveAction;
		QAction *exitAction;
		QAction *aboutAction;
		QAction *aboutQtAction;
		
		Konto_Typ *MCKT;
};

#endif
