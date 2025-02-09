#include "Environ.h"
#include <QCoreApplication>
#include <string>
#include <QDir>

Environ::Environ() {
    if (std::string msg; setHomePath(msg)) {
        setDbPath();
    }
    else {
        exit(-1);
    }
}

std::string Environ::getHomePath() {
    return homePath;
}

std::string Environ::getDbPath() {
    return dbPath;
}

// 设置home路径函数，返回是否成功设置
bool Environ::setHomePath(std::string& msg) {
    // 使用 Qt 的 QCoreApplication 类获取应用程序的目录路径
    QString appPath = QCoreApplication::applicationDirPath();
    // 使用 QDir 获取父目录
    QDir dir(appPath);
    if (dir.cdUp()) {
        // 将 QString 转换为 std::string
        homePath = dir.absolutePath().toStdString();
        // 假设获取成功
        msg = "Success";
        return true;
    } else {
        msg = "Failed to get parent directory";
        return false;
    }
}

void Environ::setDbPath() {
    // 将 homePath 转换为 QString 以便使用 Qt 的路径操作
    QString qHomePath = QString::fromStdString(homePath);
    // 拼接数据库路径
    QString qDbPath = qHomePath + "/data/bookms.db";
    // 将结果转换为 std::string 存储在 dbPath 中
    dbPath = qDbPath.toStdString();
}
