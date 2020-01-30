#ifndef READERTREEVIEW_H
#define READERTREEVIEW_H

#include <QTreeView>

class ReaderTreeItem;

class ReaderTreeView : public QTreeView
{
    Q_OBJECT

public:
    ReaderTreeView( QWidget* parent = 0 );
};

#endif // READERTREEVIEW_H
