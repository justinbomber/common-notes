#include "testpubobject.h"

TestPubTypeObject::TestPubTypeObject() {
}

void TestPubTypeObject::setDomainID(unsigned int id) {
    domain_id = id;
}

void TestPubTypeObject::wd_start() {
    run_status = false;
}

void TestPubTypeObject::wd_stop() {
    run_status = true;
}

void TestPubTypeObject::thread_stop() {
    thread_status = false;
}

void TestPubTypeObject::thread_start() {
    thread_status = true;
}

void TestPubTypeObject::setQosName(const std::string &name) {
    qosName = name;
}

void TestPubTypeObject::run() {
    qDebug() << "call TestPubTypeObject.run";

    QString pluginName = "DDSPlugin";
    DDSInterface *plug = qobject_cast<DDSInterface *>(PluginManager::instance()->getPluginObject(pluginName));
    if (plug != nullptr) {
        rti::config::Verbosity verbosity(rti::config::Verbosity::WARNING);
        rti::config::Logger::instance().verbosity(verbosity);
//        NDDSConfigLogger::get_instance()->set_verbosity(verbosity);
        try {

            dds::domain::DomainParticipant participant = plug->getDomainParticipant(domain_id,
                    "Test.xml",
                    "test_library::extTest");
            qDebug() << "TestPubTypeObject getDomainParticipant";

            const dds::core::xtypes::DynamicType& typeObject = plug->getTypeObject("Test", "Test.xml");

            dds::topic::Topic<dds::core::xtypes::DynamicData> topic = plug->getTopic(participant, "TestTopic", typeObject, "");

            qDebug() << "TestPubTypeObject getTypeObject";
            dds::pub::DataWriter<dds::core::xtypes::DynamicData> writer = plug->getWriter("implicit::AAA", topic,
                    qosName);
//            writer.retain();
            qDebug() << "TestPubTypeObject getWriter::" << QString::fromStdString(qosName);

            dds::core::xtypes::DynamicData sample(typeObject);

            qDebug() << "TestPubTypeObject new sample";
            int i = 0;
            sample.value("idx", ++i);
            // ***
//            dds::core::InstanceHandle instance = writer.register_instance(sample);

            while (thread_status) {
                while (!run_status) {
                    sample.value("my_long", 23);                     // my_long = 23
                    sample.value<std::string>("my_string", "hello"); // my_string = hello
                    // 2) Obtain the loaned member and assign its values
                    rti::core::xtypes::LoanedDynamicData loaned_member = sample.loan_value("my_foo");
                    loaned_member.get().value("x", 2);
                    loaned_member.get().value("y", 3);
                    loaned_member.return_loan(); // The destructor would do this as well.
                    // To set the values of my_sequence we can use a vector
                    std::vector<int32_t> sequence_values;
                    sequence_values.push_back(10);
                    sequence_values.push_back(20);
                    sequence_values.push_back(30);
                    sample.set_values("my_sequence", sequence_values);
                    // To set the values of my_array, since the element type is another struct
                    // we will use a LoanedDynamicData to access the array elements. To modify
                    // each element we will use another LoanedDynamicData
                    loaned_member = sample.loan_value("my_array");
                    rti::core::xtypes::LoanedDynamicData array_element = loaned_member.get().loan_value(1);
                    array_element.get().value("x", 10 + i * 2);
                    array_element.get().value("y", 11 - i);
                    loaned_member.get().loan_value(array_element, 2); // reuse array_element
                    array_element.get().value("x", 20 + i);
                    array_element.get().value("y", 21 - 1);
                    array_element.return_loan();
                    loaned_member.return_loan();
                    // We're not setting the remaining 3 elements, so they will have default values
                    // We manipulate optional members as regular members. By setting a value
                    // we are implicitly asserting its presence in the sample.
                    //                sample.value("my_optional", foo_data);
                    // This is how we can unset it:
                    sample.clear_optional_member("my_optional");

                    // Finally we just write the sample
                    writer.write(sample);
                    rti::util::sleep(dds::core::Duration::from_millisecs(1000));
                    i++;
                }
            }
//            if(run_status)
//            {
//                writer.unregister_instance(instance);
//            }
        } catch (std::exception &ex) {
            qCritical() << ex.what();
            quit();
        }
    } else
        qCritical() << "plug is null";
//    qDebug() << "thread quit " << QString::fromStdString(qosName);
    quit();
}
