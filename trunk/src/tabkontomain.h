#ifndef TABKONTOMAIN_H
#define TABKONTOMAIN_H

#include "ui_tab_konto_main.h"
#include "konto.h"

class TabKontoMain : public QWidget, public Ui::TabKontoMain{
	Q_OBJECT
	
	public:
		TabKontoMain(QWidget *parent, Konto *connectedKonto);

	private:
		Konto *konto;
};

#endif
