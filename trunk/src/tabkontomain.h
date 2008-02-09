#ifndef TABKONTOMAIN_H
#define TABKONTOMAIN_H

#include "ui_tab_konto_main.h"
#include "konto.h"
#include "AddEntry.h"
#include "KontoSettings.h"

class TabKontoMain : public QWidget, public Ui::TabKontoMain{
	Q_OBJECT
	
	public:
		TabKontoMain(QWidget *parent, Konto *connectedKonto);
		~TabKontoMain();

	public slots:
		void clickClose();
		void clickDelete();
		void clickSettings();

		//KontoSettings-Update
		void fromKontoSettings(const Konto::KontoSettings&);
	
	signals:
		void close(TabKontoMain *);
		void deleteKonto(Konto *);
		void updateSettingsDialog(Konto::KontoSettings);

	private slots:
		void showAddEntryDialog();
		void showKontoSettingsDialog();

	private:
		Konto *KontoPointer;
		AddEntry *AddEntryPointer;
		KontoSettings *KontoSettingsPointer;
};

#endif
