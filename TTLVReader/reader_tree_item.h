#ifndef READERTREEITEM_H
#define READERTREEITEM_H

#include "kmip.h"
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

    int getOffset() { return offset_; };
    int getLevel() { return level_; };

    void setTag( const BIN *pTag );
    void setType( const BIN *pType );
    void setLength( const BIN *pLength );
    void setValue( const BIN *pValue );
    void setOffset( int offset );
    void setLevel( int level );

    QString getTagHex();
    QString getTypeHex();
    QString getLengthHex();
    QString getValueHex();
    int32 getLengthInt();
    QString getTagName();
    QString getTypeName();
    QString getTitle();

    void dataReset();

private:
    BIN     *tag_;
    BIN     *type_;
    BIN     *length_;
    BIN     *value_;
    int     offset_;
    int     level_;
};

#endif // READERTREEITEM_H
