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
    void showRightPart( ReaderTreeItem *pItem );
    void getInfoView( ReaderTreeItem *pItem );

public slots:
    void CopyAsHex();
    void CopyAsBase64();
    void copy();

    void treeExpandAll();
    void treeExpandNode();
    void treeCollapseAll();
    void treeCollapseNode();

private slots:
    void onItemClicked( const QModelIndex& index );
    void leftContextMenu( QPoint point );

private:
    ReaderTreeItem* currentItem();
};

#endif // READERTREEVIEW_H
