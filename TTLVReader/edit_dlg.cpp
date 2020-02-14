#include "edit_dlg.h"
#include "mainwindow.h"
#include "reader_applet.h"
#include "reader_tree_item.h"

#include "js_bin.h"

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
    int     ret = 0;
    BIN     *pDstTag = NULL;
    BIN     *pDstType = NULL;
    BIN     *pDstLength = NULL;
    BIN     *pDstValue = NULL;

    BIN     srcTag = {0,0};
    BIN     srcType = {0,0};
    BIN     srcLength = {0,0};
    BIN     srcValue = {0,0};

    BIN     *pTTLV = readerApplet->mainWindow()->getTTLV();

    ReaderTreeItem *pItem = readerApplet->mainWindow()->currentItem();
    pDstTag = pItem->getTag();
    pDstType = pItem->getType();
    pDstLength = pItem->getLength();
    pDstValue = pItem->getValue();

    JS_BIN_decodeHex( mTagText->text().toStdString().c_str(), &srcTag );
    JS_BIN_decodeHex( mTypeText->text().toStdString().c_str(), &srcType );
    JS_BIN_decodeHex( mLengthText->text().toStdString().c_str(), &srcLength );
    JS_BIN_decodeHex( mValueText->toPlainText().toStdString().c_str(), &srcValue );

    if( pDstTag->nLen != srcTag.nLen
            || pDstType->nLen != srcType.nLen
            || pDstLength->nLen != srcLength.nLen
            || pDstValue->nLen != srcValue.nLen )
    {
        readerApplet->warningBox( "All length of value have to be the same." );
        ret = -1;
    }

    int nOffset = pItem->getOffset();

    memcpy( &pTTLV->pVal[nOffset], srcTag.pVal, srcTag.nLen );
    nOffset += srcTag.nLen;

    memcpy( &pTTLV->pVal[nOffset], srcType.pVal, srcType.nLen );
    nOffset += srcType.nLen;

    memcpy( &pTTLV->pVal[nOffset], srcLength.pVal, srcLength.nLen );
    nOffset += srcLength.nLen;

    memcpy( &pTTLV->pVal[nOffset], srcValue.pVal, srcValue.nLen );
    nOffset += srcValue.nLen;

    JS_BIN_reset( &srcTag );
    JS_BIN_reset( &srcType );
    JS_BIN_reset( &srcLength );
    JS_BIN_reset( &srcValue );

    if( ret == 0 )
    {
        readerApplet->mainWindow()->parseTree();
        QDialog::accept();
    }
}
