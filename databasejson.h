#ifndef DATABASEJSON_H
#define DATABASEJSON_H

#include "nlohmann/json.hpp"
#include <QString>
#include <QObject>
#include <QThread>
#include <vector>
#include <set>

using json = nlohmann::json;

class DatabaseJSON : public QObject
{

public:
    DatabaseJSON();

    void openJSON();
    json m_JSON;

    void getDistrictID();
    int getStaffID();
    bool checkUserData(QString& StaffID, QString& Password);
    void printLoginData(nlohmann::json);
    void initCourierInfo(std::string StaffID);
    std::string getCourierName();
    std::string getCourierDistrict();
    std::string getCourierPlace();
    std::set<std::string> getCourierNewspapers();
    QStringList getSubscriberAddresses();

public slots:


private:
    std::string m_StaffID;
    std::string m_name;
    std::string m_lastname;
    std::string m_courier_district;
    std::string m_courier_address;
    std::set<std::string> m_courier_newspapers;
    std::set<std::string> m_subscriber_addresses;
    bool checkPassword(std::string, std::string);

};

#endif // DATABASEJSON_H
