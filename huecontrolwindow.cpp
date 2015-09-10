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

    qDebug() << "isEmpty = " << doc.isEmpty();
}

void HueControlWindow::on_pushButton_clicked()
{
    if (ui->checkBox1->isChecked())
        setLightOn(true,1);
    if (ui->checkBox1_2->isChecked())
        setLightOn(true,2);
    if (ui->checkBox1_3->isChecked())
        setLightOn(true,3);
    if (ui->checkBox1_4->isChecked())
        setLightOn(true,4);

    return;

    QUrl url("http://192.168.100.230/api/b225912329de4371bdc4d2e18678263/lights/2/state");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject data_obj;
    data_obj.insert("on", true);
    //data_obj.insert("bri", 255);
    QJsonDocument doc(data_obj);
    QByteArray data = doc.toJson();
    manager->put(request, data);
}

void HueControlWindow::on_pushButton_2_clicked()
{
    if (ui->checkBox1->isChecked())
        setLightOn(false,1);
    if (ui->checkBox1_2->isChecked())
        setLightOn(false,2);
    if (ui->checkBox1_3->isChecked())
        setLightOn(false,3);
    if (ui->checkBox1_4->isChecked())
        setLightOn(false,4);

    return;
    QUrl url("http://192.168.100.230/api/b225912329de4371bdc4d2e18678263/lights/2/state");
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
     QUrl url("http://192.168.100.230/api/b225912329de4371bdc4d2e18678263/lights/2/");
     QNetworkRequest request(url);
     request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
     manager->get(request);
}

void HueControlWindow::setLightOn(bool isOn, int idx)
{
    QString url_str = "http://192.168.100.230/api/b225912329de4371bdc4d2e18678263/lights/" + QString::number(idx) + "/state/";
    QUrl url(url_str);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject data_obj;
    data_obj.insert("on", isOn);
    QJsonDocument doc(data_obj);
    QByteArray data = doc.toJson();
    manager->put(request, data);
}

void HueControlWindow::setBrightness(int bri, int idx)
{
    QString url_str = "http://192.168.100.230/api/b225912329de4371bdc4d2e18678263/lights/" + QString::number(idx) + "/state/";
    QUrl url(url_str);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject data_obj;
    data_obj.insert("bri", bri);
    QJsonDocument doc(data_obj);
    QByteArray data = doc.toJson();
    manager->put(request, data);
}

void HueControlWindow::on_horizontalSlider_sliderMoved(int position)
{

}

void HueControlWindow::on_horizontalSlider_valueChanged(int value)
{
}

void HueControlWindow::on_pushButton_4_clicked()
{
    int bri = ui->horizontalSlider->value();
    if (ui->checkBox1->isChecked())
        setBrightness(bri, 1);
    if (ui->checkBox1_2->isChecked())
        setBrightness(bri, 2);
    if (ui->checkBox1_3->isChecked())
        setBrightness(bri, 3);
    if (ui->checkBox1_4->isChecked())
        setBrightness(bri, 4);

    return;
    //int bri = ui->horizontalSlider->value();
    QUrl url("http://192.168.100.230/api/b225912329de4371bdc4d2e18678263/lights/2/state");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject data_obj;
    data_obj.insert("bri",bri);
    QJsonDocument doc(data_obj);
    QByteArray data = doc.toJson();
    manager->put(request, data);
}
