#include "searchitem.h"
#include "ui_searchitem.h"
#include "constval.h"

#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonDocument>

SearchItem::SearchItem(QWidget *parent, QString name, QString sign, QString sex) :
    QWidget(parent),
    ui(new Ui::SearchItem)
{
    ui->setupUi(this);

    ui->addButton->setIcon(QPixmap(":/icon/add"));
    ui->addButton->setIconSize(QSize(40, 40));

    ui->name->setText(name);
    ui->sign->setText(sign);

    if(sex == "MAN"){ // 如果是男生
        QPixmap pixmap(":/icon/man");
        ui->avator->setPixmap(pixmap.scaled(ui->avator->size()));
    }else{
        QPixmap pixmap(":/icon/woman");
        ui->avator->setPixmap(pixmap.scaled(ui->avator->size()));
    }

    this->name = name;

    for(int i = 0, size = ConstVal::friendsItemList->size(); i < size; i++){
        if(this->name == ConstVal::name || this->name == ConstVal::friendsItemList->at(i)->name){
            ui->addButton->setEnabled(false);
            return;
        }
    }
}

SearchItem::~SearchItem()
{
    delete ui;
}

void SearchItem::on_addButton_clicked()
{
    ui->addButton->setEnabled(false);

    QString url = "https://api.bmob.cn/1/classes/apply";

    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setRawHeader("X-Bmob-Application-Id", "ab104862b94b3bacf0e30748449a4160");
    request.setRawHeader("X-Bmob-REST-API-Key", "5f14f7a216015c661b6b5c4567b38901");
    request.setRawHeader("Content-Type", "application/json");

    QJsonObject object;
    object.insert("fromName", ConstVal::name);
    object.insert("fromSex", ConstVal::sex);
    object.insert("fromSign", ConstVal::sign);
    object.insert("toName", name);
    object.insert("state", "none");

    QNetworkAccessManager* netManager = new QNetworkAccessManager(this);
    netManager->post(request, QJsonDocument(object).toJson());
}
