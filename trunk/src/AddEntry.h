#ifndef ADDENTRY_H
#define ADDENTRY_H

#include "ui_add_entry.h"
#include "KontoEntry.h"

class FormAddEntry : public QWidget, public Ui::FormAddEntry
{
	Q_OBJECT
	public:
		FormAddEntry(QWidget *parent);
	
	public slots:	
		void clickAdd();
		void clickMore();	

	signals:
		void add(KontoEntry *);

	private:
};

#endif
