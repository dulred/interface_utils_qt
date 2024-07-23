#include "widget.h"
#include "./ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    // 设置标题
    this->setWindowTitle("天迹接口测试工具");
    // 设置图标
    this->setWindowIcon(QIcon(":/icon/icon.png"));
    // 设置窗口固定大小
    this->setFixedSize(847, 716);
    init();


}
// 初始化
void Widget::init()
{
    connect(ui->appleid,&QTextEdit::textChanged,this,&Widget::onAppleIdChanged);
    connect(ui->secret,&QTextEdit::textChanged,this,&Widget::onSecretChanged);
    connect(ui->ip,&QTextEdit::textChanged,this,&Widget::onIpChanged);
    connect(ui->path,&QTextEdit::textChanged,this,&Widget::onPathChanged);
}

void Widget::onAppleIdChanged(){
    qDebug() << "hello11";
}
void Widget::onSecretChanged(){
        qDebug() << "hello22";
}
void Widget::onIpChanged(){
        qDebug() << "hello33";
}
void Widget::onPathChanged(){
        qDebug() << "hello44";
}


Widget::~Widget()
{
    delete ui;
}


