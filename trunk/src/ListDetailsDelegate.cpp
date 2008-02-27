#include "ListDetailsDelegate.h"

#include <QModelIndex>
#include <QCheckBox>

ListDetailsDelegate::ListDetailsDelegate(qint32 ColumnKat, qint32 ColumnTax, quint32 ColumnDelete, QObject *parent) :
QItemDelegate(parent)
/******************************************************************************
* Konstruktor
*******************************************************************************/
{
	posKategorie = ColumnKat;
	posTax = ColumnTax;
	posDelete = ColumnDelete;
}


void ListDetailsDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
/******************************************************************************
* Anzeigeoptionen im nicht selektierten Zustand
*******************************************************************************/
{
	if( index.column() ==  posKategorie){

	}else if( index.column() == posTax){
		//leider keine richtige Checkbox... weiter testen..
		//drawCheck(painter, option, option.rect,  (bool)index.model() -> data(index, Qt::DisplayRole).toInt()?Qt::Checked:Qt::Unchecked );

	}else if( index.column() == posDelete ){
		
	}else{
		QItemDelegate::paint(painter, option, index);
	}
}


QWidget *ListDetailsDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
/******************************************************************************
* Methode wird aufgerufen, wenn der Editor gestartet wird
*******************************************************************************/
{
	if( index.column() ==  posKategorie){

	}else if( index.column() == posTax){

	}else if( index.column() == posDelete ){

	}else{
		return QItemDelegate::createEditor(parent, option, index);
	}
	return QItemDelegate::createEditor(parent, option, index); //debug
}


void ListDetailsDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
/******************************************************************************
* Methode wird aufgerufen um den Editor zu initialisieren
*******************************************************************************/
{
	if( index.column() ==  posKategorie){

	}else if( index.column() == posTax){

	}else if( index.column() == posDelete ){

	}else{
		QItemDelegate::setEditorData(editor, index);
	}
}


void ListDetailsDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
/******************************************************************************
* Methode wird aufgerufen um Aenderung im Item zu speichern
********************************************************************************/
{
	if( index.column() ==  posKategorie){

	}else if( index.column() == posTax){

	}else if( index.column() == posDelete ){

	}else{
		QItemDelegate::setModelData(editor,model, index);
	}
}


void ListDetailsDelegate::commitAndCloseEditor() //SLOT
/******************************************************************************
* Methode wird aufgerufen, wenn Item erfolgreich bearbeitet wurde
* erfolgreich Bearbeitet: <Enter>, <Tab>, anderes Feld selektiert...
*******************************************************************************/
{

}

