#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "reader_tree_item.h"
#include "reader_tree_model.h"
#include "reader_tree_view.h"
#include "insert_data_dlg.h"

#include <QtWidgets>
#include <QFileDialog>
#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QFile>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    initialize();

    createActions();
    createStatusBar();

    ttlv_ = NULL;
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

    left_tree_->setModel(left_model_);

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

    left_tree_->header()->setVisible(false);

    createTableMenu();
}

void MainWindow::createActions()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QToolBar *fileToolBar = addToolBar( tr("File") );

    const QIcon newIcon = QIcon::fromTheme( "document-new", QIcon(":/images/new.png"));
    QAction *newAct = new QAction( newIcon, tr("&New"), this );
    newAct->setShortcut( QKeySequence::New );
    newAct->setStatusTip( tr("Create new file" ));
    connect( newAct, &QAction::triggered, this, &MainWindow::newFile );
    fileMenu->addAction( newAct );
    fileToolBar->addAction( newAct );

    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png" ));
    QAction *openAct = new QAction( openIcon, tr("&Open..."), this);
    openAct->setShortcut(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect( openAct, &QAction::triggered, this, &MainWindow::open);
    fileMenu->addAction(openAct);
    fileToolBar->addAction(openAct);

    QMenu *toolMenu = menuBar()->addMenu(tr("&Tool"));

    QAction *insertDataAct = toolMenu->addAction(tr("&Insert data"), this, &MainWindow::insertData);
    insertDataAct->setStatusTip(tr("Insert data"));
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::createTableMenu()
{
    QStringList     labels = { tr("Field"), "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
                               "A", "B", "C", "D", "E", "F", tr("Text") };

    right_table_->horizontalHeader()->setStretchLastSection(true);
    right_table_->setColumnCount(18);


    for( int i=1; i <= 16; i++ )
        right_table_->setColumnWidth(i, 30);

    right_table_->setHorizontalHeaderLabels( labels );
    right_table_->verticalHeader()->setVisible(false);
}

void MainWindow::newFile()
{

}

void MainWindow::open()
{
    if( ttlv_ == NULL )
        ttlv_ = (BIN *)JS_calloc(1, sizeof(BIN));
    else
        JS_BIN_reset( ttlv_ );

    QString fileName = QFileDialog::getOpenFileName( this, "TTLV file",
                                    QDir::currentPath(),
                                    "All Files (*.*);;BIN files(*.bin);;Hex Files(*.hex)");

    if( !fileName.isEmpty() )
    {
        JS_BIN_fileRead( fileName.toStdString().c_str(), ttlv_ );
    }

    left_model_->parseTree();
}

void MainWindow::insertData()
{
    InsertDataDlg insertDataDlg;
    int ret = insertDataDlg.exec();

    if( ttlv_ == NULL )
        ttlv_ = (BIN *)JS_calloc(1, sizeof(BIN));
    else
        JS_BIN_reset( ttlv_ );

    if( ret == QDialog::Accepted )
    {
        QString strInput = insertDataDlg.mDataValueText->toPlainText();
        strInput.remove(QRegExp("[\t\r\n\\s]"));

        if( insertDataDlg.getType() == 0 )
            JS_BIN_decodeHex( strInput.toStdString().c_str(), ttlv_ );
        else
            JS_BIN_decodeBase64( strInput.toStdString().c_str(), ttlv_ );

        left_model_->parseTree();
        left_tree_->showRight();
    }
}
