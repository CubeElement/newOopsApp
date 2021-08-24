#include "mainwindow.h"
#include "./ui_mainwindow.h"

//#include "nlohmann/json.hpp"
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
#include <QObject>
#include <QTextEdit>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    m_num_available_newsp = 5;
    m_newsp_units = { "NT", "SD", "DW", "HER", "BUI"};
    connect(ui->button_back_from_p2d, &QPushButton::clicked,
            this, &MainWindow::moveToSelectorPage);
    connect(ui->button_back_from_p2s, &QPushButton::clicked,
            this, &MainWindow::moveToSelectorPage);
    connect(ui->sendButton_address, &QPushButton::clicked,
            this, &MainWindow::m_send_results);
    connect(ui->sendButton_support, &QPushButton::clicked,
            this, &MainWindow::m_send_results);

};

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

            if ((*value).value() != 0)
            {
            ui->page_2_list_newsp->addItem(QString((*key).text()) + ", " +
                                           QString::number((*value).value()));

            }
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
                QLabel* title = new QLabel();
                title->setText((*key).text());
                ui->layout_newsp_addresses->addWidget(title);
                for ( int j = 0 ; j < (*value).value() ; j++ )
                {
                    QLineEdit* address = new QLineEdit();
                    address->setPlaceholderText("Addr. # " + QString::number(j));
                    ui->layout_newsp_addresses->addWidget(address);
                };
            }
        } //  while end
    }
}

void MainWindow::m_add_result(std::string newsp, std::string address)
{
    std::map <std::string, std::list<std::string>>& table = this->m_results_table;
    if ( this->m_results_table.find(newsp) == this->m_results_table.end())
    {
        std::list<std::string> addr_list;
        addr_list.push_back(address);
        table[newsp] = addr_list;
    }
    else
    {
        std::list<std::string> addr_to_push;
        addr_to_push = table[newsp];
        addr_to_push.push_back(address);
        table[newsp] = addr_to_push;
    }
}

void MainWindow::m_map_result()
{
    QString* address_from_user = new QString(ui->lineEdit_address_courier->text());

    if ( sender() == ui->sendButton_support)
    {
        QTextEdit* shipping_address_annotation = new QTextEdit();
        shipping_address_annotation->setText(
                    QString("The following newspapers will be "
                            "delivered to the following address \n")
                            + address_from_user);
        ui->vlayout_report->addWidget(shipping_address_annotation);
        QHashIterator<QLabel*, QSpinBox*> iter(this->m_newsp_missing_hash);
        while (iter.hasNext())
        {
            iter.next();
            QLabel* key = iter.key();
            QSpinBox* value = iter.value();

            if ((*value).value() != 0)
            {
                QHBoxLayout* ship_unit = new QHBoxLayout();
                QLabel* newsp = new QLabel();
                QLabel* addr = new QLabel();
                newsp->setText( QString((*key).text()) );

                ship_unit->addWidget(newsp);
                ship_unit->addWidget(addr);
                ui->vlayout_report->addLayout(ship_unit);
            }
        }
    } else if ( sender() == ui->sendButton_address )
    {

    }
}
//    for (std::map <std::string, std::list<std::string>>::const_iterator it = this->m_results_table.begin();
//         it != this->m_results_table.end(); it ++)
//    {
//        std::cout << it->first << std::endl;

//        std::list <std::string> list_of_newsp = it->second;

//        std::list <std::string>::iterator iter;
//        for ( iter =  list_of_newsp.begin(); iter != list_of_newsp.end(); iter++ )
//        {
//            std::cout << *iter << std::endl;
//            QHBoxLayout* ship_unit = new QHBoxLayout();
//            QLabel* newsp = new QLabel();
//            QLabel* addr = new QLabel();
//            newsp->setText(QString::fromStdString(it->first));
//            addr->setText(QString::fromStdString(*iter));
//            ship_unit->addWidget(newsp);
//            ship_unit->addWidget(addr);
//            ui->vlayout_report->addLayout(ship_unit);
//        }
//        QSpacerItem* spacer = new QSpacerItem(100, 100, QSizePolicy::Expanding, QSizePolicy::Expanding);
//        ui->vlayout_report->addSpacerItem(spacer);
//    }

void MainWindow::m_send_results()
{
    this->m_map_result();
    ui->stackedWidget->setCurrentIndex(4);

}

void MainWindow::m_get_data_for_support()
{
//    std::string newsp_str = ((*key).text()).toStdString();
//    std::string addr_str = (ui->lineEdit_address_courier->text()).toStdString();
//    std::cout << addr_str << " an address str" << std::endl;
//    this->m_add_result(newsp_str, "addr_str");
}

void MainWindow::moveToSelectorPage()
{
    ui->stackedWidget->setCurrentIndex(1);
}

