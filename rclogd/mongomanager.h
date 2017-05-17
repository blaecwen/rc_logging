#ifndef MONGOMANAGER_H
#define MONGOMANAGER_H

#include "databasemanager.h"


namespace rclog {

class MongoManager : public DatabaseManager
{
public:
    MongoManager();

    int addDocument(const std::string &doc, const std::string &producerName) override;
};

}

#endif // MONGOMANAGER_H
