#ifndef CBLPLUGIN_H
#define CBLPLUGIN_H

#include "cblinterface.h"
#include "CBLPlugin_global.h"
#include <fstream>

class CBLPLUGIN_EXPORT CBLPlugin : public QObject, public CBLInterface {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.foyatech.miot.CBLPlugin" FILE "cblplugin.json")
    Q_INTERFACES(PluginInterface CBLInterface)

  private:
    bool readFileByLines(const std::string &path, const std::function<bool(FLSlice)> &callback);

  public:
    CBLPlugin();
    ~CBLPlugin();
    cbl::Database getAppDB(const std::string &dbName) const override;
    cbl::Database getAppDB(const std::string &dbName, const std::string &path, const bool &reset = false) const override;
    void createIndex(cbl::Database appDB, const std::string &indexName, const std::string &indexContent, std::string& result) const override;
    void deleteIndex(cbl::Database appDB, const std::string &indexName, std::string& result) const override;
    void saveMutableDocument(cbl::Database appDB, const std::string &content, std::string &uuid, std::string& result) override;
    cbl::ResultSet queryDocuments(const cbl::Database &appDB, const std::string &sqlString, std::string& result) const override;
    void jsonImport(cbl::Database appDB, const std::string &filename, std::string& uuid, std::string& result) override;
    void jsonExport(cbl::Database appDB, const std::string &filename, const std::string &sqlString, std::string& result) override;
    cbl::Replicator getReplicatorBasicPushOnly(
        const cbl::Database &appDB,
        const std::string &url,
        const std::string &uid,
        const std::string &pwd,
        std::string& result,
        const bool &purge = true,
        const std::string &pemFile = "") const override;
    cbl::Replicator getReplicatorBasicPullOnly(
        const cbl::Database &appDB,
        const std::string &url,
        const std::string &uid,
        const std::string &pwd,
        std::string& result,
        const bool &purge = true,
        const std::string &pemFile = "") const override;
    cbl::Replicator getReplicatorBasicPullOnlywFilter(
        const cbl::Database &appDB,
        const std::string &url,
        const std::string &uid,
        const std::string &pwd,
        std::string& result,
        std::function<bool(cbl::Document, CBLDocumentFlags flags)> filter,
        const bool &purge = true,
        const std::string &pemFile = "") const override;
    cbl::Replicator getReplicatorBasicPnP(
        const cbl::Database &appDB,
        const std::string &url,
        const std::string &uid,
        const std::string &pwd,
        std::string& result,
        const bool &purge = true,
        const std::string &pemFile = "") const override;
//    cbl::Replicator getReplicatorSessionPushOnly(
//        const cbl::Database &appDB,
//        const std::string &url,
//        const std::string &sessionID,
//        const std::string &cookieName,
//        std::string& result,
//        const bool &purge = true,
//        const std::string &pemFile = "") const override;
    void setDocExpiration(cbl::Database &appDB,
                          const std::string &id,
                          const int64_t expiration,
                          std::string& result) const override;
    void purgeDocument(cbl::Database &appDB,
                       const std::string &id,
                       std::string& result) const override;
//    cbl::Replicator getReplicatorSessionPullOnly(
//        const cbl::Database &appDB,
//        const std::string &url,
//        const std::string &uid,
//        const std::string &pwd,
//        std::string& result,
//        const std::string &pemFile = "") const override;
    uint64_t syncCounts = 0;
};


#endif // CBLPLUGIN_H
