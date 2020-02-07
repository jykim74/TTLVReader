#include "insert_data_dlg.h"


InsertDataDlg::InsertDataDlg(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
}

InsertDataDlg::~InsertDataDlg()
{

}

void InsertDataDlg::accept()
{
    QDialog::accept();
}

int InsertDataDlg::getType()
{
    if( mHexRadio->isChecked() )
        return 0;
    else if( mBase64Radio->isChecked() )
        return 1;
}
