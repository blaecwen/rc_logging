#include "mongomanager.h"

namespace rclog {

MongoManager::MongoManager(const std::string &node_id)
    :DatabaseManager(node_id),
     client(mongocxx::uri{})
{
    std::cerr << "Connected to mongoDB database" << std::endl;
}

int MongoManager::addDocument(const std::string &document, const std::string &producerName)
{
    try {
        bsoncxx::document::value doc = bsoncxx::from_json(document);
        DOUT << "Parsed doc: " << bsoncxx::to_json(doc.view()) << std::endl;

        using bsoncxx::builder::stream::document;
        using bsoncxx::builder::stream::finalize;

        // NOTE: here copy occures. So, if doc is big enough -- it will be slow
        auto new_doc = document{} << "storage_timestamp" << "TODO" << "message" << doc << finalize;

        // NOTE: is it efficient? may be it is better to save mongocxx::collection handle in members of the class
        mongocxx::collection collection = client["rclogd"]["local"];
        collection.insert_one(new_doc.view());
    } catch (bsoncxx::exception e) {
        std::cerr << "Error: while adding document from producer '" << producerName << "': " << e.what() << std::endl;
        return 1;
    } catch (mongocxx::bulk_write_exception e) {
        std::cerr << "Error: while adding document from producer '" << producerName << "': " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

}
