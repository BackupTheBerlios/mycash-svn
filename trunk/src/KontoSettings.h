#ifndef KONTOSETTINGS_H
#define KONTOSETTINGS_H

#include "konto.h"
#include "ui_konto_settings.h"

class KontoSettings : public QWidget, public Ui::KontoSettings{
	Q_OBJECT
	
	public:
		KontoSettings(QWidget *parent);

	public slots:
		void update(const Konto::KontoSettings& settings);

	private slots:
		void clickOk();
		void clickKontoFile();

	signals:
		void updateKonto(const Konto::KontoSettings&);

};

#endif
