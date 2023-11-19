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
#include "common.h"

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
    info_text_ = new QTextEdit();
    info_text_->setReadOnly(true);
    log_text_ = new QTextEdit();
    log_text_->setReadOnly(true);
    right_table_ = new QTableWidget;

    left_tree_->setModel(left_model_);

    hsplitter_->addWidget(left_tree_);
    hsplitter_->addWidget(vsplitter_);
    vsplitter_->addWidget(right_table_);

    text_tab_ = new QTabWidget;
    vsplitter_->addWidget(text_tab_);
    text_tab_->setTabPosition(QTabWidget::South);
    text_tab_->addTab( info_text_, tr("Information" ));
    text_tab_->addTab( log_text_, tr("Log"));

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

void MainWindow::log( const QString strLog, QColor cr )
{
    if( text_tab_->count() <= 1 ) return;

    QTextCursor cursor = log_text_->textCursor();
    //    cursor.movePosition( QTextCursor::End );

    QTextCharFormat format;
    format.setForeground( cr );
    cursor.mergeCharFormat(format);

    cursor.insertText( strLog );
    cursor.insertText( "\n" );

    log_text_->setTextCursor( cursor );
    log_text_->repaint();
}

void MainWindow::elog( const QString strLog )
{
    log( strLog, QColor(0xFF,0x00,0x00));
}

void MainWindow::info( const QString strLog, QColor cr )
{
    QTextCursor cursor = info_text_->textCursor();

    QTextCharFormat format;
    format.setForeground( cr );
    cursor.mergeCharFormat(format);

    cursor.insertText( strLog );

    info_text_->setTextCursor( cursor );
    info_text_->repaint();
}

void MainWindow::infoClear()
{
    info_text_->clear();
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

    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
    QToolBar *editToolBar = addToolBar(tr("Edit"));

    const QIcon copyIcon = QIcon::fromTheme("edit-copy", QIcon(":/images/copy.png"));
    QAction *copyAct = new QAction(copyIcon, tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the clipboard"));
    connect( copyAct, &QAction::triggered, left_tree_, &ReaderTreeView::copy );
    editMenu->addAction(copyAct);
    //    editToolBar->addAction(copyAct);

    QAction *copyAsHexAct = editMenu->addAction(tr("Copy As &Hex"), left_tree_, &ReaderTreeView::CopyAsHex);
    copyAsHexAct->setStatusTip(tr("Copy ber data as hex"));

    QAction *copyAsBase64Act = editMenu->addAction(tr("Copy As &Base64"), left_tree_, &ReaderTreeView::CopyAsBase64);
    copyAsBase64Act->setStatusTip(tr("Copy ber data as base64"));

    const QIcon expandAllIcon = QIcon::fromTheme("expand-all", QIcon(":/images/expand_all.png"));
    QAction *expandAllAct = new QAction(expandAllIcon, tr("&Expand All"), this );
    expandAllAct->setShortcut( QKeySequence(Qt::Key_F5) );
    expandAllAct->setStatusTip(tr("Show all nodes"));
    connect( expandAllAct, &QAction::triggered, left_tree_, &ReaderTreeView::treeExpandAll );
    editMenu->addAction(expandAllAct);
    editToolBar->addAction(expandAllAct);

    const QIcon expandNodeIcon = QIcon::fromTheme("expand-node", QIcon(":/images/expand_node.png"));
    QAction *expandNodeAct = new QAction(expandNodeIcon, tr("&Expand Node"), this );
    expandNodeAct->setStatusTip(tr("Show node"));
    expandNodeAct->setShortcut( QKeySequence(Qt::Key_F6));
    connect( expandNodeAct, &QAction::triggered, left_tree_, &ReaderTreeView::treeExpandNode );
    editMenu->addAction(expandNodeAct);
    editToolBar->addAction(expandNodeAct);

    const QIcon collapseAllIcon = QIcon::fromTheme("collapse-all", QIcon(":/images/collapse_all.png"));
    QAction *collapseAllAct = new QAction(collapseAllIcon, tr("&Collapse All"), this );
    collapseAllAct->setStatusTip(tr("Collapse all nodes"));
    collapseAllAct->setShortcut( QKeySequence(Qt::Key_F7));
    connect( collapseAllAct, &QAction::triggered, left_tree_, &ReaderTreeView::treeCollapseAll );
    editMenu->addAction(collapseAllAct);
    editToolBar->addAction(collapseAllAct);

    const QIcon collapseNodeIcon = QIcon::fromTheme("collapse-node", QIcon(":/images/collapse_node.png"));
    QAction *collapseNodeAct = new QAction(collapseNodeIcon, tr("&Collapse Node"), this );
    collapseNodeAct->setStatusTip(tr("Show node"));
    collapseNodeAct->setShortcut( QKeySequence(Qt::Key_F8));
    connect( collapseNodeAct, &QAction::triggered, left_tree_, &ReaderTreeView::treeCollapseNode );
    editMenu->addAction(collapseNodeAct);
    editToolBar->addAction(collapseNodeAct);

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
    right_table_->horizontalHeader()->setMinimumSectionSize(10);
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
    QString strPath = QDir::currentPath();
    QString fileName = findFile( this, JS_FILE_TYPE_BIN, strPath );
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

QString MainWindow::getInfo()
{
    return info_text_->toPlainText();
}
