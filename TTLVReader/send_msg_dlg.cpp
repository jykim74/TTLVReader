#include <QFileDialog>

#include "send_msg_dlg.h"
#include "mainwindow.h"
#include "reader_applet.h"
#include "settings_mgr.h"
#include "common.h"

#include "js_kms.h"
#include "js_ssl.h"

SendMsgDlg::SendMsgDlg(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    connect( mFindCABtn, SIGNAL(clicked()), this, SLOT(findCA()));
    connect( mFindCertBtn, SIGNAL(clicked()), this, SLOT(findCert()));
    connect( mFindPriKeyBtn, SIGNAL(clicked()), this, SLOT(findPriKey()));

    connect( mSendBtn, SIGNAL(clicked()), this, SLOT(send()));
    connect( mViewResponseBtn, SIGNAL(clicked()), this, SLOT(viewResponse()));
    connect( mCloseBtn, SIGNAL(clicked()), this, SLOT(close()));

    setDefaults();
}

SendMsgDlg::~SendMsgDlg()
{

}

void SendMsgDlg::setDefaults()
{
    QString strHost = readerApplet->settingsMgr()->KMIPHost();
    QString strPort = readerApplet->settingsMgr()->KMIPPort();


    QString strCACert = "D:/jsca/ssl_root_cert.der";
    QString strClientCert = "D:/jsca/ssl_cert.der";
    QString strClientPriKey = "D:/jsca/ssl_pri.der";

//    QString strCACert = "/Users/jykim/work/certs/root_cert.der";
//    QString strClientCert = "/Users/jykim/work/certs/client_certificate_john_smith.der";
//    QString strClientPriKey = "/Users/jykim/work/certs/client_key_john_smith.der";

    mHostText->setText( strHost );
    mPortText->setText( strPort );
    mCACertPathText->setText( strCACert );
    mClientCertPathText->setText( strClientCert );
    mClientPriKeyPathText->setText( strClientPriKey );
}


void SendMsgDlg::findCA()
{
    QString strPath = QDir::currentPath();
    QString filePath = findFile( this, JS_FILE_TYPE_CERT, strPath );
    if( filePath.isEmpty() ) return;

    mCACertPathText->setText( filePath );
}

void SendMsgDlg::findCert()
{
    QString strPath = QDir::currentPath();
    QString filePath = findFile( this, JS_FILE_TYPE_CERT, strPath );
    if( filePath.isEmpty() ) return;

    mClientCertPathText->setText( filePath );
}

void SendMsgDlg::findPriKey()
{
    QString strPath = QDir::currentPath();
    QString filePath = findFile( this, JS_FILE_TYPE_PRIKEY, strPath );
    if( filePath.isEmpty() ) return;

    mClientPriKeyPathText->setText( filePath );
}

void SendMsgDlg::send()
{
    SSL_CTX *pCTX = NULL;
    SSL *pSSL = NULL;


    BIN binCA = {0,0};
    BIN binCert = {0,0};
    BIN binPriKey = {0,0};
    BIN binResponse = {0,0};
    char *pHex = NULL;

    BIN TTLV = readerApplet->mainWindow()->getTTLV();
    if( TTLV.nLen <= 0 ) return;

    QString strHost = mHostText->text();
    QString strPort = mPortText->text();
    QString strCACertPath = mCACertPathText->text();
    QString strCertPath = mClientCertPathText->text();
    QString strPriKeyPath = mClientPriKeyPathText->text();

    JS_BIN_fileRead( strCACertPath.toStdString().c_str(), &binCA );
    JS_BIN_fileRead( strCertPath.toStdString().c_str(), &binCert );
    JS_BIN_fileRead( strPriKeyPath.toStdString().c_str(), &binPriKey );

    JS_SSL_initClient( &pCTX );
    JS_SSL_setClientCACert( pCTX, &binCA );
    JS_SSL_setCertAndPriKey( pCTX, &binPriKey, &binCert );

    JS_SSL_connect( pCTX, strHost.toStdString().c_str(), strPort.toInt(), &pSSL );
    if( pSSL == NULL ) return;

    int ret = JS_KMS_send( pSSL, &TTLV );

    ret = JS_KMS_receive( pSSL, &binResponse );
    JS_BIN_encodeHex( &binResponse, &pHex );

    if( pHex )
    {
        mResponseText->setPlainText( pHex );
        if( pHex ) JS_free( pHex );
    }

    JS_BIN_reset( &binCA );
    JS_BIN_reset( &binCert );
    JS_BIN_reset( &binPriKey );
    JS_BIN_reset( &binResponse );

    JS_SSL_clear( pSSL );
    JS_SSL_finish( &pCTX );
}

void SendMsgDlg::viewResponse()
{
    BIN binTTLV = {0,0};

    JS_BIN_decodeHex( mResponseText->toPlainText().toStdString().c_str(), &binTTLV );

    readerApplet->mainWindow()->setTTLV( &binTTLV );
    readerApplet->mainWindow()->parseTree();
    readerApplet->mainWindow()->showRight();
    readerApplet->mainWindow()->expandRoot();

    QDialog::accept();


    JS_BIN_reset( &binTTLV );
}

void SendMsgDlg::close()
{
    QDialog::reject();
}
