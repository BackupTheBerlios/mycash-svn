#ifndef KONTOSETTINGS_H
#define KONTOSETTINGS_H

#include "konto.h"
#include "ui_konto_settings.h"

class FormKontoSettings : public QWidget, public Ui::FormKontoSettings{
	Q_OBJECT
	
	public:
		FormKontoSettings(QWidget *parent);

	public slots:
		void update(const Konto::KontoSettings& settings);

	private slots:
		void clickOk();
		void clickKontoFile();

	signals:
		void updateKonto(const Konto::KontoSettings&);

};

#endif
