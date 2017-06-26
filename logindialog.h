#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QNetworkReply>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

private slots:
    void login(QNetworkReply*);

    void on_login_clicked();

    void userRegister();
private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
