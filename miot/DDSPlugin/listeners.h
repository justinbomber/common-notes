#ifndef LISTENERS_H
#define LISTENERS_H

#include <dds/pub/ddspub.hpp>
#include <dds/sub/ddssub.hpp>

// 2023/04/23 added by lunglin
template <typename T>
class BaseDataReaderListener : public dds::sub::NoOpDataReaderListener<T>
{
public:
    BaseDataReaderListener() {}
    // Notifications about data
    virtual void on_requested_deadline_missed(
        dds::sub::DataReader<T> &reader,
        const dds::core::status::RequestedDeadlineMissedStatus &status)
    {
        std::cout << "on_requested_deadline_missed callback" << std::endl;
        // Access information from the status
        std::cout << "total_count = " << status.total_count() << std::endl;
        std::cout << "total_count_change = " << status.total_count_change() << std::endl;
        dds::core::InstanceHandle handle = status.last_instance_handle();
    }
    virtual void on_sample_rejected(
        dds::sub::DataReader<T> &reader,
        const dds::core::status::SampleRejectedStatus &status)
    {
        std::cout << "on_sample_rejected callback" << std::endl;
        // Access information from the status
        std::cout << "total_count = " << status.total_count() << std::endl;
        std::cout << "total_count_change = " << status.total_count_change() << std::endl;
        std::cout << "last_reason = " << status.last_reason() << std::endl;
        dds::core::InstanceHandle handle = status.last_instance_handle();
    }
    virtual void on_sample_lost(
        dds::sub::DataReader<T> &reader,
        const dds::core::status::SampleLostStatus &status)
    {
        std::cout << "on_sample_lost callback" << std::endl;
        // Access information from the status
        std::cout << "total_count = " << status.total_count() << std::endl;
        std::cout << "total_count_change = " << status.total_count_change() << std::endl;
    }
    // Notifications about DataWriters
    virtual void on_requested_incompatible_qos(
        dds::sub::DataReader<T> &reader,
        const dds::core::status::RequestedIncompatibleQosStatus &status)
    {
        std::cout << "on_requested_incompatible_qos callback" << std::endl;
        // Access information from the status
        std::cout << "total_count = " << status.total_count() << std::endl;
        std::cout << "total_count_change = " << status.total_count_change() << std::endl;
        std::cout << "last_policy_id = " << status.last_policy_id() << std::endl;
        dds::core::policy::QosPolicyCountSeq qos_seq = status.policies();
        if (qos_seq.size() > 0)
        {
            std::cout << "policy_id of one first incompatible Qos policy = "
                      << qos_seq[0].policy_id() << std::endl;
        }
    }
    virtual void on_subscription_matched(
        dds::sub::DataReader<T> &reader,
        const dds::core::status::SubscriptionMatchedStatus &status)
    {
        std::cout << "on_subscription_matched callback" << std::endl;
        // Access information from the status
        std::cout << "total_count = " << status.total_count() << std::endl;
        std::cout << "total_count_change = " << status.total_count_change() << std::endl;
        std::cout << "current_count = " << status.current_count() << std::endl;
        std::cout << "current_count_change = " << status.current_count_change() << std::endl;
        dds::core::InstanceHandle handle = status.last_publication_handle();
        // Extension
        std::cout << "current_count_peak = "
                  << status.extensions().current_count_peak() << std::endl;

        if (status.current_count_change() > 0)
        {
            dds::topic::PublicationBuiltinTopicData publicationData = dds::sub::matched_publication_data<T>(reader, handle);

            if (publicationData.extensions().service().kind() == rti::core::policy::ServiceKind::QUEUING)
            {
                std::cout << "set foundQS = true\n";
            }
        }
    }
    virtual void on_liveliness_changed(
        dds::sub::DataReader<T> &reader,
        const dds::core::status::LivelinessChangedStatus &status)
    {
        std::cout << "on_liveliness_changed callback" << std::endl;
        // Access information from the status
        std::cout << "alive_count = " << status.alive_count() << std::endl;
        std::cout << "not_alive_count = " << status.not_alive_count() << std::endl;
        std::cout << "alive_count_change = " << status.alive_count_change() << std::endl;
        std::cout << "not_alive_count_change = " << status.not_alive_count_change() << std::endl;
        dds::core::InstanceHandle handle = status.last_publication_handle();
    }
    virtual void on_data_available(dds::sub::DataReader<T> &reader)
    {
        std::cout << "on_data_availabled callback" << std::endl;
    }
};
#endif // LISTENERS_H
