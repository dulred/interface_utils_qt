#include "widget.h"
#include "./ui_widget.h"

#include <QFile>
#include <QRandomGenerator>
#include "utils.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget), networkManager(new QNetworkAccessManager(this))
{
    ui->setupUi(this);
    // 设置标题
    this->setWindowTitle("天迹接口测试工具");
    // 设置图标
    this->setWindowIcon(QIcon(":/icon/resource/icon.png"));
    // 设置窗口固定大小
    this->setFixedSize(847, 720);
    // 加载全局样式表
    QFile file(":/qss/resource/style.qss");
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(file.readAll());
        qApp->setStyleSheet(styleSheet);
    }

    // 初始化事件
    init();
}
// 初始化
void Widget::init()
{

    connect(ui->appleid,&QTextEdit::textChanged,this,&Widget::onAppleIdChanged);
    connect(ui->secret,&QTextEdit::textChanged,this,&Widget::onSecretChanged);
    connect(ui->ip,&QTextEdit::textChanged,this,&Widget::onIpChanged);
    connect(ui->path,&QTextEdit::textChanged,this,&Widget::onPathChanged);

    connect(ui->start_time,&QDateTimeEdit::dateTimeChanged,this,&Widget::onStartTimeChanged);
    connect(ui->end_time,&QDateTimeEdit::dateTimeChanged,this,&Widget::onEndTimeChanged);

    connect(ui->jsonText,&QTextEdit::textChanged,this,&Widget::onJsonTextChanged);
    connect(ui->url,&QTextEdit::textChanged,this,&Widget::onUrlChanged);
    // 点击按钮
    connect(ui->g_ts,&QPushButton::clicked,this,&Widget::generateJson);
    connect(ui->g_url,&QPushButton::clicked,this,&Widget::generateUrl);
    connect(ui->get_button,&QPushButton::clicked,this,&Widget::onGetButtonClicked);
    connect(ui->post_button,&QPushButton::clicked,this,&Widget::onPostButtonClicked);

    // 设置时间默认值，不然下面生成json字符串如果不设置值的话会乱
    QDateTime startTime = ui->start_time->dateTime();
    start_timestamp = startTime.toSecsSinceEpoch();
    QDateTime endTime = ui->end_time->dateTime();
    end_timestamp = endTime.toSecsSinceEpoch();

    // radio默认设置post选中
    ui->post_button->setChecked(true);
    showSelectedRadioButton();

    connect(ui->send, &QPushButton::clicked, this, &Widget::onHttpRequest);
    connect(networkManager, &QNetworkAccessManager::finished, this, &Widget::onNetworkReply);

}

void Widget::onAppleIdChanged(){
    appleid = ui->appleid->toPlainText();
    qDebug() << appleid;
}
void Widget::onSecretChanged(){
    secret = ui->secret->toPlainText();
    qDebug() << secret;
}
void Widget::onIpChanged(){
    ip = ui->ip->toPlainText();
    qDebug() << ip;
}
void Widget::onPathChanged(){
    path = ui->path->toPlainText();
    qDebug() << path;
}

void Widget::onStartTimeChanged()
{
    // 获取 QDateTimeEdit 中的 QDateTime 对象
    QDateTime startTime = ui->start_time->dateTime();

    // 转换为时间戳（自1970年1月1日以来的秒数）
    start_timestamp = startTime.toSecsSinceEpoch();

    qDebug() << start_timestamp;
}

void Widget::onEndTimeChanged()
{
    // 获取 QDateTimeEdit 中的 QDateTime 对象
    QDateTime endTime = ui->end_time->dateTime();

    // 转换为时间戳（自1970年1月1日以来的秒数）
    end_timestamp = endTime.toSecsSinceEpoch();

    qDebug() << end_timestamp;
}

void Widget::onJsonTextChanged()
{
    jsonText = ui->jsonText->toPlainText();
    qDebug()<<jsonText <<"666666666666666666666666";
}

void Widget::onUrlChanged()
{
    url = ui->url->toPlainText();
    qDebug()<<url <<"7777777777777777777777777777777";
}

void Widget::showSelectedRadioButton()
{
    QList<QRadioButton *> radioButtons = { ui->get_button, ui->post_button };
    for (QRadioButton *button : radioButtons) {
        if (button->isChecked()) {
            re_way = button->text();
            qDebug() << "Selected Radio Button:" << re_way;
        }else{
            qDebug() << "No Radio Button Selected";
        }
    }

}

void Widget::generateJson()
{
    QString jsonTemplate = R"("{
        "keyword": "123456",
        "timestamp": {
            "start_time": "%1",
            "end_time": "%2"
        }
    }")";

    QString jsonString = jsonTemplate.arg(start_timestamp).arg(end_timestamp);

    ui->jsonText->setPlainText(jsonString);
}

void Widget::generateUrl()
{
    QString ts = QString::number(QDateTime::currentDateTime().toSecsSinceEpoch());
    int length = 8; // 你可以修改为你需要的长度
    QString nonce = utils::generateRandomString(length);
    QString con = ts + nonce + appleid + secret;
    QString sign = utils::md5(utils::sha1(con));
    qDebug()<< sign;
    QString urlTemplate = R"(https://%1/%2?appleid=%3&ts=%4&nonce=%5&sign=%6)";

    QString urlString = urlTemplate.arg(ip).arg(path).arg(appleid).arg(ts).arg(nonce).arg(sign);
    url = urlString;
    ui->url->setPlainText(urlString);
}

void Widget::send()
{
    onHttpRequest();
}


void Widget::onGetRequest() {
    QUrl url(this->url);
    QNetworkRequest request(url);
    networkManager->get(request);
}

void Widget::onPostRequest() {
    QUrl url(this->url);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // QJsonObject jsonObject;
    // jsonObject["title"] = "foo";
    // jsonObject["body"] = "bar";
    // jsonObject["userId"] = 1;

    // QJsonDocument jsonDoc(jsonObject);
    // QByteArray data = jsonDoc.toJson();

    networkManager->post(request, this->jsonText.toUtf8());
}

void Widget::onHttpRequest()
{
    if(re_way=="GET"){
        onGetRequest();
        qDebug()<<"-----调用GET-------";
    }else{
        onPostRequest();
        qDebug()<<"------调用POST----";
    }
}

void Widget::onNetworkReply(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QString responseString = QString::fromUtf8(responseData);

        qDebug() << "Response:" << responseString;
        ui->response->setPlainText(responseString);
        // QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        // QJsonObject jsonObject = jsonDoc.object();
        // Process JSON object if needed
        // qDebug() << "Processed JSON:" << jsonObject;

    } else {
        qDebug() << "Network error:" << reply->errorString();
    }
    reply->deleteLater();
}

void Widget::onPostButtonClicked()
{
    re_way = "POST";
}

void Widget::onGetButtonClicked()
{
   re_way = "GET";
}

Widget::~Widget()
{
    delete ui;
}



