#include "widget.h"
#include "pluginmanager.h"

#include <QApplication>
#include <QFile>
#include <QMutex> // 互鎖標頭檔案
#include <QTextStream>
#include <QDateTime>
#include <QLoggingCategory>

void MessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    static QMutex mutex;
    mutex.lock();

    QString text;
    switch (type) {
    case QtDebugMsg:
        text = QString("Debug:");
        break;
    case QtInfoMsg:
        text = QString("Info:");
        break;
    case QtWarningMsg:
        text = QString("Warning:");
        break;
    case QtCriticalMsg:
        text = QString("Critical:");
        break;
    case QtFatalMsg:
        text = QString("Fatal:");
    default:
        break;
    }

    QString context_info = QString("File:(%1) Line:(%2)").arg(QString(context.file)).arg(context.line);
    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
    QString currentDate = QString("(%1)").arg(currentTime);
    QString message = QString::fromLocal8Bit("【%1】%2 %3 %4").arg(currentTime).arg(text).arg(msg).arg(context_info);

    QString path = QCoreApplication::applicationDirPath();
    QFile file(path + "/logs/log.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream textStream(&file);
    textStream << message << "\r\n";
    file.flush();
    file.close();

    mutex.unlock();
}

int main(int argc, char *argv[]) {
#ifdef QT_NO_DEBUG
    QLoggingCategory::setFilterRules("*.debug=false\n"
                                     "*.info=true\n"
                                     "*.warning=true\n"
                                     "*.critical=true");
#endif
    qInstallMessageHandler(MessageHandler);
    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
