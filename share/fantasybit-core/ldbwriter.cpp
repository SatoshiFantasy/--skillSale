#include "ldbwriter.h"

using namespace std;
namespace fantasybit {

void LdbWriter::init(leveldb::DB * db) {

    write_sync.sync = true;
    this->db = db;

}

std::string LdbWriter::read(const std::string &id) {
    string value;
    db->Get(leveldb::ReadOptions(),id,&value);
    return value;
}

std::string LdbWriter::read(const std::string &id, GOOGLE_NAMESPACE::protobuf::Message &msg) {
    auto str = read(id);
    msg.ParseFromString(str);
    return str;
}

}
