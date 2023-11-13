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

    void log( const QString strLog, QColor cr = QColor(0x00, 0x00, 0x00) );
    void elog( const QString strLog );
    void info( const QString strLog, QColor cr = QColor(0x00, 0x00, 0x00) );

    BIN& getTTLV() { return ttlv_; };
    QTableWidget* rightTable() { return right_table_; };

    void setTTLV( const BIN *pTTLV );

    void parseTree();
    void showRight();
    ReaderTreeItem* currentItem();
    void expandRoot();

private slots:
    void newFile();
    void open();
    void insertData();
    void setting();
    void about();
    void sendMsg();
    void reqEncoder();

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
    QTabWidget      *text_tab_;
    QTextEdit       *info_text_;
    QTextEdit       *log_text_;

    BIN             ttlv_;
};
#endif // MAINWINDOW_H
