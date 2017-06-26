#ifndef SEARCHITEM_H
#define SEARCHITEM_H

#include <QWidget>

namespace Ui {
class SearchItem;
}

class SearchItem : public QWidget
{
    Q_OBJECT

public:
    explicit SearchItem(QWidget *parent, QString name, QString sign, QString sex);
    ~SearchItem();

private slots:
    void on_addButton_clicked();

private:
    Ui::SearchItem *ui;

    QString name;
};

#endif // SEARCHITEM_H
