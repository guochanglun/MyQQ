#ifndef QREGISTER_H
#define QREGISTER_H

#include <QDialog>
#include <QNetworkReply>

namespace Ui {
class QRegister;
}

class QRegister : public QDialog
{
    Q_OBJECT

public:
    explicit QRegister(QWidget *parent = 0);
    ~QRegister();

private slots:
    void on_pushButton_clicked();
    void registerReplyFinish(QNetworkReply*);

private:
    Ui::QRegister *ui;
};

#endif // QREGISTER_H
