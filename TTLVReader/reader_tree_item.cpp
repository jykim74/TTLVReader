#include "js_bin.h"
#include "kmip.h"
#include "js_kms.h"
#include "reader_tree_item.h"



ReaderTreeItem::ReaderTreeItem()
{
    tag_ = (BIN *)JS_calloc(1, sizeof(BIN));
    type_ = (BIN *)JS_calloc(1, sizeof(BIN));
    length_ = (BIN *)JS_calloc(1, sizeof(BIN));
    value_ = (BIN *)JS_calloc(1, sizeof(BIN));

    setEditable(false);
}

void ReaderTreeItem::setTag( const BIN *pTag )
{
    JS_BIN_reset( tag_ );

    if( pTag ) JS_BIN_copy( tag_, pTag );
}

void ReaderTreeItem::setType( const BIN *pType )
{
    JS_BIN_reset( type_ );
    if( pType ) JS_BIN_copy( type_, pType );
}

void ReaderTreeItem::setLength( const BIN *pLength )
{
    JS_BIN_reset( length_ );
    if( pLength ) JS_BIN_copy( length_, pLength );
}

void ReaderTreeItem::setValue( const BIN *pValue )
{
    JS_BIN_reset( value_ );
    if( pValue ) JS_BIN_copy( value_, pValue );
}

void ReaderTreeItem::setOffset( int offset )
{
    offset_ = offset;
}

void ReaderTreeItem::setLevel( int level )
{
    level_ = level;
}

QString ReaderTreeItem::getTagHex()
{
    char *pHex = NULL;
    if( tag_ == NULL ) return "";

    JS_BIN_encodeHex( tag_, &pHex );
    QString strHex = pHex;

    if( pHex ) JS_free( pHex );

    return strHex;
}

QString ReaderTreeItem::getTypeHex()
{
    char *pHex = NULL;
    if( type_ == NULL ) return "";

    JS_BIN_encodeHex( type_, &pHex );
    QString strHex = pHex;

    if( pHex ) JS_free( pHex );

    return strHex;
}

QString ReaderTreeItem::getLengthHex()
{
    char    *pHex = NULL;
    if( length_ == NULL ) return "";

    JS_BIN_encodeHex( length_, &pHex );
    QString strHex = pHex;

    if( pHex ) JS_free( pHex );

    return strHex;
}

QString ReaderTreeItem::getValueHex()
{
    char *pHex = NULL;
    if( value_ == NULL ) return "";

    JS_BIN_encodeHex( value_, &pHex );
    QString strHex = pHex;

    if( pHex ) JS_free( pHex );

    return strHex;
}

int32 ReaderTreeItem::getLengthInt()
{

    int32   len = 0;

    len = JS_BIN_int( length_ );

    return len;
}

QString ReaderTreeItem::getTagName()
{
    int nTag = -1;
    QString strName;
    if( tag_ == NULL ) return "";

    nTag = JS_BIN_int( tag_ );

    strName = JS_KMS_tagName( nTag );

    return strName;
}

QString ReaderTreeItem::getTypeName()
{
    int nType = -1;
    QString strName;

    if( type_ == NULL ) return "";

    nType = JS_BIN_int( type_ );

    strName = JS_KMS_typeName( nType );

    return strName;
}

QString ReaderTreeItem::getTitle()
{
    QString strTitle;
    QString strTag = getTagName();
    QString strType = getTypeName();
    int nType = JS_BIN_int( type_ );

    if( nType == KMIP_TYPE_INTEGER ||
            nType == KMIP_TYPE_TEXT_STRING ||
            nType == KMIP_TYPE_ENUMERATION )
    {
        QString strPrint = getPrintValue();
        strTitle = QString( "%1(%2 %3)").arg( strTag ).arg(strType).arg(strPrint);
    }
    else
    {
        strTitle = QString( "%1(%2)").arg( strTag ).arg(strType);
    }

    return strTitle;
}

QString ReaderTreeItem::getPrintValue()
{
    int nType = JS_BIN_int( type_ );
    QString strPrint;

    if( nType == KMIP_TYPE_INTEGER )
    {
        int num = JS_BIN_int( value_ );
        strPrint = QString( "%1" ).arg( num );
    }
    else if( nType == KMIP_TYPE_TEXT_STRING )
    {
        char *pTmp = (char *)JS_malloc( value_->nLen + 1 );
        memcpy( pTmp, value_->pVal, value_->nLen );
        pTmp[value_->nLen] = 0x00;
        strPrint = pTmp;

        JS_free( pTmp );
    }
    else if( nType == KMIP_TYPE_ENUMERATION )
    {
        int num = JS_BIN_int( value_ );
        strPrint = QString( "%1" ).arg(num);
    }
    else
    {
        strPrint = getValueHex();
    }

    return strPrint;
}

void ReaderTreeItem::dataReset()
{
    if( tag_ ) JS_BIN_reset( tag_ );
    if( type_ ) JS_BIN_reset( type_ );
    if( length_ ) JS_BIN_reset( length_ );
    if( value_ ) JS_BIN_reset( value_ );
}
