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

}

void ReaderTreeView::showRight()
{
    ReaderTreeModel *left_model = (ReaderTreeModel *)model();
    ReaderTreeItem  *rootItem = (ReaderTreeItem *)left_model->item(0);

    showRightFull( rootItem );
//    showRighrPart( rootItem );

    setExpanded( rootIndex(), true );
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
    BIN *pTTLV = readerApplet->mainWindow()->getTTLV();

    int row_cnt = rightTable->rowCount();
    for( int k=0; k < row_cnt; k++ )
        rightTable->removeRow(0);

    for( int i = 0; i < pTTLV->nLen; i++ )
    {
        int pos = 0;
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
        else if( i >= pItem->getOffset() + 8 && i < pItem->getOffset() + 8 + pItem->getLengthInt() )
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
}

void ReaderTreeView::showRighrPart( ReaderTreeItem *pItem )
{

}
