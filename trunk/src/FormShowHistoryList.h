#ifndef FORMSHOWHISTORYLIST_H
#define FORMSHOWHISTORYLIST_H

#include "ui_show_history_list.h"
#include "konto.h"
#include "FormShowHistoryListDetails.h"

class FormShowHistoryList : public QWidget, public Ui::FormShowHistoryList{
	Q_OBJECT

	public:
		FormShowHistoryList( QWidget *parent);
		~FormShowHistoryList();

	public slots:
		void updateTable(const Konto::VectorHistoryList& list);
		void deleteEntry(const quint32& entry);
		void updateDetails(const Konto::HistoryListDetails& details);
		void changeDetails(const Konto::HistoryListDetails& details);
	
	private slots:
		void selectRow(int row, int column);
		void showDetails(int row, int column);
		
		//void debugSel();

	signals:
		void GetUpdateDetails(quint32); //Signal zur Weitergabe des gewuenschten Eintrages| this->parent
		void DoUpdateDetails(Konto::HistoryListDetails); // Signal zur Weitergabe der Details| this -> child
		void SigDeleteEntry(quint32 entry); //Signal zum Loeschen eines Eintrages| this -> parent
		void DoChangeDetails(Konto::HistoryListDetails); // Signal zum Ändern von Eintragen| this -> parent

	private:
		FormShowHistoryListDetails *PointerFormDetails;
};

#endif
