#include "huecontrolwindow.h"
#include "ui_huecontrolwindow.h"

#define LAUNDRY_TIMEOUT 15
#define QUERY_PERIOD 10
#define COUNT_MAX LAUNDRY_TIMEOUT*60/QUERY_PERIOD
#define NUM_LIGHTS 14


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

    //static list of UI elements from QT designer
    // see Form/labels for light names
    lightCheckList = new QCheckBox*[NUM_LIGHTS];
    lightCheckList[0] = ui->checkBox1;
    lightCheckList[1] = ui->checkBox1_2;
    lightCheckList[2] = ui->checkBox1_3;
    lightCheckList[3] = ui->checkBox1_4;
    lightCheckList[4] = ui->checkBox1_5;
    lightCheckList[5] = ui->checkBox1_6;
    lightCheckList[6] = ui->checkBox1_7;
    lightCheckList[7] = ui->checkBox1_8;
    lightCheckList[8] = ui->checkBox1_9;
    lightCheckList[9] = ui->checkBox1_10;
    lightCheckList[10] = ui->checkBox1_11;
    lightCheckList[11] = ui->checkBox1_12;
    lightCheckList[12] = ui->checkBox1_13;
    lightCheckList[13] = ui->checkBox1_14;

    for (int i=0; i<NUM_LIGHTS; ++i) //allow using checkboxes to control individual lights
    {
        connect(lightCheckList[i], SIGNAL(pressed()), this, SLOT(checkPressed()));
    }

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
                        setLightOn(false,13);
                        t_count = COUNT_MAX;
                    }
                }
            }
        }
    }
}

void HueControlWindow::on_pushButton_clicked()
{
    for (int i=0; i<NUM_LIGHTS; ++i)
    {
        if (lightCheckList[i]->isChecked())
            setLightOn(true, i+1);
    }

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
    for (int i=0; i<NUM_LIGHTS; ++i)
    {
        if (lightCheckList[i]->isChecked())
            setLightOn(false, i+1);
    }

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
    for (int i=0; i<NUM_LIGHTS; ++i)
    {
        if (lightCheckList[i]->isChecked())
            setBrightness(bri, i+1);
    }
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

void HueControlWindow::checkPressed()
{
    for (int i=0; i<NUM_LIGHTS; ++i)
    {
        if (QObject::sender() == lightCheckList[i])
        {
            qDebug() << "check " << i;
            if (lightCheckList[i]->isChecked()) //NOTE: because this happens before toggle, logic is REVERSED!
                setLightOn(false, i+1);
            else
                setLightOn(true, i+1);
        }
    }

}
