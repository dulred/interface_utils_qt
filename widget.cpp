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

    connect(ui->appid,&QTextEdit::textChanged,this,&Widget::onAppleIdChanged);
    connect(ui->secret,&QTextEdit::textChanged,this,&Widget::onSecretChanged);
    connect(ui->ip,&QTextEdit::textChanged,this,&Widget::onIpChanged);
    connect(ui->path,&QTextEdit::textChanged,this,&Widget::onPathChanged);
    connect(ui->port,&QTextEdit::textChanged,this,&Widget::onPortChanged);

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
    start_timestamp = QString::number(startTime.toSecsSinceEpoch());
    QDateTime endTime = ui->end_time->dateTime();
    end_timestamp = QString::number(endTime.toSecsSinceEpoch());

    // radio默认设置post选中
    ui->post_button->setChecked(true);
    showSelectedRadioButton();

    connect(ui->send, &QPushButton::clicked, this, &Widget::onHttpRequest);
    connect(networkManager, &QNetworkAccessManager::finished, this, &Widget::onNetworkReply);

}

void Widget::onAppleIdChanged(){
    appid = ui->appid->toPlainText().trimmed();
    qDebug() << appid;
}
void Widget::onSecretChanged(){
    secret = ui->secret->toPlainText().trimmed();
    qDebug() << secret;
}
void Widget::onIpChanged(){
    ip = ui->ip->toPlainText().trimmed();
    qDebug() << ip;
}
void Widget::onPathChanged(){
    path = ui->path->toPlainText().trimmed();
    qDebug() << path;
}

void Widget::onStartTimeChanged()
{
    // 获取 QDateTimeEdit 中的 QDateTime 对象
    QDateTime startTime = ui->start_time->dateTime();

    // 转换为时间戳（自1970年1月1日以来的秒数）
    start_timestamp = QString::number(startTime.toSecsSinceEpoch());

    qDebug() << start_timestamp;
}

void Widget::onEndTimeChanged()
{
    // 获取 QDateTimeEdit 中的 QDateTime 对象
    QDateTime endTime = ui->end_time->dateTime();

    // 转换为时间戳（自1970年1月1日以来的秒数）
    end_timestamp = QString::number(endTime.toSecsSinceEpoch());

    qDebug() << end_timestamp;
}

void Widget::onJsonTextChanged()
{
    jsonText = ui->jsonText->toPlainText();
}

void Widget::onUrlChanged()
{
    url = ui->url->toPlainText();
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
    QString jsonTemplate = R"({
        "keyword": "123456",
        "date": {
            "begin": "%1",
            "end": "%2"
        }
    })";

    QString jsonString = jsonTemplate.arg(start_timestamp).arg(end_timestamp);

    ui->jsonText->setPlainText(jsonString);
}

void Widget::generateUrl()
{
    QString ts = QString::number(QDateTime::currentDateTime().toSecsSinceEpoch());
    int length = 8; // 你可以修改为你需要的长度
    QString nonce = utils::generateRandomString(length);
    qDebug()<< nonce;
    QString con = appid + secret + nonce + ts;
    QString sign = utils::sign(con);
    qDebug()<< sign << "---" << sign.length();
    QString urlTemplate = R"(http://%1:%2/%3?appid=%4&nonce=%5&ts=%6&sign=%7)";

    QString urlString = urlTemplate.arg(ip).arg(port).arg(path).arg(appid).arg(nonce).arg(ts).arg(sign);
    url = urlString.trimmed();
    qDebug()<< url;
    ui->url->setPlainText(urlString);
}

void Widget::send()
{
    onHttpRequest();
}

void Widget::onPortChanged()
{
    port = ui->port->toPlainText().trimmed();
    qDebug() << port;
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
    // networkManager->post(request, data);
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
        // 获取 Content-Type 响应头
        QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();

        // 根据 Content-Type 判断编码
        QString responseString;
        if (contentType.contains("charset=UTF-8", Qt::CaseInsensitive)) {
            responseString = QString::fromUtf8(responseData);
        } else if (contentType.contains("charset=UTF-16", Qt::CaseInsensitive)) {
            responseString = QString::fromUtf16(reinterpret_cast<const ushort*>(responseData.constData()));
        } else {
            // 处理默认的 UTF-8 编码
            responseString = QString::fromUtf8(responseData);
        }

        // 如果内容包含 Unicode 转义字符
        responseString = utils::decodeUnicodeEscapes(responseString);

        qDebug() << "Response:" << responseString;
        ui->response->setPlainText(responseString);

        // QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        // QJsonObject jsonObject = jsonDoc.object();
        // Process JSON object if needed
        // qDebug() << "Processed JSON:" << jsonObject;

    } else {
        ui->response->setPlainText("something is error");
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

