#ifndef READERTREEITEM_H
#define READERTREEITEM_H

#include "js_bin.h"

#include <QStandardItem>

class ReaderTreeItem : public QStandardItem
{
public:
    ReaderTreeItem();

    BIN* getTag() { return tag_; };
    BIN* getType() { return type_; };
    BIN* getLength() { return length_; };
    BIN* getValue() { return value_; };

    void setTag( const BIN *pTag );
    void setType( const BIN *pType );
    void setLength( const BIN *pLength );
    void setValue( const BIN *pValue );

private:
    BIN     *tag_;
    BIN     *type_;
    BIN     *length_;
    BIN     *value_;
};

#endif // READERTREEITEM_H
