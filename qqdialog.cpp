#include "qqdialog.h"
#include "ui_qqdialog.h"
#include "qpeopleitem.h"
#include "constval.h"
#include "searchitem.h"
#include "applyitem.h"

#include <QBoxLayout>
#include <QHBoxLayout>
#include <QListView>
#include <QStandardItem>
#include <QListWidget>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QDebug>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QLabel>
#include <QTimer>
#include <QIcon>

QQDialog::QQDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QQDialog)
{
    ui->setupUi(this);

    setWindowTitle(ConstVal::name + "  " + ConstVal::sign);

    this->btnIconSize.setHeight(40);
    this->btnIconSize.setWidth(40);

    ui->chatButton->setIconSize(this->btnIconSize);
    ui->friendsButton->setIconSize(this->btnIconSize);
    ui->mineButton->setIconSize(this->btnIconSize);

    this->initIcon();
    this->initRequire();
    this->initFriends();

    this->on_chatButton_clicked();

    // 定时任务
    QTimer *msgTimer = new QTimer(this);
    msgTimer->setInterval(3000); // 3秒
    connect(msgTimer,SIGNAL(timeout()), this, SLOT(msgRefresh()));
    msgTimer->start();

    QTimer *applyTimer = new QTimer(this);
    applyTimer->setInterval(10000); // 10秒
    connect(applyTimer,SIGNAL(timeout()), this, SLOT(applyRefresh()));
    applyTimer->start();
}

QQDialog::~QQDialog()
{
    delete ui;
}

void QQDialog::on_chatButton_clicked()
{
    this->initIcon();
    ui->chatButton->setIcon(QPixmap(":/icon/search_active"));

    ui->title->setText("添加好友");
    ui->stackedWidget->setCurrentIndex(0);
}

void QQDialog::on_friendsButton_clicked()
{
    this->initIcon();
    ui->friendsButton->setIcon(QPixmap(":/icon/friends_active"));

    ui->title->setText("好友");
    ui->stackedWidget->setCurrentIndex(4);
}

void QQDialog::on_mineButton_clicked()
{
    this->initIcon();
    ui->mineButton->setIcon(QPixmap(":/icon/apply_active"));

    ui->title->setText("好友申请");
    ui->stackedWidget->setCurrentIndex(3);
}

void QQDialog::initFriends()
{
    QString hostname = ConstVal::name;
    QString url = "https://api.bmob.cn/1/classes/friends?where={\"hostname\":\""+hostname+"\"}";

    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setRawHeader("X-Bmob-Application-Id", "09d385fd8fc9c6c8bc35a5598ba5c5df");
    request.setRawHeader("X-Bmob-REST-API-Key", "a3d8486b6a800fade0acac9d64d5101b");
    request.setRawHeader("Content-Type", "application/json");

    QNetworkAccessManager* netManager = new QNetworkAccessManager(this);
    netManager->get(request);
    connect(netManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(friendReplyFinish(QNetworkReply*)));
}

void QQDialog::initRequire()
{
    QString hostname = ConstVal::name;
    QString url = "https://api.bmob.cn/1/classes/apply?where={\"toName\":\""+hostname+"\", \"state\":\"none\"}";

    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setRawHeader("X-Bmob-Application-Id", "09d385fd8fc9c6c8bc35a5598ba5c5df");
    request.setRawHeader("X-Bmob-REST-API-Key", "a3d8486b6a800fade0acac9d64d5101b");
    request.setRawHeader("Content-Type", "application/json");

    QNetworkAccessManager* netManager = new QNetworkAccessManager(this);
    netManager->get(request);
    connect(netManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(requireReplyFinish(QNetworkReply*)));
}

void QQDialog::initIcon()
{
    ui->chatButton->setIcon(QPixmap(":/icon/search"));
    ui->friendsButton->setIcon(QPixmap(":/icon/friends"));
    ui->mineButton->setIcon(QPixmap(":/icon/apply"));
}

void QQDialog::setReadMsgState(QString msgId)
{
    QString url = "https://api.bmob.cn/1/classes/msg/" + msgId;

    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setRawHeader("X-Bmob-Application-Id", "09d385fd8fc9c6c8bc35a5598ba5c5df");
    request.setRawHeader("X-Bmob-REST-API-Key", "a3d8486b6a800fade0acac9d64d5101b");
    request.setRawHeader("Content-Type", "application/json");

    QJsonObject obj;
    obj.insert("state", "see");

    QNetworkAccessManager* netManager = new QNetworkAccessManager(this);
    netManager->put(request, QJsonDocument(obj).toJson());
}

void QQDialog::addOtherMsgToContent(QPeopleItem* item, QString msg)
{

    if(ConstVal::currentPeople->keys.size() <= 10){
        goChat(ConstVal::currentPeople);
        return;
    }

    QLabel* label = new QLabel(ui->currentChatContent);
    label->setText(msg);

    QLabel* avator = new QLabel(ui->currentChatContent);

    QHBoxLayout *hboxlayout = new QHBoxLayout();
    hboxlayout->setAlignment(Qt::AlignLeft);

    QPixmap manPixmap(":/icon/man");
    manPixmap = manPixmap.scaled(50, 50);

    QPixmap womanPixmap(":/icon/woman");
    womanPixmap = womanPixmap.scaled(50, 50);

    QPixmap otherPixmap;

    if(item->sex == "MAN"){
        otherPixmap = manPixmap;
    }else{
        otherPixmap = womanPixmap;
    }

    avator->setPixmap(otherPixmap);

    if(item->sex == "WOMAN"){
        label->setStyleSheet("background: #fef; border: 1px solid #666; border-radius: 5px; padding: 5;");
    }else{
        label->setStyleSheet("background: #eee; border: 1px solid #666; border-radius: 5px; padding: 5;");
    }

    hboxlayout->addWidget(avator);
    hboxlayout->addWidget(label);

    QHBoxLayout* layout = static_cast<QHBoxLayout*>(ui->currentChatContent->layout());
    layout->addLayout(hboxlayout);
}

void QQDialog::friendReplyFinish(QNetworkReply * reply)
{
    QJsonObject result =  QJsonDocument::fromJson(reply->readAll()).object();
    QJsonArray array = result.value("results").toArray();

    QHBoxLayout* layout = static_cast<QHBoxLayout*>(ui->friendContent->layout());

    // 先删除子控件
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != 0) {
        delete child;
    }

    for(int i = 0, size = array.size(); i < size; i++){
        QJsonObject obj = array.at(i).toObject();
        QPeopleItem* item = new QPeopleItem(ui->friendContent,
                                            obj.value("friendName").toString(),
                                            obj.value("friendSign").toString(),
                                            obj.value("friendSex").toString());
        layout->addWidget(item);
        ConstVal::friendsItemList->append(item);
        connect(item, SIGNAL(Clicked(QPeopleItem*)), this, SLOT(goChat(QPeopleItem*)));
    }
    layout->addStretch();
    reply->deleteLater();
}

void QQDialog::goChat(QPeopleItem *item)
{
    ConstVal::currentPeople = item;

    item->msgHint->hide();

    ui->title->setText(item->name);

    QList<QString> keys = item->keys;
    QList<QString> values = item->values;

    QHBoxLayout* layout = static_cast<QHBoxLayout*>(ui->currentChatContent->layout());

    // 先删除子控件
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != 0) {
        delete child;
    }

    QPixmap manPixmap(":/icon/man");
    manPixmap = manPixmap.scaled(50, 50);

    QPixmap womanPixmap(":/icon/woman");
    womanPixmap = womanPixmap.scaled(50, 50);

    QPixmap myPixmap;
    QPixmap otherPixmap;

    if(ConstVal::sex == "MAN"){
        myPixmap = manPixmap;
    }else{
        myPixmap = womanPixmap;
    }

    if(item->sex == "MAN"){
        otherPixmap = manPixmap;
    }else{
        otherPixmap = womanPixmap;
    }

    // 添加子控件
    for(int i = 0, size = keys.size(); i < size; i++){

        QLabel* label = new QLabel(ui->currentChatContent);
        label->setText(values.at(i));

        QLabel* avator = new QLabel(ui->currentChatContent);

        QHBoxLayout *hboxlayout = new QHBoxLayout();
        if(keys.at(i).contains("me")){
            hboxlayout->setAlignment(Qt::AlignRight);
            avator->setPixmap(myPixmap);
            label->setStyleSheet("background: white; border: 1px solid #666; border-radius: 5px; padding: 5;");

            hboxlayout->addWidget(label);
            hboxlayout->addWidget(avator);
        }else{
            avator->setPixmap(otherPixmap);
            hboxlayout->setAlignment(Qt::AlignLeft);

            if(item->sex == "WOMAN"){
                label->setStyleSheet("background: #fef; border: 1px solid #666; border-radius: 5px; padding: 5;");
            }else{
                label->setStyleSheet("background: #eee; border: 1px solid #666; border-radius: 5px; padding: 5;");
            }

            hboxlayout->addWidget(avator);
            hboxlayout->addWidget(label);
        }
        layout->addLayout(hboxlayout);
    }
    layout->addStretch();

    ui->stackedWidget->setCurrentIndex(1);
}

void QQDialog::on_searchFriend_clicked()
{
    QString searchAccount = ui->searchEdit->text().trimmed();
    if(searchAccount.size() <= 0) return;

    QString url = "https://api.bmob.cn/1/classes/quser?where={\"account\":\""+searchAccount+"\"}";

    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setRawHeader("X-Bmob-Application-Id", "09d385fd8fc9c6c8bc35a5598ba5c5df");
    request.setRawHeader("X-Bmob-REST-API-Key", "a3d8486b6a800fade0acac9d64d5101b");

    QNetworkAccessManager* netManager = new QNetworkAccessManager(this);
    netManager->get(request);
    connect(netManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(searchReplyFinish(QNetworkReply*)));
}

void QQDialog::searchReplyFinish(QNetworkReply * reply)
{
    QJsonObject result =  QJsonDocument::fromJson(reply->readAll()).object();
    QJsonArray array = result.value("results").toArray();

    QHBoxLayout* layout = static_cast<QHBoxLayout*>(ui->searchContent->layout());

    // 先删除子控件
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != 0) {
        delete child;
    }
    for(int i = 0, size = array.size(); i < size; i++){
        QJsonObject obj = array.at(i).toObject();
        SearchItem* item = new SearchItem(ui->searchContent,
                                            obj.value("name").toString(),
                                            obj.value("sign").toString(),
                                            obj.value("sex").toString());
        layout->addWidget(item);
    }
    layout->addStretch();

    ui->stackedWidget->setCurrentIndex(2);
    ui->title->setText("搜索列表");

    reply->deleteLater();
}

void QQDialog::requireReplyFinish(QNetworkReply * reply)
{

    QJsonObject result =  QJsonDocument::fromJson(reply->readAll()).object();
    QJsonArray array = result.value("results").toArray();

    QHBoxLayout* layout = static_cast<QHBoxLayout*>(ui->requireContent->layout());

    // 先删除子控件
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != 0) {
        delete child;
    }
    for(int i = 0, size = array.size(); i < size; i++){
        QJsonObject obj = array.at(i).toObject();
        ApplyItem* item = new ApplyItem(ui->requireContent,
                                            obj.value("objectId").toString(),
                                            obj.value("fromName").toString(),
                                            obj.value("fromSign").toString(),
                                            obj.value("fromSex").toString());
        layout->addWidget(item);
    }
    layout->addStretch();

    reply->deleteLater();
}

void QQDialog::on_sendButton_clicked()
{
    QString msg = ui->msgEdit->text();
    if(msg.trimmed().size() <= 0 || ConstVal::currentPeople == NULL){
        return;
    }

    ui->msgEdit->clear();

    QNetworkRequest request;
    QString url = "https://api.bmob.cn/1/classes/msg";
    request.setUrl(QUrl(url));
    request.setRawHeader("X-Bmob-Application-Id", "09d385fd8fc9c6c8bc35a5598ba5c5df");
    request.setRawHeader("X-Bmob-REST-API-Key", "a3d8486b6a800fade0acac9d64d5101b");
    request.setRawHeader("Content-Type", "application/json");

    QJsonObject obj;
    obj.insert("fromName", ConstVal::name);
    obj.insert("toName", ConstVal::currentPeople->name);
    obj.insert("msg", msg);
    obj.insert("state", "none");

    QNetworkAccessManager* netManager = new QNetworkAccessManager(this);
    netManager->post(request, QJsonDocument(obj).toJson());

    ConstVal::currentPeople->keys.append("me");
    ConstVal::currentPeople->values.append(msg);

    if(ConstVal::currentPeople->keys.size() <= 10){
        goChat(ConstVal::currentPeople);
        return;
    }

    QLabel* label = new QLabel(ui->currentChatContent);
    label->setText(msg);

    QLabel* avator = new QLabel(ui->currentChatContent);

    QHBoxLayout *hboxlayout = new QHBoxLayout();
    hboxlayout->setAlignment(Qt::AlignRight);

    QPixmap manPixmap(":/icon/man");
    manPixmap = manPixmap.scaled(50, 50);

    QPixmap womanPixmap(":/icon/woman");
    womanPixmap = womanPixmap.scaled(50, 50);

    QPixmap myPixmap;

    if(ConstVal::sex == "MAN"){
        myPixmap = manPixmap;
    }else{
        myPixmap = womanPixmap;
    }

    avator->setPixmap(myPixmap);

    label->setStyleSheet("background: white; border: 1px solid #666; border-radius: 5px; padding: 5;");

    hboxlayout->addWidget(label);
    hboxlayout->addWidget(avator);

    QHBoxLayout* layout = static_cast<QHBoxLayout*>(ui->currentChatContent->layout());
    layout->addLayout(hboxlayout);
}

void QQDialog::msgRefresh()
{
    QNetworkRequest request;
    QString url = "https://api.bmob.cn/1/classes/msg?where={\"toName\": \"" + ConstVal::name + "\", \"state\": \"none\"}";
    request.setUrl(QUrl(url));
    request.setRawHeader("X-Bmob-Application-Id", "09d385fd8fc9c6c8bc35a5598ba5c5df");
    request.setRawHeader("X-Bmob-REST-API-Key", "a3d8486b6a800fade0acac9d64d5101b");
    request.setRawHeader("Content-Type", "application/json");

    QNetworkAccessManager* netManager = new QNetworkAccessManager(this);
    netManager->get(request);

    connect(netManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(msgTimerReplyFinish(QNetworkReply*)));
}

void QQDialog::applyRefresh()
{
    this->initRequire();
}

void QQDialog::msgTimerReplyFinish(QNetworkReply *reply)
{
    QJsonObject result =  QJsonDocument::fromJson(reply->readAll()).object();
    QJsonArray array = result.value("results").toArray();

    for(int i = 0, size = array.size(); i < size; i++){

        // 获取Object
        QJsonObject obj = array.at(i).toObject();

        // 获取信息
        QString id = obj.value("objectId").toString();
        QString fromName = obj.value("fromName").toString();
        QString msg = obj.value("msg").toString();

        // 如果列表中包含id，则不需要重复添加消息
        if(ConstVal::msgIdList->contains(id)){
            return;
        }

        // 添加到列表中，防止重复获取消息
        ConstVal::msgIdList->append(id);

        // 找到当前的发送着的people
        QPeopleItem * item;
        for(int j = 0, size = ConstVal::friendsItemList->size(); j < size; j++){
            item = ConstVal::friendsItemList->at(j);
            if(item->objectName() == fromName){
                break;
            }
        }

        // 存储消息
        item->msgHint->show();
        item->keys.append("other");
        item->values.append(msg);

        // 如果当前正在聊天界面并且正在和发送人聊天，则直接把消息添加到消息列表中
        if(ui->stackedWidget->currentIndex() == 1 && item->name == ConstVal::currentPeople->name){
            item->msgHint->hide();
            this->addOtherMsgToContent(item, msg);
        }

        // 设置消息状态为已读
        this->setReadMsgState(id);
    }

    reply->deleteLater();
}
