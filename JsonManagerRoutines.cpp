#include "JsonManagerRoutines.h"


JsonManager::JsonManager(const string& json_str) {
    

    try {
        json j = json::parse(json_str);
        // Legge i valori dei campi dell'oggetto JSON e li salva in variabili membro della classe
        name_ = j["Name"];
        status_ = j["Status"];
        only_on_change_ = j["Only_On_Change"];
        condition_id_ = j["Condition"]["Id"];
        condition_msg_ = j["Condition"]["Message"];
        condition_operator_ = j["Condition"]["Operator"];
        condition_command_ = j["Condition"]["Value"];
        actions_id_ = j["Action"]["Id"];
        actions_command_ = j["Action"]["Command"];

        // Legge l'array di stringhe dal campo "param" dell'oggetto JSON, se presente
        if (j["Action"].find("Function_Parameters") != j["Action"].end())
        {
            // Verifica se il valore associato al campo "param" è un array di stringhe
            if (j["Action"]["Function_Parameters"].is_array())
            {
                // Legge l'array di stringhe dal campo "param" e lo inserisce nel vettore "actions_param_"
                actions_param_.insert(actions_param_.end(), j["Action"]["Function_Parameters"].begin(), j["Action"]["Function_Parameters"].end());
            }
        }
    }
    catch (json::parse_error) {
        return;
    }
}

// Restituisce il valore del campo "name"
string JsonManager::getName() const {
    return name_;
}

// Restituisce il valore del campo "status"
string JsonManager::getStatus() const {
    return status_;
}

// Restituisce il valore del campo "condition.id"
string JsonManager::getConditionId() const {
    return condition_id_;
}

// Restituisce il valore del campo "condition.msg"
string JsonManager::getConditionMsg() const {
    return condition_msg_;
}

string JsonManager::getOnlyOnChange() const {
    return only_on_change_;
}

// Restituisce il valore del campo "condition.operator"
string JsonManager::getConditionOperator() const {
    return condition_operator_;
}

// Restituisce il valore del campo "condition.command"
string JsonManager::getConditionCommand() const {
    return condition_command_;
}

// Restituisce il valore del campo "actions.id"
string JsonManager::getActionsId() const {
    return actions_id_;
}

// Restituisce il valore del campo "actions.command"
string JsonManager::getActionsCommand() const {
    return actions_command_;
}

// Restituisce il vettore di stringhe contenente i valori del campo "actions.param"
vector<string> JsonManager::getActionsParameters() const {
    return actions_param_;
}