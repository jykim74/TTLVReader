#include "js_bin.h"
#include "kmip.h"
#include "reader_tree_item.h"

ReaderTreeItem::ReaderTreeItem()
{
    tag_ = (BIN *)JS_calloc(1, sizeof(BIN));
    type_ = (BIN *)JS_calloc(1, sizeof(BIN));
    length_ = (BIN *)JS_calloc(1, sizeof(BIN));
    value_ = (BIN *)JS_calloc(1, sizeof(BIN));
}

void ReaderTreeItem::setTag( const BIN *pTag )
{
    if( pTag )
        JS_BIN_copy( tag_, pTag );
    else
        JS_BIN_reset( tag_ );
}

void ReaderTreeItem::setType( const BIN *pType )
{

}

void ReaderTreeItem::setLength( const BIN *pLength )
{

}

void ReaderTreeItem::setValue( const BIN *pValue )
{

}
