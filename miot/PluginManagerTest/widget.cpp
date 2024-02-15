#include "widget.h"
#include "ui_widget.h"
#include "calculateinterface.h"
#include "ddsinterface.h"
#include "cblinterface.h"
#include "utilsinterface.h"
#include <dds/core/QosProvider.hpp>
// 當要在字串後面加上；"_sl"來表示字串為fleece::slice時需加上以下敘述
// 1. #include <fleece/Fleece.hh>
// 2. #include <fleece/Mutable.hh>
// 3. using namespace fleece;

// using namespace QtCharts;

Widget::Widget(QWidget *parent)
    : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    path = QCoreApplication::applicationDirPath();
    ui->leLib->setText(path + "/plugins/libCalculatePlugin.so");
    QString errMsg = "";
    pluginManager = PluginManager::instance();
    pluginManager->loadAllPlugins(QCoreApplication::applicationDirPath()+"/plugins/",
                                  "{\"plugins\":[\"libCBLPlugin.so\",\"libDDSPlugin.so\",\"libUtilsPlugin.so\",\"libCalculatePlugin.so\"]}", errMsg);
    if (!errMsg.isEmpty())
    {
        this->close();
        qCritical() << errMsg;
        return;
    }
    QString pluginName = "CBLPlugin";
    CBLInterface *plug = qobject_cast<CBLInterface *>(pluginManager->getPluginObject(pluginName));
    if (plug == nullptr)
    {
        qDebug() << "plugin object is null";
        return;
    }
    else
    {
        try
        {
            appDB = plug->getAppDB("VMD.cbl", "/home/justin/project/build/MIoT_Plugins-Desktop_Qt_6_6_1_GCC_64bit-Debug/db/", false);
            if (!appDB)
            {
                qCritical() << "Error open appDB";
                appDB.close();
                appDB = nullptr;
            }
        }
        catch (CBLError &err)
        {
            this->close();
            qCritical() << "initializing database error domain = " << err.domain << ": Code = " << err.code;
        }
    }
}

Widget::~Widget()
{
    if (wd_thread.isRunning())
    {
        wd_thread.wd_stop();
        wd_thread.thread_stop();
        wd_thread.wait();
        wd_thread.deleteLater();
    }
    if (wd_thread1.isRunning())
    {
        wd_thread1.wd_stop();
        wd_thread1.thread_stop();
        wd_thread1.wait();
        wd_thread1.deleteLater();
    }
    if (rd_thread.isRunning())
    {
        rd_thread.rd_stop();
        rd_thread.thread_stop();
        rd_thread.wait();
        rd_thread.deleteLater();
    }
    if (pluginManager)
    {
        pluginManager->unloadAllPlugins();
        delete pluginManager;
        pluginManager = nullptr;
    }
    appDB.close();
    appDB = nullptr;
    delete ui;
}

void Widget::on_btnSum_clicked()
{
    QString pluginName = "CalculatePlugin";
    CalculateInterface *plug = qobject_cast<CalculateInterface *>(pluginManager->getPluginObject(pluginName));
    if (plug == nullptr)
    {
        qDebug() << "loader is null";
        return;
    }
    else
    {
        ui->leSum->setText(QString::number(plug->getSum(ui->leX->text().toDouble(), ui->leY->text().toDouble())));
    }
}

void Widget::on_btnClose_clicked()
{
    this->close();
}

void Widget::on_btnUnload_clicked()
{
    pluginManager->unloadPlugin(ui->leLib->text());
}

void Widget::on_btnLoad_clicked()
{
    pluginManager->loadPlugin(ui->leLib->text());
}

void Widget::on_btnSetDomain_clicked()
{
    QString pluginName = "DDSPlugin";
    DDSInterface *plug = qobject_cast<DDSInterface *>(pluginManager->getPluginObject(pluginName));
    if (plug == nullptr)
    {
        qDebug() << "plugin object is null";
        return;
    }
    else
    {
        try
        {
            dds::domain::DomainParticipant participant = plug->getDomainParticipant(ui->leDomainID->text().toInt(),
                                                                                    "Test.xml",
                                                                                    "test_library::extTest");
        }
        catch (std::exception &ex)
        {
            qCritical() << ex.what();
        }
    }
}

void Widget::on_btnSetPartition_clicked()
{
    QString pluginName = "DDSPlugin";
    DDSInterface *plug = qobject_cast<DDSInterface *>(pluginManager->getPluginObject(pluginName));
    if (plug == nullptr)
    {
        qDebug() << "plugin object is null";
        return;
    }
    else
    {
        try
        {
            dds::domain::DomainParticipant participant = plug->getDomainParticipant(ui->leDomainID->text().toInt(),
                                                                                    "Test.xml",
                                                                                    "test_library::test");
            plug->setPubPartition(ui->lePartition->text().toStdString());

            plug->setSubPartition(ui->lePartition->text().toStdString());
        }
        catch (std::exception &ex)
        {
            qDebug() << ex.what();
        }
    }
}

void Widget::on_btnPub_clicked()
{
    wd_thread.setQosName("test_library::extTest::W1");
    wd_thread.setDomainID(ui->leDomainID->text().toInt());
    wd_thread.thread_start();
    wd_thread.wd_start();
    if (!wd_thread.isRunning())
        wd_thread.start();
    wd_thread1.setQosName("test_library::extTest::W2");
    wd_thread1.setDomainID(ui->leDomainID->text().toInt());
    wd_thread1.thread_start();
    wd_thread1.wd_start();
    if (!wd_thread1.isRunning())
        wd_thread1.start();
}

void Widget::on_btnStopPub_clicked()
{
    if (wd_thread.isRunning())
    {
        wd_thread.wd_stop();
        wd_thread.thread_stop();
        wd_thread.wait();
    }
    if (!wd_thread.isRunning())
    {
        QString pluginName = "DDSPlugin";
        DDSInterface *plug = qobject_cast<DDSInterface *>(pluginManager->getPluginObject(pluginName));
        try
        {
            dds::domain::DomainParticipant participant = plug->getDomainParticipant(ui->leDomainID->text().toInt(),
                                                                                    "Test.xml",
                                                                                    "test_library::test");
            dds::pub::Publisher pub = plug->getPublisher();
            std::string entityName = "implicit::AAA";
            dds::pub::DataWriter<dds::core::xtypes::DynamicData> writer =
                rti::pub::find_datawriter_by_name<
                    dds::pub::DataWriter<dds::core::xtypes::DynamicData>>(
                    pub,
                    entityName);
            if (writer != dds::core::null)
                qDebug() << "Found writer";
            else
                qDebug() << "writer is null";
        }
        catch (std::exception &ex)
        {
            qDebug() << ex.what();
        }
    }
}

void Widget::on_btnStopThread_clicked()
{
    if (wd_thread.isRunning())
    {
        wd_thread.wd_stop();
        wd_thread.thread_stop();
        wd_thread.wait();
    }
    if (wd_thread1.isRunning())
    {
        wd_thread1.wd_stop();
        wd_thread1.thread_stop();
        wd_thread1.wait();
    }
    if (rd_thread.isRunning())
    {
        rd_thread.rd_stop();
        rd_thread.thread_stop();
        rd_thread.wait();
    }
}

void Widget::on_btnSub_clicked()
{
    rd_thread.setDomainID(ui->leDomainID->text().toInt());
    rd_thread.thread_start();
    rd_thread.rd_start();

    if (!rd_thread.isRunning())
        rd_thread.start();
}

void Widget::on_btnStopSub_clicked()
{
    if (rd_thread.isRunning())
    {
        rd_thread.rd_stop();
        rd_thread.thread_stop();
        rd_thread.wait();
    }
}

void Widget::on_btnAddPubPartitions_clicked()
{
    QString pluginName = "DDSPlugin";
    DDSInterface *plug = qobject_cast<DDSInterface *>(pluginManager->getPluginObject(pluginName));
    if (plug == nullptr)
    {
        qCritical() << "plugin object is null";
        return;
    }
    else
    {
        try
        {
            dds::domain::DomainParticipant participant = plug->getDomainParticipant(ui->leDomainID->text().toInt(),
                                                                                    "Test.xml",
                                                                                    "test_library::test");
            plug->addPubPartition(ui->lePartitions->text().toStdString(), ',');
        }
        catch (std::exception &ex)
        {
            qCritical() << ex.what();
        }
    }
}

void Widget::on_btnAddSubPartitions_clicked()
{
    QString pluginName = "DDSPlugin";
    DDSInterface *plug = qobject_cast<DDSInterface *>(pluginManager->getPluginObject(pluginName));
    if (plug == nullptr)
    {
        qCritical() << "plugin object is null";
        return;
    }
    else
    {
        try
        {
            dds::domain::DomainParticipant participant = plug->getDomainParticipant(ui->leDomainID->text().toInt(),
                                                                                    "Test.xml",
                                                                                    "test_library::test");
            plug->addSubPartition(ui->lePartitions->text().toStdString(), ',');
        }
        catch (std::exception &ex)
        {
            qCritical() << ex.what();
        }
    }
}

void Widget::on_pushButton_clicked()
{
    QString pluginName = "CBLPlugin";
    CBLInterface *plug = qobject_cast<CBLInterface *>(pluginManager->getPluginObject(pluginName));
    if (plug == nullptr)
    {
        qCritical() << "plugin object is null";
        return;
    }
    else
    {
        try
        {
            cbl::Database appDB = plug->getAppDB("VMD.cbl", "/home/lunglin/Documents/GitHub/VMD/db/", false);
            if (!appDB)
                qDebug() << "appDB not exists";
            else
            {
                qDebug() << "Close appDB";
                appDB.close();
                appDB = nullptr;
            }
        }
        catch (std::exception &ex)
        {
            qCritical() << ex.what();
        }
    }
}

void Widget::on_pushButton_3_clicked()
{
    QString pluginName = "CBLPlugin";
    CBLInterface *plug = qobject_cast<CBLInterface *>(pluginManager->getPluginObject(pluginName));
    if (plug == nullptr)
    {
        qCritical() << "plugin object is null";
        return;
    }
    else
    {
        //        cbl::Transaction trans(appDB);
        std::string uuid;
        //        for(int i = 0; i < 10; i++) {
        std::string result;
//        std::string sqlstr = "{\"data_source\":\"MDSConnectivity\",\"vmd_id\":\"c97c628b-d801-4732-9906-5306ab0a42c0\",\"patient_id\":\"AH-3652\",\"room_id\":\"ROOM-A\",\"bed_id\":\"BED-ID0001\",\"source_timestamp\":1611254635}";
        //      std::string sqlstr="{\"name\":{\"first\":\"輝隆\",\"last\":\"林\"},\"gender\":\"female\",\"birthday\":\"1990-09-02\",\"contact\":{\"address\":{\"street\":\"20 14th St\",\"zip\":\"67761\",\"city\":\"Wallace\",\"state\":\"KS\"},\"email\":[\"rickie.bakaler@nosql-matters.org\"],\"region\":\"785\",\"phone\":[\"785-8179421\",\"785-2447694\"]},\"likes\":[\"biking\"],\"memberSince\":\"2008-01-11\"}";
        std::string sqlstr="{\"data_source\":\"VMD\",\"vmd_id\":\"1299f22f-b5e3-4eac-bab6-1eda3b3caaab\",\"source_timestamp\":1661841948}";
        plug->saveMutableDocument(appDB, sqlstr, uuid, result);
        if (result != "IP200")
        {
            qCritical() << result.c_str();
            //                break;
        }
        //        }
        //        trans.commit();
    }
}

void Widget::on_pushButton_4_clicked()
{
    QString pluginName = "CBLPlugin";
    CBLInterface *plug = qobject_cast<CBLInterface *>(pluginManager->getPluginObject(pluginName));
    if (plug == nullptr)
    {
        qCritical() << "plugin object is null";
        return;
    }
    else
    {
        try
        {
            std::string msg;
            int64_t expiration = QDateTime::currentMSecsSinceEpoch() + 6 * 60 * 1000;
            qDebug() << "expiration =" << expiration;
            // META(patient).expiration IS NOT VALUED 相當於IS NULL
            cbl::ResultSet results = plug->queryDocuments(appDB, "SELECT DISTINCT patient.name, META(patient).id, META() as metadata FROM _ AS patient WHERE patient.birthday like '199%' ORDER BY patient.birthday", msg);
            qDebug() << "msg =" << msg.c_str();
            if (msg == "IP200")
            {
                // start transaction

                for (auto &result : results)
                {

                    fleece::Value name = result[0];
                    fleece::Value id = result[1];
                    fleece::Dict metadata = result[2].asDict();
                    fleece::Dict dict = name.asDict();
                    qDebug() << metadata["id"].asstring().c_str() << ","
                             << dict["first"].asstring().c_str() << "_" << dict["last"].asstring().c_str();
                    plug->setDocExpiration(appDB, id.asstring(), expiration, msg);
                    qDebug() << msg.c_str();
                }
            }
            else
            {
                qCritical() << msg.c_str();
            }
        }
        catch (std::exception &ex)
        {
            qCritical() << ex.what();
        }
    }
}

void Widget::on_pushButton_2_clicked()
{
    QString pluginName = "CBLPlugin";
    CBLInterface *plug = qobject_cast<CBLInterface *>(pluginManager->getPluginObject(pluginName));
    if (plug == nullptr)
    {
        qCritical() << "plugin object is null";
        return;
    }
    else
    {
        std::string uuid;
        std::string result;
        plug->jsonImport(appDB, path.toStdString() + "/name.json", uuid, result);
        if (result != "IP200")
            qCritical() << result.c_str();
    }
}

void Widget::on_pushButton_5_clicked()
{
    QString pluginName = "CBLPlugin";
    CBLInterface *plug = qobject_cast<CBLInterface *>(pluginManager->getPluginObject(pluginName));
    if (plug == nullptr)
    {
        qDebug() << "plugin object is null";
        return;
    }
    else
    {
        std::string result;
        plug->jsonExport(appDB, path.toStdString() + "/output/export.json", "SELECT meta(patient).id, patient FROM _ AS patient WHERE patient.birthday like '199%' ORDER BY patient.birthday", result);
        if (result != "IP300")
            qCritical() << result.c_str();
    }
}

void Widget::on_btnBatterystatus_clicked()
{
    QString pluginName = "UtilsPlugin";
    UtilsInterface *plug = qobject_cast<UtilsInterface *>(pluginManager->getPluginObject(pluginName));
    if (plug == nullptr)
    {
        qDebug() << "plugin object is null";
        return;
    }
    else
    {
        qDebug() << plug->getNICNames();
        ui->lineEdit->setText(QString::number(plug->getPowerInfo().percentage) + " " + QString::number(plug->getPowerInfoProc().percentage) + " " + QString::number(plug->isInterfaceOnline("enp60s0", 2, 1)));
        ui->lineEdit_2->setText(QString::number(plug->getPowerInfo().ac_state) + " " + QString::number(plug->getPowerInfoProc().ac_state) + " " + QString::number(plug->isInterfaceOnline("enxaafe9dc017aa", 2, 2)));
    }
}

void Widget::on_btnRepStart_clicked()
{
    QString pluginName = "CBLPlugin";
    CBLInterface *plug = qobject_cast<CBLInterface *>(pluginManager->getPluginObject(pluginName));
    if (plug == nullptr)
    {
        qDebug() << pluginName << " plugin object is null";
        return;
    }
    else
    {
        // 這些設定值應該放在應用程式端的設定檔案中
        std::string vmdid = "";
        //        std::string url = "ws://10.1.1.243:4984/displayitemssyncdb";
        std::string url = "ws://10.1.1.243:4984/vmdsyncdb";
        std::string uid = "vmd_sync_user";
        std::string pwd = "openice400";
        std::string result;
        try
        {
            // get replicator from plugin api
            if (repSyncTest.ref() == nullptr)
                repSyncTest = plug->getReplicatorBasicPushOnly(appDB, url, uid, pwd, result, false);

            if (repSyncTest.status().error.code != 0)
                std::cerr << "call on_btnRepStart_clicked" << repSyncTest.status().error.code << "," << repSyncTest.status().error.domain << ":" << repSyncTest.status().error.internal_info;
            else
                // replicator start to sync
                repSyncTest.start();
            qDebug() << "after start replicator status =" << repSyncTest.status().error.code;
        }
        catch (CBLError &ex)
        {
            std::cerr << "call on_btnRepStart_clicked" << ex.code << "," << ex.domain << ":" << ex.internal_info;
        }
        catch (std::exception &ex)
        {
            std::cerr << "other exceptions:" << ex.what();
        }
    }
}

void Widget::on_btnRepStop_clicked()
{
    std::string vmdid = "";
    //    std::string url = "ws://10.1.1.241:4984/displayitemssyncdb";
    std::string url = "ws://10.1.1.243:4984/vmdsyncdb";
    std::string uid = "vmd_sync_user";
    std::string pwd = "openice400";
    try
    {
        if (repSyncTest.status().error.code != 0)
            std::cerr << "call on_btnRepStart_clicked" << repSyncTest.status().error.code << "," << repSyncTest.status().error.domain << ":" << repSyncTest.status().error.internal_info;
        else
            // replicator stop to sync
            repSyncTest.stop();
    }
    catch (CBLError &ex)
    {
        std::cerr << "call on_btnRepStop_clicked" << ex.code << "," << ex.domain << ":" << ex.internal_info;
    }
    catch (std::exception &ex)
    {
        std::cerr << "other exceptions:" << ex.what();
    }
}
