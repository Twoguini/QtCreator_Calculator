#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>

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

    ui->Display->setText(QString::number(calcVal));
    QPushButton *numButtons[10];
    for(int i = 0; i < 10; ++i){
        QString butName = "B" + QString::number(i);
        numButtons[i] = MainWindow::findChild<QPushButton *>(butName);
        connect(numButtons[i], SIGNAL(released()), this,
                SLOT(NumPressed()));
    }

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

void MainWindow::NumPressed(){
    QPushButton *button = (QPushButton *)sender();
    double butVal = button->text().toDouble();
    QString displayVal = ui->Display->text();
    if ((displayVal == "0")){
        ui->Display->setText(QString::number(butVal));
    } else {
        QString newVal = displayVal + QString::number(butVal);
        double dbNewVal = newVal.toDouble();
        ui->Display->setText(QString::number(dbNewVal, 'g', 16));
    }

}

void MainWindow::DotPressed(){
    QString displayVal = ui->Display->text();
    if (!displayVal.contains(".")){
        ui->Display->setText(displayVal + ".");
    } else {
        ui->Display->setText(displayVal);
    }
}

void MainWindow::MathBTNPressed(){
    divTrigger = false;
    multTrigger = false;
    addTrigger = false;
    subTrigger = false;
    percentageTrigger = false;
    calcVal = ui->Display->text().toDouble();
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

void MainWindow::EqualBTNPressed(){
    double solution = 0.0;
    QString displayVal = ui->Display->text();
    if(divTrigger || multTrigger || addTrigger || subTrigger || percentageTrigger){
        if(addTrigger){
            solution = calcVal + displayVal.toDouble();
        } else if (subTrigger){
            solution = calcVal - displayVal.toDouble();
        } else if (multTrigger){
            solution = calcVal * displayVal.toDouble();
        } else if (divTrigger){
            solution = calcVal / displayVal.toDouble();
        } else{
            solution = (calcVal / 100) * displayVal.toDouble();
        }
    }
    ui->Display->setText(QString::number(solution));
}

void MainWindow::ClearBTNPressed(){
    calcVal = 0.0;
    ui->Display->setText(QString::number(calcVal));
}

void MainWindow::DeleteChar(){
    QStringList displayVal = ui->Display->text().split("", Qt::SkipEmptyParts);
    displayVal.removeLast();
    if(QString::compare(displayVal.join(""), "", Qt::CaseInsensitive) == 0){
        ui->Display->setText("0");
    } else
        ui->Display->setText(displayVal.join(""));
}

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

