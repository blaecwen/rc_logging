#ifndef MONGOMANAGER_H
#define MONGOMANAGER_H

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/exception/exception.hpp>

#include <mongocxx/exception/bulk_write_exception.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

#include <iostream>

#include "databasemanager.h"
#include "utils.hpp"

namespace rclog {

// TODO: somehow restrict creating other instances of the class!
class MongoManager : public DatabaseManager
{
public:
    MongoManager();

    int addDocument(const std::string &document, const std::string &producerName) override;

private:

    mongocxx::instance mongoInstance;   // NOTE: A unique instance of the Mongocxx driver MUST be kept around
    mongocxx::client client;
};

}

#endif // MONGOMANAGER_H
