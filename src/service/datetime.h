#ifndef DATETIME_H
#define DATETIME_H

#include <string>

/**
 * @brief 获取当前日期时间
 * @return 格式为 "YYYY-MM-dd HH:mm:ss" 的日期时间
 */
std::string getCurrentDateTime();

/**
 * @brief 获取当前日期
 * @return 格式为 "YYYY-MM-dd" 的日期
 */
std::string getCurrentDate();

/**
 * @brief 获取当前时间
 * @return 格式为 "HH:mm:ss" 的时间
 */
std::string getCurrentTime();

/**
 * @brief 获取当前时间并生成到期时间
 * @param currentDateTime 当前的当前时间
 * @param dueDateTime 生成的到期时间
 */
void getCurrentDateTimeAndGenerateDueDateTime(std::string& currentDateTime, std::string& dueDateTime);

#endif
