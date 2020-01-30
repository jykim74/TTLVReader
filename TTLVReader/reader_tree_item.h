#ifndef READERTREEITEM_H
#define READERTREEITEM_H

#include "js_bin.h"

#include <QStandardItem>

class ReaderTreeItem : public QStandardItem
{
public:
    ReaderTreeItem();

private:
    BIN     tag_;
    BIN     type_;
    BIN     length_;
    BIN     value_;
};

#endif // READERTREEITEM_H
