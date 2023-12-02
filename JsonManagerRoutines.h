/*
* Author: Andrea Spina
* FirstB: 01/04/2023
* Lenguage: c++
* Descrition: This program creates an MQTT client that allows you to add management of Shelly devices present on the MQTT broker.
* It supports the creation and deletion of devices, as well as the creation, modification, and deletion of 1-to-1 routines.
* With this program, you can easily manage your Shelly devices from a single interface and automate their behavior with routines.
*/
#ifndef JSONMANAGER_H
#define JSONMANAGER_H

#include "json.hpp"
#include <iostream>

using json = nlohmann::json;
using namespace std;

class JsonManager {
public:
    JsonManager(const string& json_str);

    string getName() const;
    string getStatus() const;
    string getOnlyOnChange() const;
    string getConditionId() const;
    string getConditionOperator() const;
    string getConditionCommand() const;
    string getActionsId() const;
    string getActionsCommand() const;
    string getConditionMsg() const;
    vector<string> getActionsParameters() const; //ritorna il contenuto di parametri
    

private:
    string name_;
    string status_;
    string only_on_change_;
    string condition_id_;
    string condition_operator_;
    string condition_command_;
    string actions_id_;
    string actions_command_;
    string condition_msg_;
    vector<string> actions_param_;
};

#endif // JSON_MANAGER_H