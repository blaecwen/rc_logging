#include "databasemanager.h"

namespace rclog {


DatabaseManager::DatabaseManager(const std::string &node_id)
{
    // NOTE: verify that node_id has correct format! Otherwise print warning
    node_id_ = node_id;

    std::cerr << "Node ID is '" << node_id << "'" << std::endl;
}

}
