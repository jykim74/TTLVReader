#include "mainwindow.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include "reader_applet.h"
#include "i18n_helper.h"
#include "settings_mgr.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName( "JS" );
    QCoreApplication::setOrganizationDomain( "jssoft.com" );
    QCoreApplication::setApplicationName( "TTLVReader" );

    QCommandLineParser parser;
    parser.setApplicationDescription( QCoreApplication::applicationName() );
    parser.addHelpOption();
    parser.addPositionalArgument( "file", "a file to open" );
    parser.process(app);

    I18NHelper::getInstance()->init();

    ReaderApplet mApplet;
    readerApplet = &mApplet;
    readerApplet->setCmd( argv[0] );
    readerApplet->start();

    QFont font;
    QString strFont = readerApplet->settingsMgr()->getFontFamily();

    font.setFamily( strFont );
    app.setFont(font);

    return app.exec();
}
