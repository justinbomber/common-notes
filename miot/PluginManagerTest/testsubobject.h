#ifndef TESTSUBOBJECT_H
#define TESTSUBOBJECT_H

#include <QThread>
#include <dds/pub/DataWriter.hpp>
#include "pluginmanager.h"
#include "ddsinterface.h"

class TestSubTypeObject : public QThread
{
    Q_OBJECT
private:
    unsigned int domain_id;
    bool run_status = false;
    bool thread_status = true;
protected:
    void run() override;
public:
    TestSubTypeObject();
    void setDomainID(unsigned int id);
    void rd_start();
    void rd_stop();
    void thread_stop();
    void thread_start();

};

#endif // TESTSUBOBJECT_H
