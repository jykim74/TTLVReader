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
    JS_BIN_reset( tag_ );

    if( pTag ) JS_BIN_copy( tag_, pTag );
}

void ReaderTreeItem::setType( const BIN *pType )
{
    JS_BIN_reset( type_ );
    if( pType ) JS_BIN_copy( type_, pType );
}

void ReaderTreeItem::setLength( const BIN *pLength )
{
    JS_BIN_reset( length_ );
    if( pLength ) JS_BIN_copy( length_, pLength );
}

void ReaderTreeItem::setValue( const BIN *pValue )
{
    JS_BIN_reset( value_ );
    if( pValue ) JS_BIN_copy( value_, pValue );
}
