#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include  "databasejson.h"

#include <iostream>
#include <array>
#include <QMainWindow>
#include <QObject>
#include <QLabel>
#include <QSpinBox>
#include <iterator>

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
    void onButtonSigninClicked(QString staff_id);
    void on_button_option_proceed_clicked();

private:
    Ui::MainWindow *ui;

private:
    void moveToSelectorPage();
    void createSelectorList(std::set<std::string>);
    void createSingleAddressList();
    void createMultipleAddressList();

    int m_num_available_newsp;
    std::vector<std::string> m_newsp_units;
    QHash<QLabel*, QSpinBox*> m_NewspMissingHash;
    void showReport();
    void sendReport();
    inline QString getCourierAddress();

    /* output datastructures: id, count, list(address) */
    std::map <std::string, int> m_ReportNewsp;
    std::map <int, int> m_ReportCounters;
    std::map <int, std::list<QLineEdit*>> m_ReportAddresses;

    inline void addNewsp(std::string newsp_name, int id);
    inline int getNewspId(std::string newsp_name) const;

    inline void addNewspCounter(int id, int count);
    inline int getNewspCounter(int id) const;

    inline void addNewspAddr(int id, QLineEdit*);
    inline std::list<QLineEdit*> getNewspAddr(int id) const;

    inline void clearReportData();

private:
    DatabaseJSON db;
};

#endif // MAINWINDOW_H
