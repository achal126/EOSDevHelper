#include "settingsframe.h"
#include "ui_settingsframe.h"
#include "misc/config.h"

#include "codebase/utility/httpclient.h"

#include <QJsonDocument>

extern QString base_url;
extern QString url_port;
extern bool skip_signature;

SettingsFrame::SettingsFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::SettingsFrame),
    httpc (new HttpClient)
{
    ui->setupUi(this);
    ui->lineEditPort->setValidator(new QRegExpValidator(QRegExp("(\\d{1,5})"), this));

    ui->lineEditUrl->setText(base_url);
    ui->lineEditPort->setText(url_port);
    ui->checkBoxSkipSignature->setChecked(skip_signature);

    ui->checkBoxSkipSignature->setVisible(false);
}

SettingsFrame::~SettingsFrame()
{
    delete ui;
    delete httpc;
}

void SettingsFrame::on_pushButtonConnect_clicked()
{
    base_url = ui->lineEditUrl->text();
    url_port = ui->lineEditPort->text();
    skip_signature = ui->checkBoxSkipSignature->isChecked();

    Config cfg;
    cfg.saveSettings();

    if (httpc) {
        httpc->request(FunctionID::get_info);
        connect(httpc, &HttpClient::responseData, [=](const QByteArray& data){
            auto formatData = QJsonDocument::fromJson(data).toJson(QJsonDocument::Indented);
            ui->textEditOutput->setText(QString::fromStdString(formatData.toStdString()));
        });
    }
}
