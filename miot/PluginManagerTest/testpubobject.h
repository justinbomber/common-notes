#ifndef TESTPUBOBJECT_H
#define TESTPUBOBJECT_H

#include <QThread>
#include <dds/pub/DataWriter.hpp>
// #include <DataWriter.hpp>
#include "pluginmanager.h"
#include "ddsinterface.h"




class TestPubTypeObject : public QThread {
    Q_OBJECT
  private:
    unsigned int domain_id;
    bool run_status = false;
    bool thread_status = true;
    std::string qosName;

  protected:
    void run() override;

  public:
    TestPubTypeObject();
    void setDomainID(unsigned int id);
    void wd_start();
    void wd_stop();
    void thread_stop();
    void thread_start();
    void setQosName(const std::string & name);
};

#endif // TESTPUBOBJECT_H
