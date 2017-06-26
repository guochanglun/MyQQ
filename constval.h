#ifndef CONSTVAL_H
#define CONSTVAL_H

#include <QString>
#include "qpeopleitem.h"
#include <QList>
#include <QStringList>

class ConstVal
{
public:
    ConstVal();

    static QString account;
    static QString name;
    static QString password;
    static QString sex;
    static QString sign;

    static QPeopleItem* currentPeople;

    static QList<QPeopleItem*>* friendsItemList;

    static QStringList* msgIdList;

};

#endif // CONSTVAL_H
