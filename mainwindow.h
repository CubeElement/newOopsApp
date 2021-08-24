#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <array>
#include <QMainWindow>
#include <QObject>
#include <QLabel>
#include <QSpinBox>

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
    void on_button_signin_clicked();
    void on_button_option_proceed_clicked();

private:
    Ui::MainWindow *ui;

private:
    void moveToSelectorPage();
    void set_staff_id();
    int m_staff_id;

public:
    int m_num_available_newsp;
    void make_newsp_list(int n);
    std::vector<std::string> m_newsp_units;
    QHash<QLabel*, QSpinBox*> m_newsp_missing_hash;
    void m_add_result(std::string, std::string);
    void m_map_result();
    std::map <std::string, std::list<std::string>> m_results_table;
    void m_send_results();
    void m_get_data_for_support();

};
#endif // MAINWINDOW_H
