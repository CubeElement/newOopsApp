#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "nlohmann/json.hpp"
#include <iostream>
#include <vector>

#include <QSpinBox>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QSizePolicy>
#include <QMap>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    m_num_available_newsp = 5;
    m_newsp_units = { "NT", "SD", "DW", "HER", "BUI"};
    //QHash<QObject*, QObject*> m_newsp_missing_hash;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::set_staff_id()
{
    QString staff_id;
    staff_id = ui->line_staff_id->text();
    this->m_staff_id = staff_id.toInt();
}

void MainWindow::make_newsp_list(int n)
{
    for ( int i = 0; i < n; i++ )
    {
        QHBoxLayout* p_layout = new QHBoxLayout();
        p_layout->setAlignment(ui->layout_page1, Qt::AlignCenter);

        QLabel* newsp_name = new QLabel(ui->page_1);
        QSpinBox* newsp_count = new QSpinBox(ui->page_1);
        newsp_name->setText( QString::fromStdString( this->m_newsp_units[i]) );
        newsp_count->setValue(0);
        this->m_newsp_missing_hash[newsp_name] = newsp_count;

        p_layout->addWidget(newsp_name);
        p_layout->addWidget(newsp_count);
        ui->layout_page1->addLayout(p_layout);
    }
    QSpacerItem* spacer = new QSpacerItem(100, 100, QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->layout_page1->addSpacerItem(spacer);
    //qDebug() << this->m_newsp_missing_hash;
}

void MainWindow::on_button_signin_clicked()
{
    set_staff_id();
    ui->stackedWidget->setCurrentIndex(1);
    ui->label_staff_id->setText(QString("Hello, ") + QString::number(this->m_staff_id));
    make_newsp_list(this->m_num_available_newsp);
}


void MainWindow::on_button_option_proceed_clicked()
{
    if ( ui->radio_address->isChecked() )
    {
        ui->stackedWidget->setCurrentIndex(2);
        this->get_missing_newsp();
        qDebug() << this->get_missing_newsp().size();

    } else if ( ui->radio_delivery->isChecked() )
    {
        ui->stackedWidget->setCurrentIndex(3);
    }
}

QHash <QObject*, QObject*> MainWindow::get_missing_newsp()
{
    return this->m_newsp_missing_hash;
}

void MainWindow::on_button_back_from_p2d_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_button_back_from_p2s_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_button_back_to_main_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

