#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "reader_tree_item.h"
#include "reader_tree_model.h"
#include "reader_tree_view.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    initialize();
}

MainWindow::~MainWindow()
{

}

void MainWindow::initialize()
{
    hsplitter_ = new QSplitter(Qt::Horizontal);
    vsplitter_ = new QSplitter(Qt::Vertical);
    left_tree_ = new ReaderTreeView(this);
    left_model_ = new ReaderTreeModel(this);
    right_text_ = new QTextEdit();
    right_table_ = new QTableWidget;

    hsplitter_->addWidget(left_tree_);
    hsplitter_->addWidget(vsplitter_);
    vsplitter_->addWidget(right_table_);
    vsplitter_->addWidget(right_text_);

    QList <int> vsizes;
    vsizes << 1200 << 500;
    vsplitter_->setSizes( vsizes );

    QList <int> sizes;
    sizes << 500 << 1200;
    resize( 1024, 768 );

    hsplitter_->setSizes( sizes );
    setCentralWidget(hsplitter_);
}
