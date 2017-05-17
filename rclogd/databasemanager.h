#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <string>

namespace rclog {

class DatabaseManager
{
public:
    DatabaseManager();

    virtual int addDocument(const std::string& doc, const std::string& producerName) = 0;

};

}


#endif // DATABASEMANAGER_H
