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

    void on_button_back_from_p2d_clicked();

    void on_button_back_from_p2s_clicked();

private:
    Ui::MainWindow *ui;

private:

    void set_staff_id();
    int m_staff_id;
public:
    int m_num_available_newsp;
    void make_newsp_list(int n);
    std::vector<std::string> m_newsp_units;
    QHash<QLabel*, QSpinBox*> m_newsp_missing_hash;

};
#endif // MAINWINDOW_H
