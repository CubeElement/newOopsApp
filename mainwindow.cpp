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

void MainWindow::moveToSelectorPage()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_button_signin_clicked()
{
    set_staff_id();
    ui->stackedWidget->setCurrentIndex(1);
    ui->label_staff_id->setText(QString("Hello, ") + QString::number(this->m_staff_id));
    make_newsp_list(this->m_num_available_newsp);
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
    QSpacerItem* spacer = new QSpacerItem(100, 100,
                                          QSizePolicy::Expanding,
                                          QSizePolicy::Expanding);
    ui->layout_page1->addSpacerItem(spacer);
}

void MainWindow::on_button_option_proceed_clicked()
{
    this->clearOutput();
    int id = 0;

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
                /* filling an output table */
                this->add_NewspIdOutput(((*key).text()).toStdString(), id);
                this->add_NewspCountOutput(id, (*value).text().toInt());
                id++;
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
                this->add_NewspIdOutput(((*key).text()).toStdString(), id);
                this->add_NewspCountOutput(id, (*value).text().toInt());
                id++;

                QLabel* title = new QLabel();
                title->setText((*key).text());
                ui->layout_newsp_addresses->addWidget(title);
                for ( int j = 0 ; j < (*value).value() ; j++ )
                {
                    QLineEdit* address = new QLineEdit();

                    address->setPlaceholderText("Addr. # " + QString::number(j));
                    address->setText("Addr. # " + QString::number(j));
                    ui->layout_newsp_addresses->addWidget(address);

                    /* filling an output table */
                    this->add_NewspAddrOutput(this->get_NewspIdOutput(((*key).text()).toStdString()), address);
                }
            } //  if end
        } //  while end
    } //  else end
}

void MainWindow::m_send_results()
{
    this->m_map_result();
    ui->stackedWidget->setCurrentIndex(4);

}

void MainWindow::m_map_result()
{

    QLabel* shipping_address_annotation = new QLabel();
    shipping_address_annotation->setText(QString("Your request have been sent \n"));
    ui->vlayout_report->addWidget(shipping_address_annotation);
    if (sender() == ui->sendButton_support)
    {
        QLabel* courier_addr = new QLabel();
        QString* address_from_user = this->getCourierAddress();
        ui->vlayout_report->addWidget(courier_addr);
        courier_addr->setText(QString("Courier's address: ") + address_from_user);
    }
    std::map <std::string, int>::iterator it;
    for ( it = this->m_newspIdOutput.begin(); it != this->m_newspIdOutput.end(); it++ )
    {
        if (sender() == ui->sendButton_address)
        {
            std::list <QLineEdit*> get_address = (this->get_NewspAddrOutput(it->second));
            std::list <QLineEdit*>::iterator iter;
            for (iter = get_address.begin(); iter != get_address.end(); iter++)
            {
                QHBoxLayout* hlayout = new QHBoxLayout();
                QLabel* newsp = new QLabel();
                QLabel* addr = new QLabel();
                newsp->setText(QString::fromStdString(it->first));
                addr->setText(QString((*iter)->text()));
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
            np_count->setText(QString::number(this->get_NewspCountOutput(it->second))
                              + " pcs.");
            hlayout->addWidget(newsp);
            hlayout->addWidget(np_count);
            ui->vlayout_report->addLayout(hlayout);
        }
    }
}

inline QString* MainWindow::getCourierAddress()
{
    QString* address = new QString(ui->lineEdit_address_courier->text());
    return address;
}

inline void MainWindow::add_NewspIdOutput(std::string newsp_name, int id)
{
    this->m_newspIdOutput.insert(std::pair<std::string, int> (newsp_name, id));
}

inline int MainWindow::get_NewspIdOutput(std::string newsp_name)
{
    std::map<std::string, int>::iterator pos = this->m_newspIdOutput.find(newsp_name);
    if (pos == this->m_newspIdOutput.end()) {
        throw std::string("There is no newspaper in the list found!");
    } else {
        int value = pos->second;
        return value;
    }
}

inline void MainWindow::add_NewspCountOutput(int id, int count)
{
    this->m_newspCountOutput.insert(std::pair<int, int> (id, count));
}

inline int MainWindow::get_NewspCountOutput(int id)
{
    return this->m_newspCountOutput[id];
}

inline void MainWindow::add_NewspAddrOutput(int id, QLineEdit* address)
{
    std::map <int, std::list<QLineEdit*>>& table = this->m_newspAddrOutput;
        if ( this->m_newspAddrOutput.find(id) == this->m_newspAddrOutput.end())
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

inline std::list<QLineEdit*> MainWindow::get_NewspAddrOutput(int id)
{
    return this->m_newspAddrOutput[id];
}

inline void MainWindow::clearOutput()
{
    this->m_newspIdOutput.clear();
    this->m_newspCountOutput.clear();
    this->m_newspAddrOutput.clear();
}
