#ifndef ADDENTRY_H
#define ADDENTRY_H

#include "ui_add_entry.h"
#include "KontoEntry.h"

class AddEntry : public QWidget, public Ui::AddEntry
{
	Q_OBJECT
	public:
		AddEntry(QWidget *parent);
		void clickAdd();
		void clickMore();	

	signals:
		void add(KontoEntry *);

	private:
};

#endif
