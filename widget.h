#ifndef WIDGET_H
#define WIDGET_H

#include <QRadioButton>
#include <QWidget>
#include <QNetworkAccessManager>
QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    void init();

private slots:
    void onAppleIdChanged();
    void onSecretChanged();
    void onIpChanged();
    void onPathChanged();
    void onStartTimeChanged();
    void onEndTimeChanged();
    void onJsonTextChanged();
    void onUrlChanged();
    void showSelectedRadioButton();
    void generateJson();
    void generateUrl();
    void send();
    void onPortChanged();

private:
    Ui::Widget *ui;
    QString appid;
    QString secret;
    QString ip;
    QString path;
    qint64 start_timestamp;
    qint64 end_timestamp;
    QString jsonText;
    QString url;
    QString re_way;
    QString port;
private slots:
    void onGetRequest();
    void onPostRequest();
    void onHttpRequest();
    void onNetworkReply(QNetworkReply *reply);
    void onPostButtonClicked();
    void onGetButtonClicked();

private:
    QNetworkAccessManager *networkManager;
};
#endif // WIDGET_H
