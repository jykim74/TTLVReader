#ifndef READERTREEMODEL_H
#define READERTREEMODEL_H

#include <QStandardItemModel>

class ReaderTreeItem;

class ReaderTreeModel : public QStandardItemModel
{
    Q_OBJECT

public:
    ReaderTreeModel( QObject *parent = 0 );

    int parseTree();
    int parseConstruct( int offset, ReaderTreeItem *pParentItem );

private:
    int getItem( int offset, ReaderTreeItem *pItem );
};

#endif // READERTREEMODEL_H
