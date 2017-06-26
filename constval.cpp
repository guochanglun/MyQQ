#include "constval.h"
#include <QString>
#include <QList>
#include <QStringList>

QString ConstVal::account = "";
QString ConstVal::name = "";
QString ConstVal::password = "";
QString ConstVal::sex = "";
QString ConstVal::sign = "";

QPeopleItem* ConstVal::currentPeople = NULL;
QList<QPeopleItem*>* ConstVal::friendsItemList = new QList<QPeopleItem*>();
QStringList* ConstVal::msgIdList = new QStringList();
ConstVal::ConstVal()
{

}
