#include "mongomanager.h"

#include <iostream>

namespace rclog {

MongoManager::MongoManager()
{

}

int MongoManager::addDocument(const std::string &doc, const std::string &producerName)
{
    std::cout << "doc: '" << doc << "'\nproducer: '" << producerName << "'" << std::endl;

    // TODO: catch json parse erorr exception: boost::property_tree::json_parser::json_parser_error
//        std::stringstream ss(data_);
//        boost::property_tree::ptree msg_json;
//        boost::property_tree::read_json(ss, msg_json);

//        boost::property_tree::ptree root;
//        root.add_child("message", msg_json);
//        root.put("timestamp", "time_now");      // TODO: add current timestamp
//        root.put("node_id", NODE_ID);           // TODO: should get pid of source proccess somehow

//        std::stringstream sout;
//        boost::property_tree::write_json(sout, root);
//        std::string res_msg = sout.str();
//        std::cout << res_msg << std::endl;

    return 0;
}

}
