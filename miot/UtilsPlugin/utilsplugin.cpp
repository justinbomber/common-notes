#include "utilsplugin.h"
#include <QUuid>
#include <QDebug>
#include <QProcess>

#ifdef __cplusplus
extern "C" {
#endif

#include <libacpi.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#ifdef __cplusplus
}
#endif

UtilsPlugin::UtilsPlugin() {
}

std::string UtilsPlugin::getUUID() const {
    return QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString();
}

powerInfo UtilsPlugin::getPowerInfo() const {
    // Battery
    powerInfo binfo;
    binfo.percentage = 0;
    binfo.charge_time = 0;
    binfo.remaining_time = 0;
    binfo.ac_state = 0;
    // We allocate memory for the global data structure of the library
    global_t *global = static_cast<global_t*>(malloc(sizeof(global_t)));

    if(check_acpi_support() == SUCCESS) {
        // Initialize the structure for
        init_acpi_batt(global);     // Batteries
        init_acpi_acadapt(global);  // AC adapter

        // AC Adapter
        read_acpi_acstate(global);  // Read State
        adapter_t *ac = &global->adapt;
        binfo.ac_state = ac->ac_state;
        qInfo() << "II501:AC Adatper status =" << binfo.ac_state;

        qDebug() << "DI501:battery batt_count =" << global->batt_count;
        int batteryCount = 0;
        for(int i = 0; i < global->batt_count; i++) {
            read_acpi_batt(i);
            if(batteries[i].percentage > 0) {
                // 百分比相加後平均（當多個電池時）
                binfo.percentage += batteries[i].percentage;
                // 以最大充電時間為基準
                binfo.charge_time = (batteries[i].charge_time < binfo.charge_time ? binfo.charge_time : batteries[i].charge_time);
                // 以最大可用電時間為基準
                binfo.remaining_time = (batteries[i].remaining_time < binfo.remaining_time ? binfo.remaining_time : batteries[i].remaining_time);
                batteryCount++;
            }
        }
        if(batteryCount > 1)
            binfo.percentage = binfo.percentage / batteryCount;

        qInfo() <<  "II500:Battery Percentage =" << binfo.percentage << "%";

    } else {
        // Not Supported ACPI
        binfo.percentage = -1;
    }

    free(global);

    return binfo;
}

powerInfo UtilsPlugin::getPowerInfoProc() const {
    QProcess powerProc;
    powerInfo binfo;
    binfo.percentage = 0;
    binfo.charge_time = 0;
    binfo.remaining_time = 0;
    binfo.ac_state = 0;
    powerProc.start("acpi", QStringList() << "-ab");
    if(!powerProc.waitForFinished())
        return binfo;
    QByteArray result = powerProc.readAll();
    qDebug() << "DI502:result = " << QString(result);
    QList<QByteArray> lst = result.split('\n');
    for(int i = 0; i < lst.length(); i++) {
        qDebug() << "DI502:lst =" << QString(lst[i]);
        QString tmp = QString(lst[i]);
        if(tmp != "") {
            if(tmp.contains("Battery")) {
                binfo.percentage = tmp.split(",")[1].replace("%", "").replace(" ", "").toInt();
            }
            if(tmp.contains("Adapter") && tmp.split(":")[1].replace(" ", "") == "on-line") {
                binfo.ac_state = 0;
            } else {
                binfo.ac_state = 1;
            }
            qInfo() << "II502: Power percentage =" << binfo.percentage;
            qInfo() << "II502: AC Adapter state =" << binfo.ac_state;
        }
    }
    return binfo;
}

QStringList UtilsPlugin::getNICNames() const {
    // Find interface names of NICs
    QStringList result;

    // Run the "nmcli device status" command...
    QProcess proc;
    proc.start("nmcli", QStringList() << "device" << "status");
    if(!proc.waitForFinished())
        return result;

    // result...
    QByteArray raw = proc.readAll();

    // Convert the result to the "strList" (sample),
    //
    // [0] DEVICE TYPE     STATE     CONNECTION
    // [1] enp0s8 ethernet connected Wired connection 2
    // [2] enp0s3 ethernet connected Wired connection 2
    // [3] lo     loopback unmanaged --
    // [4] (no data; length = 0)
    //
    QString tmp = raw;
    QStringList strList = tmp.split("\n");

    // Skip the 1st row. Finding interface names in strList...
    for (int index = 1; index < strList.size(); index++) {
        if (strList[index].length() > 0) {
            int pos = strList[index].indexOf(QChar(Qt::Key_Space));
            if (pos == -1) continue;

            QString nicName = strList[index].mid(0, pos);
            if (nicName.compare("lo") == 0) continue;

            result.push_back(nicName);
        }
    }

    return result;
}


bool UtilsPlugin::isInterfaceOnline(const std::string &interface, const int protocol, const int socketType) const {
    struct ifreq ifr;
    int sock = socket(protocol, socketType, IPPROTO_IP);
    qDebug() << "socket file descriptor =" << QString::number(sock);
    memset(&ifr, 0, sizeof(ifr));
    // 將介面名稱帶入資料結構ifreq
    strncpy(ifr.ifr_name, interface.c_str(), sizeof(interface));
    // 使用ioctl取得介面資訊
    if (ioctl(sock, SIOCGIFFLAGS, &ifr) < 0) {
        perror("SIOCGIFFLAGS");
        qCritical() << "FI503: Interface error - NOT_FOUND.";
    }
    close(sock);
    // IFF_UP:代表硬體有載入,但不見得有在運作
    // IFF_RUNNING:代表硬體有載入且運作中
    return !!(ifr.ifr_flags & IFF_RUNNING);
}

QJsonObject UtilsPlugin::loadJsonFile(const std::string &filepath, const std::string &mode) {
    QFile infile(QString(filepath.c_str()));
    infile.open((mode == "r" ? QIODevice::ReadOnly : QIODevice::ReadWrite) | QIODevice::Text);
    QByteArray data = infile.readAll();
    infile.close();
    QJsonObject json;
    QJsonParseError errorPtr;
    QJsonDocument doc = QJsonDocument::fromJson(data, &errorPtr);
    if (doc.isNull() || errorPtr.error != QJsonParseError::NoError)
        return json;
    else
        return doc.object();
}

void UtilsPlugin::loadJsonFile(const std::string &filepath, const std::string &mode, QJsonObject &jobj) {
    QFile infile(QString(filepath.c_str()));
    infile.open((mode == "r" ? QIODevice::ReadOnly : QIODevice::ReadWrite) | QIODevice::Text);
    QByteArray data = infile.readAll();
    infile.close();
    QJsonObject json;
    QJsonParseError errorPtr;
    QJsonDocument doc = QJsonDocument::fromJson(data, &errorPtr);
    if (doc.isNull() || errorPtr.error != QJsonParseError::NoError)
        jobj = json;
    else
        jobj = doc.object();
}

void UtilsPlugin::sqlStringFormat(const std::map<int, std::string> &data, std::string &sqlString) {
//    qDebug() << "DF111: " << sqlString.c_str();
    std::map<int, std::string>::const_reverse_iterator rit;
    for(rit=data.rbegin(); rit!=data.rend(); ++rit){
        if(sqlString.find("$" + std::to_string((*rit).first)) != std::string::npos) {
            sqlString.replace((sqlString.insert(sqlString.find("$" + std::to_string((*rit).first)), (*rit).second)).find("$" + std::to_string((*rit).first)), ((*rit).first < 10 ? 2 : 3), "");
//            qDebug() << "DF111: " << (*rit).first << ":" << sqlString.c_str();
        }
    }
    qDebug() << "DF111:" << sqlString.c_str();
}

std::string UtilsPlugin::str_toupper(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
    [](unsigned char c) {
        return std::toupper(c);    // correct
    }
                  );
    return s;
}

void UtilsPlugin::mdcValueFormatter(const std::string& dataType, const std::string& code, const float value, const std::string& unit, char* result) {
    if(dataType.compare("DeviceSettings") == 0) {
        if(code.compare("01") == 0)
            sprintf(result, (unit.empty() ? "%.0f %s" : "%.0f (%s)"), value, unit.c_str());
        else if(code.compare("04") == 0)
            sprintf(result, (unit.empty() ? "%.3f %s" : "%.3f (%s)"), value, unit.c_str());
        else if(code.compare("05") == 0)
            sprintf(result, (unit.empty() ? "%.2f %s" : "%.2f (%s)"), value, unit.c_str());
        else if(code.compare("07") == 0)
            sprintf(result, (unit.empty() ? "%.1f %s" : "%.1f (%s)"), value, unit.c_str());
        else if(code.compare("08") == 0)
            sprintf(result, (unit.empty() ? "%.1f %s" : "%.1f (%s)"), value, unit.c_str());
        else if(code.compare("09") == 0)
            sprintf(result, (unit.empty() ? "%.1f %s" : "%.1f (%s)"), value, unit.c_str());
        else if(code.compare("0B") == 0)
            sprintf(result, (unit.empty() ? "%.1f %s" : "%.1f (%s)"), value, unit.c_str());
        else if(code.compare("0C") == 0)
            sprintf(result, (unit.empty() ? "%.1f %s" : "%.1f (%s)"), value, unit.c_str());
        else if(code.compare("11") == 0)
            sprintf(result, (unit.empty() ? "%.1f %s" : "%.1f (%s)"), value, unit.c_str());
        else if(code.compare("12") == 0)
            sprintf(result, (unit.empty() ? "%.0f %s" : "%.0f (%s)"), value, unit.c_str());
        else if(code.compare("13") == 0)
            sprintf(result, (unit.empty() ? "%.1f %s" : "%.1f (%s)"), value, unit.c_str());
        else if(code.compare("29") == 0)
            sprintf(result, (unit.empty() ? "%.1f %s" : "%.1f (%s)"), value, unit.c_str());
        else if(code.compare("42") == 0)
            sprintf(result, (unit.empty() ? "%.1f %s" : "%.1f (%s)"), value, unit.c_str());
        else if(code.compare("44") == 0)
            sprintf(result, (unit.empty() ? "%.3f %s" : "%.3f (%s)"), value, unit.c_str());
        else if(code.compare("45") == 0)
            sprintf(result, (unit.empty() ? "%.1f %s" : "%.1f (%s)"), value, unit.c_str());
        else if(code.compare("4E") == 0)
            sprintf(result, (unit.empty() ? "%.0f %s" : "%.0f (%s)"), value, unit.c_str());
        else if(code.compare("4F") == 0)
            sprintf(result, (unit.empty() ? "%.0f %s" : "%.0f (%s)"), value, unit.c_str());
        else
            sprintf(result, (unit.empty() ? "%.1f %s" : "%.1f (%s)"), value, unit.c_str());
    } else if(dataType.compare("Observation") == 0) {
        if(code.compare("07") == 0)
            sprintf(result, (unit.empty() ? "%.0f %s" : "%.0f (%s)"), value, unit.c_str());
        else if(code.compare("7A") == 0)
            sprintf(result, (unit.empty() ? "%.2f %s" : "%.2f (%s)"), value, unit.c_str());
        else if(code.compare("B8") == 0)
            sprintf(result, (unit.empty() ? "%.2f %s" : "%.2f (%s)"), value, unit.c_str());
        else if(code.compare("MV") == 0)
            sprintf(result, (unit.empty() ? "%.2f %s" : "%.2f (%s)"), value, unit.c_str());
        else if(code.compare("RSI") == 0)
            sprintf(result, (unit.empty() ? "%.3f %s" : "%.3f (%s)"), value, unit.c_str());
        else if(code.compare("IER") == 0)
            sprintf(result, (unit.empty() ? "%.3f %s" : "%.3f (%s)"), value, unit.c_str());
        else
            sprintf(result, (unit.empty() ? "%.1f %s" : "%.1f (%s)"), value, unit.c_str());
    }
}
