#include "qregister.h"
#include "ui_qregister.h"

#include <QDebug>
#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkRequest>
#include <QMessageBox>

QRegister::QRegister(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QRegister)
{
    ui->setupUi(this);
    this->setWindowTitle("注册MyQQ");
    ui->sign->setText("这个人很懒，什么也没有留下...");
}

QRegister::~QRegister()
{
    delete ui;
}

void QRegister::on_pushButton_clicked()
{
    QString name = ui->name->text().trimmed();
    QString account = ui->account->text().trimmed();
    QString password = ui->password->text().trimmed();
    QString sex = ui->comboBox->currentText().trimmed();
    QString sign = ui->sign->text();
    if(sex == "男"){
        sex = "MAN";
    }else{
        sex = "WOMAN";
    }

    if(name.size() == 0
            || account.size() == 0
            || password.size() == 0
            || sex.size() == 0
            || sign.size() == 0){
        return;
    }

    QNetworkRequest request;
    QString url = "https://api.bmob.cn/1/classes/quser";
    request.setUrl(QUrl(url));
    request.setRawHeader("X-Bmob-Application-Id", "ab104862b94b3bacf0e30748449a4160");
    request.setRawHeader("X-Bmob-REST-API-Key", "5f14f7a216015c661b6b5c4567b38901");
    request.setRawHeader("Content-Type", "application/json");

    QJsonObject obj;
    obj.insert("account", account);
    obj.insert("sign", sign);
    obj.insert("sex", sex);
    obj.insert("name", name);
    obj.insert("password", password);

    QNetworkAccessManager* netManager = new QNetworkAccessManager(this);
    QNetworkReply* reply = netManager->post(request, QJsonDocument(obj).toJson());

    connect(netManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(registerReplyFinish(QNetworkReply*)));
}

void QRegister::registerReplyFinish(QNetworkReply *reply)
{
    qDebug()<<reply->readAll();
    QMessageBox::information(this, "信息", "注册成功，马上登录吧！");
    this->deleteLater();
    reply->deleteLater();
}
