#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "reader_tree_item.h"
#include "reader_tree_model.h"
#include "reader_tree_view.h"
#include "insert_data_dlg.h"
#include "reader_applet.h"
#include "about_dlg.h"
#include "settings_dlg.h"
#include "send_msg_dlg.h"
#include "edit_dlg.h"
#include "req_encoder_dlg.h"

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

    setAcceptDrops(true);

    ttlv_.nLen = 0;
    ttlv_.pVal = 0x00;
}

MainWindow::~MainWindow()
{

}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    foreach (const QUrl &url, event->mimeData()->urls()) {
        QString fileName = url.toLocalFile();
        qDebug() << "Dropped file:" << fileName;
        openTTLV( fileName );
        return;
    }
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

    sizes << 400 << 1200;
    resize( 1024, 768 );

    hsplitter_->setSizes( sizes );
    setCentralWidget(hsplitter_);

    left_tree_->header()->setVisible(false);

    createTableMenu();
}

void MainWindow::showWindow()
{
    showNormal();
    show();
    raise();
    activateWindow();
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
    QToolBar *toolToolBar = addToolBar( tr("Tool") );

    const QIcon insertIcon = QIcon::fromTheme("document-insert", QIcon(":/images/insert.jpg"));
    QAction *insertDataAct = new QAction( insertIcon, tr("&Insert data"), this );
    insertDataAct->setStatusTip(tr("Insert data"));
    connect( insertDataAct, &QAction::triggered, this, &MainWindow::insertData );
    toolMenu->addAction( insertDataAct );
    toolToolBar->addAction( insertDataAct );

    const QIcon encoderIcon = QIcon::fromTheme("document-encoder", QIcon(":/images/encoder.png"));
    QAction *reqEncoderAct = new QAction( encoderIcon, tr("ReqEncoder"), this);
    reqEncoderAct->setStatusTip(tr("Request message encoder"));
    connect( reqEncoderAct, &QAction::triggered, this, &MainWindow::reqEncoder);
    toolMenu->addAction( reqEncoderAct );
    toolToolBar->addAction( reqEncoderAct );

    const QIcon sendIcon = QIcon::fromTheme("document-send", QIcon(":/images/send.png"));
    QAction *sendMsgAct = new QAction( sendIcon, tr("Send Msg"), this );
    sendMsgAct->setStatusTip(tr("Send Message to KMIP Server"));
    connect( sendMsgAct, &QAction::triggered, this, &MainWindow::sendMsg );
    toolMenu->addAction( sendMsgAct );
    toolToolBar->addAction( sendMsgAct );

    QAction *editAct = toolMenu->addAction( tr("&Edit"), this, &MainWindow::editItem );
    editAct->setStatusTip(tr("Edit TTLV"));

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    QToolBar *helpToolBar = addToolBar( tr("Help" ));

    QAction *settingAct = helpMenu->addAction( tr("&Settings"), this, &MainWindow::setting );
    settingAct->setStatusTip(tr("Set the variable"));

    const QIcon aboutIcon = QIcon::fromTheme( "document-about", QIcon(":/images/ttlvreader.png"));
    QAction *aboutAct = new QAction( aboutIcon, tr( "&About TTLVReader"), this );
    aboutAct->setStatusTip(tr("Show the TTVLReader" ));
    connect( aboutAct, &QAction::triggered, this, &MainWindow::about );
    helpMenu->addAction( aboutAct );
    helpToolBar->addAction( aboutAct );
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::createTableMenu()
{
    QStringList     labels = { tr("Field"), "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
                               "A", "B", "C", "D", "E", "F", tr("Text") };

    QString style = "QHeaderView::section {background-color:#404040;color:#FFFFFF;}";
    right_table_->horizontalHeader()->setStyleSheet( style );

    right_table_->horizontalHeader()->setStretchLastSection(true);
    right_table_->setColumnCount(18);


    for( int i=1; i <= 16; i++ )
        right_table_->setColumnWidth(i, 30);

    right_table_->setHorizontalHeaderLabels( labels );
    right_table_->verticalHeader()->setVisible(false);
}

void MainWindow::newFile()
{
    QString cmd = readerApplet->cmd();
    QProcess *process = new QProcess();
    process->setProgram( readerApplet->cmd() );
    process->start();
}

int MainWindow::openTTLV(const QString pPath)
{
    JS_BIN_reset( &ttlv_ );

    if( !pPath.isEmpty() )
    {
        JS_BIN_fileRead( pPath.toStdString().c_str(), &ttlv_ );
    }

    left_model_->parseTree();

    return 0;
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName( this, "TTLV file",
                                    QDir::currentPath(),
                                    "All Files (*.*);;BIN files(*.bin);;Hex Files(*.hex)");


    if( fileName.isEmpty() ) return;

    openTTLV( fileName );

    QModelIndex ri = left_model_->index(0,0);
    left_tree_->expand(ri);
}

void MainWindow::insertData()
{
    InsertDataDlg insertDataDlg;
    int ret = insertDataDlg.exec();

    JS_BIN_reset( &ttlv_ );

    if( ret == QDialog::Accepted )
    {
        QString strInput = insertDataDlg.mDataValueText->toPlainText();
        strInput.remove(QRegExp("[\t\r\n\\s]"));

        if( insertDataDlg.getType() == 0 )
            JS_BIN_decodeHex( strInput.toStdString().c_str(), &ttlv_ );
        else
            JS_BIN_decodeBase64( strInput.toStdString().c_str(), &ttlv_ );

        left_model_->parseTree();
        left_tree_->showRight();

        QModelIndex ri = left_model_->index(0,0);
        left_tree_->expand(ri);
    }
}

void MainWindow::setting()
{
    SettingsDlg settingsDlg;
    settingsDlg.exec();
}

void MainWindow::about()
{
    AboutDlg aboutDlg;
    aboutDlg.exec();
}

void MainWindow::sendMsg()
{
    SendMsgDlg sendMsgDlg;
    sendMsgDlg.exec();
}

void MainWindow::reqEncoder()
{
    ReqEncoderDlg reqEncoderDlg;
    int ret = reqEncoderDlg.exec();

    if( ret == QDialog::Accepted )
    {
        JS_BIN_reset( &ttlv_ );
        JS_BIN_copy( &ttlv_, &reqEncoderDlg.getData() );
        left_model_->parseTree();
        left_tree_->showRight();

        QModelIndex ri = left_model_->index(0,0);
        left_tree_->expand(ri);
    }
}

void MainWindow::editItem()
{
    EditDlg editDlg;
    editDlg.exec();
}

void MainWindow::saveItem()
{
    QFileDialog fileDlg(this, tr("Save as..."));
    fileDlg.setAcceptMode(QFileDialog::AcceptSave);
    fileDlg.setDefaultSuffix("ber");
    if( fileDlg.exec() != QDialog::Accepted )
        return;

    QString fileName = fileDlg.selectedFiles().first();

    ReaderTreeItem *pItem = currentItem();
    if( pItem == NULL ) return;

    BIN binData = {0,0};

    JS_BIN_appendBin( &binData, pItem->getTag() );
    JS_BIN_appendBin( &binData, pItem->getType() );
    JS_BIN_appendBin( &binData, pItem->getLength() );
    JS_BIN_appendBin( &binData, pItem->getValue() );

    JS_BIN_fileWrite( &binData, fileName.toStdString().c_str() );
    JS_BIN_reset( &binData );
}

void MainWindow::saveItemValue()
{
    QFileDialog fileDlg(this, tr("Save as..."));
    fileDlg.setAcceptMode(QFileDialog::AcceptSave);
    fileDlg.setDefaultSuffix("ber");
    if( fileDlg.exec() != QDialog::Accepted )
        return;

    QString fileName = fileDlg.selectedFiles().first();

    ReaderTreeItem *pItem = currentItem();
    if( pItem == NULL ) return;

    JS_BIN_fileWrite( pItem->getValue(), fileName.toStdString().c_str() );
}

void MainWindow::setTTLV( const BIN *pTTLV )
{
    JS_BIN_reset( &ttlv_ );
    JS_BIN_copy( &ttlv_, pTTLV );
}

void MainWindow::parseTree()
{
    left_model_->parseTree();
}

void MainWindow::showRight()
{
    left_tree_->showRight();
}

ReaderTreeItem* MainWindow::currentItem()
{
    ReaderTreeItem *item = NULL;
    QModelIndex index = left_tree_->currentIndex();

    item = (ReaderTreeItem *)left_model_->itemFromIndex( index );

    return item;
}

void MainWindow::expandRoot()
{
    QModelIndex ri = left_model_->index(0,0);
    left_tree_->expand(ri);
}
