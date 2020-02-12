#ifndef READERTREEVIEW_H
#define READERTREEVIEW_H

#include <QTreeView>

class ReaderTreeItem;

class ReaderTreeView : public QTreeView
{
    Q_OBJECT

public:
    ReaderTreeView( QWidget* parent = 0 );

    void showRight();
    void showRightFull( ReaderTreeItem *pItem );
    void showRighrPart( ReaderTreeItem *pItem );

private slots:
    void onItemClicked( const QModelIndex& index );
    void ShowContextMenu( QPoint point );
};

#endif // READERTREEVIEW_H
