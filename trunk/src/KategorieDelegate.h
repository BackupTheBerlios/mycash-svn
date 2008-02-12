#ifndef KATEGORIEDELEGATE_H
#define KATEGORIEDELEGATE_H

#include <QItemDelegate>
#include <QPainter>

class KategorieDelegate : public QItemDelegate
{
	Q_OBJECT
	public:
		KategorieDelegate(int durationColumn, QObject *parent = 0);
		void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
		QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
		void setEditorData(QWidget *editor, const QModelIndex &index) const;
		void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

	private slots:
		void commitAndCloseEditor();

	private:
		int durationColumn;
};
#endif
