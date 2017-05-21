#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <string>
#include <iostream>

namespace rclog {

class DatabaseManager
{
public:
    DatabaseManager() = delete;
    DatabaseManager(const std::string& node_id);

    virtual int addDocument(const std::string& doc, const std::string& producerName) = 0;

protected:
    std::string node_id_;
};

}


#endif // DATABASEMANAGER_H
