#include "ddsplugin.h"

#include <dds/domain/find.hpp>
#include <dds/topic/find.hpp>
#include <dds/topic/ContentFilteredTopic.hpp>
#include <dds/pub/find.hpp>
#include <dds/sub/find.hpp>
#include <dds/core/policy/CorePolicy.hpp>
#include <dds/core/QosProvider.hpp>
#include <dds/core/xtypes/DynamicType.hpp>
#include <dds/core/xtypes/StructType.hpp>
//#include <QString>

DDSPlugin *DDSPlugin::dds_instance = nullptr;
dds::domain::DomainParticipant participant = nullptr;
dds::pub::Publisher publisher = nullptr;
dds::sub::Subscriber subscriber = nullptr;
dds::core::QosProvider default_provider = nullptr;

DDSPlugin::DDSPlugin() {
}

DDSPlugin::~DDSPlugin() {
    qDebug() << "DP490:<*********  Destructing DDSPlugin  **********>";
    try {
        if (publisher != dds::core::null) {
            qDebug() << "DP490:Call publisher.close()";
            publisher.close();
            publisher = nullptr;
        }
        if (subscriber != dds::core::null) {
            qDebug() << "DP490:Call subscriber.close()";
            subscriber.close();
            subscriber = nullptr;
        }
        if (participant != dds::core::null) {
            qDebug() << "DP490:Call participant.close()";
            participant.close();
            participant = nullptr;
        }
    } catch(dds::core::Exception &ex) {
        qCritical() << "ER499:" << ex.what();
    }

    // 加上這一行會導致plugin重新載入出現問題
    // This method uses a static variable. To avoid undefined behavior in the order of destruction
    // it shouldn't be called in the destructor of a type that for which other global or static variables exist.
    //    dds::domain::DomainParticipant::finalize_participant_factory();
}

std::vector<std::string> DDSPlugin::split(const std::string &s, const char delimiter) {
    std::vector<std::string> result;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        result.push_back(token);
    }
    return result;
}

dds::domain::DomainParticipant DDSPlugin::getDomainParticipant(
    const unsigned int domain_id,
    const std::string &qosPath,
    const std::string &qosLibName) {
    // We're going to configure the default QoS Provider to load ExampleQos.xml
    // and to ignore the NDDS_QOS_PROFILES environment variable and the file
    // USER_QOS_PROFILES.xml
    rti::core::QosProviderParams params;
    params.url_profile({qosPath});
    params.ignore_environment_profile(true);
    params.ignore_user_profile(true);
    // To ensure that the new configuration takes effect before any other
    // profiles are loaded, set the new parameters before accessing
    // QosProvider::Default().
    rti::core::default_qos_provider_params(params);
    if (default_provider == dds::core::null)
        default_provider = dds::core::QosProvider::Default();

    participant = dds::domain::find(domain_id);
    if (participant == dds::core::null)
        participant = dds::domain::DomainParticipant(domain_id, default_provider.participant_qos(qosLibName));

    if (publisher == dds::core::null)
        publisher = dds::pub::Publisher(participant, default_provider.publisher_qos(qosLibName));

    if (subscriber == dds::core::null)
        subscriber = dds::sub::Subscriber(participant, default_provider.subscriber_qos(qosLibName));

    return participant;
}

dds::pub::Publisher DDSPlugin::getPublisher() {
    return publisher;
}

dds::pub::Publisher DDSPlugin::getPublisher(const std::string &qosLibName) {
    return dds::pub::Publisher(participant, default_provider.publisher_qos(qosLibName));
}

void DDSPlugin::setPubPartition(
    dds::pub::Publisher &publisher,
    const std::string &newPartition) {
    dds::pub::qos::PublisherQos pubQos = publisher.qos();
    auto &curPartition = pubQos.policy<dds::core::policy::Partition>();
    std::vector<std::string> partitionNames = curPartition.name();
    if (partitionNames.empty())
        partitionNames.push_back(newPartition);
    else
        partitionNames[0] = newPartition;
    curPartition.name(partitionNames);
    publisher.qos(pubQos << curPartition);
}

void DDSPlugin::setPubPartition(
    const std::string &newPartition) {
    dds::pub::qos::PublisherQos pubQos = publisher.qos();
    auto &curPartition = pubQos.policy<dds::core::policy::Partition>();
    std::vector<std::string> partitionNames = curPartition.name();
    if (partitionNames.empty())
        partitionNames.push_back(newPartition);
    else
        partitionNames[0] = newPartition;
    curPartition.name(partitionNames);
    publisher.qos(pubQos << curPartition);
}

void DDSPlugin::addPubPartition(
    dds::pub::Publisher &publisher,
    const std::string &partitions,
    const char delimiter) {
    dds::pub::qos::PublisherQos pubQos = publisher.qos();
    auto &curPartition = pubQos.policy<dds::core::policy::Partition>();
    std::vector<std::string> partitionNames = curPartition.name();
    std::vector<std::string> addPartitionNames = split(partitions, delimiter);
    std::vector<std::string>::iterator it;
    for (it = addPartitionNames.begin(); it != addPartitionNames.end(); ++it) {
        partitionNames.push_back(it->data());
    }
    curPartition.name(partitionNames);
    publisher.qos(pubQos << curPartition);
}

void DDSPlugin::addPubPartition(const std::string &partitions,
                                const char delimiter) {
    dds::pub::qos::PublisherQos pubQos = publisher.qos();
    auto &curPartition = pubQos.policy<dds::core::policy::Partition>();
    std::vector<std::string> partitionNames = curPartition.name();
    std::vector<std::string> addPartitionNames = split(partitions, delimiter);
    std::vector<std::string>::iterator it;
    for (it = addPartitionNames.begin(); it != addPartitionNames.end(); ++it) {
        partitionNames.push_back(it->data());
    }
    curPartition.name(partitionNames);
    publisher.qos(pubQos << curPartition);
}

dds::sub::Subscriber DDSPlugin::getSubscrier() {
    return subscriber;
}

dds::sub::Subscriber DDSPlugin::getSubscrier(const std::string &qosLibName) {
    return dds::sub::Subscriber(participant, default_provider.subscriber_qos(qosLibName));
}

void DDSPlugin::setSubPartition(
    dds::sub::Subscriber &subscriber,
    const std::string &newPartition) {
    dds::sub::qos::SubscriberQos subQos = subscriber.qos();
    auto &curPartition = subQos.policy<dds::core::policy::Partition>();
    std::vector<std::string> partitionNames = curPartition.name();
    if (partitionNames.empty())
        partitionNames.push_back(newPartition);
    else
        partitionNames[0] = newPartition;
    curPartition.name(partitionNames);
    subscriber.qos(subQos << curPartition);
}

void DDSPlugin::addSubPartition(dds::sub::Subscriber &subscriber, const std::string &partitions, const char delimiter) {
    dds::sub::qos::SubscriberQos subQos = subscriber.qos();
    auto &curPartition = subQos.policy<dds::core::policy::Partition>();
    std::vector<std::string> partitionNames = curPartition.name();
    std::vector<std::string> addPartitionNames = split(partitions, delimiter);
    std::vector<std::string>::iterator it;
    for (it = addPartitionNames.begin(); it != addPartitionNames.end(); ++it) {
        partitionNames.push_back(it->data());
    }
    curPartition.name(partitionNames);
    subscriber.qos(subQos << curPartition);
}

void DDSPlugin::addSubPartition(const std::string &partitions, const char delimiter) {
    dds::sub::qos::SubscriberQos subQos = subscriber.qos();
    auto &curPartition = subQos.policy<dds::core::policy::Partition>();
    std::vector<std::string> partitionNames = curPartition.name();
    std::vector<std::string> addPartitionNames = split(partitions, delimiter);
    std::vector<std::string>::iterator it;
    for (it = addPartitionNames.begin(); it != addPartitionNames.end(); ++it) {
        partitionNames.push_back(it->data());
    }
    curPartition.name(partitionNames);
    subscriber.qos(subQos << curPartition);
}

void DDSPlugin::setSubPartition(
    const std::string &newPartition) {
    dds::sub::qos::SubscriberQos subQos = subscriber.qos();
    auto &curPartition = subQos.policy<dds::core::policy::Partition>();
    std::vector<std::string> partitionNames = curPartition.name();
    if (partitionNames.empty())
        partitionNames.push_back(newPartition);
    else
        partitionNames[0] = newPartition;
    curPartition.name(partitionNames);
    subscriber.qos(subQos << curPartition);
}

dds::core::xtypes::DynamicType DDSPlugin::getTypeObject(
    const std::string &typeName,
    const std::string &xmlPath) const {
    // Create a QosProvider (or use the default one)
    dds::core::QosProvider qos_provider(xmlPath);
    const dds::core::xtypes::DynamicType &typeObject =
        qos_provider.extensions().type(typeName);
    return typeObject;
}
dds::core::xtypes::DynamicType DDSPlugin::getTypeObject(
    const std::string &typeName) const {
    // Create a QosProvider (or use the default one)
    const dds::core::xtypes::DynamicType &typeObject =
        default_provider.extensions().type(typeName);
    return typeObject;
}

dds::topic::Topic<dds::core::xtypes::DynamicData> DDSPlugin::getTopic(const std::string &topicName) {
    return dds::topic::find<dds::topic::Topic<dds::core::xtypes::DynamicData>>(
               participant,
               topicName);
}

dds::topic::Topic<dds::core::xtypes::DynamicData> DDSPlugin::getTopic(const dds::domain::DomainParticipant &participant,
        const std::string &topicName) {
    return dds::topic::find<dds::topic::Topic<dds::core::xtypes::DynamicData>>(
               participant,
               topicName);
}

dds::topic::Topic<dds::core::xtypes::DynamicData> DDSPlugin::getTopic(
    const std::string &topicName,
    const dds::core::xtypes::DynamicType &typeObject,
    const std::string &qosName) {
    dds::topic::Topic<dds::core::xtypes::DynamicData> topic =
        dds::topic::find<dds::topic::Topic<dds::core::xtypes::DynamicData>>(
            participant,
            topicName);
    if (topic == dds::core::null) {
        dds::topic::qos::TopicQos qos = default_provider.topic_qos();
        if(qosName != "")
            qos = default_provider.topic_qos(qosName);
        return dds::topic::Topic<dds::core::xtypes::DynamicData>(
                   participant,
                   topicName,
                   typeObject,
                   qos,
                   NULL,
                   dds::core::status::StatusMask::none());
    }
    return topic;
}

dds::topic::Topic<dds::core::xtypes::DynamicData> DDSPlugin::getTopic(
    const dds::domain::DomainParticipant &participant,
    const std::string &topicName,
    const dds::core::xtypes::DynamicType &typeObject,
    const std::string &qosName) {
    dds::topic::Topic<dds::core::xtypes::DynamicData> topic =
        dds::topic::find<dds::topic::Topic<dds::core::xtypes::DynamicData>>(
            participant,
            topicName);
    if (topic == dds::core::null) {
        dds::topic::qos::TopicQos qos = default_provider.topic_qos();
        if(qosName != "")
            qos = default_provider.topic_qos(qosName);
        return dds::topic::Topic<dds::core::xtypes::DynamicData>(
                   participant,
                   topicName,
                   typeObject,
                   qos,
                   NULL,
                   dds::core::status::StatusMask::none());
    }
    return topic;
}

dds::topic::ContentFilteredTopic<dds::core::xtypes::DynamicData> DDSPlugin::getFilterTopic(
    const std::string &srcTopicName,
    const std::string &filterTopicName,
    const std::string &qosName,
    const dds::core::xtypes::DynamicType &typeObject,
    const std::vector<std::string> &params,
    const std::string &conditions) {
    std::string filterName = "Filter" + srcTopicName;
    if (!filterTopicName.empty())
        filterName = filterTopicName;

    // 以名稱取得Topic entity, 若不存在會長新的topic出來
    dds::topic::Topic<dds::core::xtypes::DynamicData> topic = getTopic(participant,
            srcTopicName,
            typeObject,
            qosName);
    if (topic == dds::core::null)
        return nullptr;

    // 以名稱取得Content Filter Topic entity, 若不存在會長新的topic出來
    dds::topic::ContentFilteredTopic<dds::core::xtypes::DynamicData> cftopic =
        dds::topic::find<dds::topic::ContentFilteredTopic<dds::core::xtypes::DynamicData>>(
            participant,
            filterName);

    if(cftopic == dds::core::null)
        return dds::topic::ContentFilteredTopic<dds::core::xtypes::DynamicData>(
                   topic,
                   filterName,
                   dds::topic::Filter(conditions,
                                      params));
    else // 可以變更現有ContentFilterTopic過濾條件值
        cftopic.extensions().filter_parameters(params.begin(), params.end());

    return cftopic;
}

dds::topic::ContentFilteredTopic<dds::core::xtypes::DynamicData> DDSPlugin::getFilterTopic(
    const dds::topic::Topic<dds::core::xtypes::DynamicData> &topic,
    const std::string &filterTopicName,
    const std::vector<std::string> &params,
    const std::string &conditions) {
    std::string filterName = "Filter" + topic.name();
    if (!filterTopicName.empty())
        filterName = filterTopicName;

    // 以名稱取得Content Filter Topic entity, 若不存在會長新的topic出來
    dds::topic::ContentFilteredTopic<dds::core::xtypes::DynamicData> cftopic =
        dds::topic::find<dds::topic::ContentFilteredTopic<dds::core::xtypes::DynamicData>>(
            participant,
            filterName);

    if(cftopic == dds::core::null)
        return dds::topic::ContentFilteredTopic<dds::core::xtypes::DynamicData>(
                   topic,
                   filterName,
                   dds::topic::Filter(conditions,
                                      params));
    else // 可以變更現有ContentFilterTopic過濾條件值
        cftopic.extensions().filter_parameters(params.begin(), params.end());

    return cftopic;
}

dds::topic::ContentFilteredTopic<dds::core::xtypes::DynamicData> DDSPlugin::getFilterTopic(
    const dds::topic::Topic<dds::core::xtypes::DynamicData> &topic,
    const std::string &filterTopicName,
    const std::string &conditions) {
    std::string filterName = "Filter" + topic.name();
    if (!filterTopicName.empty())
        filterName = filterTopicName;

    // 以名稱取得Content Filter Topic entity, 若不存在會長新的topic出來
    dds::topic::ContentFilteredTopic<dds::core::xtypes::DynamicData> cftopic =
        dds::topic::find<dds::topic::ContentFilteredTopic<dds::core::xtypes::DynamicData>>(
            participant,
            filterName);

    if(cftopic == dds::core::null)
        return dds::topic::ContentFilteredTopic<dds::core::xtypes::DynamicData>(
                   topic,
                   filterName,
                   dds::topic::Filter(conditions));
    else { // 可以變更現有ContentFilterTopic過濾條件值
        dds::topic::Filter filter(conditions);
        cftopic.extensions().filter(filter);
    }

    return cftopic;
}

dds::pub::DataWriter<dds::core::xtypes::DynamicData> DDSPlugin::getWriter(
    const dds::topic::Topic<dds::core::xtypes::DynamicData> &topic,
    const std::string &qosName) {
    dds::pub::qos::DataWriterQos qos = default_provider.datawriter_qos();
    if(qosName != "")
        qos = default_provider.datawriter_qos(qosName);
    return dds::pub::DataWriter<dds::core::xtypes::DynamicData>(
               publisher,
               topic,
               qos,
               NULL,
               dds::core::status::StatusMask::none());
}

dds::pub::DataWriter<dds::core::xtypes::DynamicData> DDSPlugin::getWriter(
    const std::string &topicName,
    const std::string &qosName) {
    /* List of writers returned by the find function */
    std::vector<dds::pub::DataWriter<dds::core::xtypes::DynamicData>> writers;

    /* Get the list of writers */
    int writer_count = dds::pub::find <
                       dds::pub::DataWriter<dds::core::xtypes::DynamicData >> (
                           publisher,
                           topicName, //不使用非同步傳輸,所以,不實做與設定Listener
                           std::back_inserter(writers));

    /* All we need is at least one writer. If there are multiple let’s use the
       first one returned. If no writers are found we create one
    */
    if (writer_count) {
        return writers[0];
    } else {
        dds::pub::qos::DataWriterQos qos = default_provider.datawriter_qos();
        if(qosName != "")
            qos = default_provider.datawriter_qos(qosName);
        return dds::pub::DataWriter<dds::core::xtypes::DynamicData>(
                   publisher,
                   getTopic(topicName),
                   qos,
                   NULL,
                   dds::core::status::StatusMask::none());
    }
}

dds::pub::DataWriter<dds::core::xtypes::DynamicData> DDSPlugin::getWriter(
    const dds::pub::Publisher &publisher,
    const std::string &topicName,
    const dds::core::xtypes::DynamicType &typeObject,
    const std::string &topQosName,
    const std::string &qosName) {
    /* List of writers returned by the find function */
    std::vector<dds::pub::DataWriter<dds::core::xtypes::DynamicData>> writers;

    /* Get the list of readers */
    int writer_count = dds::pub::find <
                       dds::pub::DataWriter<dds::core::xtypes::DynamicData >> (
                           publisher,
                           topicName,
                           std::back_inserter(writers));

    /* All we need is at least one reader. If there are multiple let’s use the
       first one returned. If no readers are found we create one
    */
    if (writer_count) {
        return writers[0];
    } else {
        dds::pub::qos::DataWriterQos qos = default_provider.datawriter_qos();
        if(qosName != "")
            qos = default_provider.datawriter_qos(qosName);
        return dds::pub::DataWriter<dds::core::xtypes::DynamicData>(
                   publisher,
                   getTopic(topicName, typeObject, topQosName),
                   qos,
                   NULL,
                   dds::core::status::StatusMask::none());
    }
}

dds::sub::DataReader<dds::core::xtypes::DynamicData> DDSPlugin::getReader(
    const dds::topic::Topic<dds::core::xtypes::DynamicData> &topic,
    const std::string &qosName) {
    dds::sub::qos::DataReaderQos qos = default_provider.datareader_qos();
    if (qosName != "")
        qos = default_provider.datareader_qos(qosName);
    return dds::sub::DataReader<dds::core::xtypes::DynamicData>(
               subscriber,
               topic,
               qos,
               NULL,
               dds::core::status::StatusMask::none());
}

dds::sub::DataReader<dds::core::xtypes::DynamicData> DDSPlugin::getReader_w_listener(
        const dds::topic::Topic<dds::core::xtypes::DynamicData> &topic,
        const std::shared_ptr<BaseDataReaderListener<dds::core::xtypes::DynamicData>> listener,
        const dds::core::status::StatusMask status,
        const std::string &qosName)
{
    dds::sub::qos::DataReaderQos qos = default_provider.datareader_qos();
    if (qosName != "")
        qos = default_provider.datareader_qos(qosName);
    return dds::sub::DataReader<dds::core::xtypes::DynamicData>(
               subscriber,
               topic,
               qos,
               listener,
               status);
}

dds::sub::DataReader<dds::core::xtypes::DynamicData> DDSPlugin::getFilterReader(
    const dds::topic::ContentFilteredTopic<dds::core::xtypes::DynamicData> &topic,
    const std::string &qosName) {
    dds::sub::qos::DataReaderQos qos = default_provider.datareader_qos();
    if (qosName != "")
        qos = default_provider.datareader_qos(qosName);
    return dds::sub::DataReader<dds::core::xtypes::DynamicData>(
               subscriber,
               topic,
               qos,
               NULL,
               dds::core::status::StatusMask::none());
}

dds::sub::DataReader<dds::core::xtypes::DynamicData> DDSPlugin::getReader(
    const std::string &topicName,
    const std::string &qosName) {
    /* List of readers returned by the find function */
    std::vector<dds::sub::DataReader<dds::core::xtypes::DynamicData>> readers;

    /* Get the list of readers */
    int reader_count = dds::sub::find <
                       dds::sub::DataReader<dds::core::xtypes::DynamicData >> (
                           subscriber,
                           topicName,
                           std::back_inserter(readers));

    /* All we need is at least one reader. If there are multiple let’s use the
       first one returned. If no readers are found we create one
    */
    if (reader_count) {
        return readers[0];
    } else {
        dds::sub::qos::DataReaderQos qos = default_provider.datareader_qos();
        if (qosName != "")
            qos = default_provider.datareader_qos(qosName);
        return dds::sub::DataReader<dds::core::xtypes::DynamicData>(
                   subscriber,
                   getTopic(topicName),
                   qos);
    }
}

dds::sub::DataReader<dds::core::xtypes::DynamicData> DDSPlugin::getReader(
    const dds::sub::Subscriber &subscriber,
    const std::string &topicName,
    const dds::core::xtypes::DynamicType &typeObject,
    const std::string &topQosName,
    const std::string &qosName) {
    /* List of readers returned by the find function */
    std::vector<dds::sub::DataReader<dds::core::xtypes::DynamicData>> readers;

    /* Get the list of readers */
    int reader_count = dds::sub::find <
                       dds::sub::DataReader<dds::core::xtypes::DynamicData >> (
                           subscriber,
                           topicName,
                           std::back_inserter(readers));

    /* All we need is at least one reader. If there are multiple let’s use the
       first one returned. If no readers are found we create one
    */
    if (reader_count) {
        return readers[0];
    } else {
        dds::sub::qos::DataReaderQos qos = default_provider.datareader_qos();
        if (qosName != "")
            qos = default_provider.datareader_qos(qosName);
        return dds::sub::DataReader<dds::core::xtypes::DynamicData>(
                   subscriber,
                   getTopic(topicName, typeObject, topQosName),
                   qos,
                   NULL,
                   dds::core::status::StatusMask::none());
    }
}

dds::pub::DataWriter<dds::core::xtypes::DynamicData> DDSPlugin::getWriter(
    const std::string& entityName,
    const dds::topic::Topic<dds::core::xtypes::DynamicData> &topic,
    const std::string &qosName) {
    dds::pub::qos::DataWriterQos qos = default_provider.datawriter_qos();
    if(qosName != "")
        qos = default_provider.datawriter_qos(qosName);
    qos << rti::core::policy::EntityName(entityName);

    return dds::pub::DataWriter<dds::core::xtypes::DynamicData>(
               publisher,
               topic,
               qos,
               NULL,
               dds::core::status::StatusMask::none());
}
