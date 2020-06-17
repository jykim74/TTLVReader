#include "js_kms.h"
#include "req_encoder_dlg.h"

#include <QFileDialog>

const QStringList kObjetType = { "SecretKey", "PrivateKey", "PublicKey", "Certificate" };

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

    connect( mGetAttributeListBtn, SIGNAL(clicked()), this, SLOT(clickGetAttributeList()));

    initialize();
}

ReqEncoderDlg::~ReqEncoderDlg()
{
    JS_BIN_reset( &data_ );
}

void ReqEncoderDlg::initialize()
{
    mObjectTypeCombo->addItems( kObjetType );
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
    int ret = 0;
    JS_BIN_reset( &data_ );

    BIN binIV = {0};
    BIN binPlain = {0};

    QString strUUID = mUUIDText->text();
    QString strInput = mInputText->toPlainText();

    JS_BIN_set( &binIV, (unsigned char *)"1234567890123456", 16);
    JS_BIN_set( &binPlain, (unsigned char *)strInput.toStdString().c_str(), strInput.length() );

    ret = JS_KMS_encodeEncryptReq( strUUID.toStdString().c_str(), &binIV, &binPlain, &data_ );

    JS_BIN_reset( &binIV );
    JS_BIN_reset( &binPlain );

    if( ret == 0 )
    {
        QDialog::accept();
    }
}

void ReqEncoderDlg::clickDecrypt()
{
    int ret = 0;
    JS_BIN_reset( &data_ );
    BIN binIV = {0};
    BIN binEncrypt = {0};

    QString strUUID = mUUIDText->text();
    QString strInput = mInputText->toPlainText();

    JS_BIN_set( &binIV, (unsigned char *)"1234567890123456", 16);
    JS_BIN_decodeHex( strInput.toStdString().c_str(), &binEncrypt );

    ret = JS_KMS_encodeDecryptReq( strUUID.toStdString().c_str(), &binIV, &binEncrypt, &data_ );

    JS_BIN_reset( &binIV );
    JS_BIN_reset( &binEncrypt );

    if( ret == 0 )
    {
        QDialog::accept();
    }
}

void ReqEncoderDlg::clickSign()
{
    int ret = 0;
    JS_BIN_reset( &data_ );
    BIN binPlain = {0};

    QString strUUID = mUUIDText->text();
    QString strInput = mInputText->toPlainText();

    JS_BIN_set( &binPlain, (unsigned char *)strInput.toStdString().c_str(), strInput.length() );

    ret = JS_KMS_encodeSignReq( strUUID.toStdString().c_str(), &binPlain, &data_ );

    JS_BIN_reset( &binPlain );

    if( ret == 0 )
    {
        QDialog::accept();
    }
}

void ReqEncoderDlg::clickVerify()
{
    int ret = 0;
    JS_BIN_reset( &data_ );

    BIN binPlain = {0};
    BIN binSign = {0};

    QString strUUID = mUUIDText->text();
    QString strInput = mInputText->toPlainText();
    QString strOutput = mOutputText->toPlainText();

    JS_BIN_set( &binPlain, (unsigned char *)strInput.toStdString().c_str(), strInput.length() );
    JS_BIN_decodeHex( strOutput.toStdString().c_str(), &binSign );

    ret = JS_KMS_encodeVerifyReq( strUUID.toStdString().c_str(), &binPlain, &binSign, &data_ );

    JS_BIN_reset( &binPlain );
    JS_BIN_reset( &binSign );

    if( ret == 0 )
    {
        QDialog::accept();
    }
}

void ReqEncoderDlg::clickRegister()
{
    int ret = 0;
    JS_BIN_reset( &data_ );

    int nType = 0;
    BIN binInput = {0};

    QString strInput = mInputText->toPlainText();

    if( mObjectTypeCombo->currentIndex() == 0 )
        nType = JS_KMS_OBJECT_TYPE_SECRET;
    else if( mObjectTypeCombo->currentIndex() == 1 )
        nType = JS_KMS_OBJECT_TYPE_PRIKEY;
    else if( mObjectTypeCombo->currentIndex() == 2 )
        nType = JS_KMS_OBJECT_TYPE_PUBKEY;
    else if( mObjectTypeCombo->currentIndex() == 3 )
        nType = JS_KMS_OBJECT_TYPE_CERT;

    JS_BIN_decodeHex( strInput.toStdString().c_str(), &binInput );

    ret = JS_KMS_encodeRegisterReq( nType, &binInput, &data_ );

    JS_BIN_reset( &binInput );

    if( ret == 0 )
    {
        QDialog::accept();
    }
}

void ReqEncoderDlg::clickCreateKeyPair()
{
    int ret = 0;
    int nAlg = -1;
    int nParam = 2048;

    JS_BIN_reset( &data_ );

    ret = JS_KMS_encodeCreateKeyPairReq( nAlg, nParam, &data_ );
    if( ret == 0 )
    {
        QDialog::accept();
    }
}

void ReqEncoderDlg::clickGetAttributeList()
{
    int ret = 0;
    JS_BIN_reset( &data_ );

    QString strUUID = mUUIDText->text();

    ret = JS_KMS_encodeGetAttributeListReq( strUUID.toStdString().c_str(), &data_ );
    if( ret == 0 )
    {
        QDialog::accept();
    }
}
