#ifndef DDSINTERFACE_H
#define DDSINTERFACE_H

#include <QtCore>
#include "plugininterface.h"
#include "listeners.h"

#include <dds/domain/DomainParticipant.hpp>
#include <dds/pub/ddspub.hpp>
#include <dds/sub/ddssub.hpp>
#include <dds/core/xtypes/DynamicData.hpp>
#include <rti/util/util.hpp>     // for sleep()
#include <rti/config/Logger.hpp> // for logging

#define DDSInterface_iid "com.foyatech.miot.DDSInterface"

class DDSInterface : public PluginInterface {
  public:
    virtual ~DDSInterface() {}

    /**
     * @brief getDomainParticipant 取得唯一的DomainParticipant
     * @param domain_id
     * @param qosPath QoS profile 檔案名稱
     * @param qosLibName 使用library::profile
     * @return dds::domain::DomainParticipant
     */
    virtual dds::domain::DomainParticipant getDomainParticipant(const unsigned int domain_id,
            const std::string &qosPath,
            const std::string &qosLibName) = 0;
    /**
     * @brief getPublisher 取得唯一的Publisher
     * @return dds::pub::Publisher
     */
    virtual dds::pub::Publisher getPublisher() = 0;

    /**
     * @brief getNewPublisher 取得新的Publisher
     * @return dds::pub::Publisher
     */
    virtual dds::pub::Publisher getPublisher(const std::string &qosLibName) = 0;

    /**
     * @brief setPubPartition 變更指定的Publisher的partition
     * @param publisher 應用程式中所使用的publisher
     * @param newPartition partition的文字字串
     */
    virtual void setPubPartition(dds::pub::Publisher &publisher, const std::string &newPartition) = 0;

    /**
     * @brief setPubPartition 變更預設的Publisher的partition
     * @param newPartition partition的文字字串
     */
    virtual void setPubPartition(const std::string &newPartition) = 0;

    /**
     * @brief addPubPartition 新增partition(s)給指定的Publisher
     * @param publisher 指定的Publisher
     * @param partitions 以字串分隔符號串接的字串
     * @param delimiter 字串分隔符號
     */
    virtual void addPubPartition(dds::pub::Publisher &publisher, const std::string &partitions, const char delimiter) = 0;

    /**
     * @brief addPubPartition 新增partition(s)給全域的Publisher
     * @param partitions 以字串分隔符號串接的字串
     * @param delimiter 字串分隔符號
     */
    virtual void addPubPartition(const std::string &partitions, const char delimiter) = 0;

    /**
     * @brief getSubscrier 取得唯一的Subscriber
     * @return dds::sub::Subscriber
     */
    virtual dds::sub::Subscriber getSubscrier() = 0;

    /**
     * @brief getSubscrier 取得新的Subscriber
     * @return dds::sub::Subscriber
     */
    virtual dds::sub::Subscriber getSubscrier(const std::string &qosLibName) = 0;

    /**
     * @brief setSubPartition 設定新的partition給指定的Subscriber
     * @param subscriber 指定的Subscriber
     * @param newPartition 新partiton參數值（字串）
     */
    virtual void setSubPartition(dds::sub::Subscriber &subscriber, const std::string &newPartition) = 0;

    /**
     * @brief setSubPartition 設定新的partition給全域的Subscriber
     * @param newPartition 新partiton參數值（字串）
     */
    virtual void setSubPartition(const std::string &newPartition) = 0;

    /**
     * @brief addSubPartition 新增partition(s)給指定的Subscriber
     * @param subscriber 指定的Subscriber
     * @param partitions 以字串分隔符號串接的字串
     * @param delimiter 字串分隔符號
     */
    virtual void addSubPartition(dds::sub::Subscriber &subscriber, const std::string &partitions, const char delimiter) = 0;

    /**
     * @brief addSubPartition 新增partition(s)給全域的Subscriber
     * @param partitions 以字串分隔符號串接的字串
     * @param delimiter 字串分隔符號
     */
    virtual void addSubPartition(const std::string &partitions, const char delimiter) = 0;

    /**
    * @brief DDSPlugin::getTypeObject
    * @param typeName 資料型別名稱
    * @param xmlPath 紀錄資料型別的xl檔案路徑/名稱
    * @return dds::core::xtypes::DynamicType
    */
    virtual dds::core::xtypes::DynamicType getTypeObject(const std::string &typeName,
            const std::string &xmlPath) const = 0;
    virtual dds::core::xtypes::DynamicType getTypeObject(
        const std::string &typeName) const = 0;
    /**
     * @brief getTopic 使用Topic名稱取得Topic物件
     * @param topicName Topic名稱
     * @return dds::topic::Topic<dds::core::xtypes::DynamicData>
     */
    virtual dds::topic::Topic<dds::core::xtypes::DynamicData> getTopic(const std::string &topicName) = 0;

    /**
     * @brief getTopic 取得Topic物件
     * @param participant 指定domain participant
     * @param topicName Topic名稱
     * @return dds::topic::Topic<dds::core::xtypes::DynamicData>
     */
    virtual dds::topic::Topic<dds::core::xtypes::DynamicData> getTopic(const dds::domain::DomainParticipant &participant,
            const std::string &topicName) = 0;

    /**
     * @brief getTopic 取得Topic物件
     * @param topicName Topic名稱
     * @param typeObject Topic資料格式
     * @param qosName Topic Layer QoS名稱
     * @return dds::topic::Topic<dds::core::xtypes::DynamicData>
     */
    virtual dds::topic::Topic<dds::core::xtypes::DynamicData> getTopic(const std::string &topicName,
            const dds::core::xtypes::DynamicType &typeObject,
            const std::string &qosName) = 0;

    /**
     * @brief getTopic 取得Topic物件
     * @param participant 指定domain participant
     * @param topicName Topic名稱
     * @param typeObject Topic資料格式
     * @param qosName Topic Layer QoS名稱
     * @return dds::topic::Topic<dds::core::xtypes::DynamicData>
     */
    virtual dds::topic::Topic<dds::core::xtypes::DynamicData> getTopic(const dds::domain::DomainParticipant &participant,
            const std::string &topicName,
            const dds::core::xtypes::DynamicType &typeObject,
            const std::string &qosName) = 0;

    /**
     * @brief getFilterTopic 取得或建立新的ContentFilterTopic
     *       （建議在DataReader端使用,在DataWriter端執行有較大的限制）,但在DataReader端執行就會有維護上的問題
     * @param srcTopicName 建立ContentFilterTopic資料來源Topic名稱
     * @param filterTopicName 新建立ContentFilterTopic名稱（若未輸入,則預設為Filter+srcTopicName）
     * @param qosName Topic Layer QoS名稱
     * @param typeObject
     * @param params 過濾條件值
     * @param conditions 過濾條件字串（Where敘述）
     *        ex: 數值類
     *        "(X < %0 or X > %1) and (Y < %2 or Y > %3)" , X,Y為IDL設計的欄位名稱
     *        ex: 文字類
     *        "X MATCH %0", X為IDL設計的欄位名稱
     * @return dds::topic::ContentFilteredTopic<dds::core::xtypes::DynamicData>
     */
    virtual dds::topic::ContentFilteredTopic<dds::core::xtypes::DynamicData> getFilterTopic(const std::string &srcTopicName,
            const std::string &filterTopicName,
            const std::string &qosName,
            const dds::core::xtypes::DynamicType &typeObject,
            const std::vector<std::string> &params,
            const std::string &conditions) = 0;

    /**
     * @brief getFilterTopic 取得或建立新的ContentFilterTopic
     * @param topic dds::topic::Topic<dds::core::xtypes::DynamicData> entity
     * @param filterTopicName 新建立ContentFilterTopic名稱（若未輸入,則預設為Filter+topic name）
     * @param params 過濾條件值
     * @param conditions 過濾條件字串（Where敘述）
     *        ex: 數值類
     *        "(X < %0 or X > %1) and (Y < %2 or Y > %3)" , X,Y為IDL設計的欄位名稱
     *        ex: 文字類
     *        "X MATCH %0", X為IDL設計的欄位名稱
     * @return dds::topic::ContentFilteredTopic<dds::core::xtypes::DynamicData>
     */
    virtual dds::topic::ContentFilteredTopic<dds::core::xtypes::DynamicData> getFilterTopic(const dds::topic::Topic<dds::core::xtypes::DynamicData> &topic,
            const std::string &filterTopicName,
            const std::vector<std::string> &params,
            const std::string &conditions) = 0;

    /**
     * @brief getFilterTopic 取得或建立新的ContentFilterTopic
     * @param topic dds::topic::Topic<dds::core::xtypes::DynamicData> entity
     * @param filterTopicName 新建立ContentFilterTopic名稱（若未輸入,則預設為Filter+topic name）
     * @param conditions 過濾條件字串（Where敘述）
     * @return dds::topic::ContentFilteredTopic<dds::core::xtypes::DynamicData>
     */
    virtual dds::topic::ContentFilteredTopic<dds::core::xtypes::DynamicData> getFilterTopic(const dds::topic::Topic<dds::core::xtypes::DynamicData> &topic,
            const std::string &filterTopicName,
            const std::string &conditions) = 0;

    /**
     * @brief getWriter 產生指定的DomainParticipant所屬的DataWriter
     * @param topic topic dds::topic::Topic<dds::core::xtypes::DynamicData> entity
     * @param qosName Writer Layer QoS名稱
     * @return dds::pub::DataWriter<dds::core::xtypes::DynamicData>
     */
    virtual dds::pub::DataWriter<dds::core::xtypes::DynamicData> getWriter(const dds::topic::Topic<dds::core::xtypes::DynamicData> &topic,
            const std::string &qosName) = 0;

    /**
     * @brief getWriter 產生指定的DomainParticipant所屬的DataWriter
     * @param topicName DataWriter寫入資料的Topic名稱（已經存在）
     * @param qosName Writer Layer QoS名稱
     * @return dds::pub::DataWriter<dds::core::xtypes::DynamicData>
     */
    virtual dds::pub::DataWriter<dds::core::xtypes::DynamicData> getWriter(const std::string &topicName,
            const std::string &qosName) = 0;

    /**
     * @brief getWriter getWriter 產生指定的DomainParticipant所屬的DataWriter
     * @param publisher 指定的Publisher
     * @param topicName DataWriter寫入資料的Topic名稱（已經存在）
     * @param typeObject Topic資料格式物件
     * @param topQosName Topic Layer qosName QoS名稱
     * @param qosName Writer Layer qosName QoS名稱
     * @return dds::pub::DataWriter<dds::core::xtypes::DynamicData>
     */
    virtual dds::pub::DataWriter<dds::core::xtypes::DynamicData> getWriter(const dds::pub::Publisher &publisher,
            const std::string &topicName,
            const dds::core::xtypes::DynamicType &typeObject,
            const std::string &topQosName,
            const std::string &qosName) = 0;

    /**
     * @brief getReader 產生指定的DomainParticipant所屬的DataReader
     * @param topic dds::topic::Topic<dds::core::xtypes::DynamicData> entity
     * @param qosName Reader Layer QoS名稱
     * @return dds::sub::DataReader<dds::core::xtypes::DynamicData>
     */
    virtual dds::sub::DataReader<dds::core::xtypes::DynamicData> getReader(const dds::topic::Topic<dds::core::xtypes::DynamicData> &topic,
            const std::string &qosName) = 0;

    /**
     * @brief getReader_w_listener 產生指定的DomainParticipant所屬的DataReader with listener
     * @param topic  DataReader讀取資料的Topic名稱（已經存在）
     * @param listener
     * @param status 需要監聽的event
     * @param qosName Reader Layer QoS名稱
     * @return dds::sub::DataReader<dds::core::xtypes::DynamicData>
     */
    virtual dds::sub::DataReader<dds::core::xtypes::DynamicData> getReader_w_listener(
            const dds::topic::Topic<dds::core::xtypes::DynamicData> &topic,
            const std::shared_ptr<BaseDataReaderListener<dds::core::xtypes::DynamicData>> listener,
            const dds::core::status::StatusMask status,
            const std::string &qosName) = 0;

    /**
     * @brief getFilterReader
     * @param topic FilterTopic參照的Topic物件
     * @param qosName Reader Layer QoS名稱
     * @return dds::sub::DataReader<dds::core::xtypes::DynamicData>
     */
    virtual dds::sub::DataReader<dds::core::xtypes::DynamicData> getFilterReader(
        const dds::topic::ContentFilteredTopic<dds::core::xtypes::DynamicData> &topic,
        const std::string &qosName) = 0;

    /**
     * @brief getReader 產生指定的DomainParticipant所屬的DataReader
     * @param topicName DataReader讀取資料的Topic名稱（已經存在）
     * @param qosName Reader Layer QoS名稱
     * @return dds::sub::DataReader<dds::core::xtypes::DynamicData>
     */
    virtual dds::sub::DataReader<dds::core::xtypes::DynamicData> getReader(const std::string &topicName,
            const std::string &qosName) = 0;

    /**
     * @brief getReader 產生指定的DomainParticipant所屬的DataReader
     * @param subscriber 指定的subscriber
     * @param topicName DataReader讀取資料的Topic名稱（已經存在）
     * @param typeObject topic資料格式物件
     * @param topQosName Topic Layer QoS名稱
     * @param qosName Reader Layer QoS名稱
     * @return dds::sub::DataReader<dds::core::xtypes::DynamicData>
     */
    virtual dds::sub::DataReader<dds::core::xtypes::DynamicData> getReader(const dds::sub::Subscriber &subscriber,
            const std::string &topicName,
            const dds::core::xtypes::DynamicType &typeObject,
            const std::string &topQosName,
            const std::string &qosName) = 0;

    virtual dds::pub::DataWriter<dds::core::xtypes::DynamicData> getWriter(const std::string& entityName,
            const dds::topic::Topic<dds::core::xtypes::DynamicData> &topic,
            const std::string &qosName) = 0;
};

Q_DECLARE_INTERFACE(DDSInterface, DDSInterface_iid)

#endif // DDSINTERFACE_H
