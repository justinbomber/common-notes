/*************************************************************************************************
 * Date        Version  Description                                                 modified-By
 * 2023/07/06  0.1.0    change cbl-lite library from 3.0.2 to 3.1                   lunglin
 *                      for replicator timeout & can not reconnect to sync gateway
 *
 *************************************************************************************************/

#include "cblplugin.h"
#include <QUuid>
#include <chrono>

using slice = cbl::slice;

CBLPlugin::CBLPlugin()
{
}

CBLPlugin::~CBLPlugin()
{
}

cbl::Database CBLPlugin::getAppDB(const std::string &dbName) const
{
    return cbl::Database(slice(dbName));
}

cbl::Database CBLPlugin::getAppDB(const std::string &dbName, const std::string &path, const bool &reset) const
{
    CBLDatabaseConfiguration config;
    config.directory = FLString(slice(path));
    if (reset && cbl::Database::exists(slice(dbName), slice(path)))
        cbl::Database::deleteDatabase(slice(dbName), slice(path));
    return cbl::Database(slice(dbName), config);
}

void CBLPlugin::createIndex(cbl::Database appDB, const std::string &indexName, const std::string &indexContent, std::string &result) const
{
    result = "IP100";
    try
    {
        CBLValueIndexConfiguration index = {};
        index.expressionLanguage = kCBLN1QLLanguage;
        index.expressions = slice(indexContent);
        appDB.createValueIndex(slice(indexName), index);
    }
    catch (CBLError &err)
    {
        fleece::alloc_slice message = CBLError_Message(&err);
        result = "FP101: createIndex error: " + message.asString();
    }
}

void CBLPlugin::deleteIndex(cbl::Database appDB, const std::string &indexName, std::string &result) const
{
    result = "IP100";
    try
    {
        appDB.deleteIndex(slice(indexName));
    }
    catch (CBLError &err)
    {
        fleece::alloc_slice message = CBLError_Message(&err);
        result = "FP102: deleteIndex error: " + message.asString();
    }
}

void CBLPlugin::saveMutableDocument(cbl::Database appDB, const std::string &content, std::string &uuid, std::string &result)
{
    result = "IP200";
    try
    {
        if (uuid.empty())
            uuid = QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString();
        cbl::MutableDocument doc(slice(uuid.c_str()));
        doc.propertiesAsJSON();
        doc.setPropertiesAsJSON(slice(content));
        appDB.saveDocument(doc);
    }
    catch (CBLError &err)
    {
        fleece::alloc_slice message = CBLError_Message(&err);
        result = "FP201: saveMutableDocument error: " + message.asString();
    }
}

cbl::ResultSet CBLPlugin::queryDocuments(const cbl::Database &appDB, const std::string &sqlString, std::string &result) const
{
    result = "IP200";
    try
    {
        cbl::Query query(appDB, kCBLN1QLLanguage, slice(sqlString));
//        qInfo() << "explain_plan = " << query.explain().c_str();
//        std::cout << "explain_plan = " << query.explain() << "\n";
        return query.execute();
    }
    catch (CBLError &err)
    {
        fleece::alloc_slice message = CBLError_Message(&err);
        result = "FP202: queryDocuments error: " + message.asString();
        return cbl::ResultSet{};
    }
}

bool CBLPlugin::readFileByLines(const std::string &path, const std::function<bool(FLSlice)> &callback)
{
    std::fstream fd(path.c_str(), std::ios_base::in);
    std::vector<char> buf(1000000); // The Wikipedia dumps have verrry long lines
    if (fd.fail())
        return false;

    while (fd.good())
    {
        fd.getline(buf.data(), buf.size());
        auto len = fd.gcount();
        if (len <= 0)
            break;
        --len;
        if (!callback({buf.data(), (size_t)len}))
            return false;
    }
    fd.close();
    return true;
}

void CBLPlugin::jsonImport(cbl::Database appDB, const std::string &filename, std::string &uuid, std::string &result)
{
    int count = 0;
    result = "IP200";
    uuid = "";
    cbl::Transaction trans(appDB);
    try
    {
        readFileByLines(filename, [&](FLSlice line)
                        {
            saveMutableDocument(appDB, std::string((char*)line.buf, line.size), uuid, result);
            if(result != "IP200")
                return false;
            count++;
            return true; });
        trans.commit();
    }
    catch (CBLError &err)
    {
        fleece::alloc_slice message = CBLError_Message(&err);
        result = "FP202: jsonImport error " + message.asString();
        trans.abort();
    }
}

void CBLPlugin::jsonExport(cbl::Database appDB, const std::string &filename, const std::string &sqlString, std::string &result)
{
    int count = 0;
    std::fstream fd(filename.c_str(), std::ios_base::out);
    if (fd.fail())
    {
        result = "FP301: jsonExport error = Open file error!";
        return;
    }
    try
    {
        cbl::ResultSet results = queryDocuments(appDB, sqlString, result);
        for (auto result : results)
        {
            std::string tmp = result.toJSON().asString() + "\n";
            fd.write(tmp.c_str(), result.toJSON().size + 1);
            count++;
        }
        fd.flush();
        fd.close();
        result = "IP300";
    }
    catch (CBLError &err)
    {
        fleece::alloc_slice message = CBLError_Message(&err);
        result = "FP204: jsonExport error = " + message.asString();
        fd.close();
    }
    catch (std::exception &err)
    {
        result = "FP301: jsonExport error," + std::string(err.what());
        fd.close();
    }
}

cbl::Replicator CBLPlugin::getReplicatorBasicPushOnly(
    const cbl::Database &appDB,
    const std::string &url,
    const std::string &uid,
    const std::string &pwd,
    std::string &result,
    const bool &purge,
    const std::string &pemFile) const
{
    result = "IP100";
    // modified by lunglin 2023/07/06
//    cbl::ReplicatorConfiguration repConfig{appDB};
    cbl::ReplicatorConfiguration repConfig{appDB, cbl::Endpoint::urlEndpoint(slice(url))};
    // modified by lunglin 2023/07/06
//    repConfig.endpoint.setURL(slice(url));
    // ToDo: pwd decryption
    // 2023/07/06 modified by lunglin for testing
//    repConfig.authenticator.setBasic(slice(uid), slice(pwd));
    repConfig.authenticator = cbl::Authenticator::basicAuthenticator(slice(uid), slice(pwd));
    repConfig.enableAutoPurge = purge;
    repConfig.replicatorType = kCBLReplicatorTypePush;
    repConfig.continuous = true;
    try
    {
        if (!pemFile.empty())
        {
            char cert_buf[10000];
            FILE *cert_file = std::fopen(pemFile.c_str(), "r");
            size_t read = std::fread(cert_buf, 1, sizeof(cert_buf), cert_file);
            if (read > 0)
                repConfig.pinnedServerCertificate.append(cert_buf);
        }
        return cbl::Replicator{repConfig};
    }
    catch (CBLError &err)
    {
        fleece::alloc_slice message = CBLError_Message(&err);
        result = "FP204: getReplicatorBasicPushOnly error: " + message.asString();
        return cbl::Replicator{};
    }
}

cbl::Replicator CBLPlugin::getReplicatorBasicPullOnly(
    const cbl::Database &appDB,
    const std::string &url,
    const std::string &uid,
    const std::string &pwd,
    std::string &result,
    const bool &purge,
    const std::string &pemFile) const
{
    result = "IP100";
    // modified by lunglin 2023/07/06
//    cbl::ReplicatorConfiguration repConfig{appDB};
    cbl::ReplicatorConfiguration repConfig{appDB, cbl::Endpoint::urlEndpoint(slice(url))};
    // modified by lunglin 2023/07/06
//    repConfig.endpoint.setURL(slice(url));
//    qInfo() << "======== CBL::Database::Pull() ::::::: " << repConfig.database.name().c_str();
    // ToDo: pwd decryption
    // modified by lunglin 2023/07/06
//    repConfig.authenticator.setBasic(slice(uid), slice(pwd));
    repConfig.authenticator = cbl::Authenticator::basicAuthenticator(slice(uid), slice(pwd));
    repConfig.enableAutoPurge = purge;
    repConfig.replicatorType = kCBLReplicatorTypePull;
    repConfig.continuous = true;
    try
    {
        if (!pemFile.empty())
        {
            char cert_buf[10000];
            FILE *cert_file = std::fopen(pemFile.c_str(), "r");
            size_t read = std::fread(cert_buf, 1, sizeof(cert_buf), cert_file);
            if (read > 0)
                repConfig.pinnedServerCertificate.append(cert_buf);
        }
        return cbl::Replicator{repConfig};
    }
    catch (CBLError &err)
    {
        fleece::alloc_slice message = CBLError_Message(&err);
        result = "FP204: getReplicatorBasicPullOnly error: " + message.asString();
        return cbl::Replicator{};
    }
}

//static bool filter(cbl::Document document, CBLDocumentFlags flags)
//{
//    if (flags == kCBLDocumentFlagsDeleted || flags == kCBLDocumentFlagsAccessRemoved)
//        return false;

//    qDebug() << "data_source = " << document.properties().asDict().get("data_source").asstring().c_str();
//    qDebug() << "Current time:" << std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
//    qDebug() << "Source timestamp:" << document.properties().get("source_timestamp").asDict().get("sec").asUnsigned();
//    qDebug() << "Diff times:" << (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count() -
//                         document.properties().get("source_timestamp").asDict().get("sec").asUnsigned());
//    if ((document.properties().asDict().get("data_source").asstring() == "NumericVisibility" ||
//         document.properties().asDict().get("data_source").asstring() == "Observation" ||
//         document.properties().asDict().get("data_source").asstring() == "RTObservation") &&
//        (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count() -
//         document.properties().get("source_timestamp").asDict().get("sec").asUnsigned()) < 24 * 60 * 60)
//    {
//        return true;
//    }
//    return false;
//}

cbl::Replicator CBLPlugin::getReplicatorBasicPullOnlywFilter(
    const cbl::Database &appDB,
    const std::string &url,
    const std::string &uid,
    const std::string &pwd,
    std::string &result,
    std::function<bool(cbl::Document, CBLDocumentFlags flags)> filter,
    const bool &purge,
    const std::string &pemFile) const
{
    result = "IP100";
    // modified by lunglin 2023/07/06
//    cbl::ReplicatorConfiguration repConfig{appDB};
    cbl::ReplicatorConfiguration repConfig{appDB, cbl::Endpoint::urlEndpoint(slice(url))};
    // modified by lunglin 2023/07/06
//    repConfig.endpoint.setURL(slice(url));
//    qInfo() << "======== CBL::Database::Pull() ::::::: " << repConfig.database.name().c_str();
    // ToDo: pwd decryption
    // modified by lunglin 2023/07/06
//    repConfig.authenticator.setBasic(slice(uid), slice(pwd));
    repConfig.authenticator = cbl::Authenticator::basicAuthenticator(slice(uid), slice(pwd));
    repConfig.enableAutoPurge = purge;
    repConfig.replicatorType = kCBLReplicatorTypePull;
    repConfig.continuous = true;
    repConfig.pullFilter = filter;
    try
    {
        if (!pemFile.empty())
        {
            char cert_buf[10000];
            FILE *cert_file = std::fopen(pemFile.c_str(), "r");
            size_t read = std::fread(cert_buf, 1, sizeof(cert_buf), cert_file);
            if (read > 0)
                repConfig.pinnedServerCertificate.append(cert_buf);
        }
        return cbl::Replicator{repConfig};
    }
    catch (CBLError &err)
    {
        fleece::alloc_slice message = CBLError_Message(&err);
        result = "FP204: getReplicatorBasicPullOnly error: " + message.asString();
        return cbl::Replicator{};
    }
}

cbl::Replicator CBLPlugin::getReplicatorBasicPnP(
    const cbl::Database &appDB,
    const std::string &url,
    const std::string &uid,
    const std::string &pwd,
    std::string &result,
    const bool &purge,
    const std::string &pemFile) const
{
    result = "IP100";
    // modified by lunglin 2023/07/06
//    cbl::ReplicatorConfiguration repConfig{appDB};
    cbl::ReplicatorConfiguration repConfig{appDB, cbl::Endpoint::urlEndpoint(slice(url))};
    // modified by lunglin 2023/07/06
//    repConfig.endpoint.setURL(slice(url));
    // ToDo: pwd decryption
    // modified by lunglin 2023/07/06
//    repConfig.authenticator.setBasic(slice(uid), slice(pwd));
    repConfig.authenticator = cbl::Authenticator::basicAuthenticator(slice(uid), slice(pwd));
    repConfig.enableAutoPurge = purge;
    repConfig.replicatorType = kCBLReplicatorTypePushAndPull;
    repConfig.continuous = true;
    try
    {
        if (!pemFile.empty())
        {
            char cert_buf[10000];
            FILE *cert_file = std::fopen(pemFile.c_str(), "r");
            size_t read = std::fread(cert_buf, 1, sizeof(cert_buf), cert_file);
            if (read > 0)
                repConfig.pinnedServerCertificate.append(cert_buf);
        }
        return cbl::Replicator{repConfig};
    }
    catch (CBLError &err)
    {
        fleece::alloc_slice message = CBLError_Message(&err);
        result = "FP204: getReplicatorBasicPullOnly error: " + message.asString();
        return cbl::Replicator{};
    }
}

// cbl::Replicator CBLPlugin::getReplicatorSessionPushOnly(const cbl::Database &appDB,
//         const std::string &url,
//         const std::string &sessionID,
//         const std::string &cookieName,
//         std::string &result,
//         const bool &purge,
//         const std::string &pemFile) const {
//     result = "IP100";
//     cbl::ReplicatorConfiguration repConfig{appDB};
//     repConfig.endpoint.setURL(slice(url));
//     repConfig.authenticator.setSession(slice(sessionID), slice(cookieName));
//     repConfig.enableAutoPurge = purge;
//     repConfig.replicatorType = kCBLReplicatorTypePush;
//     repConfig.continuous = true;
//     // ToDo: connect to TWCC DB by pem file
//     //    repConfig.headers
//     try {
//         if(!pemFile.empty()) {
//             char cert_buf[10000];
//             FILE* cert_file = std::fopen(pemFile.c_str(), "r");
//             size_t read = std::fread(cert_buf, 1, sizeof(cert_buf), cert_file);
//             if(read > 0)
//                 repConfig.pinnedServerCertificate.append(cert_buf);
//         }
//         return cbl::Replicator{repConfig};
//     } catch(CBLError &err) {
//         fleece::alloc_slice message = CBLError_Message(&err);
//         result = "FP204: getReplicatorSessionPushOnly error: " + message.asString();
//         return cbl::Replicator{};
//     }
// }

void CBLPlugin::setDocExpiration(cbl::Database &appDB,
                                 const std::string &id,
                                 const int64_t expiration,
                                 std::string &result) const
{
    result = "IP200";
    try
    {
        appDB.setDocumentExpiration(slice(id), expiration);
    }
    catch (CBLError &err)
    {
        fleece::alloc_slice message = CBLError_Message(&err);
        result = "FP204: setDocEpiration error: " + message.asString();
    }
}

void CBLPlugin::purgeDocument(cbl::Database &appDB,
                              const std::string &id,
                              std::string &result) const
{
    result = "IP200";
    try
    {
        appDB.purgeDocument(slice(id));
    }
    catch (CBLError &err)
    {
        fleece::alloc_slice message = CBLError_Message(&err);
        result = "FP204: purgeDocument error: " + message.asString();
    }
}

// cbl::Replicator CBLPlugin::getReplicatorSessionPullOnly(const cbl::Database &appDB,
//         const std::string &url,
//         const std::string &sessionID,
//         const std::string &cookieName,
//         std::string &result,
//         const std::string &pemFile) const {
//     result = "IP100";
//     cbl::ReplicatorConfiguration repConfig{appDB};
//     repConfig.endpoint.setURL(slice(url));
//     repConfig.authenticator.setSession(slice(sessionID), slice(cookieName));
//     repConfig.enableAutoPurge = false;
//     repConfig.replicatorType = kCBLReplicatorTypePull;
//     repConfig.continuous = true;
//     // ToDo: connect to TWCC DB by pem file
//     //    repConfig.headers
//     try {
//         if(!pemFile.empty()) {
//             char cert_buf[10000];
//             FILE* cert_file = std::fopen(pemFile.c_str(), "r");
//             size_t read = std::fread(cert_buf, 1, sizeof(cert_buf), cert_file);
//             if(read > 0)
//                 repConfig.pinnedServerCertificate.append(cert_buf);
//         }
//         return cbl::Replicator{repConfig};
//     } catch(CBLError &err) {
//         fleece::alloc_slice message = CBLError_Message(&err);
//         result = "FP204: getReplicatorSessionPullOnly error: " + message.asString();
//         return cbl::Replicator{};
//     }
// }
