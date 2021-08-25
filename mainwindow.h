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

    int m_num_available_newsp;
    void make_newsp_list(int n);
    std::vector<std::string> m_newsp_units;
    QHash<QLabel*, QSpinBox*> m_newsp_missing_hash;
    void m_map_result();
    void m_send_results();
    inline QString* getCourierAddress();

    /* output datastructures: id, count, list(address) */
    std::map <std::string, int> m_newspIdOutput;
    std::map <int, int> m_newspCountOutput;
    std::map <int, std::list<QString*>> m_newspAddrOutput;

    inline void add_NewspIdOutput(std::string newsp_name, int id);
    inline int get_NewspIdOutput(std::string newsp_name);

    inline void add_NewspCountOutput(int id, int count);
    inline int get_NewspCountOutput(int id);

    inline void add_NewspAddrOutput(int id, QString*);
    inline std::list<QString*> get_NewspAddrOutput(int id);

    inline void clearOutput();
};

#endif // MAINWINDOW_H
