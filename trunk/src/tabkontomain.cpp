#include <QWidget>
#include <QTextStream>

#include "tabkontomain.h"
#include "konto.h"

TabKontoMain::TabKontoMain(QWidget *parent = 0, Konto *connectedKonto = 0) : QWidget( parent )
{
	setupUi(this);
	konto = connectedKonto;

	QTextStream console(stdout);
	console << "TabKontoMain()" << "\n\r";
}
