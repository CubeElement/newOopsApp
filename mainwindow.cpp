#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <iostream>
#include <vector>

#include <QSpinBox>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QSizePolicy>
#include <QDebug>
#include <QObject>
#include <iterator>
#include <QCompleter>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    connect(ui->button_back_from_p2d, &QPushButton::clicked,
            this, &MainWindow::moveToSelectorPage);
    connect(ui->button_back_from_p2s, &QPushButton::clicked,
            this, &MainWindow::moveToSelectorPage);
    connect(ui->sendButton_address, &QPushButton::clicked,
            this, &MainWindow::sendReport);
    connect(ui->sendButton_support, &QPushButton::clicked,
            this, &MainWindow::sendReport);
    connect(ui->button_signin, &QPushButton::clicked,
            this,
            [=]() { MainWindow::onButtonSigninClicked(ui->line_staff_id->text(),
                                                      ui->line_password->text()); });

};

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::moveToSelectorPage()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::onButtonSigninClicked(QString staff_id, QString password)
{
    if ( db.checkUserData(staff_id, password) )
    {
        ui->stackedWidget->setCurrentIndex(1);
        ui->label_staff_id->setText(QString("Hello, ") +
                                    QString::fromStdString(db.getCourierName()));
        createSelectorList(db.getCourierNewspapers());
    } else
    {
        messageBox("Authorization failed!");
    }
}

void MainWindow::messageBox(std::string message_text)
{
    QMessageBox message;
    message.setText(QString::fromStdString(message_text));
    message.setIcon(QMessageBox::Warning);
    message.setSizeIncrement(120, 40);
    message.exec();
}

void MainWindow::createSelectorList(const std::set<std::string>& units_list)
{
    std::set<std::string>::iterator it;
    for ( it = units_list.begin(); it != units_list.end(); it++ )
    {
        QHBoxLayout* p_layout = new QHBoxLayout();
        p_layout->setAlignment(ui->layout_page1, Qt::AlignCenter);

        QLabel* newsp_name = new QLabel(ui->page_1);
        QSpinBox* newsp_count = new QSpinBox(ui->page_1);
        newsp_name->setText( QString::fromStdString( *it ));
        newsp_count->setValue(0);
        QObject::connect(this,
                         &MainWindow::sendSelectionValues,
                         this,
                         [=]() { MainWindow::receiveSelectionValues(
                                newsp_name->text(),
                                newsp_count->value());
                               }
                        );

        p_layout->addWidget(newsp_name);
        p_layout->addWidget(newsp_count);
        ui->layout_page1->addLayout(p_layout);
    }
    QSpacerItem* spacer = new QSpacerItem(100, 100,
                                          QSizePolicy::Expanding,
                                          QSizePolicy::Expanding);
    ui->layout_page1->addSpacerItem(spacer);
}

void MainWindow::on_button_option_proceed_clicked()
{
    if ( ui->radio_address->isChecked() )
    {
        ui->stackedWidget->setCurrentIndex(2);
        ui->lineEdit_address_courier->setText(QString::fromStdString(db.getCourierPlace()));
        ui->page_2_list_newsp->clear();
    }
    else if ( ui->radio_delivery->isChecked() )
    {
        ui->stackedWidget->setCurrentIndex(3);
        /* clear children of layout */
        QLayoutItem *child;
        while ((child = ui->layout_newsp_addresses->takeAt(0)) != 0) {
            delete child->widget();
            delete child;
        }
    }
    emit sendSelectionValues();
}

void MainWindow::receiveSelectionValues(QString name, int count)
{
    QString count_str = QString::number(count);
    if ( ui->stackedWidget->currentIndex() == 2 && count != 0)
    {
        ui->page_2_list_newsp->addItem(name + " " + count_str + " pcs.");
        QObject::connect(this,
                         &MainWindow::sendReportData,
                         this,
                         [=]() { MainWindow::showReport(
                                name,
                                count_str); });
    } else if ( ui->stackedWidget->currentIndex() == 3  && count != 0)
    {
        QLabel* title = new QLabel();
        title->setText(name);
        ui->layout_newsp_addresses->addWidget(title);
        for ( int j = 0 ; j < count ; j++ )
        {
            QLineEdit* address = new QLineEdit();
            address->setPlaceholderText("Addr. # " + QString::number(j));
            QStringList* addrlist = new QStringList(db.getSubscriberAddresses());
            QCompleter* completer = new QCompleter(*addrlist, this);
            address->setCompleter(completer);
            ui->layout_newsp_addresses->addWidget(address);
            QObject::connect(this,
                             &MainWindow::sendReportData,
                             this,
                             [=]() { MainWindow::showReport(
                                    name,
                                    address->text()); });
        }
    }
}

void MainWindow::sendReport()
{
    ui->stackedWidget->setCurrentIndex(4);

    QLabel* shipping_address_annotation = new QLabel();
    shipping_address_annotation->setText(QString("Your request have been sent \n"));
    ui->vlayout_report->addWidget(shipping_address_annotation);
    if (sender() == ui->sendButton_support)
    {
        QLabel* courier_addr = new QLabel();
        QString address_from_user = this->getCourierAddress();
        courier_addr->setText(QString("Courier's address: ") + address_from_user);
        ui->vlayout_report->addWidget(courier_addr);
    }

    emit sendReportData();
}

void MainWindow::showReport(QString name, QString info_value)
{
    QHBoxLayout* hlayout = new QHBoxLayout();
    QLabel* newsp = new QLabel();
    QLabel* info = new QLabel();
    newsp->setText(name);
    info->setText(info_value);
    hlayout->addWidget(newsp);
    hlayout->addWidget(info);
    ui->vlayout_report->addLayout(hlayout);

    QSpacerItem* spacer = new QSpacerItem(100, 100,
                                          QSizePolicy::Expanding,
                                          QSizePolicy::Expanding);
    ui->vlayout_report->addSpacerItem(spacer);
}

inline QString MainWindow::getCourierAddress()
{
    QString address = ui->lineEdit_address_courier->text();
    return address;
}
