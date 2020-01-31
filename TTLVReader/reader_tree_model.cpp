#include "reader_tree_model.h"

ReaderTreeModel::ReaderTreeModel( QObject *parent )
    : QStandardItemModel( parent )
{

}

int ReaderTreeModel::parseTree()
{
    return 0;
}

int ReaderTreeModel::parseConstruct( int offset, ReaderTreeItem *pParentItem )
{
    return 0;
}

int ReaderTreeModel::getItem( int offset, ReaderTreeItem *pItem )
{
    return 0;
}
