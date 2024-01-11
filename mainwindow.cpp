#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include<QDir>


double calcVal = 0.0;
bool divTrigger = false;
bool multTrigger = false;
bool addTrigger = false;
bool subTrigger = false;
bool percentageTrigger = false;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Indentify which Number Button was pressed
    ui->Display->setText(QString::number(calcVal));
    QPushButton *numButtons[10];
    for(int i = 0; i < 10; ++i){
        QString butName = "B" + QString::number(i);
        numButtons[i] = MainWindow::findChild<QPushButton *>(butName);
        connect(numButtons[i], SIGNAL(released()), this,
                SLOT(NumPressed()));
    }

    //Connect button to it's func
    connect(ui->Dot, SIGNAL(released()), this,
            SLOT(DotPressed()));

    connect(ui->Sum, SIGNAL(released()), this,
            SLOT(MathBTNPressed()));
    connect(ui->Minus, SIGNAL(released()), this,
            SLOT(MathBTNPressed()));
    connect(ui->Divided, SIGNAL(released()), this,
            SLOT(MathBTNPressed()));
    connect(ui->Times, SIGNAL(released()), this,
            SLOT(MathBTNPressed()));
    connect(ui->Percentage, SIGNAL(released()), this,
            SLOT(MathBTNPressed()));

    connect(ui->Equals, SIGNAL(released()), this,
            SLOT(EqualBTNPressed()));

    connect(ui->Clear, SIGNAL(released()), this,
            SLOT(ClearBTNPressed()));

    connect(ui->Delete, SIGNAL(released()), this,
            SLOT(DeleteChar()));

    connect(ui->Close, SIGNAL(released()), this,
            SLOT(CloseApp()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Adds the pressed number button to the display
void MainWindow::NumPressed(){
    QPushButton *button = (QPushButton *)sender();
    double butVal = button->text().toDouble();
    QString displayVal = ui->Display->toPlainText();
    if ((displayVal.toDouble() == 0 && displayVal != "0.")){
        ui->Display->setText(QString::number(butVal));
    } else if(butVal == 0){
        QString newVal = displayVal + "0";
        ui->Display->setText(newVal);
    } else {
        QString newVal = displayVal + QString::number(butVal);
        double dbNewVal = newVal.toDouble();
        ui->Display->setText(QString::number(dbNewVal, 'g', 16));
    }

}

//Add the decimal dot to display
void MainWindow::DotPressed(){
    QString displayVal = ui->Display->toPlainText();
    if (!displayVal.contains(".")){
        ui->Display->setText(displayVal + ".");
    } else {
        ui->Display->setText(displayVal);
    }
}

//Identify which math button was pressed
void MainWindow::MathBTNPressed(){
    divTrigger = false;
    multTrigger = false;
    addTrigger = false;
    subTrigger = false;
    percentageTrigger = false;
    calcVal = ui->Display->toPlainText().toDouble();
    QPushButton *button = (QPushButton *)sender();
    QString butVal = button->text();
    if (QString::compare(butVal, "/", Qt::CaseInsensitive) == 0){
        divTrigger = true;
    } else if (QString::compare(butVal, "*", Qt::CaseInsensitive) == 0){
        multTrigger = true;
    } else if (QString::compare(butVal, "+", Qt::CaseInsensitive) == 0){
        addTrigger = true;
    } else if (QString::compare(butVal, "-", Qt::CaseInsensitive) == 0){
        subTrigger = true;
    } else if (QString::compare(butVal, "%") == 0){
        percentageTrigger = true;
    }
    ui->Display->setText(" ");
}

QString MainWindow::ReadFile(QString name, int id){
    QFile infoJSON;
    QJsonObject object;
    QString result = "";

    infoJSON.setFileName(name);
    infoJSON.open(QIODevice::ReadOnly | QIODevice::Text);

    QByteArray data = infoJSON.readAll();
    infoJSON.close();
    if(data == "") {
        return "Não foi possível achar o arquivo";
    }
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

    if(jsonDoc.isEmpty()){
        return "Arquivo Corrompido e/ou não é .JSON";
    }
    QJsonObject objJson = jsonDoc.object();
    QJsonArray jsonArray = objJson.value("Libraries").toArray();
    for (int i = 0; i < jsonArray.size(); i++) {
        object = jsonArray[i].toObject();
        if (object["Id"] == id){
            /*QJsonDocument doc(object);
            QString strJson(doc.toJson(QJsonDocument::Indented));*/
            result = QString("Id: %1\nNome: %2\nPurchase: %3").arg(object["Id"].toDouble()).arg(object["Name"].toString()).arg(object["Purchase"].toDouble());
            i = jsonArray.size() -1;
        }
    }
    qDebug() << result;
    if (QString::compare(result,"") == 0){
        return "Esse Id não existe";
    } else {
        return result;
    }
}

//Do the math
void MainWindow::EqualBTNPressed(){
    double solution = 0.0;
    QString filePath = QDir::currentPath();
    qDebug() << filePath;
    QString displayVal = ui->Display->toPlainText();
    if(divTrigger || multTrigger || addTrigger || subTrigger || percentageTrigger){
        if(addTrigger){
            solution = calcVal + displayVal.toDouble();
            addTrigger = false;
        } else if (subTrigger){
            solution = calcVal - displayVal.toDouble();
            subTrigger = false;
        } else if (multTrigger){
            solution = calcVal * displayVal.toDouble();
            multTrigger = false;
        } else if (divTrigger){
            solution = calcVal / displayVal.toDouble();
            divTrigger = false;
        } else{
            solution = (calcVal / 100) * displayVal.toDouble();
            percentageTrigger = false;
        }
        ui->Display->setText(QString::number(solution));
    } else {
        ui->Display->setFont(QFont("Segoe UI", 20, QFont::Bold));
        ui->Display->setText(ReadFile("D:\\Programming\\QtCreator-Calc\\Calculator\\JSONinfo\\teste.json", displayVal.toInt()));
    }

}

//Clear the display
void MainWindow::ClearBTNPressed(){
    calcVal = 0.0;
    ui->Display->setFont(QFont("Segoe UI", 65, QFont::Bold));
    ui->Display->setText(QString::number(calcVal));
}

//Deletes the last char
void MainWindow::DeleteChar(){
    /*QStringList displayVal = ui->Display->text().split("", Qt::SkipEmptyParts);
    displayVal.removeLast();
    if(QString::compare(displayVal.join(""), "", Qt::CaseInsensitive) == 0){
        ui->Display->setText("0");
    } else
        ui->Display->setText(displayVal.join(""));*/

    QString displayVal = ui->Display->toPlainText();
    displayVal.chop(1);
    if(QString::compare(displayVal, "", Qt::CaseInsensitive) == 0){
        ui->Display->setText("0");
    } else {
        ui->Display->setText(displayVal);
    }
}

//Asks if the client want to close the application
void MainWindow::CloseApp(){
    QMessageBox closeAskBox;
    closeAskBox.setText("Você Realmente Deseja Fechar o Programa?");
    QPushButton *yesBTN = closeAskBox.addButton(QMessageBox::Yes);
    QPushButton *noBTN = closeAskBox.addButton(QMessageBox::No);
    closeAskBox.setIcon(QMessageBox::Question);
    closeAskBox.exec();
    if (closeAskBox.clickedButton() == yesBTN){
        close();
    } else {
        return;
    }
}

