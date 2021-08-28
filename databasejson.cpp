#include "databasejson.h"

#include <iostream>
#include <fstream>
#include <QDebug>
#include <stdexcept>


DatabaseJSON::DatabaseJSON()
{
    this->openJSON();
    this->getDistrictID();
}

void DatabaseJSON::openJSON()
{
    json j;  // Create a JSON object
    std::ifstream jfile("database/database.json");
    if ( jfile.is_open() )
    {
        jfile >> j;  // read the JSON file form in the form of file flow
//        std::cout << "File content: " << j;
        this->m_JSON = j;
    } else
    {
        throw std::string("File not found!");
    }
}

void DatabaseJSON::getDistrictID()
{

}

bool DatabaseJSON::checkUserData(QString& StaffID)
{
//    this->printLoginData(this->m_JSON["Zusteller"][StaffID.toStdString()]);
    qDebug() << "StaffID: " << StaffID;
    json::iterator iter;
    iter = this->m_JSON["Zusteller"].find(StaffID.toStdString());
    if ( iter != this->m_JSON["Zusteller"].end() )
    {
        return true;
    }
    else
    {
        return false;
    }
}

void DatabaseJSON::printLoginData(nlohmann::json infdata)
{
    std::cout << infdata << std::endl;
}
