#ifndef REQ_ENCODER_DLG_H
#define REQ_ENCODER_DLG_H

#include <QDialog>
#include "js_bin.h"
#include "ui_req_encoder_dlg.h"

namespace Ui {
class ReqEncoderDlg;
}

class ReqEncoderDlg : public QDialog, public Ui::ReqEncoderDlg
{
    Q_OBJECT

public:
    explicit ReqEncoderDlg(QWidget *parent = nullptr);
    ~ReqEncoderDlg();
    BIN& getData() { return data_; };

private slots:
    void clickGet();
    void clickActivate();
    void clickCreate();
    void clickDestroy();
    void clickEncrypt();
    void clickDecrypt();
    void clickSign();
    void clickVerify();
    void clickRegister();
    void clickCreateKeyPair();

    void clickGetAttributeList();
    void clickSetAttribute();
    void clickGetAttribute();
    void clickModifyAttribute();
    void clickDeleteAttribute();
    void clickRevoke();
    void clickMAC();
    void clickLocate();
    void clickDeriveKey();
    void clickCreateSplitKey();
    void clickJoinSplitKey();
    void clickRNGRetrieve();
    void clickRNGSeed();
    void clickHash();


    void findInput();
    void algChanged( int index );

private:
    void initialize();
    BIN data_;
};

#endif // REQ_ENCODER_DLG_H
