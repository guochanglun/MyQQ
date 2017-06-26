#ifndef QPEOPLEITEM_H
#define QPEOPLEITEM_H

#include <QWidget>
#include <QMap>
#include <QDateTime>
#include <QLabel>

namespace Ui {
class QPeopleItem;
}

class QPeopleItem : public QWidget
{
    Q_OBJECT

signals:
    void Clicked(QPeopleItem*);

public:
    explicit QPeopleItem(QWidget *parent, QString name, QString sign, QString sex);
    ~QPeopleItem();

    QString name;
    QString sign;
    QString sex;

    QList<QString> keys;
    QList<QString> values;

    QLabel* msgHint;

protected:
    void mouseReleaseEvent(QMouseEvent* );

private:
    Ui::QPeopleItem *ui;
};

#endif // QPEOPLEITEM_H
