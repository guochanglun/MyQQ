#include "applyitem.h"
#include "ui_applyitem.h"
#include "constval.h"

#include <QDebug>
#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkRequest>

ApplyItem::ApplyItem(QWidget *parent, QString id, QString name, QString sign, QString sex) :
    QWidget(parent),
    ui(new Ui::ApplyItem)
{
    ui->setupUi(this);
    ui->name->setText(name);
    ui->sign->setText(sign);

    if(sex == "MAN"){ // 如果是男生
        QPixmap pixmap(":/icon/man");
        ui->avator->setPixmap(pixmap.scaled(ui->avator->size()));
    }else{
        QPixmap pixmap(":/icon/woman");
        ui->avator->setPixmap(pixmap.scaled(ui->avator->size()));
    }

    this->objectId = id;
    this->sex = sex;
    this->sign = sign;
    this->name = name;
}

ApplyItem::~ApplyItem()
{
    delete ui;
}

void ApplyItem::on_agree_clicked()
{
    ui->agree->setEnabled(false);
    ui->disagree->setEnabled(false);

    // 需要添加两条消息
    QNetworkRequest request;
    QString url = "https://api.bmob.cn/1/classes/friends";
    request.setUrl(QUrl(url));
    request.setRawHeader("X-Bmob-Application-Id", "09d385fd8fc9c6c8bc35a5598ba5c5df");
    request.setRawHeader("X-Bmob-REST-API-Key", "a3d8486b6a800fade0acac9d64d5101b");
    request.setRawHeader("Content-Type", "application/json");

    QJsonObject obj1;
    obj1.insert("hostname", ConstVal::name);
    obj1.insert("friendName", this->name);
    obj1.insert("friendSign", this->sign);
    obj1.insert("friendSex", this->sex);

    QJsonObject obj2;
    obj2.insert("hostname", this->name);
    obj2.insert("friendName", ConstVal::name);
    obj2.insert("friendSign", ConstVal::sign);
    obj2.insert("friendSex", ConstVal::sex);

    QNetworkAccessManager* netManager = new QNetworkAccessManager(this);
    netManager->post(request, QJsonDocument(obj1).toJson());
    netManager->post(request, QJsonDocument(obj2).toJson());

    url = "https://api.bmob.cn/1/classes/apply/" + this->objectId;
    request.setUrl(QUrl(url));
    QJsonObject obj;
    obj.insert("state", "agree");
    netManager->put(request, QJsonDocument(obj).toJson());
}

void ApplyItem::on_disagree_clicked()
{
    ui->agree->setEnabled(false);
    ui->disagree->setEnabled(false);

    QNetworkRequest request;
    QString url = "https://api.bmob.cn/1/classes/apply/" + this->objectId;
    request.setUrl(QUrl(url));
    request.setRawHeader("X-Bmob-Application-Id", "09d385fd8fc9c6c8bc35a5598ba5c5df");
    request.setRawHeader("X-Bmob-REST-API-Key", "a3d8486b6a800fade0acac9d64d5101b");
    request.setRawHeader("Content-Type", "application/json");

    QJsonObject obj;
    obj.insert("state", "disagree");

    QNetworkAccessManager* netManager = new QNetworkAccessManager(this);
    netManager->put(request, QJsonDocument(obj).toJson());
}
