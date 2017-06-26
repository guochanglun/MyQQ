#include "qpeopleitem.h"
#include "ui_qpeopleitem.h"
#include <QLabel>
#include <QPixmap>

QPeopleItem::QPeopleItem(QWidget *parent, QString name, QString sign, QString sex) :
    QWidget(parent),
    ui(new Ui::QPeopleItem)
{
    setObjectName(name);

    ui->setupUi(this);

    ui->name->setText(name);
    ui->sign->setText(sign);
    ui->msgTint->hide();

    this->msgHint = ui->msgTint;

    if(sex == "MAN"){ // 如果是男生
        QPixmap pixmap(":/icon/man");
        ui->avator->setPixmap(pixmap.scaled(ui->avator->size()));
    }else{
        QPixmap pixmap(":/icon/woman");
        ui->avator->setPixmap(pixmap.scaled(ui->avator->size()));
    }

    this->name = name;
    this->sign = sign;
    this->sex = sex;

    this->keys.append("other");
    this->values.append("hello");
}

QPeopleItem::~QPeopleItem()
{
    delete ui;
}

void QPeopleItem::mouseReleaseEvent(QMouseEvent *)
{
    emit Clicked(this);
}
