#ifndef COMMON_H
#define COMMON_H

#include <QString>
#include <QWidget>

const QColor kAddrColor( 220, 220, 250 );
const QColor kTextColor( 225, 225, 225 );
const QColor kValueColor( 245, 245, 203 );
const QColor kTagColor( 102, 255, 102 );
const QColor kLenColor( 240, 214, 255 );
const QColor kLenTypeColor( Qt::cyan );

enum { JS_FILE_TYPE_CERT, JS_FILE_TYPE_PRIKEY, JS_FILE_TYPE_TXT, JS_FILE_TYPE_BER, JS_FILE_TYPE_BIN };

QString findFile( QWidget *parent, int nType, const QString strPath );


#endif // COMMON_H
