#include "edit_dlg.h"
#include "mainwindow.h"
#include "reader_applet.h"
#include "reader_tree_item.h"

EditDlg::EditDlg(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    initialize();
}

EditDlg::~EditDlg()
{

}

void EditDlg::initialize()
{
    ReaderTreeItem *pItem = readerApplet->mainWindow()->currentItem();
    if( pItem == NULL ) return;

    mTagText->setText( pItem->getTagHex() );
    mTypeText->setText( pItem->getTypeHex() );
    mLengthText->setText( pItem->getLengthHex() );
    mValueText->setPlainText( pItem->getValueHex() );
}

void EditDlg::accept()
{
    QDialog::accept();
}
