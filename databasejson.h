#ifndef DATABASEJSON_H
#define DATABASEJSON_H

#include "nlohmann/json.hpp"

using json = nlohmann::json;

class DatabaseJSON
{
public:
    DatabaseJSON();

    void init();
};

#endif // DATABASEJSON_H
