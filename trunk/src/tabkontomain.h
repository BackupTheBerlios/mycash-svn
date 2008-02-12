#ifndef TABKONTOMAIN_H
#define TABKONTOMAIN_H

#include "ui_tab_konto_main.h"
#include "konto.h"
#include "AddEntry.h"
#include "KontoSettings.h"
#include "FormShowHistoryList.h"

class TabKontoMain : public QWidget, public Ui::TabKontoMain{
	Q_OBJECT
	
	public:
		TabKontoMain(QWidget *parent, Konto *connectedKonto);
		~TabKontoMain();

	public slots:
		//FormKontoSettings-Update
		void fromFormKontoSettings(const Konto::KontoSettings&);
	
	signals:
		void close(TabKontoMain *);
		void deleteKonto(Konto *);
		void updateSettingsDialog(Konto::KontoSettings);
		void updateHistoryListDialog(Konto::VectorHistoryList&);

	private slots:
		void clickClose();
		void clickDelete();
		void clickSettings();
		void clickHistoryList();
		void showFormAddEntryDialog();
		void showFormKontoSettingsDialog();
		void showHistoryListDialog();

	private:
		Konto *KontoPointer;
		FormAddEntry *FormAddEntryPointer;
		FormKontoSettings *FormKontoSettingsPointer;
		FormShowHistoryList *FormShowHistoryListPointer;
};

#endif
