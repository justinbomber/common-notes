#ifndef DDSPLUGIN_H
#define DDSPLUGIN_H

#include "DDSPlugin_global.h"
#include "ddsinterface.h"

class DDSPLUGIN_EXPORT DDSPlugin : public QObject, public DDSInterface {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.foyatech.miot.DDSPlugin" FILE "ddsplugin.json")
    Q_INTERFACES(PluginInterface DDSInterface)
  private:
    static DDSPlugin *dds_instance;

  public:
    DDSPlugin();

    ~DDSPlugin();

    static DDSPlugin *instance() {
        if (dds_instance == nullptr)
            dds_instance = new DDSPlugin();
        return dds_instance;
    }

    std::vector<std::string> split(const std::string& s, const char delimiter);

    dds::domain::DomainParticipant getDomainParticipant(const unsigned int domain_id,
            const std::string &qosPath,
            const std::string &qosLibName) override;

    dds::pub::Publisher getPublisher() override;

    dds::pub::Publisher getPublisher(const std::string &qosLibName) override;

    void setPubPartition(dds::pub::Publisher &publisher, const std::string &newPartition) override;

    void setPubPartition(const std::string &newPartition) override;

    void addPubPartition(dds::pub::Publisher &publisher, const std::string &partitions, const char delimiter) override;

    void addPubPartition(const std::string &partitions, const char delimiter) override;

    dds::sub::Subscriber getSubscrier() override;

    dds::sub::Subscriber getSubscrier(const std::string &qosLibName) override;

    void setSubPartition(dds::sub::Subscriber &subscriber, const std::string &newPartition) override;

    void setSubPartition(const std::string &newPartition) override;

    void addSubPartition(dds::sub::Subscriber &subscriber, const std::string &partitions, const char delimiter) override;

    void addSubPartition(const std::string &partitions, const char delimiter) override;

    dds::core::xtypes::DynamicType getTypeObject(const std::string &typeName,
            const std::string &xmlPath) const override;

    dds::core::xtypes::DynamicType getTypeObject(
        const std::string &typeName) const override;

    dds::topic::Topic<dds::core::xtypes::DynamicData> getTopic(const std::string &topicName) override;

    dds::topic::Topic<dds::core::xtypes::DynamicData> getTopic(
        const dds::domain::DomainParticipant &participant,
        const std::string &topicName) override;

    dds::topic::Topic<dds::core::xtypes::DynamicData> getTopic(
        const std::string &topicName,
        const dds::core::xtypes::DynamicType &typeObject,
        const std::string &qosName) override;

    dds::topic::Topic<dds::core::xtypes::DynamicData> getTopic(
        const dds::domain::DomainParticipant &participant,
        const std::string &topicName,
        const dds::core::xtypes::DynamicType &typeObject,
        const std::string &qosName) override;

    dds::topic::ContentFilteredTopic<dds::core::xtypes::DynamicData> getFilterTopic(
        const std::string &srcTopicName,
        const std::string &filterTopicName,
        const std::string &qosName,
        const dds::core::xtypes::DynamicType &typeObject,
        const std::vector<std::string> &params,
        const std::string &conditions) override;

    dds::topic::ContentFilteredTopic<dds::core::xtypes::DynamicData> getFilterTopic(
        const dds::topic::Topic<dds::core::xtypes::DynamicData> &topic,
        const std::string &filterTopicName,
        const std::vector<std::string> &params,
        const std::string &conditions) override;

    dds::topic::ContentFilteredTopic<dds::core::xtypes::DynamicData> getFilterTopic(
        const dds::topic::Topic<dds::core::xtypes::DynamicData> &topic,
        const std::string &filterTopicName,
        const std::string &conditions) override;

    dds::pub::DataWriter<dds::core::xtypes::DynamicData> getWriter(
        const dds::topic::Topic<dds::core::xtypes::DynamicData> &topic,
        const std::string &qosName) override;

    dds::pub::DataWriter<dds::core::xtypes::DynamicData> getWriter(
        const std::string &topicName,
        const std::string &qosName) override;

    dds::pub::DataWriter<dds::core::xtypes::DynamicData> getWriter(
        const dds::pub::Publisher &publisher,
        const std::string &topicName,
        const dds::core::xtypes::DynamicType &typeObject,
        const std::string &topQosName,
        const std::string &qosName) override;

    dds::sub::DataReader<dds::core::xtypes::DynamicData> getReader(
        const dds::topic::Topic<dds::core::xtypes::DynamicData> &topic,
        const std::string &qosName) override;

    dds::sub::DataReader<dds::core::xtypes::DynamicData> getReader_w_listener(
            const dds::topic::Topic<dds::core::xtypes::DynamicData> &topic,
            const std::shared_ptr<BaseDataReaderListener<dds::core::xtypes::DynamicData>> listener,
            const dds::core::status::StatusMask status,
            const std::string &qosName) override;

    dds::sub::DataReader<dds::core::xtypes::DynamicData> getFilterReader(
        const dds::topic::ContentFilteredTopic<dds::core::xtypes::DynamicData> &topic,
        const std::string &qosName) override;

    dds::sub::DataReader<dds::core::xtypes::DynamicData> getReader(
        const std::string &topicName,
        const std::string &qosName) override;

    dds::sub::DataReader<dds::core::xtypes::DynamicData> getReader(
        const dds::sub::Subscriber &subscriber,
        const std::string &topicName,
        const dds::core::xtypes::DynamicType &typeObject,
        const std::string &topQosName,
        const std::string &qosName) override;

    dds::pub::DataWriter<dds::core::xtypes::DynamicData> getWriter(
        const std::string& entityName,
        const dds::topic::Topic<dds::core::xtypes::DynamicData> &topic,
        const std::string &qosName) override;    
};

#endif // DDSPLUGIN_H
