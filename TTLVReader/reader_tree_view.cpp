#include <QMenu>
#include <QGuiApplication>
#include <QClipboard>

#include "reader_tree_view.h"
#include "reader_tree_item.h"
#include "reader_tree_model.h"

#include <QStandardItemModel>
#include <QTreeView>
#include <QTableWidget>
#include <QFileDialog>

#include "js_bin.h"
#include "mainwindow.h"
#include "reader_applet.h"
#include "settings_mgr.h"
#include "common.h"

ReaderTreeView::ReaderTreeView( QWidget *parent )
    : QTreeView(parent)
{
    connect( this, SIGNAL(clicked(const QModelIndex&)), this, SLOT(onItemClicked(const QModelIndex&)));
    setContextMenuPolicy(Qt::CustomContextMenu);

    connect( this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(leftContextMenu(QPoint)));

    QFile qss(":/ttlvreader.qss");
    qss.open( QFile::ReadOnly );
    setStyleSheet(qss.readAll());
    qss.close();
}

ReaderTreeItem* ReaderTreeView::currentItem()
{
    QModelIndex index = currentIndex();

    ReaderTreeModel *tree_model = (ReaderTreeModel *)model();
    ReaderTreeItem *item = (ReaderTreeItem *)tree_model->itemFromIndex(index);

    return item;
}


void ReaderTreeView::showRight()
{
    ReaderTreeModel *left_model = (ReaderTreeModel *)model();
    ReaderTreeItem  *rootItem = (ReaderTreeItem *)left_model->item(0);

    SettingsMgr *setMgr = readerApplet->settingsMgr();

    if( setMgr->showFullText() )
        showRightFull( rootItem );
    else
        showRightPart( rootItem );

    setExpanded( rootIndex(), true );
}

void ReaderTreeView::onItemClicked( const QModelIndex& index )
{
    ReaderTreeModel *left_model = (ReaderTreeModel *)model();
    ReaderTreeItem *item = (ReaderTreeItem *)left_model->itemFromIndex(index);

    SettingsMgr *setMgr = readerApplet->settingsMgr();

    if( setMgr->showFullText() )
        showRightFull( item );
    else
        showRightPart( item );
}

void ReaderTreeView::leftContextMenu( QPoint point )
{
    QMenu menu(this);

    menu.addAction( tr("Edit"), readerApplet->mainWindow(), &MainWindow::editItem );
    menu.addAction( tr("SaveItem"), readerApplet->mainWindow(), &MainWindow::saveItem );
    menu.addAction( tr("SaveItemValue"), readerApplet->mainWindow(), &MainWindow::saveItemValue );

    menu.exec(QCursor::pos());
}

static char getch( unsigned char c )
{
    if( isprint(c) )
        return c;
    else {
        return '.';
    }
}

void ReaderTreeView::showRightFull( ReaderTreeItem *pItem )
{
    int line = 0;

    QString text;
    QString hex;
    QColor green(Qt::green);
    QColor yellow(Qt::yellow);
    QColor cyan(Qt::cyan);
    QColor lightGray(Qt::lightGray);


    int len_len = 0;
    int start_col = 0;
    int start_row = 0;

    QTableWidget* rightTable = readerApplet->mainWindow()->rightTable();
    BIN TTLV = readerApplet->mainWindow()->getTTLV();

    int row_cnt = rightTable->rowCount();
    for( int k=0; k < row_cnt; k++ )
        rightTable->removeRow(0);

    for( int i = 0; i < TTLV.nLen; i++ )
    {
        int pos = 0;
        int len = 0;
        int pad = 0;

        if( i % 16 == 0 )
        {
            rightTable->insertRow(line);
            QString address;
            address.sprintf( "0x%08X", i );
            rightTable->setItem( line, 0, new QTableWidgetItem(address));
            rightTable->item( line, 0 )->setBackgroundColor( QColor(220,220,250) );
        }

        hex.sprintf( "%02X", TTLV.pVal[i] );
        pos = (i%16) + 1;
        rightTable->setItem( line, pos, new QTableWidgetItem(hex));

        len = pItem->getLengthInt();
        pad = 8 - (len % 8);
        if( pad == 8 ) pad = 0;

        if( i >= pItem->getOffset() && i < pItem->getOffset() + 3 )
        {
            rightTable->item( line, pos )->setBackgroundColor(green);
        }
        else if( i == pItem->getOffset() + 3 )
        {
            rightTable->item( line, pos )->setBackgroundColor(yellow);
        }
        else if( i >= pItem->getOffset() + 4 && i < pItem->getOffset() + 8 )
        {
            rightTable->item( line, pos )->setBackgroundColor(cyan);
        }
        else if( i >= pItem->getOffset() + 8 && i < pItem->getOffset() + 8 + len )
        {
            rightTable->item( line, pos )->setBackgroundColor(kValueColor);
        }
        else if( i >= (pItem->getOffset() + 8 + len) && i < (pItem->getOffset() + 8 + len + pad ))
        {
            rightTable->item( line, pos )->setBackgroundColor(lightGray);
        }


        text += getch( TTLV.pVal[i] );

        if( i % 16 - 15 == 0 )
        {
            rightTable->setItem( line, 17, new QTableWidgetItem(text));
            rightTable->item( line, 17 )->setBackgroundColor(QColor(210,240,210));
            text.clear();
            line++;
        }
    }

    if( !text.isEmpty() )
    {
        rightTable->setItem( line, 17, new QTableWidgetItem(text));
        rightTable->item( line, 17 )->setBackgroundColor(QColor(210,240,210));
    }

    getInfoView( pItem );
}

void ReaderTreeView::showRightPart( ReaderTreeItem *pItem )
{
    int line = 0;

    QString text;
    QString hex;
    QColor green(Qt::green);
    QColor yellow(Qt::yellow);
    QColor cyan(Qt::cyan);
    QColor lightGray(Qt::lightGray);
    BIN     binPart = {0,0};

    int length = 0;
    int pad = 0;

    QTableWidget* rightTable = readerApplet->mainWindow()->rightTable();
    BIN TTLV = readerApplet->mainWindow()->getTTLV();

    length = pItem->getLengthInt();
    pad = 8 - length % 8;
    if( pad == 8 ) pad = 0;

    JS_BIN_set( &binPart, TTLV.pVal + pItem->getOffset(), 8 + length + pad );

    int row_cnt = rightTable->rowCount();
    for( int k=0; k < row_cnt; k++ )
        rightTable->removeRow(0);

    for( int i = 0; i < binPart.nLen; i++ )
    {
        int pos = 0;

        if( i % 16 == 0 )
        {
            rightTable->insertRow(line);
            QString address;
            address.sprintf( "0x%08X", i );
            rightTable->setItem( line, 0, new QTableWidgetItem(address));
            rightTable->item( line, 0 )->setBackgroundColor( QColor(220,220,250) );
        }

        hex.sprintf( "%02X", binPart.pVal[i] );
        pos = (i%16) + 1;
        rightTable->setItem( line, pos, new QTableWidgetItem(hex));

        if( i >= 0 && i < 3 )
        {
            rightTable->item( line, pos )->setBackgroundColor(green);
        }
        else if( i == 3 )
        {
            rightTable->item( line, pos )->setBackgroundColor(yellow);
        }
        else if( i >= 4 && i < 8 )
        {
            rightTable->item( line, pos )->setBackgroundColor(cyan);
        }
        else if( i >= 8 && i < 8 + length )
        {
            rightTable->item( line, pos )->setBackgroundColor(kValueColor);
        }
        else if( i >= (8 + length ) && i < ( 8 + length + pad ))
        {
            rightTable->item( line, pos )->setBackgroundColor(lightGray);
        }


        text += getch( binPart.pVal[i] );

        if( i % 16 - 15 == 0 )
        {
            rightTable->setItem( line, 17, new QTableWidgetItem(text));
            rightTable->item( line, 17 )->setBackgroundColor(QColor(210,240,210));
            text.clear();
            line++;
        }
    }

    if( !text.isEmpty() )
    {
        rightTable->setItem( line, 17, new QTableWidgetItem(text));
        rightTable->item( line, 17 )->setBackgroundColor(QColor(210,240,210));
    }

    getInfoView( pItem );

    JS_BIN_reset( &binPart );
}

void ReaderTreeView::getInfoView(ReaderTreeItem *pItem)
{
    readerApplet->mainWindow()->infoClear();
    readerApplet->info( "========================================================================\n" );
    readerApplet->info( "== TTLV Information\n" );
    readerApplet->info( "========================================================================\n" );

    readerApplet->info( QString( "Tag    : %1(%2)\n" ).arg( pItem->getTagName() ).arg( pItem->getTagHex() ));
    readerApplet->info( QString( "Type   : %1(%2)\n").arg( pItem->getTypeName() ).arg( pItem->getTypeHex() ));
    readerApplet->info( QString( "Length : %1(%2)\n" ).arg( pItem->getLengthInt() ).arg( pItem->getLengthHex() ));
    readerApplet->info( QString( "Offset : %1(%2)\n").arg( pItem->getOffset() ).arg( pItem->getOffset(), 0, 16) );
    readerApplet->info( QString( "Value  : %1").arg( pItem->getPrintValue() ) );
}

void ReaderTreeView::CopyAsHex()
{
    char *pHex = NULL;
    ReaderTreeItem* item = currentItem();
    if( item == NULL )
    {
        readerApplet->warningBox( tr( "There is no selected item"), this );
        return;
    }

    QClipboard *clipboard = QGuiApplication::clipboard();

    BIN *pVal = item->getValue();
    JS_BIN_encodeHex( pVal, &pHex );
    clipboard->setText(pHex);
    if( pHex ) JS_free(pHex);
}

void ReaderTreeView::CopyAsBase64()
{
    char *pBase64 = NULL;
    ReaderTreeItem* item = currentItem();
    if( item == NULL )
    {
        readerApplet->warningBox( tr( "There is no selected item"), this );
        return;
    }

    QClipboard *clipboard = QGuiApplication::clipboard();
    BIN *pVal = item->getValue();
    JS_BIN_encodeBase64( pVal, &pBase64 );
    clipboard->setText(pBase64);
    if( pBase64 ) JS_free(pBase64);
}

void ReaderTreeView::copy()
{
    ReaderTreeItem* item = currentItem();
    if( item == NULL )
    {
        readerApplet->warningBox( tr( "There is no selected item"), this );
        return;
    }

    QClipboard *clipboard = QGuiApplication::clipboard();

    QString strLog = readerApplet->mainWindow()->getInfo();
    clipboard->setText(strLog);
}

void ReaderTreeView::treeExpandAll()
{
    expandAll();
}

void ReaderTreeView::treeExpandNode()
{
    QModelIndex index = currentIndex();
    expand(index);
}

void ReaderTreeView::treeCollapseAll()
{
    collapseAll();
}

void ReaderTreeView::treeCollapseNode()
{
    QModelIndex index = currentIndex();
    collapse(index);
}
