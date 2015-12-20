#include "huecontrolwindow.h"
#include "ui_huecontrolwindow.h"

#define LAUNDRY_TIMEOUT 15
#define QUERY_PERIOD 10
#define COUNT_MAX LAUNDRY_TIMEOUT*60/QUERY_PERIOD
#define NUM_LIGHTS 8


HueControlWindow::HueControlWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HueControlWindow)
{
    timer = new QTimer(this);
    ui->setupUi(this);
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(syncRequestFinished(QNetworkReply*)));

    connect(timer, SIGNAL(timeout()), this, SLOT(updateLightStatus()));

    timer->start(QUERY_PERIOD*1000); //query period is in seconds
    t_count = COUNT_MAX;

    lightCheckList = new QCheckBox*[8];
    lightCheckList[0] = ui->checkBox1;
    lightCheckList[1] = ui->checkBox1_2;
    lightCheckList[2] = ui->checkBox1_3;
    lightCheckList[3] = ui->checkBox1_4;
    lightCheckList[4] = ui->checkBox1_5;
    lightCheckList[5] = ui->checkBox1_6;
    lightCheckList[6] = ui->checkBox1_7;
    lightCheckList[7] = ui->checkBox1_8;

}

HueControlWindow::~HueControlWindow()
{
    delete ui;
}

void HueControlWindow::syncRequestFinished(QNetworkReply *reply)
{

    QByteArray data = reply->readAll();

    QJsonDocument doc = QJsonDocument::fromJson(data);

    qDebug()<< "object count = "<< doc.object().count();

    QJsonObject jObj = doc.object();
    //this is one expected form of json response
    // for a query of a single light. others (such as a put ack) will have different formats!

    for (QJsonObject::const_iterator it = jObj.begin(); it != jObj.end(); it++)
    {
        // if we get this, we don't actually know which light we got it from...
        // unless we only query a single light, ever.
        if (it.key() == "state")
        {
            QJsonObject stateObj = it.value().toObject();
            for (QJsonObject::const_iterator state_it = stateObj.begin(); state_it != stateObj.end(); state_it++)
            {
                if (state_it.key() == "on")
                {
                    bool isOn = state_it.value().toBool();
                    qDebug() << "light is on = " << isOn;

                    if (isOn) {
                        t_count--;
                        ui->lcdNumber->display(QString::number(t_count*QUERY_PERIOD)); //in seconds
                    }
                    else {
                        ui->lcdNumber->display(QString::number(999));
                        t_count = COUNT_MAX; //reset counter
                    }
                    if (t_count <= 0)
                    {   //hard coded laundry light switchoff
                        qDebug() << "timer up. turning light off!!";
                        //setLightOn(false,4);
                        t_count = COUNT_MAX;
                    }
                }
            }
        }
    }
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
    if (ui->checkBox1_5->isChecked())
        setLightOn(true, 5);
    if (ui->checkBox1_6->isChecked())
        setLightOn(true, 6);
    if (ui->checkBox1_7->isChecked())
        setLightOn(true, 7);
    if (ui->checkBox1_8->isChecked())
        setLightOn(true, 8);

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
    if (ui->checkBox1_5->isChecked())
        setLightOn(false,5);
    if (ui->checkBox1_6->isChecked())
        setLightOn(false,6);
    if (ui->checkBox1_7->isChecked())
        setLightOn(false,7);
    if (ui->checkBox1_8->isChecked())
        setLightOn(false, 8);

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
     QUrl url("http://192.168.100.230/api/b225912329de4371bdc4d2e18678263/lights/4/");
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
    if (ui->checkBox1_4->isChecked())
        setBrightness(bri, 4);
    if (ui->checkBox1_5->isChecked())
        setBrightness(bri, 5);
    if (ui->checkBox1_7->isChecked())
        setBrightness(bri, 6);
    if (ui->checkBox1_7->isChecked())
        setBrightness(bri, 7);
    if (ui->checkBox1_8->isChecked())
        setBrightness(bri, 8);

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

void HueControlWindow::queryLight(int idx)
{   //note2self: if ask for index, we need to asynchronously
    // figure out if returned message is actually related
    // to the light we asked for, or potentially another
    // previously queued event. (unlikely, but possible situation!)

    QString url_str = "http://192.168.100.230/api/b225912329de4371bdc4d2e18678263/lights/" + QString::number(idx) +"/";
    QUrl url(url_str);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    manager->get(request);

}

void HueControlWindow::updateLightStatus()
{
    qDebug() << "timed light update triggered";
    queryLight(4);
}

void HueControlWindow::selectAllChecks(bool checked)
{
    for (int i=0; i<NUM_LIGHTS; i++)
    {
        if (lightCheckList[i] != NULL)
            lightCheckList[i]->setChecked(checked);
    }
}

void HueControlWindow::on_pushButton_SelAll_clicked()
{
    selectAllChecks(true);
}

void HueControlWindow::on_pushButton_SelNone_clicked()
{
    selectAllChecks(false);
}
