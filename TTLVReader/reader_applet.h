#ifndef READERAPPLET_H
#define READERAPPLET_H

#include <QObject>
#include <QMessageBox>

class MainWindow;

class ReaderApplet : public QObject
{
    Q_OBJECT
public:
    ReaderApplet(QObject *parent = nullptr);

    void start();

    MainWindow* mainWindow() { return main_win_; };

private:
    Q_DISABLE_COPY(ReaderApplet)

    MainWindow* main_win_;
};

extern ReaderApplet *readerApplet;

#endif // READERAPPLET_H
