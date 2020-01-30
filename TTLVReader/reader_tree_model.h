#ifndef READERTREEMODEL_H
#define READERTREEMODEL_H

#include <QStandardItemModel>

class ReaderTreeModel : public QStandardItemModel
{
    Q_OBJECT

public:
    ReaderTreeModel( QObject *parent = 0 );
};

#endif // READERTREEMODEL_H
