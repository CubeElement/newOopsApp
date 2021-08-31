#include "mainwindow.h"
#include "./ui_mainwindow.h"

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
#include <stdexcept>
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
    this->clearReportData();
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
    if ( ui->stackedWidget->currentIndex() == 2 )
    {
        ui->page_2_list_newsp->addItem(name + " " + QString::number(count)
                                       + " pcs.");
    } else if ( ui->stackedWidget->currentIndex() == 3 )
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
        }
    }
}

void MainWindow::sendReport()
{
    ui->stackedWidget->setCurrentIndex(4);
    this->showReport();
}

void MainWindow::showReport()
{
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
    std::map <std::string, int>::iterator it;
    for ( it = this->m_ReportNewsp.begin(); it != this->m_ReportNewsp.end(); it++ )
    {
        if (sender() == ui->sendButton_address)
        {
            std::list <QLineEdit*> get_address = (this->getNewspAddr(it->second));
            std::list <QLineEdit*>::iterator iter;
            for (iter = get_address.begin(); iter != get_address.end(); iter++)
            {
                QHBoxLayout* hlayout = new QHBoxLayout();
                QLabel* newsp = new QLabel();
                QLabel* addr = new QLabel();
                newsp->setText(QString::fromStdString(it->first));
                addr->setText((*iter)->text());
                hlayout->addWidget(newsp);
                hlayout->addWidget(addr);
                ui->vlayout_report->addLayout(hlayout);
            }
        } else if (sender() == ui->sendButton_support)
        {
            QHBoxLayout* hlayout = new QHBoxLayout();
            QLabel* newsp = new QLabel();
            QLabel* np_count = new QLabel();
            newsp->setText(QString::fromStdString(it->first));
            np_count->setText(QString::number(this->getNewspCounter(it->second))
                              + " pcs.");
            hlayout->addWidget(newsp);
            hlayout->addWidget(np_count);
            ui->vlayout_report->addLayout(hlayout);
        }
    }
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

inline void MainWindow::addNewsp(std::string newsp_name, int id)
{
    this->m_ReportNewsp.insert(std::pair<std::string, int> (newsp_name, id));
}

inline int MainWindow::getNewspId(std::string newsp_name) const
{
    std::map<std::string, int>::const_iterator pos = this->m_ReportNewsp.find(newsp_name);
    if (pos == this->m_ReportNewsp.end()) {
        throw std::string("There is no newspaper in the list found!");
    } else {
        int value = pos->second;
        return value;
    }
}

inline void MainWindow::addNewspCounter(int id, int count)
{
    this->m_ReportCounters.insert(std::pair<int, int> (id, count));
}

inline int MainWindow::getNewspCounter(int id) const
{
    return this->m_ReportCounters.find(id)->second;
}

inline void MainWindow::addNewspAddr(int id, QLineEdit* address)
{
    std::map <int, std::list<QLineEdit*>>& table = this->m_ReportAddresses;
    if ( this->m_ReportAddresses.find(id) == this->m_ReportAddresses.end())
    {
        std::list<QLineEdit*> addr_list;
        addr_list.push_back(address);
        table[id] = addr_list;
    }
    else
    {
        std::list<QLineEdit*> addr_to_push;
        addr_to_push = table[id];
        addr_to_push.push_back(address);
        table[id] = addr_to_push;
    }
}

inline std::list<QLineEdit*> MainWindow::getNewspAddr(int id) const
{
    return this->m_ReportAddresses.find(id)->second;
}

inline void MainWindow::clearReportData()
{
    this->m_ReportNewsp.clear();
    this->m_ReportCounters.clear();
    this->m_ReportAddresses.clear();
}
