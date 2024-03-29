#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJsonDocument>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void NumPressed();
    void MathBTNPressed();
    void EqualBTNPressed();
    void ClearBTNPressed();
    void DotPressed();
    void DeleteChar();
    void CloseApp();
    QString ReadFile(QString name, int id);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
