#include "testsubobject.h"
#include <dds/core/ddscore.hpp>

TestSubTypeObject::TestSubTypeObject() {
}

void TestSubTypeObject::setDomainID(unsigned int id) {
    domain_id = id;
}

void TestSubTypeObject::rd_start() {
    run_status = false;
}

void TestSubTypeObject::rd_stop() {
    run_status = true;
}

void TestSubTypeObject::thread_stop() {
    thread_status = false;
}

void TestSubTypeObject::thread_start() {
    thread_status = true;
}

int process_data(dds::sub::DataReader<dds::core::xtypes::DynamicData> reader) {
    // Take all samples
    int count = 0;
    dds::sub::LoanedSamples<dds::core::xtypes::DynamicData> samples = reader.take();
    for (const auto& sample : samples) {
        if (sample.info().valid()) {
            count++;
            std::cout << sample.data() << std::endl;
        } else {
            std::cout << "Instance state changed to "
                      << sample.info().state().instance_state() << std::endl;
        }
    }

    return count;
}

void TestSubTypeObject::run() {
    QString pluginName = "DDSPlugin";
    DDSInterface *plug = qobject_cast<DDSInterface *>(PluginManager::instance()->getPluginObject(pluginName));
    if (plug == nullptr) {
        qDebug() << "plugin object is null";
        return;
    } else {
        rti::config::Verbosity verbosity(rti::config::Verbosity::WARNING);
        rti::config::Logger::instance().verbosity(verbosity);
        try {
            dds::domain::DomainParticipant participant = plug->getDomainParticipant(domain_id,
                    "Test.xml",
                    "test_library::extTest");
            // 由xml取得資料格式設定併產生DynamicType物件
            dds::core::xtypes::DynamicType const &typeObject = plug->getTypeObject("Test", "Test.xml");

            dds::topic::Topic<dds::core::xtypes::DynamicData> topic = plug->getTopic(participant, "TestTopic", typeObject, "");

            dds::sub::DataReader<dds::core::xtypes::DynamicData> reader = plug->getReader(topic,
                    "test_library::extTest::R1");

            dds::core::xtypes::DynamicData sample(typeObject);


            unsigned int samples_read = 0;
            dds::sub::cond::ReadCondition read_condition(
                reader,
                dds::sub::status::DataState::any(),
            [reader, &samples_read]() {
                samples_read += process_data(reader);
            });

            // WaitSet will be woken when the attached condition is triggered
            dds::core::cond::WaitSet waitset;
            waitset += read_condition;

            while(thread_status) {
                while (!run_status) {

                    std::cout << "TesTopic subscriber sleeping up to 1 sec..." << std::endl;

                    // Run the handlers of the active conditions. Wait for up to 1 second.

                    waitset.dispatch(dds::core::Duration(1));
                }
            }

            quit();
        } catch (std::exception &ex) {
            qDebug() << ex.what();
            quit();
        }
    }
}
