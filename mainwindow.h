#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include  "databasejson.h"

#include <iostream>
#include <QMainWindow>
#include <QObject>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onButtonSigninClicked(QString staff_id, QString password);
    void on_button_option_proceed_clicked();
    void receiveSelectionValues(QString, int);
    void showReport(QString, QString);

signals:
    void sendSelectionValues();
    void sendReportData();

private:
    Ui::MainWindow *ui;
    DatabaseJSON db;

private:
    void moveToSelectorPage();
    void createSelectorList(const QSet<QString>& units_list);
    void sendReport();

    inline QString getCourierAddress();
    void messageBox(std::string);

private:
};

#endif // MAINWINDOW_H
