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
#include <QSet>
#include <QSetIterator>


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
    connect(ui->button_option_proceed, &QPushButton::clicked,
            this, &MainWindow::onButtonOptionProceedClicked );
    connect(ui->button_signin, &QPushButton::clicked,
            this, [=]() { MainWindow::onButtonSigninClicked(
                          ui->line_staff_id->text(),
                          ui->line_password->text() ); } );

    ui->progressBar->setValue(25);
    ui->progressBar->setFormat("1 / 4 Authentication");
};

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::moveToSelectorPage()
{
    ui->stackedWidget->setCurrentIndex(1);
    m_isNotEmptySelection = false;

    ui->progressBar->setValue(50);
    ui->progressBar->setFormat("2 / 4 Selection");
}

void MainWindow::onButtonSigninClicked(QString staff_id, QString password)
{
    if ( db.checkUserData(staff_id, password) )
    {
        this->moveToSelectorPage();
        ui->label_staff_id->setText(QString("Hello, ") +
                                    db.getCourierName());
        addSelectorList(db.getCourierNewspapers());
    } else
    {
        messageBox("Authorization failed");
    }
}

void MainWindow::addSelectorList(const QSet<QString>& units_list)
{
    QSetIterator<QString> it(units_list);
    while ( it.hasNext())
    {
        QString unit_item = it.next();
        QHBoxLayout* p_layout = new QHBoxLayout();
        p_layout->setAlignment(ui->layout_page1, Qt::AlignCenter);

        QLabel* newsp_name = new QLabel(ui->page_1);
        QSpinBox* newsp_count = new QSpinBox(ui->page_1);
        newsp_name->setText( unit_item );
        newsp_count->setValue(0);
        newsp_count->setMaximum( db.getNewspCount(unit_item) );
        QObject::connect(this, &MainWindow::sendSelectionValues,
                         this, [=]() { MainWindow::addWidgetsLists(
                                       newsp_name->text(),
                                       newsp_count->value()); } );
        connect(this, &MainWindow::selectionStatus,
                this, [=]() { MainWindow::setSelectionStatus(newsp_count->value()); } );
        p_layout->addWidget(newsp_name);
        p_layout->addWidget(newsp_count);
        ui->layout_page1->addLayout(p_layout);
    }
    QSpacerItem* spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding,
                                                QSizePolicy::Expanding);
    ui->layout_page1->addSpacerItem(spacer);
}
void MainWindow::setSelectionStatus(int val)
{
    if ( val != 0 )
    {
        this->m_isNotEmptySelection = true;
    }
}

void MainWindow::onButtonOptionProceedClicked()
{
    QLayoutItem *child;
    while ((child = ui->layout_newsp_addresses->takeAt(0)) != 0) {
        delete child->widget();
        delete child;
    }
    while (ui->page_2_list_newsp->count() != 0) {
        QListWidgetItem *child2 = ui->page_2_list_newsp->takeItem(0);
        delete child2;
    }
    emit selectionStatus();
    if ( this->m_isNotEmptySelection )
    {
        if ( ui->radio_address->isChecked() )
        {
            ui->stackedWidget->setCurrentIndex(2);
            ui->lineEdit_address_courier->setText(db.getCourierPlace());
            emit sendSelectionValues();
        }
        else if ( ui->radio_delivery->isChecked() )
        {
            ui->stackedWidget->setCurrentIndex(3);
            emit sendSelectionValues();
            QSpacerItem* spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding,
                                                        QSizePolicy::Expanding);
            ui->layout_newsp_addresses->addSpacerItem(spacer);
        }
    } else
    {
        messageBox("Selection list should not be empty");
    }

    ui->progressBar->setValue(75);
    ui->progressBar->setFormat("3 / 4 Input");
}

void MainWindow::addWidgetsLists(QString name, int count)
{
    QString count_str = QString::number(count);
    if ( ui->stackedWidget->currentIndex() == 2 && count != 0)
    {
        ui->page_2_list_newsp->addItem(name + " " + count_str + " pcs.");
        QObject::connect(this, &MainWindow::sendReportSingleAddr,
                         this, [=]() { MainWindow::showReport(
                                       name,
                                       count_str + " pcs."); } );
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
            QObject::connect(this, &MainWindow::sendReportMultiAddr,
                             this, [=]() { MainWindow::showReport(
                                           name,
                                           address->text()); } );
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
        courier_addr->setText(QString("Courier's address:\n") + address_from_user);
        ui->vlayout_report->addWidget(courier_addr);
        emit sendReportSingleAddr();
    }
    else
    {
        emit sendReportMultiAddr();
    }

    QSpacerItem* spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding,
                                                QSizePolicy::Expanding);
    ui->vlayout_report->addSpacerItem(spacer);
}

void MainWindow::showReport(const QString& name, const QString& info_value)
{
    QHBoxLayout* hlayout = new QHBoxLayout();
    hlayout->setSizeConstraint(QLayout::SetNoConstraint);
    QLabel* newsp = new QLabel();
    QLabel* info = new QLabel();
    newsp->setText(name);
    info->setText(info_value);
    newsp->setGeometry(50, 10, 0, 0);
    newsp->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
    hlayout->addWidget(newsp);
    hlayout->addWidget(info);
    ui->vlayout_report->addLayout(hlayout);

    ui->progressBar->setValue(100);
    ui->progressBar->setFormat("4 / 4 Results");
}

inline QString MainWindow::getCourierAddress()
{
    QString address = ui->lineEdit_address_courier->text();
    return address;
}

void MainWindow::messageBox(std::string message_text)
{
    QMessageBox message;
    message.setText(QString::fromStdString(message_text));
    message.setIcon(QMessageBox::Warning);
    message.setSizeIncrement(120, 40);
    message.exec();
}

void MainWindow::on_report_page_finish_clicked()
{
    this->close();
}

