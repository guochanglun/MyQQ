#ifndef QQDIALOG_H
#define QQDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include "qpeopleitem.h"

namespace Ui {
class QQDialog;
}

class QQDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QQDialog(QWidget *parent = 0);
    ~QQDialog();

private slots:
    void on_chatButton_clicked();

    void on_friendsButton_clicked();

    void on_mineButton_clicked();

    void friendReplyFinish(QNetworkReply *);

    void goChat(QPeopleItem* item);

    void on_searchFriend_clicked();

    void searchReplyFinish(QNetworkReply*);

    void requireReplyFinish(QNetworkReply*);

    void on_sendButton_clicked();

    void msgRefresh();

    void applyRefresh();

    void msgTimerReplyFinish(QNetworkReply*);

private:
    Ui::QQDialog *ui;

    QSize btnIconSize;

    void initFriends();
    void initRequire();

    void initIcon();

    void setReadMsgState(QString msgId);

    void addOtherMsgToContent(QPeopleItem* item, QString msg);

};

#endif // QQDIALOG_H
