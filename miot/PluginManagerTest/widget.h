#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "pluginmanager.h"
#include "testpubobject.h"
#include "testsubobject.h"
#include "cbl++/CouchbaseLite.hh"
#include "fleece/Fleece.hh"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget {
    Q_OBJECT

  public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    TestPubTypeObject wd_thread;
    TestPubTypeObject wd_thread1;
    TestSubTypeObject rd_thread;

  private slots:
    void on_btnSum_clicked();

    void on_btnClose_clicked();

    void on_btnUnload_clicked();

    void on_btnLoad_clicked();

    void on_btnSetDomain_clicked();

    void on_btnSetPartition_clicked();

    void on_btnPub_clicked();

    void on_btnStopPub_clicked();

    void on_btnStopThread_clicked();

    void on_btnSub_clicked();

    void on_btnStopSub_clicked();

    void on_btnAddPubPartitions_clicked();

    void on_btnAddSubPartitions_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_5_clicked();

    void on_btnBatterystatus_clicked();

    void on_btnRepStart_clicked();

    void on_btnRepStop_clicked();

  private:
    Ui::Widget *ui;
    int32_t domain_id;

    PluginManager *pluginManager;
    cbl::Database appDB;
    QString path;
    cbl::Replicator repSyncTest;
};
#endif // WIDGET_H
