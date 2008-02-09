#ifndef TABKONTOMAIN_H
#define TABKONTOMAIN_H

#include "ui_tab_konto_main.h"
#include "konto.h"
#include "AddEntry.h"

class TabKontoMain : public QWidget, public Ui::TabKontoMain{
	Q_OBJECT
	
	public:
		TabKontoMain(QWidget *parent, Konto *connectedKonto);

	public slots:
		void clickClose();
		void clickDelete();
	
	signals:
		void close(TabKontoMain *);
		void deleteKonto(Konto *);

	private slots:
		void showAddEntryDialog();

	private:
		Konto *KontoPointer;
		AddEntry *AddEntryPointer;
};

#endif
