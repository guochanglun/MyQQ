#ifndef APPLYITEM_H
#define APPLYITEM_H

#include <QWidget>

namespace Ui {
class ApplyItem;
}

class ApplyItem : public QWidget
{
    Q_OBJECT

public:
    explicit ApplyItem(QWidget *parent, QString id, QString name, QString sign, QString sex);
    ~ApplyItem();

private slots:

    void on_agree_clicked();

    void on_disagree_clicked();

private:
    Ui::ApplyItem *ui;

    QString objectId;
    QString name;
    QString sign;
    QString sex;

};

#endif // APPLYITEM_H
