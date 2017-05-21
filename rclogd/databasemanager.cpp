#include "databasemanager.h"

namespace rclog {


DatabaseManager::DatabaseManager(const std::string &node_id)
{
    node_id_ = node_id;

    std::cerr << "Node ID is '" << node_id << "'" << std::endl;
}

}
