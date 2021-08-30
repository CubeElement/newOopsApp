#include "databasejson.h"

#include <iostream>
#include <fstream>
#include <QDebug>
#include <stdexcept>
#include <map>


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

bool DatabaseJSON::checkUserData(QString& StaffID, QString& Password)
{
    std::string StaffID_str = StaffID.toStdString();
    std::string Password_str = Password.toStdString();
    json::iterator iter;
    iter = this->m_JSON["Zusteller"].find(StaffID_str);
    if ( iter != this->m_JSON["Zusteller"].end()
         && this->checkPassword(StaffID_str, Password_str))
    {
        initCourierInfo(StaffID_str);
        return true;
    }
    else
    {
        return false;
    }
}

bool DatabaseJSON::checkPassword(std::string StaffID, std::string Password)
{
    const std::map<std::string, std::string> passwords =
    {
        {"1001", "1001"},
        {"1052", "1052"}
    };
    std::map<std::string, std::string>::const_iterator iter;
    iter = passwords.find(StaffID);
    if ( iter != passwords.end() && iter->second == Password )
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

void DatabaseJSON::initCourierInfo(std::string StaffID)
{
    this->m_StaffID = StaffID;
    this->m_name = this->m_JSON["Zusteller"][StaffID]["Name"];
    this->m_lastname = this->m_JSON["Zusteller"][StaffID]["Nachname"];
    this->m_courier_district = this->m_JSON["Zusteller"][StaffID]["Bezirk"];
    this->m_courier_address = this->m_JSON["Zusteller"][StaffID]["Abladestelle"];
    json subscribers = this->m_JSON["BezNummer"][this->m_courier_district];
    for (const auto &sub : subscribers)
    {
        this->m_courier_newspapers.insert(sub["Obj./Aus."].get<std::string>());
        std::string city = sub["Ort"].get<std::string>();
        std::string street = sub["Strasse"].get<std::string>();
        std::string house = std::to_string(sub["Hausnr"].get<int>());
        this->m_subscriber_addresses.insert(street + ", " +
                                            house + " " + city);
    }
}

std::string DatabaseJSON::getCourierName()
{
    return this->m_name;
}

std::string DatabaseJSON::getCourierDistrict()
{
    return this->m_courier_district;
}

std::set<std::string> DatabaseJSON::getCourierNewspapers()
{
    return this->m_courier_newspapers;
}

std::string DatabaseJSON::getCourierPlace()
{
    return this->m_courier_address;
}

QStringList DatabaseJSON::getSubscriberAddresses()
{
    QStringList subscr_qstringlist;
    std::set<std::string> subscr_set = this->m_subscriber_addresses;
    for ( auto& elem : subscr_set )
    {
        subscr_qstringlist << QString::fromStdString(elem);
    }
    return subscr_qstringlist;
}
