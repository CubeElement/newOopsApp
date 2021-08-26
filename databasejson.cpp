#include "databasejson.h"

#include <iostream>
#include <fstream>

DatabaseJSON::DatabaseJSON()
{
    this->init();
}

void DatabaseJSON::init()
{
    // testing json input/output
    json j;			// Create a JSON object
    std::ifstream jfile("database/database.json");
    jfile >> j;		// read the JSON file form in the form of file flow
    //bool happy = j.at("happy");
    // Print the values
    std::cout << "Department: " << j["Zusteller"]["1001"] << std::endl;
}
