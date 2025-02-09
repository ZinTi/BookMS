#include "datetime.h"
#include <chrono>
#include <iomanip>
#include <sstream>

std::string getCurrentDateTime() {
    // 获取当前系统时钟的时间点
    const auto now = std::chrono::system_clock::now();
    // 将时间点转换为 time_t 类型
    const std::time_t time = std::chrono::system_clock::to_time_t(now);
    // 转换为本地时间
    const std::tm tm = *std::localtime(&time);
    std::stringstream ss;
    // 按照 yyy-MM-dd HH:mm:ss 的格式输出时间
    ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::string getCurrentDate() {
    // 获取当前系统时钟的时间点
    const auto now = std::chrono::system_clock::now();
    // 将时间点转换为 time_t 类型
    const std::time_t time = std::chrono::system_clock::to_time_t(now);
    // 转换为本地时间
    const std::tm tm = *std::localtime(&time);
    std::stringstream ss;
    // 按照 yyyy-MM-dd 的格式输出日期
    ss << std::put_time(&tm, "%Y-%m-%d");
    return ss.str();
}

std::string getCurrentTime() {
    // 获取当前系统时钟的时间点
    const auto now = std::chrono::system_clock::now();
    // 将时间点转换为 time_t 类型
    const std::time_t time = std::chrono::system_clock::to_time_t(now);
    // 转换为本地时间
    const std::tm tm = *std::localtime(&time);
    std::stringstream ss;
    // 按照 HH:mm:ss 的格式输出时间
    ss << std::put_time(&tm, "%H:%M:%S");
    return ss.str();
}

void getCurrentDateTimeAndGenerateDueDateTime(std::string& currentDateTime, std::string& dueDateTime) {
    // 获取当前系统时钟的时间点
    const auto now = std::chrono::system_clock::now();
    // 将时间点转换为 time_t 类型
    const std::time_t current_time = std::chrono::system_clock::to_time_t(now);

    // 转换为本地时间并生成 currentDateTime
    std::tm tm_current = *std::localtime(&current_time);
    std::stringstream ss_current;
    ss_current << std::put_time(&tm_current, "%Y-%m-%d %H:%M:%S");
    currentDateTime = ss_current.str();

    // 计算30天后的日期时间
    std::tm tm_due = tm_current;
    tm_due.tm_mday += 30; // 增加30天
    std::mktime(&tm_due); // 调整tm结构体以正确表示时间

    // 生成dueDateTime
    std::stringstream ss_due;
    ss_due << std::put_time(&tm_due, "%Y-%m-%d %H:%M:%S");
    dueDateTime = ss_due.str();
}