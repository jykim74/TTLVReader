#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QTableWidget>
#include <QTextEdit>

#include "js_bin.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class ReaderTreeView;
class ReaderTreeModel;
class ReaderTreeItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void initialize();
    void showWindow();

    BIN* getTTLV() { return ttlv_; };
    QTableWidget* rightTable() { return right_table_; };
    QTextEdit* rightText() { return right_text_; };

    void parseTree();
    void showRight();
    ReaderTreeItem* currentItem();

private slots:
    void newFile();
    void open();
    void insertData();
    void setting();
    void about();
    void sendMsg();

    virtual void dragEnterEvent( QDragEnterEvent * event );
    virtual void dropEvent( QDropEvent *event );

public slots:
    void editItem();
    void saveItem();
    void saveItemValue();

private:
    void createActions();
    void createStatusBar();
    void createTableMenu();

    int openTTLV( const QString pPath );

    QSplitter   *hsplitter_;
    QSplitter   *vsplitter_;

    ReaderTreeView  *left_tree_;
    ReaderTreeModel *left_model_;
    QTableWidget    *right_table_;
    QTextEdit       *right_text_;

    BIN             *ttlv_;
};
#endif // MAINWINDOW_H
