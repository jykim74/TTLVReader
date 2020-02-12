#include "reader_tree_view.h"
#include "reader_tree_item.h"
#include "reader_tree_model.h"

#include <QStandardItemModel>
#include <QTreeView>
#include <QTableWidget>

#include "js_bin.h"
#include "mainwindow.h"
#include "reader_applet.h"

ReaderTreeView::ReaderTreeView( QWidget *parent )
    : QTreeView(parent)
{
    connect( this, SIGNAL(clicked(const QModelIndex&)), this, SLOT(onItemClicked(const QModelIndex&)));
    setContextMenuPolicy(Qt::CustomContextMenu);
}

void ReaderTreeView::showRight()
{
    ReaderTreeModel *left_model = (ReaderTreeModel *)model();
    ReaderTreeItem  *rootItem = (ReaderTreeItem *)left_model->item(0);

    showRightFull( rootItem );
//    showRightPart( rootItem );

    setExpanded( rootIndex(), true );
}

void ReaderTreeView::onItemClicked( const QModelIndex& index )
{
    ReaderTreeModel *left_model = (ReaderTreeModel *)model();
    ReaderTreeItem *item = (ReaderTreeItem *)left_model->itemFromIndex(index);



    showRightFull( item );
}

void ReaderTreeView::ShowContextMenu( QPoint point )
{

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
    QColor magenta(Qt::magenta);
    QColor lightGray(Qt::lightGray);


    int len_len = 0;
    int start_col = 0;
    int start_row = 0;

    QTableWidget* rightTable = readerApplet->mainWindow()->rightTable();
    BIN *pTTLV = readerApplet->mainWindow()->getTTLV();

    int row_cnt = rightTable->rowCount();
    for( int k=0; k < row_cnt; k++ )
        rightTable->removeRow(0);

    for( int i = 0; i < pTTLV->nLen; i++ )
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
        }

        hex.sprintf( "%02X", pTTLV->pVal[i] );
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
            rightTable->item( line, pos )->setBackgroundColor(magenta);
        }
        else if( i >= (pItem->getOffset() + 8 + len) && i < (pItem->getOffset() + 8 + len + pad ))
        {
            rightTable->item( line, pos )->setBackgroundColor(lightGray);
        }


        text += getch( pTTLV->pVal[i] );

        if( i % 16 - 15 == 0 )
        {
            rightTable->setItem( line, 17, new QTableWidgetItem(text));
            text.clear();
            line++;
        }
    }

    if( !text.isEmpty() ) rightTable->setItem( line, 17, new QTableWidgetItem(text));

    QTextEdit *rightText = readerApplet->mainWindow()->rightText();
    QString strInfo = getInfoView( pItem );

    rightText->setText( strInfo );
}

void ReaderTreeView::showRightPart( ReaderTreeItem *pItem )
{

}

QString ReaderTreeView::getInfoView(ReaderTreeItem *pItem)
{
    QString strView;
    QString strPart;

    strPart = QString( "Tag: %1(%2)\n" ).arg( pItem->getTagName() ).arg( pItem->getTagHex() );
    strView += strPart;

    strPart = QString( "Type: %1(%2)\n").arg( pItem->getTypeName() ).arg( pItem->getTypeHex() );
    strView += strPart;

    strPart = QString( "Length: %1(%2)\n" ).arg( pItem->getLengthInt() ).arg( pItem->getLengthHex() );
    strView += strPart;

    strPart = QString( "\nValue\n %1").arg( pItem->getValueHex() );
    strView += strPart;

    return strView;
}
