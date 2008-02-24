#ifndef LISTDETAILSDELEGATE_H
#define LISTDETAILSDELEGATE_H

#include <QItemDelegate>
#include <QPainter>

class ListDetailsDelegate : public QItemDelegate{
	Q_OBJECT
	public:
		ListDetailsDelegate(qint32 ColumnKat = -1, qint32 ColumnTax = -1, quint32 ColumnDelete = -1, QObject *parent = 0);
		void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
		QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
		void setEditorData(QWidget *editor, const QModelIndex &index) const;
		void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;


	private slots:
		void commitAndCloseEditor();

	private:
		qint32 posKategorie;
		qint32 posTax;
		qint32 posDelete;
};

#endif
