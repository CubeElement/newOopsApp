#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "nlohmann/json.hpp"
#include <iostream>
#include <vector>

#include <QSpinBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QSizePolicy>
#include <QMap>
#include <QDebug>
#include <QHashIterator>
#include <QGroupBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    m_num_available_newsp = 5;
    m_newsp_units = { "NT", "SD", "DW", "HER", "BUI"};
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
        ui->page_2_list_newsp->clear();
        QHashIterator<QLabel*, QSpinBox*> iter(this->m_newsp_missing_hash);
        while (iter.hasNext()) {
            iter.next();
            QLabel* key = iter.key();
            QSpinBox* value = iter.value();
            ui->page_2_list_newsp->addItem(QString((*key).text()) + ", " +
                                           QString::number((*value).value()));
            //qDebug() << "Object text info: " << (*key).text() << Qt::endl;
            //qDebug() << iter.key() << iter.value() << Qt::endl;
        }
        ui->page_2_list_newsp->sortItems(Qt::AscendingOrder);

    } else if ( ui->radio_delivery->isChecked() )
    {
        ui->stackedWidget->setCurrentIndex(3);
        QLayoutItem *child;
        while ((child = ui->layout_newsp_addresses->takeAt(0)) != 0) {
            delete child->widget();
            delete child;
        }
        QHashIterator<QLabel*, QSpinBox*> iter(this->m_newsp_missing_hash);
        while (iter.hasNext())
        {
            iter.next();
            QLabel* key = iter.key();
            QSpinBox* value = iter.value();

            if ((*value).value() != 0)
            {
                QGroupBox* groupbox = new QGroupBox(ui->scroll_newsp_addresses);

                groupbox->setTitle((*key).text());
                ui->layout_newsp_addresses->addWidget(groupbox);
                QVBoxLayout* vlayout = new QVBoxLayout();
                for ( int j = 0 ; j < (*value).value() ; j++ )
                {
                    QLineEdit* address = new QLineEdit(groupbox);
                    vlayout->addWidget(address);
                    address->setPlaceholderText("Addr. #" + QString::number(j));
                };
                ui->layout_newsp_addresses->addLayout(vlayout);
                vlayout = nullptr;
                delete vlayout;
            }
        } //  while newsp iterator.next()
    }
}

void MainWindow::on_button_back_from_p2d_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_button_back_from_p2s_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


