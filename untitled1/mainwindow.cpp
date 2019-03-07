#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>
#include <QHttpPart>
#include <QFile>
#include <QHttpMultiPart>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->manager = new QNetworkAccessManager(this);
      connect(this->manager, SIGNAL(finished(QNetworkReply*)),
              this, SLOT(replyFinished(QNetworkReply*)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

    QUrlQuery param;
    param.addQueryItem("name",ui->lineEdit->text());
    param.addQueryItem("email",ui->lineEdit_2->text());

    QByteArray dataarray;

    dataarray.append(param.toString());

    QUrl url("http://localhost/request.php");
    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      QVariant("application/x-www-form-urlencoded"));

    manager->post(QNetworkRequest(request), dataarray);


//    QUrl url("http://localhost/request.php?name="+ui->lineEdit->text()+"&email="+ui->lineEdit_2->text());
//    QNetworkRequest request;
//    request.setUrl(url);
//    request.setHeader(QNetworkRequest::ContentTypeHeader,
//                      QVariant("application/x-www-form-urlencoded"));

//    manager->get(QNetworkRequest(request));


}

void MainWindow::replyFinished(QNetworkReply *reply){
    //Use the reply as you wish
    qDebug()<<"reply"<<(QString) reply->readAll();

//    reply->open(QIODevice::ReadOnly);

//        // if the response is correct
//        if(reply->error() == QNetworkReply::NoError)
//        {
//            QByteArray str=(reply->readAll());
//            QString response = QString::fromUtf8(str.data(), str.size());
//            qDebug()<<" re "<<response;
//        }
//        //error sever
//        else
//            qDebug()<<"error response server";
}

void MainWindow::on_pushButton_2_clicked()
{
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    // add image
    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader,QVariant("form-data; name=\"image\"; filename=\"preview.jpg\""));
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpeg"));

    // open file
    QFile *file = new QFile("://preview.jpg");
    if (!file->open(QIODevice::ReadOnly)) {
        qDebug() << "# Could not upload file, could not open file";
//        return false;
    }

    // read file and set data into object
    QByteArray fileContent(file->readAll());
    imagePart.setBody(fileContent);
    multiPart->append(imagePart);

    // set url
    QUrl url("http://localhost/request1.php");
    QNetworkRequest request(url);

//    QLoggingCategory::setFilterRules("qt.network.ssl.w arning=false");

    // create network manager
    QNetworkAccessManager * manager;
    manager = new QNetworkAccessManager();

    manager->post(request, multiPart);

    qDebug() << "# Done sending upload request";
}
