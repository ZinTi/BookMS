#ifndef ENVIRON_H
#define ENVIRON_H

#include <string>
class Environ;

class Environ {
public:
    Environ();
    std::string getHomePath();
    std::string getDbPath();

private:
    std::string homePath; // 根目录
    std::string dbPath; // db目录
    /**
     * @brief 获取当前程序路径
     * @param msg 反馈消息
     * @return 成功与否
     */
    bool setHomePath(std::string& msg);
    void setDbPath();
};


#endif //ENVIRON_H
