#include "kmip.h"
#include "js_kms.h"

#include "reader_tree_model.h"
#include "reader_tree_item.h"
#include "reader_tree_view.h"

#include "mainwindow.h"
#include "reader_applet.h"

ReaderTreeModel::ReaderTreeModel( QObject *parent )
    : QStandardItemModel( parent )
{

}

int ReaderTreeModel::parseTree()
{
    int     ret = 0;
    int     offset = 0;

    ReaderTreeItem  *pRootItem = new ReaderTreeItem();

    clear();

    pRootItem->setOffset(offset);
    pRootItem->setLevel(0);

    offset = getItem( offset, pRootItem );
    insertRow( 0, pRootItem );

    pRootItem->setText( pRootItem->getTagHex() );

    BIN *pType = pRootItem->getType();

    if( pType && pType->pVal[0] == 0x01 ) // In case of structure
    {
        ret = parseConstruct( offset + 8, pRootItem );
    }

    return 0;
}

int ReaderTreeModel::parseConstruct( int offset, ReaderTreeItem *pParentItem )
{
    int         ret = 0;
    int         next_offset = 0;
    int         bStructed = 0;
    int         start_offset = offset;
    int         level = pParentItem->getLevel() + 1;

    BIN         *pTTLV = readerApplet->mainWindow()->getTTLV();

    if( pTTLV == NULL || pTTLV->nLen <= offset ) return -1;

    do {
        ReaderTreeItem *pItem = new ReaderTreeItem();
        pItem->setOffset( offset );
        pItem->setLevel( level );

        next_offset = getItem( offset, pItem );

        pItem->setText( pItem->getTagHex() );
        pParentItem->appendRow( pItem );

        if( pItem->getTypeHex() == "01" )
            bStructed = 1;
        else
            bStructed = 0;

        if( bStructed )
        {
            parseConstruct( offset + 8, pItem );
        }

        offset = next_offset;
    } while ( next_offset > 0 && next_offset < pTTLV->nLen );

    return 0;
}

int ReaderTreeModel::getItem( int offset, ReaderTreeItem *pItem )
{
    int     next_offset = 0;
    int     position = 0;
    int     length = 0;

    BIN     *pTTLV = readerApplet->mainWindow()->getTTLV();
    if( pTTLV == NULL ) return -1;

    return 0;
}
