#ifndef FORMSHOWHISTORYLIST_H
#define FORMSHOWHISTORYLIST_H

#include "ui_show_history_list.h"
#include "konto.h"

class FormShowHistoryList : public QWidget, public Ui::FormShowHistoryList{
	public:
		FormShowHistoryList( QWidget *parent);

	public slots:
		void update(Konto::VectorHistoryList& list);
};

#endif
