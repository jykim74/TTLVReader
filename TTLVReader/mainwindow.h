#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QTableWidget>
#include <QTextEdit>

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

private slots:
    void newFile();
    void open();

private:
    void createActions();
    void createStatusBar();

    QSplitter   *hsplitter_;
    QSplitter   *vsplitter_;

    ReaderTreeView  *left_tree_;
    ReaderTreeModel *left_model_;
    QTableWidget    *right_table_;
    QTextEdit       *right_text_;
};
#endif // MAINWINDOW_H
