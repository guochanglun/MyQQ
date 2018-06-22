#include "logindialog.h"
#include "ui_logindialog.h"
#include "clickablelabel.h"
#include "qqdialog.h"
#include "constval.h"
#include "qregister.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include <QPixmap>
#include <QFont>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    QPixmap pixmap(":/icon/login_bg");
    setAutoFillBackground(true);   // 这个属性一定要设置
    QPalette pal(palette());
    pal.setBrush(QPalette::Window, QBrush(pixmap.scaled(size(), Qt::IgnoreAspectRatio,
                      Qt::SmoothTransformation)));
    setPalette(pal);

    // 添加clickable label
    ClickableLabel* label = new ClickableLabel(this);
    label->setGeometry(320, 320, 141, 20);
    label->setText("没有账号？立即注册");
    label->setStyleSheet("color: white;");
    label->setCursor(Qt::PointingHandCursor);

    connect(label, SIGNAL(Clicked()), this, SLOT(userRegister()));

}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::login(QNetworkReply *reply)
{
    QJsonObject object = QJsonDocument::fromJson(reply->readAll()).object();
    QJsonArray array = object.value("results").toArray();
    if(array.size() >= 1){
        QJsonObject obj = array.at(0).toObject();
        ConstVal::name = obj.value("name").toString();
        ConstVal::password = obj.value("password").toString();
        ConstVal::sex = obj.value("sex").toString();
        ConstVal::account = obj.value("account").toString();
        ConstVal::sign = obj.value("sign").toString();

        this->close();
        QQDialog dialog;
        dialog.exec();
    }else{
        QMessageBox::information(this, "提示", "账号或密码错错误");
        ui->login->setEnabled(true);
    }
}

void LoginDialog::on_login_clicked()
{
    QString account = ui->account->text().trimmed();
    QString password = ui->password->text().trimmed();

    if(account.size() <= 0 || password.size() <= 0){
        return;
    }

    // 禁用按钮
    ui->login->setEnabled(false);

    // 登录
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QNetworkRequest request;

    QString url = QString("https://api.bmob.cn/1/classes/quser")
            + "?where={"
            + "\"account\":" + "\""+account+"\","
            + "\"password\":" + "\""+password+"\""
            +"}";

    request.setUrl(QUrl(url));
    request.setRawHeader("X-Bmob-Application-Id", "ab104862b94b3bacf0e30748449a4160");
    request.setRawHeader("X-Bmob-REST-API-Key", "5f14f7a216015c661b6b5c4567b38901");
    manager->get(request);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(login(QNetworkReply*)));
}

void LoginDialog::userRegister()
{
    QRegister* reg = new QRegister(this);
    this->hide();
    reg->exec();

    this->show();
}
