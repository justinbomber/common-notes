#ifndef UTILSINTERFACE_H
#define UTILSINTERFACE_H

#include <QtCore>
#include "pluginmanager.h"
#include "plugininterface.h"
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <map>

#define UtilsInterface_iid "com.foyatech.miot.UtilsInterface"

typedef struct {
    /* calculated states */
    int percentage;              /**< remaining battery percentage */
    int charge_time;             /**< remaining time to fully charge the battery in minutes */
    int remaining_time;          /**< remaining battery life time in minutes */
    int ac_state;       /**< AC Adatper state 0: Runs on AC adapter, 1: Runs on Batteries, 2: Unknown Information */
} powerInfo;

class UtilsInterface : public PluginInterface {
  public:
    virtual ~UtilsInterface() {}
    /**
     * @brief 取得UUID
     * @return std::string
     */
    virtual std::string getUUID() const = 0;

    /**
     * @brief getPowerInfo 使用acpi api取得電池目前電量與AC Adapter狀態
     * @return struct powerInfo
     */
    virtual powerInfo getPowerInfo() const = 0;

    /**
     * @brief getPowerInfoProc 使用系統指令acpi -ab取得電池目前電量與AC Adapter狀態
     * @return struct powerInfo
     */
    virtual powerInfo getPowerInfoProc() const = 0;

    /**
     * @brief getNICNames 使用系統指令nmcli device status取得本機的網路實體界面名稱清單；但不包含 lo 界面名稱
     * @return QStringList; 每一個 Element 都是本機的一個網路實體界面名稱
     */
    virtual QStringList getNICNames() const = 0;

    /**
     * @brief isInterfaceOnline 檢查網路界面是否運作中(TCP)
     * @param interface 網路界面名稱
     * @param protocol 參考bits/socket.h中protocol families段落說明
     *        2: PF_INET, 10: PF_INET6
     * @param socketType
     *        1: SOCK_STREAM, 2: SOCK_DGRAM
     * @return bool 0:false 1:true
     */
    virtual bool isInterfaceOnline(const std::string &interface, const int protocol, const int socketType) const = 0;

    /**
     * @brief loadJsonFile 將json檔案轉為QJsonObject
     * @param filepath 檔案路徑與名稱
     * @param mode r: 唯讀 非r: 可讀寫
     * @return QJsonObject
     */
    virtual QJsonObject loadJsonFile(const std::string &filepath, const std::string &mode) = 0;

    /**
     * @brief loadJsonFile 將json檔案轉為QJsonObject
     * @param filepath 檔案路徑與名稱
     * @param mode r: 唯讀 非r: 可讀寫
     * @param jobj QJsonObject ref.
     * @return void
     */
    virtual void loadJsonFile(const std::string &filepath, const std::string &mode, QJsonObject &jobj) = 0;

    /**
     * @brief sqlStringFormat 將sql字串中以$開頭的變數以data中的實際資料取
     * @param data 每一個變數的實際資料值
     *    repTarget[0] = vmd_id
     *    repTarget[1] = patient_id
     *    repTarget[2] = mdc_code
     *    repTarget[3] = model
     *    repTarget[4] = source_timestamp.sec(cur)
     *    repTarget[5] = source_timestamp.nanosec(cur)
     *    repTarget[6] = limit_type
     *    repTarget[7] = channel_id
     *    repTarget[8] = code
     *    repTarget[9] = description
     *    repTarget[10] = value
     *    repTarget[11] = unit
     *    repTarget[12] = mdc_unit
     *    repTarget[13] = value(org)
     *    repTarget[14] = source_timestamp.sec(org)
     *    repTarget[15] = source_timestamp.nanosec(org)
     *    repTarget[16] = std::to_string(QDateTime::currentSecsSinceEpoch());
     *    repTarget[17] = unit
     *    repTarget[18] = mode
     *    repTarget[19] = std::to_string(QDateTime::currentSecsSinceEpoch());
     *    repTarget[20] = binding_time
     * @param sqlString 實際可執行的sql字串
     */
    virtual void sqlStringFormat(const std::map<int, std::string> &data, std::string &sqlString) = 0;

    /**
     * @brief str_toupper 將串中字元更改為大寫回傳
     * @param s
     * @return
     */
    virtual std::string str_toupper(std::string s) = 0;

    /**
     * @brief mdcValueFormatter 提供量測資料數值格式與單位串接
     * @param dataType 資料類型： Observation, DeviceSettings
     * @param code 量測項目代碼
     * @param value 量測數值
     * @param unit 量測單位
     * @param result 格式後字串
     * @note 1. 2022/10/12 配合新增計算MV與RSI新增格式設定
     *       2.            新增輸入參數dataType以區分不同資料類型代碼相同且不同格式
     */
    virtual void mdcValueFormatter(const std::string& dataType, const std::string& code, const float value, const std::string& unit, char* result) = 0;
};

Q_DECLARE_INTERFACE(UtilsInterface, UtilsInterface_iid)

#endif // UTILSINTERFACE_H
