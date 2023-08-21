#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include <QDebug>
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QRegularExpression>
#include <QScroller>
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->listWidget->verticalScrollBar(), &QScrollBar::valueChanged,
            this, &MainWindow::on_listWidget_verticalScrollBar_valueChanged);
    connect(ui->listWidget_2->verticalScrollBar(), &QScrollBar::valueChanged,
            this, &MainWindow::on_listWidget_2_verticalScrollBar_valueChanged);
    QScroller::grabGesture(ui->plainTextEdit_2,QScroller::LeftMouseButtonGesture);
    QScroller::grabGesture(ui->listWidget_2,QScroller::LeftMouseButtonGesture);
    QScroller::grabGesture(ui->tableWidget,QScroller::LeftMouseButtonGesture);
    ui->listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_2_clicked()
{
    QString html_url=ui->lineEdit->text();
//    qInfo() << html_url;
    QNetworkAccessManager manager;
    QUrl url(html_url);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/115.0.0.0 Safari/537.36 Edg/115.0.1901.203");


    QNetworkReply *reply = manager.get(request);
//    qInfo()<<reply->readAll();
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, [&]{
//        qInfo() << "Status code:" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        loop.quit();
    });
    loop.exec();
    QString StatueCode="状态代码：";
    StatueCode+=QString::number(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt());
    ui->label->setText(StatueCode);
    QByteArray content = reply->readAll();
    ui->plainTextEdit_2->setPlainText(QString::fromUtf8(content));
//    qInfo() << "Content:" << QString::fromUtf8(content);
    reply->deleteLater();
}



void MainWindow::on_pushButton_clicked()
{
    if(ui->listWidget_2->count()==0) return ;
    // 初始化
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(1);
    ui->tableWidget->setColumnCount(1);
//    QString XpathStr = ui->plainTextEdit->toPlainText();

    // 获取网页内容
    QString HtmlStr = ui->plainTextEdit_2->toPlainText();

    //先解析第一个表达式到表格
    int j=0;
    QListWidgetItem *item1 = ui->listWidget_2->item(0);
    if(item1->text().isEmpty()) {
        ui->label_2->setText("表达式为空");
        return;
    }
    QRegularExpression re(item1->text());
    QRegularExpressionMatchIterator it = re.globalMatch(HtmlStr);
    while (it.hasNext()) {
        QRegularExpressionMatch match = it.next();
        QTableWidgetItem *item = new QTableWidgetItem(match.captured(0));
        ui->tableWidget->setItem(j++, 0, item);
        ui->tableWidget->insertRow(j);
    }
    ui->tableWidget->insertColumn(1);

    //再解析后续的表达式到后续的表格
    //row:行
    //Column:列
//    qInfo()<<ui->listWidget_2->count();
//    qInfo()<<ui->tableWidget->rowCount();
    int k=0;
    int rowCount=ui->tableWidget->rowCount()-1;
    for (int i = 1; i < ui->listWidget_2->count(); i++) {
        QListWidgetItem *item1 = ui->listWidget_2->item(i);
        if(item1->text().isEmpty()) {
            ui->label_2->setText("表达式为空");
            return ;
        }
        QRegularExpression re(item1->text());
        for (int j = 0; j < rowCount; j++) {
            QTableWidgetItem *item2=ui->tableWidget->item(j,i-1);
//            qInfo()<<item2->text();
            if(item2->text().isEmpty()) break;
            QRegularExpressionMatchIterator it = re.globalMatch(item2->text());
            while (it.hasNext()) {
                QRegularExpressionMatch match = it.next();
                QTableWidgetItem *item3 = new QTableWidgetItem(match.captured(0));
                ui->tableWidget->setItem(k++, i, item3);
                if(k==(ui->tableWidget->rowCount())) ui->tableWidget->insertRow(k);
            }
//            qInfo()<<i<<" "<<j<<" "<<k;
        }
        ui->tableWidget->insertColumn(i+1);
//        qInfo()<<ui->tableWidget->rowCount();
        rowCount=k;
        k=0;
    }
    ui->label_2->setText("解析成功");
}


void MainWindow::on_pushButton_3_clicked()
{
    QListWidgetItem * item1 = new QListWidgetItem;
    QListWidgetItem * item2 = new QListWidgetItem;
    int itemCount = ui->listWidget->count();
    item1->setText(QString::number(itemCount));
    item2->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
    ui->listWidget->addItem(item1);
    ui->listWidget_2->addItem(item2);

}


void MainWindow::on_pushButton_5_clicked()
{
    ui->plainTextEdit_2->clear();
}

void MainWindow::on_listWidget_verticalScrollBar_valueChanged(int value)
{
    ui->listWidget_2->verticalScrollBar()->setValue(value);
}

void MainWindow::on_listWidget_2_verticalScrollBar_valueChanged(int value)
{
    ui->listWidget->verticalScrollBar()->setValue(value);
}

void MainWindow::on_pushButton_4_clicked()
{
    int lastRow = ui->listWidget->count() - 1;
    if (lastRow >= 0) {
        QListWidgetItem *item = ui->listWidget->takeItem(lastRow);
        delete item;
        QListWidgetItem *item2 = ui->listWidget_2->takeItem(lastRow);
        delete item2;
    }
}

