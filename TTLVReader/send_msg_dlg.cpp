#include "send_msg_dlg.h"

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
    QString strHost = "127.0.0.1";
    QString strPort = "5696";
    QString strCACert = "/Users/jykim/work/certs/root_cert.der";
    QString strClientCert = "/Users/jykim/work/certs/client_certificate_john_smith.der";
    QString strClientPriKey = "/Users/jykim/work/certs/client_key_john_smith.der";

    mHostText->setText( strHost );
    mPortText->setText( strPort );
    mCACertPathText->setText( strCACert );
    mClientCertPathText->setText( strClientCert );
    mClientPriKeyPathText->setText( strClientPriKey );
}

void SendMsgDlg::findCA()
{

}

void SendMsgDlg::findCert()
{

}

void SendMsgDlg::findPriKey()
{

}

void SendMsgDlg::send()
{

}

void SendMsgDlg::viewResponse()
{

}

void SendMsgDlg::close()
{
    QDialog::reject();
}
