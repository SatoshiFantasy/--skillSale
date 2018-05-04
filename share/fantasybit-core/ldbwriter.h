#ifndef LDBWRITER_H
#define LDBWRITER_H

#include <leveldb/db.h>
//#include <qdebug>
#include <utils.h>
#include <string>
#include <StateData.pb.h>

namespace fantasybit {

class LdbWriter {
    leveldb::DB *db;
    leveldb::WriteOptions write_sync{};

public:

    void init(leveldb::DB * = nullptr);


    std::string write(const std::string &key, const std::string &val) {
        //qDebug() << key.data() << val.data();
        if (db->Put(write_sync,key,val).ok())
            return key;
        else return "";
    }
    std::string write(const std::string &val) {
        auto ret = pb::hashit(val).str();
        write(ret,val);
        return ret;
    }

    std::string write(const GOOGLE_NAMESPACE::protobuf::Message &msg) {
        return write(msg.SerializeAsString());
    }

    std::string write(const fantasybit::MerkleTree &tree) {
        return write(tree.root(),tree.SerializeAsString());
    }

    std::string write(const fantasybit::KeyValue &kv) {
        return write(kv.key(),kv.value());
    }


    std::string read(const std::string &id) ;

    /**
     * @brief read and parse protobuf
     * @param id
     * @param msg
     * @return
     */
    std::string read(const std::string &id,
                     GOOGLE_NAMESPACE::protobuf::Message &msg);

    ~LdbWriter() {
//        if ( db ) delete db;
    }
};

}

#endif // LDBWRITER_H

