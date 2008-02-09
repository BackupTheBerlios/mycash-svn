#ifndef TABMAIN_H
#define TABMAIN_H

#include "ui_tab_main.h"

class TabMain : public QWidget, public Ui::TabMain{
	Q_OBJECT

	public:
		TabMain(QWidget *parent);
};
#endif
