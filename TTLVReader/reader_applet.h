#ifndef READERAPPLET_H
#define READERAPPLET_H

#include <QObject>
#include <QMessageBox>

class MainWindow;
class SettingsMgr;

class ReaderApplet : public QObject
{
    Q_OBJECT
public:
    ReaderApplet(QObject *parent = nullptr);

    void start();

    MainWindow* mainWindow() { return main_win_; };
    SettingsMgr* settingsMgr() { return settings_mgr_; };

    void messageBox(const QString& msg, QWidget *parent=0);
    void warningBox(const QString& msg, QWidget *parent=0);
    bool yesOrNoBox(const QString& msg, QWidget *parent=0, bool default_val=true);
    bool detailedYesOrNoBox(const QString& msg, const QString& detailed_text, QWidget *parent, bool default_val=true);
    QMessageBox::StandardButton yesNoCancelBox(const QString& msg,
                                               QWidget *parent,
                                               QMessageBox::StandardButton default_btn);
    bool yesOrCancelBox(const QString& msg, QWidget *parent, bool default_ok);

    QString getBrand();
    bool closingDown() { return in_exit_ || about_to_quit_; };

    void restartApp();

private:
    Q_DISABLE_COPY(ReaderApplet)

    MainWindow* main_win_;
    SettingsMgr* settings_mgr_;

    bool started_;
    bool in_exit_;
    bool about_to_quit_;
};

extern ReaderApplet *readerApplet;

#endif // READERAPPLET_H
