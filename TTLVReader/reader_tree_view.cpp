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

    }
}

void ReaderTreeView::showRighrPart( ReaderTreeItem *pItem )
{

}
