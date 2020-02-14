#ifndef SEND_MSG_DLG_H
#define SEND_MSG_DLG_H

#include <QDialog>
#include "ui_send_msg_dlg.h"

namespace Ui {
class SendMsgDlg;
}

class SendMsgDlg : public QDialog, public Ui::SendMsgDlg
{
    Q_OBJECT

public:
    explicit SendMsgDlg(QWidget *parent = nullptr);
    ~SendMsgDlg();

private slots:
    void findCA();
    void findCert();
    void findPriKey();
    void send();
    void viewResponse();
    void close();

private:
    void setDefaults();
    QString getFilePath();
};

#endif // SEND_MSG_DLG_H
