#include "about_dlg.h"
#include "reader_applet.h"
#include "auto_update_service.h"
#include "js_gen.h"
#include "about_dlg.h"

AboutDlg::AboutDlg(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    setWindowTitle(tr("About %1").arg(readerApplet->getBrand()));
    setWindowFlags( (windowFlags() & ~Qt::WindowContextHelpButtonHint) | Qt::WindowStaysOnTopHint );

    version_label_ = tr( "About %1 (%2)").arg( "TTLVReader").arg(STRINGIZE(TTLV_READER_VERSION));
    mVersionLabel->setText( version_label_ );

    connect(mOKBtn, SIGNAL(clicked()), this, SLOT(close()));
#ifdef _AUTO_UPDATE
    if( AutoUpdateService::instance()->shouldSupportAutoUpdate() )
    {
        mCheckUpdateBtn->setVisible(true);
        connect(mCheckUpdateBtn, SIGNAL(clicked()), this, SLOT(checkUpdate()));
    }
#endif

    QString strAbout = tr("This is freeware tool to decode TTLV in kmip "
            "If you do not use this for commercial purposes, "
            "you can use it freely "
            "If you have any opinions on this tool, please send me a mail." );

    QString strLibVersion = JS_GEN_getBuildInfo();

    strAbout += "\r\n\r\nLibrary: ";
    strAbout += strLibVersion;

    strAbout += "\r\n";
    strAbout += getBuild();
    strAbout += "\r\n";

    QString strAppend = tr( "Copyright (C) 2019 ~ 2020 JongYeob Kim\r\n"
            "mailto : jykim74@gmail.com");

    strAbout += strAppend;

    mAboutText->setText( strAbout );
}

AboutDlg::~AboutDlg()
{
//    delete ui;
}

QString AboutDlg::getBuild()
{
    QString strBuild = QString( "Build Date: %1 %2").arg( __DATE__ ).arg( __TIME__ );
    return strBuild;
}

#ifdef _AUTO_UPDATE
void AboutDlg::checkUpdate()
{
    AutoUpdateService::instance()->checkUpdate();
    close();
}
#endif
