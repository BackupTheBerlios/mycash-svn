#ifndef NEUESKONTODIALOG_H
#define NEUESKONTODIALOG_H

#include <QDialog>
#include <QWidget>
#include "konto.h"
#include "ui_neueskonto.h"

class NeuesKontoDialog : public QDialog, public Ui::NeuesKontoDialog/*, public QWidget*/{
	Q_OBJECT
	
	public:
		NeuesKontoDialog(QWidget *parent = 0);
	signals:
		void add(Konto *);
	public slots:
		void clickAdd();
	private:
		void clearLines();
	
};

#endif
