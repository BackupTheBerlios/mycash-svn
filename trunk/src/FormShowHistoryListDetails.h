#ifndef FORMSHOWHISTORYLISTDETAILS_H
#define FORMSHOWHISTORYLISTDETAILS_H

#include "ui_show_entry_details.h"
#include "konto.h"
#include <QWidget>


class FormShowHistoryListDetails : public QWidget, public Ui::FormShowHistoryListDetails{
	Q_OBJECT
	public:
		FormShowHistoryListDetails(QWidget *parent);

	private slots:
		void getDelete();

	public slots:
		void getData();
	signals:
		void doDelete( quint32 );

	private:
		void clearAndClose();
		quint32 EntryNumber;
}; 

#endif
