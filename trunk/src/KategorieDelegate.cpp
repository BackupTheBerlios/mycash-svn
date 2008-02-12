#include "KategorieDelegate.h"

#include <QModelIndex>

KategorieDelegate::KategorieDelegate(int durationColumn, QObject *parent) : QItemDelegate( parent )
/******************************************************************************
* Konstruktor fuer KategorieDelegate
*******************************************************************************/
{
	this -> durationColumn = durationColumn;
}


void KategorieDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
/******************************************************************************
* Anzeigeoptionen im nicht selektierten Zustand
*******************************************************************************/
{
	if( index.column() == durationColumn ){
		drawDisplay( painter, option, option.rect, "aaa" );
		drawFocus( painter, option, option.rect );
	}else{
		QItemDelegate::paint(painter, option, index);
	}
}


void KategorieDelegate::commitAndCloseEditor() //SLOT
/******************************************************************************
* Methode wird aufgerufen, wenn Item erfolgreich bearbeitet wurde
* erfolgreich Bearbeitet: <Enter>, <Tab>, anderes Feld selektiert...
*******************************************************************************/
{
	
}


QWidget *KategorieDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
/******************************************************************************
* Methode wird aufgerufen, wenn der Editor gestartet wird
*******************************************************************************/
{
	if( index.column() == durationColumn ){

		return QItemDelegate::createEditor( parent, option, index );
	}else{
		return QItemDelegate::createEditor( parent, option, index );
	}
}


void KategorieDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
/******************************************************************************
* Methode wird aufgerufen um den Editor zu initialisieren
*******************************************************************************/
{
	if( index.column() == durationColumn ){

	}else{
		QItemDelegate::setEditorData(editor, index);
	}
}


void KategorieDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
/******************************************************************************
* Methode wird aufgerufen um Aenderung im Item zu speichern
*******************************************************************************/
{
	if( index.column() == durationColumn ){

	}else{
		QItemDelegate::setModelData(editor, model, index);
	}
}


