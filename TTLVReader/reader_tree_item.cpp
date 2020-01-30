#include "kmip.h"
#include "reader_tree_item.h"

ReaderTreeItem::ReaderTreeItem()
{
    memset( &tag_, 0x00, sizeof(BIN));
    memset( &type_, 0x00, sizeof(BIN));
    memset( &length_, 0x00, sizeof(BIN));
    memset( &value_, 0x00, sizeof(BIN));
}
