#include "KontoSettings.h"

#include <QWidget>
#include <Qt>
#include <QFileDialog>
#include <QMessageBox>

#include "konto.h"
#include "configs.h"

FormKontoSettings::FormKontoSettings(QWidget *parent) : QWidget( parent )
/******************************************************************************
* Konstruktur
*******************************************************************************/
{
	setupUi(this);

	connect( buttonFileDialog,
			SIGNAL( clicked() ),
			this,
			SLOT( clickKontoFile() )
			);

	connect( buttonOk,
			SIGNAL( clicked() ),
			this,
			SLOT( clickOk() )
			);

	//Validator BLZ
	QRegExp regExp ( "[1-9][0-9]{0,7}" );
	lineBLZ -> setValidator ( new  QRegExpValidator ( regExp, this ) );

}


void FormKontoSettings::update(const Konto::KontoSettings& settings) // SLOT
/******************************************************************************
* Methode aktualisiert die Felder
*******************************************************************************/
{
	lineKontoName -> setText( settings.KontoName );
	
	textKontoBeschreibung -> setPlainText( settings.KontoBeschreibung );
	
	lineFile -> setText( settings.FileName );
	
	lineBLZ -> setText( settings.BLZ );
	
	lineBankName -> setText ( settings.BankName );
	
	//stelle ComboBox auf richtigen KontoTyp ein
	
	spinLimit -> setValue( settings.Limit );
	
	if( settings.canBeNegativ ){
		boxUnderLimit -> setCheckState( Qt::Unchecked );
	}else{
		boxUnderLimit -> setCheckState( Qt::Checked );
	}

}


void FormKontoSettings::clickOk() // private SLOT
/******************************************************************************
* Methode wird ausgeführt, wenn buttonOk das Signal clicked() sendet
*******************************************************************************/
{
	Konto::KontoSettings temp;
	
	temp.KontoName = lineKontoName -> text();
	temp.KontoBeschreibung = textKontoBeschreibung -> toPlainText();
	temp.BLZ = lineBLZ -> text();
	temp.BankName = lineBankName -> text();
	temp.KontoTyp = 0; //Anpassen, comboKontoTyp
	temp.FileName = lineFile -> text();
	temp.Limit = spinLimit -> value();
	temp.canBeNegativ = boxUnderLimit -> checkState();

	emit updateKonto( temp );
}


void FormKontoSettings::clickKontoFile()
/******************************************************************************
* Methode wird ausgefuehrt, wenn buttonFileDialog geklickt wird
*******************************************************************************/
{
	QString file = QFileDialog::getSaveFileName(this,
												tr("%1 - Select file for account '%2'").arg(AppName).arg(lineKontoName -> text()),
												".",
												tr ( "%1 konten (%2)" ).arg ( AppName ).arg ( END_KONTO )
												);
	if( !file.isEmpty() ){
		lineFile -> setText( file );
	}
}
