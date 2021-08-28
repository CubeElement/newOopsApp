#ifndef DATABASEJSON_H
#define DATABASEJSON_H

#include "nlohmann/json.hpp"
#include <QString>
#include <QObject>
#include <QThread>

using json = nlohmann::json;

class DatabaseJSON : public QThread
{

public:
    DatabaseJSON();

    void openJSON();
    json m_JSON;

    void getDistrictID();
    int getStaffID();
    int setStaffID();
    bool checkUserData(QString& StaffID);
    void printLoginData(nlohmann::json);

public slots:


private:
    int m_StaffID;
};

#endif // DATABASEJSON_H
