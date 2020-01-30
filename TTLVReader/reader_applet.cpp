#include "reader_applet.h"

#include "mainwindow.h"

ReaderApplet    *readerApplet;

ReaderApplet::ReaderApplet( QObject *parent )
    : QObject(parent)
{
    main_win_ = new MainWindow;
}

void ReaderApplet::start()
{
    main_win_->show();
}
