#ifndef CBLINTERFACE_H
#define CBLINTERFACE_H
#include "plugininterface.h"
#include "cbl++/CouchbaseLite.hh"
#include "cbl/CBLBase.h"
#include "fleece/slice.hh"
#include <QUuid>
#include <iostream>
#include <functional>

#define CBLInterface_iid "com.foyatech.miot.CBLInterface"

class CBLInterface : public PluginInterface {
  public:
    virtual ~CBLInterface() {}

    /**
    * @brief getAppDB 取得現有或新增資料庫
    * @param dbName 資料庫名稱
    * @return 資料庫參考
    */
    virtual cbl::Database getAppDB(const std::string &dbName) const = 0;

    /**
     * @brief getAppDB 取得現有或新增資料庫
     * @param dbName 資料庫名稱
     * @param path 資料庫檔案路徑
     * @param reset true:重開新資料庫檔案 false:延用已存在資料庫檔案
     * @return 資料庫參考
     */
    virtual cbl::Database getAppDB(const std::string &dbName, const std::string &path, const bool &reset) const = 0;

    /**
    * @brief createIndex 新建資料庫index
    * @param appDB 資料庫物件參考
    * @param indexName index名稱
    * @param indexContent 建立index的欄位內容
    * @param result 執行結果訊息字串 (IP100:正確 其他：錯誤）
    */
    virtual void createIndex(cbl::Database appDB, const std::string &indexName, const std::string &indexContent, std::string& result) const = 0;

    /**
     * @brief deleteIndex 刪除資料庫index
     * @param appDB 資料庫物件參考
     * @param indexName index名稱
     * @param result 執行結果訊息字串 (IP100:正確 其他：錯誤）
     */
    virtual void deleteIndex(cbl::Database appDB, const std::string &indexName, std::string& result) const = 0;

    /**
     * @brief saveMutableDocument 儲存資料
     * @param appDB 資料庫物件參考
     * @param content json資料內容
     * @param uuid 文件ID
     * @param result 執行結果訊息字串 (IP200:正確 其他：錯誤）
     */
    virtual void saveMutableDocument(cbl::Database appDB, const std::string &content, std::string &uuid, std::string& result) = 0;

    /**
     * @brief queryDocuments 查詢資料
     * @param appDB 資料庫物件參考
     * @param sqlString 查詢所使用N1QL字串
     * @param result 執行結果訊息字串 (IP200:正確 其他：錯誤）
     * @return 資料集（JSON）
     */
    virtual cbl::ResultSet queryDocuments(const cbl::Database &appDB, const std::string &sqlString, std::string& result) const = 0;

    /**
     * @brief jsonImport 由檔案匯入JSON資料
     * @param appDB 資料庫物件參考
     * @param filename 匯入檔案的完整路徑與檔案名稱
     * @param result 執行結果訊息字串 (IP200:正確 其他：錯誤）
     */
    virtual void jsonImport(cbl::Database appDB, const std::string &filename, std::string& uuid, std::string&result) = 0;

    /**
     * @brief jsonExport 由資料庫匯出JSON資料至檔案
     * @param appDB 資料庫物件參考
     * @param filename 匯出檔案的完整路徑與檔案名稱
     * @param sqlString 匯出資料的查詢sql
     * @param 執行結果訊息字串 (IP300:正確 其他：錯誤）
     */
    virtual void jsonExport(cbl::Database appDB, const std::string &filename, const std::string &sqlString, std::string& result) = 0;

    // ToDo: internal db sync to backend db api
    /**
     * @brief getReplicatorBasicPushOnly 取得Couchbase Lite Replicator
     *        在使用replicator時若要重0開始則應該設定resetCheckpoint = true
     * @param appDB VMD中的internal db
     * @param url replication對象 ws://ip address:port/bucket
     * @param uid 登入資料庫帳號
     * @param pwd 登入資料庫密碼
     * @param result 執行結果訊息字串 (IP100:正確 其他：錯誤）
     * @param purge 是否自動清除 default = true
     * @param pemFile pem檔案路徑名稱
     * @return cbl::Replicator
     */
    virtual cbl::Replicator getReplicatorBasicPushOnly(const cbl::Database &appDB,
            const std::string &url,
            const std::string &uid,
            const std::string &pwd,
            std::string&result,
            const bool &purge = true,
            const std::string &pemFile = "") const = 0;

    /**
     * @brief getReplicatorBasicPullOnly 取得Couchbase Lite Replicator
     *        在使用replicator時若要重0開始則應該設定resetCheckpoint = true
     * @param appDB VMD中的internal db
     * @param url replication對象 ws://ip address:port/bucket
     * @param uid 登入資料庫帳號
     * @param pwd 登入資料庫密碼
     * @param result 執行結果訊息字串 (IP100:正確 其他：錯誤）
     * @param purge 是否自動清除 default = true
     * @param pemFile pem檔案路徑名稱
     * @return cbl::Replicator
     */
    virtual cbl::Replicator getReplicatorBasicPullOnly(const cbl::Database &appDB,
            const std::string &url,
            const std::string &uid,
            const std::string &pwd,
            std::string&result,
            const bool &purge = true,
            const std::string &pemFile = "") const = 0;

    virtual cbl::Replicator getReplicatorBasicPullOnlywFilter(
        const cbl::Database &appDB,
        const std::string &url,
        const std::string &uid,
        const std::string &pwd,
        std::string& result,
        std::function<bool(cbl::Document, CBLDocumentFlags flags)> filter,
        const bool &purge = true,
        const std::string &pemFile = "") const  = 0;

    /**
     * @brief getReplicatorBasicPnP 取得Couchbase Lite Replicator
     *        在使用replicator時若要重0開始則應該設定resetCheckpoint = true
     * @param appDB VMD中的internal db
     * @param url replication對象 ws://ip address:port/bucket
     * @param uid 登入資料庫帳號
     * @param pwd 登入資料庫密碼
     * @param result 執行結果訊息字串 (IP100:正確 其他：錯誤）
     * @param purge 是否自動清除 default = true
     * @param pemFile pem檔案路徑名稱
     * @return cbl::Replicator
     */
    virtual cbl::Replicator getReplicatorBasicPnP(const cbl::Database &appDB,
            const std::string &url,
            const std::string &uid,
            const std::string &pwd,
            std::string&result,
            const bool &purge = true,
            const std::string &pemFile = "") const = 0;

    /**
     * @brief getReplicatorSession 取得Couchbase Lite Replicator with
     * @param appDB VMD中的internal db
     * @param url replication對象 ws://ip address:port/bucket
     * @param sessionID
     * @param cookieName
     * @param result 執行結果訊息字串 (IP100:正確 其他：錯誤）
     * @param purge 是否自動清除 default = true
     * @param pemFile pem檔案路徑名稱
     * @return cbl::Replicator
     */
//    virtual cbl::Replicator getReplicatorSessionPushOnly(const cbl::Database &appDB,
//            const std::string &url,
//            const std::string &sessionID,
//            const std::string &cookieName,
//            std::string&result,
//            const bool &purge = true,
//            const std::string &pemFile = "") const = 0;

    /**
     * @brief setDocExpiration 設置文件保存期限
     * @param appDB VMD中的internal db
     * @param id 文件id
     * @param expiration 資料保存期限(ms)
     * @param result 執行結果訊息字串 (IP200:正確 其他：錯誤）
     */
    virtual void setDocExpiration(cbl::Database &appDB,
                                  const std::string &id,
                                  const int64_t expiration,
                                  std::string& result) const = 0;

    /**
     * @brief purgeDocument 清除文件
     * @param appDB VMD中的internal db
     * @param id 文件id
     * @param result 執行結果訊息字串 (IP200:正確 其他：錯誤）
     */
    virtual void purgeDocument(cbl::Database &appDB,
                               const std::string &id,
                               std::string& result) const = 0;

    /**
     * @brief getReplicatorSessionPullOnly 取得Couchbase Lite Replicator with
     * @param appDB VMD中的internal db
     * @param url replication對象 ws://ip address:port/bucket
     * @param sessionID
     * @param cookieName
     * @param result 執行結果訊息字串 (IP100:正確 其他：錯誤）
     * @param pemFile pem檔案路徑名稱
     * @return cbl::Replicator
     */

//    virtual cbl::Replicator getReplicatorSessionPullOnly(const cbl::Database &appDB,
//            const std::string &url,
//            const std::string &sessionID,
//            const std::string &cookieName,
//            std::string&result,
//            const std::string &pemFile = "") const = 0;
};

Q_DECLARE_INTERFACE(CBLInterface, CBLInterface_iid)
#endif // CBLINTERFACE_H
