#include "mainwindow.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include "reader_applet.h"
#include "i18n_helper.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName( "JS" );
    QCoreApplication::setOrganizationDomain( "jssoft.com" );
    QCoreApplication::setApplicationName( "TTLVReader" );

    QFile qss(":/ttlvreader.qss");
    qss.open( QFile::ReadOnly );
    app.setStyleSheet(qss.readAll());

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

    return app.exec();
}
