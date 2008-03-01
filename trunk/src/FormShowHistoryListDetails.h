/**
* @file FormShowHistoryListDetails.h
*
* @brief Datei beinhaltet die Deklarationen der Klasse FormShowHistoryListDetails.
*/
#ifndef FORMSHOWHISTORYLISTDETAILS_H
#define FORMSHOWHISTORYLISTDETAILS_H

#include "ui_show_entry_details.h"
#include "konto.h"
#include <QWidget>

/**
* @class FormShowHistoryListDetails
*
* @brief Klasse enthält Funktionen für das FormShowHistoryListDetails - Formular.
*
* Dieses Formular zeigt Informationen über ein einzelnen Kontoeintrag an. Diese sind in diesen Formular auch änderbar.
*/
class FormShowHistoryListDetails : public QWidget, public Ui::FormShowHistoryListDetails{

	Q_OBJECT
	public:
		FormShowHistoryListDetails(QWidget *parent);

	private slots:
		void getDelete(void);
		void buttonOkClicked( void );

	public slots:
		void getData(const Konto::HistoryListDetails& details);

	signals:
		void doDelete( quint32 );
		void sendData(Konto::HistoryListDetails );

	private:
		void clearAndClose(void);
		void clear(void);
		quint32 EntryNumber;

		enum Column{VERWENDUNG, KATEGORIE, BETRAG, STEUER, LOESCHEN};
}; 

#endif
