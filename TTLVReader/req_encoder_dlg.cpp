#include "js_kms.h"
#include "req_encoder_dlg.h"

#include <QFileDialog>

ReqEncoderDlg::ReqEncoderDlg(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    data_.nLen = 0;
    data_.pVal = 0;

    connect( mFindInputBtn, SIGNAL(clicked()), this, SLOT(findInput()));
    connect( mChangeBtn, SIGNAL(clicked()), this, SLOT(clickChange()));

    connect( mCreateBtn, SIGNAL(clicked()), this, SLOT(clickCreate()));
    connect( mActivateBtn, SIGNAL(clicked()), this, SLOT(clickActivate()));
    connect( mGetBtn, SIGNAL(clicked()), this, SLOT(clickGet()));
    connect( mDestroyBtn, SIGNAL(clicked()), this, SLOT(clickDestroy()));
    connect( mEncryptBtn, SIGNAL(clicked()), this, SLOT(clickEncrypt()));
    connect( mDecryptBtn, SIGNAL(clicked()), this, SLOT(clickDecrypt()));
    connect( mSignBtn, SIGNAL(clicked()), this, SLOT(clickSign()));
    connect( mVerifyBtn, SIGNAL(clicked()), this, SLOT(clickVerify()));
    connect( mRegisterBtn, SIGNAL(clicked()), this, SLOT(clickRegister()));
    connect( mCreateKeyPairBtn, SIGNAL(clicked()), this, SLOT(clickCreateKeyPair()));
}

ReqEncoderDlg::~ReqEncoderDlg()
{
    JS_BIN_reset( &data_ );
}

void ReqEncoderDlg::initialize()
{

}

void ReqEncoderDlg::clickChange()
{
    QString strOutput = mOutputText->toPlainText();

    mInputText->clear();
    mInputText->setPlainText( strOutput );
}

void ReqEncoderDlg::findInput()
{
    BIN binFile = {0,0};
    char *pHex = NULL;

    QString fileName = QFileDialog::getOpenFileName( this, "Object file",
                                    QDir::currentPath(),
                                    "All Files (*.*);;BIN files(*.bin);;Hex Files(*.hex)");

    JS_BIN_fileRead( fileName.toStdString().c_str(), &binFile );
    JS_BIN_encodeHex( &binFile, &pHex );

    if( pHex )
    {
        mInputText->setPlainText( pHex );
        JS_free( pHex );
    }

    JS_BIN_reset( &binFile );
}


void ReqEncoderDlg::clickGet()
{
    int ret = 0;
    JS_BIN_reset( &data_ );

    QString strUUID = mUUIDText->text();

    ret = JS_KMS_encodeGetReq( "tesetUser", "testPasswd", strUUID.toStdString().c_str(), &data_ );
    if( ret == 0 )
    {
        QDialog::accept();
    }
}

void ReqEncoderDlg::clickActivate()
{
    int ret = 0;
    JS_BIN_reset( &data_ );

    QString strUUID = mUUIDText->text();

    ret = JS_KMS_encodeActivateReq( strUUID.toStdString().c_str(), &data_ );
    if( ret == 0 )
    {
        QDialog::accept();
    }
}

void ReqEncoderDlg::clickCreate()
{
    int ret = 0;
    JS_BIN_reset( &data_ );

    ret = JS_KMS_encodeCreateReq( &data_ );

    if( ret == 0 )
    {
        QDialog::accept();
    }
}

void ReqEncoderDlg::clickDestroy()
{
    int ret = 0;
    JS_BIN_reset( &data_ );

    QString strUUID = mUUIDText->text();

    ret = JS_KMS_encodeDestroyReq( strUUID.toStdString().c_str(), &data_ );
    if( ret == 0 )
    {
        QDialog::accept();
    }
}

void ReqEncoderDlg::clickEncrypt()
{

}

void ReqEncoderDlg::clickDecrypt()
{

}

void ReqEncoderDlg::clickSign()
{

}

void ReqEncoderDlg::clickVerify()
{

}

void ReqEncoderDlg::clickRegister()
{

}

void ReqEncoderDlg::clickCreateKeyPair()
{

}
