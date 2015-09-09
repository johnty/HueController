#include "huecontrolwindow.h"
#include "ui_huecontrolwindow.h"

HueControlWindow::HueControlWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HueControlWindow)
{
    ui->setupUi(this);
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(syncRequestFinished(QNetworkReply*)));

}

HueControlWindow::~HueControlWindow()
{
    delete ui;
}

void HueControlWindow::syncRequestFinished(QNetworkReply *reply)
{

    QByteArray data = reply->readAll();

    QJsonDocument doc = QJsonDocument::fromBinaryData(data);

    qDebug() << data;
}

void HueControlWindow::on_pushButton_clicked()
{
    QUrl url("http://192.168.100.230/api/b225912329de4371bdc4d2e18678263/lights/4/state");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject data_obj;
    data_obj.insert("on", true);
    QJsonDocument doc(data_obj);
    QByteArray data = doc.toJson();
    manager->put(request, data);
}

void HueControlWindow::on_pushButton_2_clicked()
{
    QUrl url("http://192.168.100.230/api/b225912329de4371bdc4d2e18678263/lights/4/state");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject data_obj;
    data_obj.insert("on", false);
    QJsonDocument doc(data_obj);
    QByteArray data = doc.toJson();
    manager->put(request, data);
}

void HueControlWindow::on_pushButton_3_clicked()
{
     QUrl url("http://192.168.100.230/api/b225912329de4371bdc4d2e18678263/");
     QNetworkRequest request(url);
     request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
     manager->get(request);
}
