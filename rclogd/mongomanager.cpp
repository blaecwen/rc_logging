#include "mongomanager.h"

namespace rclog {

MongoManager::MongoManager()
    :client(mongocxx::uri{})
{
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
