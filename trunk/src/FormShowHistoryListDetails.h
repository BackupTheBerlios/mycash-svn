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
		void getDelete(void);

	public slots:
		void getData(const Konto::HistoryListDetails& details);

	signals:
		void doDelete( quint32 );

	private:
		void clearAndClose(void);
		void clear(void);
		quint32 EntryNumber;
}; 

#endif
