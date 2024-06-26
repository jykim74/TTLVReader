#include "js_kms.h"
#include "req_encoder_dlg.h"
#include "js_pki.h"
#include "js_pkcs11.h"
#include "common.h"

#include <QFileDialog>

const QStringList kObjetType = { "SecretKey", "PrivateKey", "PublicKey", "Certificate" };
const QStringList kAlgList = { "RSA", "ECDSA", "AES" };
const QStringList kRSAOptionList = { "1024", "2048", "3072", "4096" };
const QStringList kECDSAOptionList = { "P-256" };
const QStringList kHashList = { "None", "SHA1", "SHA256", "SHA384", "SHA512" };

/*
const QStringList kAttrList = { "",
    "Unique Identifier", "Name", "Object Type",
    "Cryptographic Algorithm", "Cryptographic Length", "Cryptographic Parameters", "Cryptographic Domain Parameters",
    "Certificate Type", "Certificate Length", "X.509 Certificate Identifier", "X.509 Certificate Subject",
    "X.509 Certificate Issuer", "Certificate Identifier", "Certificate Subject", "Certificate Issuer",
    "Digital Signature Algorithm", "Digest", "Operation Policy Name", "Cryptographic Usage Mask",
    "Lease Time", "Usage Limits", "State", "Initial Date",
    "Activation Date", "Process Start Date", "Protect Stop Date", "Deactivation Date",
    "Destroy Date", "Compromise Occurrence Date", "Compromise Date", "Revocation Reason",
    "Archive Date", "Object Group", "Fresh", "Link",
    "Application Specific Information", "Contact Information", "Last Change Date",
    "Custom Attribute", "Alternative Name", "Key Value Present", "Key Value Location",
    "Original Creation Date", "Sensitive"
};
*/

const QStringList kAttrList = { "",
                                "Unique Identifier",
                                "Name",
                                "Object Type",
                                "Cryptographic Algorithm",
                                "Cryptographic Length",
                                "Operation Policy Name",
                                "Cryptographic Usage Mask",
                                "State",
                                "Initial Date",
};

static int _getMech( int nAlg, QString strHash )
{
    if( nAlg == JS_PKI_KEY_TYPE_RSA )
    {
        if( strHash == "None" )
            return CKM_RSA_PKCS;
        else if( strHash == "SHA1" )
            return CKM_SHA1_RSA_PKCS;
        else if( strHash == "SHA256" )
            return CKM_SHA256_RSA_PKCS;
        else if( strHash == "SHA384" )
            return CKM_SHA384_RSA_PKCS;
        else if( strHash == "SHA512" )
            return CKM_SHA512_RSA_PKCS;
        else
            return -1;
    }
    else if( nAlg == JS_PKI_KEY_TYPE_ECC )
    {
        if( strHash == "None" )
            return CKM_ECDSA;
        else if( strHash == "SHA1" )
            return CKM_ECDSA_SHA1;
        else if( strHash == "SHA256" )
            return CKM_ECDSA_SHA256;
        else if( strHash == "SHA384" )
            return CKM_ECDSA_SHA384;
        else if( strHash == "SHA512" )
            return CKM_ECDSA_SHA512;
        else
            return -1;
    }

    return -1;
}

static int _getMechHash( QString strHash )
{
    if( strHash == "SHA1" )
        return CKM_SHA_1;
    else if( strHash == "SHA256" )
        return CKM_SHA256;
    else if( strHash == "SHA384" )
        return CKM_SHA384;
    else if( strHash == "SHA512" )
        return CKM_SHA512;
    else
        return -1;
}

ReqEncoderDlg::ReqEncoderDlg(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    data_.nLen = 0;
    data_.pVal = 0;

    connect( mFindInputBtn, SIGNAL(clicked()), this, SLOT(findInput()));

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
    connect( mAddAttributeBtn, SIGNAL(clicked()), this, SLOT(clickAddAttribute()));
    connect( mGetAttributes, SIGNAL(clicked()), this, SLOT(clickGetAttributes()));
    connect( mModifyAttributeBtn, SIGNAL(clicked()), this, SLOT(clickModifyAttribute()));
    connect( mDeleteAttributeBtn, SIGNAL(clicked()), this, SLOT(clickDeleteAttribute()));
    connect( mRevokeBtn, SIGNAL(clicked()), this, SLOT(clickRevoke()));
    connect( mMACBtn, SIGNAL(clicked()), this, SLOT(clickMAC()));
    connect( mLocateBtn, SIGNAL(clicked()), this, SLOT(clickLocate()));
    connect( mDeriveKeyBtn, SIGNAL(clicked()), this, SLOT(clickDeriveKey()));
    connect( mCreateSplitKeyBtn, SIGNAL(clicked()), this, SLOT(clickCreateSplitKey()));
    connect( mJoinSplitKeyBtn, SIGNAL(clicked()), this, SLOT(clickJoinSplitKey()));
    connect( mRNGRetrieveBtn, SIGNAL(clicked()), this, SLOT(clickRNGRetrieve()));
    connect( mRNGSeedBtn, SIGNAL(clicked()), this, SLOT(clickRNGSeed()));
    connect( mHashBtn, SIGNAL(clicked()), this, SLOT(clickHash()));

    connect( mAlgCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(algChanged(int)));

    initialize();
}

ReqEncoderDlg::~ReqEncoderDlg()
{
    JS_BIN_reset( &data_ );
}

void ReqEncoderDlg::initialize()
{
    mObjectTypeCombo->addItems( kObjetType );
    mAlgCombo->addItems( kAlgList );
    mOptionCombo->addItems( kRSAOptionList );
    mHashCombo->addItems( kHashList );
    mAttributeCombo->addItems( kAttrList );
}

void ReqEncoderDlg::algChanged( int index )
{
    mOptionCombo->clear();

   if( index == 0 )
   {
       mOptionLabel->setText( tr("KeyLength") );
       mOptionCombo->addItems(kRSAOptionList);
   }
   else
   {
       mOptionLabel->setText( tr("NamedCurve" ) );
       mOptionCombo->addItems(kECDSAOptionList);
   }
}
void ReqEncoderDlg::findInput()
{
    BIN binFile = {0,0};
    char *pHex = NULL;

    QString strPath = QDir::currentPath();
    QString fileName = findFile( this, JS_FILE_TYPE_BIN, strPath );
    if( fileName.isEmpty() ) return;

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

    Authentication sAuth = {0};
    JS_KMS_makeAuthentication( mUserIDText->text().toStdString().c_str(), mPasswdText->text().toStdString().c_str(), &sAuth );

    QString strUUID = mUUIDText->text();

    ret = JS_KMS_encodeGetReq( &sAuth, strUUID.toStdString().c_str(), &data_ );
    if( ret == 0 )
    {
        QDialog::accept();
    }

    JS_KMS_resetAuthentication( &sAuth );
}

void ReqEncoderDlg::clickActivate()
{
    int ret = 0;
    JS_BIN_reset( &data_ );

    QString strUUID = mUUIDText->text();
    Authentication sAuth = {0};
    JS_KMS_makeAuthentication( mUserIDText->text().toStdString().c_str(), mPasswdText->text().toStdString().c_str(), &sAuth );


    ret = JS_KMS_encodeActivateReq( &sAuth, strUUID.toStdString().c_str(), &data_ );

    JS_KMS_resetAuthentication( &sAuth );

    if( ret == 0 )
    {
        QDialog::accept();
    }
}

void ReqEncoderDlg::clickCreate()
{
    int ret = 0;
    JS_BIN_reset( &data_ );

    Authentication sAuth = {0};
    JS_KMS_makeAuthentication( mUserIDText->text().toStdString().c_str(), mPasswdText->text().toStdString().c_str(), &sAuth );

    ret = JS_KMS_encodeCreateReq( &sAuth, &data_ );

    JS_KMS_resetAuthentication( &sAuth );

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

    Authentication sAuth = {0};
    JS_KMS_makeAuthentication( mUserIDText->text().toStdString().c_str(), mPasswdText->text().toStdString().c_str(), &sAuth );


    ret = JS_KMS_encodeDestroyReq( &sAuth, strUUID.toStdString().c_str(), &data_ );

    JS_KMS_resetAuthentication( &sAuth );

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

    Authentication sAuth = {0};
    JS_KMS_makeAuthentication( mUserIDText->text().toStdString().c_str(), mPasswdText->text().toStdString().c_str(), &sAuth );


    JS_BIN_set( &binIV, (unsigned char *)"1234567890123456", 16);
    JS_BIN_set( &binPlain, (unsigned char *)strInput.toStdString().c_str(), strInput.length() );

    ret = JS_KMS_encodeEncryptReq( &sAuth, strUUID.toStdString().c_str(), &binIV, &binPlain, &data_ );

    JS_BIN_reset( &binIV );
    JS_BIN_reset( &binPlain );

    JS_KMS_resetAuthentication( &sAuth );

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

    Authentication sAuth = {0};
    JS_KMS_makeAuthentication( mUserIDText->text().toStdString().c_str(), mPasswdText->text().toStdString().c_str(), &sAuth );


    JS_BIN_set( &binIV, (unsigned char *)"1234567890123456", 16);
    JS_BIN_decodeHex( strInput.toStdString().c_str(), &binEncrypt );

    ret = JS_KMS_encodeDecryptReq( &sAuth, strUUID.toStdString().c_str(), &binIV, &binEncrypt, &data_ );

    JS_BIN_reset( &binIV );
    JS_BIN_reset( &binEncrypt );

    JS_KMS_resetAuthentication( &sAuth );

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
    int nMech = 0;
    int nAlg = 0;
    QString strHash;

    QString strUUID = mUUIDText->text();
    QString strInput = mInputText->toPlainText();

    Authentication sAuth = {0};
    JS_KMS_makeAuthentication( mUserIDText->text().toStdString().c_str(), mPasswdText->text().toStdString().c_str(), &sAuth );


    if( mAlgCombo->currentIndex() == 0 )
        nAlg = JS_PKI_KEY_TYPE_RSA;
    else if( mAlgCombo->currentIndex() == 1 )
        nAlg = JS_PKI_KEY_TYPE_ECC;

    strHash = mHashCombo->currentText();

    nMech = _getMech( nAlg, strHash );

    JS_BIN_set( &binPlain, (unsigned char *)strInput.toStdString().c_str(), strInput.length() );

    ret = JS_KMS_encodeSignReq( &sAuth, strUUID.toStdString().c_str(), nMech, &binPlain, &data_ );

    JS_BIN_reset( &binPlain );

    JS_KMS_resetAuthentication( &sAuth );

    if( ret == 0 )
    {
        QDialog::accept();
    }
}

void ReqEncoderDlg::clickVerify()
{
    int ret = 0;
    int nAlg = 0;
    int nMech = 0;
    QString strHash;

    JS_BIN_reset( &data_ );

    BIN binPlain = {0};
    BIN binSign = {0};

    QString strUUID = mUUIDText->text();
    QString strInput = mInputText->toPlainText();
    QString strOutput = mOutputText->toPlainText();

    Authentication sAuth = {0};
    JS_KMS_makeAuthentication( mUserIDText->text().toStdString().c_str(), mPasswdText->text().toStdString().c_str(), &sAuth );


    if( mAlgCombo->currentIndex() == 0 )
        nAlg = JS_PKI_KEY_TYPE_RSA;
    else if( mAlgCombo->currentIndex() == 1 )
        nAlg = JS_PKI_KEY_TYPE_ECC;

    strHash = mHashCombo->currentText();

    nMech = _getMech( nAlg, strHash );

    JS_BIN_set( &binPlain, (unsigned char *)strInput.toStdString().c_str(), strInput.length() );
    JS_BIN_decodeHex( strOutput.toStdString().c_str(), &binSign );

    ret = JS_KMS_encodeVerifyReq( &sAuth, strUUID.toStdString().c_str(), nMech, &binPlain, &binSign, &data_ );

    JS_BIN_reset( &binPlain );
    JS_BIN_reset( &binSign );

    JS_KMS_resetAuthentication( &sAuth );

    if( ret == 0 )
    {
        QDialog::accept();
    }
}

void ReqEncoderDlg::clickRegister()
{
    int ret = 0;
    int nAlg = 0;
    int nParam = 0;
    JS_BIN_reset( &data_ );

    int nType = 0;
    BIN binInput = {0};

    Authentication sAuth = {0};
    JS_KMS_makeAuthentication( mUserIDText->text().toStdString().c_str(), mPasswdText->text().toStdString().c_str(), &sAuth );


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

    if( mAlgCombo->currentIndex() == 0 )
    {
        nAlg = JS_PKI_KEY_TYPE_RSA;
        nParam = mOptionCombo->currentText().toInt();
    }
    else if( mAlgCombo->currentIndex() == 1 )
    {
        nAlg = JS_PKI_KEY_TYPE_ECC;
        nParam = KMIP_CURVE_P_256;
    }

    ret = JS_KMS_encodeRegisterReq( &sAuth, nAlg, nParam, nType, &binInput, &data_ );

    JS_BIN_reset( &binInput );
    JS_KMS_resetAuthentication( &sAuth );

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

    Authentication sAuth = {0};
    JS_KMS_makeAuthentication( mUserIDText->text().toStdString().c_str(), mPasswdText->text().toStdString().c_str(), &sAuth );


    if( mAlgCombo->currentIndex() == 0 )
    {
        nAlg = JS_PKI_KEY_TYPE_RSA;
        nParam = mOptionCombo->currentText().toInt();
    }
    else if( mAlgCombo->currentIndex() == 1 )
    {
        nAlg = JS_PKI_KEY_TYPE_ECC;
        nParam = KMIP_CURVE_P_256;
    }

    ret = JS_KMS_encodeCreateKeyPairReq( &sAuth, nAlg, nParam, &data_ );

    JS_KMS_resetAuthentication( &sAuth );

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

    Authentication sAuth = {0};
    JS_KMS_makeAuthentication( mUserIDText->text().toStdString().c_str(), mPasswdText->text().toStdString().c_str(), &sAuth );


    ret = JS_KMS_encodeGetAttributeListReq( &sAuth, strUUID.toStdString().c_str(), &data_ );

    JS_KMS_resetAuthentication( &sAuth );

    if( ret == 0 )
    {
        QDialog::accept();
    }
}

void ReqEncoderDlg::clickAddAttribute()
{
    int ret = 0;
    JS_BIN_reset( &data_ );

    QString strUUID = mUUIDText->text();
    QString strAttrName = mAttributeCombo->currentText();
    QString strAttrValue = mInputText->toPlainText();

    Authentication sAuth = {0};
    JS_KMS_makeAuthentication( mUserIDText->text().toStdString().c_str(), mPasswdText->text().toStdString().c_str(), &sAuth );

    ret = JS_KMS_encodeAddAttributeReq( &sAuth, strUUID.toStdString().c_str(), strAttrName.toStdString().c_str(), strAttrValue.toStdString().c_str(), &data_ );

    JS_KMS_resetAuthentication( &sAuth );

    if( ret == 0 )
    {
        QDialog::accept();
    }
}

void ReqEncoderDlg::clickGetAttributes()
{
    int ret = 0;
    JS_BIN_reset( &data_ );

    QString strUUID = mUUIDText->text();

    Authentication sAuth = {0};
    JS_KMS_makeAuthentication( mUserIDText->text().toStdString().c_str(), mPasswdText->text().toStdString().c_str(), &sAuth );


    ret = JS_KMS_encodeGetAttributesReq( &sAuth, strUUID.toStdString().c_str(), NULL, &data_ );

    JS_KMS_resetAuthentication( &sAuth );

    if( ret == 0 )
    {
        QDialog::accept();
    }
}

void ReqEncoderDlg::clickModifyAttribute()
{
    int ret = 0;
    JS_BIN_reset( &data_ );

    QString strUUID = mUUIDText->text();
    QString strAttrName = mAttributeCombo->currentText();
    QString strAttrValue = mInputText->toPlainText();

    Authentication sAuth = {0};
    JS_KMS_makeAuthentication( mUserIDText->text().toStdString().c_str(), mPasswdText->text().toStdString().c_str(), &sAuth );

    ret = JS_KMS_encodeModifyAttributeReq( &sAuth, strUUID.toStdString().c_str(), strAttrName.toStdString().c_str(), strAttrValue.toStdString().c_str(), &data_ );

    JS_KMS_resetAuthentication( &sAuth );

    if( ret == 0 )
    {
        QDialog::accept();
    }
}

void ReqEncoderDlg::clickDeleteAttribute()
{
    int ret = 0;
    JS_BIN_reset( &data_ );

    QString strUUID = mUUIDText->text();
    QString strAttrName = mAttributeCombo->currentText();

    Authentication sAuth = {0};
    JS_KMS_makeAuthentication( mUserIDText->text().toStdString().c_str(), mPasswdText->text().toStdString().c_str(), &sAuth );

    ret = JS_KMS_encodeDeleteAttributeReq( &sAuth, strUUID.toStdString().c_str(), strAttrName.toStdString().c_str(), 0, &data_ );

    JS_KMS_resetAuthentication( &sAuth );

    if( ret == 0 )
    {
        QDialog::accept();
    }
}

void ReqEncoderDlg::clickRevoke()
{

}

void ReqEncoderDlg::clickMAC()
{

}

void ReqEncoderDlg::clickLocate()
{
    int ret = 0;
    int nAlg = -1;

    JS_BIN_reset( &data_ );

    Authentication sAuth = {0};
    JS_KMS_makeAuthentication( mUserIDText->text().toStdString().c_str(), mPasswdText->text().toStdString().c_str(), &sAuth );


    if( mAlgCombo->currentIndex() == 0 )
    {
        nAlg = JS_PKI_KEY_TYPE_RSA;
    }
    else if( mAlgCombo->currentIndex() == 1 )
    {
        nAlg = JS_PKI_KEY_TYPE_ECC;
    }
    else if( mAlgCombo->currentIndex() == 2 )
    {
        nAlg = JS_PKI_KEY_TYPE_AES;
    }

    ret = JS_KMS_encodeLocateReq( &sAuth, nAlg, &data_ );

    JS_KMS_resetAuthentication( &sAuth );

    if( ret == 0 )
    {
        QDialog::accept();
    }
}

void ReqEncoderDlg::clickDeriveKey()
{

}

void ReqEncoderDlg::clickCreateSplitKey()
{

}

void ReqEncoderDlg::clickJoinSplitKey()
{

}

void ReqEncoderDlg::clickRNGRetrieve()
{
    int ret = 0;
    int nLen = mLenText->text().toInt();

    Authentication sAuth = {0};
    JS_KMS_makeAuthentication( mUserIDText->text().toStdString().c_str(), mPasswdText->text().toStdString().c_str(), &sAuth );

    ret = JS_KMS_encodeRNGRetrieveReq( &sAuth, nLen, &data_ );

    JS_KMS_resetAuthentication( &sAuth );

    if( ret == 0 ) QDialog::accept();
}

void ReqEncoderDlg::clickRNGSeed()
{
    int ret = 0;
    BIN binSrc = {0};

    QString strInput = mInputText->toPlainText();
    JS_BIN_decodeHex( strInput.toStdString().c_str(), &binSrc );

    Authentication sAuth = {0};
    JS_KMS_makeAuthentication( mUserIDText->text().toStdString().c_str(), mPasswdText->text().toStdString().c_str(), &sAuth );

    ret = JS_KMS_encodeRNGSeedReq( &sAuth, &binSrc, &data_ );

    JS_KMS_resetAuthentication( &sAuth );

    if( ret == 0 ) QDialog::accept();
}

void ReqEncoderDlg::clickHash()
{
    int ret = 0;
    BIN binSrc = {0};

    QString strHash = mHashCombo->currentText();
    QString strInput = mInputText->toPlainText();

    Authentication sAuth = {0};
    JS_KMS_makeAuthentication( mUserIDText->text().toStdString().c_str(), mPasswdText->text().toStdString().c_str(), &sAuth );


    int nMech = _getMechHash( strHash );

    JS_BIN_decodeHex( strInput.toStdString().c_str(), &binSrc );

    ret = JS_KMS_encodeHashReq( &sAuth, nMech, &binSrc, &data_ );

    JS_KMS_resetAuthentication( &sAuth );

    if( ret == 0 )
    {
        QDialog::accept();
    }
}
