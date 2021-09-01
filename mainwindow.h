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
    void onButtonOptionProceedClicked();
    void addWidgetsLists(QString, int);
    void showReport(const QString&, const QString&);
    void setSelectionStatus(int);
    void on_report_page_finish_clicked();

signals:
    void sendSelectionValues();
    void sendReportSingleAddr();
    void sendReportMultiAddr();
    void selectionStatus();

private:
    Ui::MainWindow *ui;
    DatabaseJSON db;

public:
    void moveToSelectorPage();
    void addSelectorList(const QSet<QString>& units_list);
    void sendReport();
    inline QString getCourierAddress();
    void messageBox(std::string);
    bool m_isNotEmptySelection;
};

#endif // MAINWINDOW_H
