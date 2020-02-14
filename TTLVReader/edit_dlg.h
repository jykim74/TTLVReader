#ifndef EDIT_DLG_H
#define EDIT_DLG_H

#include <QDialog>
#include "ui_edit_dlg.h"

namespace Ui {
class EditDlg;
}

class EditDlg : public QDialog, public Ui::EditDlg
{
    Q_OBJECT

public:
    explicit EditDlg(QWidget *parent = nullptr);
    ~EditDlg();

private slots:
    virtual void accept();

private:
    void initialize();
};

#endif // EDIT_DLG_H
