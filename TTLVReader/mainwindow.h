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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void initialize();

    BIN* getTTLV() { return ttlv_; };
    QTableWidget* rightTable() { return right_table_; };

private slots:
    void newFile();
    void open();
    void insertData();

private:
    void createActions();
    void createStatusBar();
    void createTableMenu();

    QSplitter   *hsplitter_;
    QSplitter   *vsplitter_;

    ReaderTreeView  *left_tree_;
    ReaderTreeModel *left_model_;
    QTableWidget    *right_table_;
    QTextEdit       *right_text_;

    BIN             *ttlv_;
};
#endif // MAINWINDOW_H
