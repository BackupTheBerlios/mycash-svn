#ifndef FORMSHOWHISTORYLIST_H
#define FORMSHOWHISTORYLIST_H

#include "ui_show_history_list.h"
#include "konto.h"

class FormShowHistoryList : public QWidget, public Ui::FormShowHistoryList{
	Q_OBJECT

	public:
		FormShowHistoryList( QWidget *parent);

	public slots:
		void updateTable(const Konto::VectorHistoryList& list);
		void selectRow(int row, int column);

		//void debugSel();
};

#endif
