#include "UserService.h"

#include "Environ.h"
#include "UserDAO.h"
#include "PwdUtils.h"

extern Environ* BookMSEnv;
extern UserInfo* currentUserInfo;

bool UserService::getUserInfoByUserId(std::string& msg, const std::string& userId, UserInfo& userInfo) {
    bool result = false; // 返回值
    DAO::UserDAO userDAO(BookMSEnv->getDbPath());

    if(userDAO.userExists(userId) == false){
        msg = "失败！目标用户不存在: " + userId;
        result = false;
        return result;
    }

    if (currentUserInfo->userType == SystemAdmin || currentUserInfo->userType == UserAdmin || currentUserInfo->userType == LibraryAdmin) {
        userInfo = userDAO.getUserById(userId);
        msg = "成功！执行查询操作完成！";
        result = true;
    }
    else if (currentUserInfo->userType == FacultyBorrower || currentUserInfo->userType == OrdinaryBorrower) {
        if (currentUserInfo->id == userId) {
            userInfo = userDAO.getUserById(userId);
            result = true;
            msg = "成功！查询到" + userId + "的用户信息";
        }
        else {
            msg = "失败！" + currentUserInfo->userType + "只能查询个人用户信息，无权查询其他用户";
            result = false;
        }
    }
    else if (currentUserInfo->userType == DisabledStatus) {
        msg = "失败！已冻结用户无权查询用户信息！";
        result = false;
    }
    else {
        msg = "失败！当前用户未登录或不合法";
        result = false;
    }
    return result;
}

int UserService::getAllUser(std::string& msg, const std::string& username, const int gender, const std::string& email, const std::string& telephone, const std::string& address, const std::string& userType, std::vector<UserInfo>& usersInfo){
    int result = 0; // 查询到的用户数
    DAO::UserDAO userDAO(BookMSEnv->getDbPath());
    std::map<std::string, std::string> conditions; // 筛选条件map，如果不需要筛选传空map
    std::vector<std::string> orderBy; // 排序条件的vector，如果不需要排序传空vector
    if(username != "" && !username.empty()){
        conditions["username"] = username;
        orderBy.emplace_back("username ASC"); // 按username升序
    }
    if(gender==1){ // 筛选男性
        conditions["gender"] = "1"; // 数据库中那个true或者"1"代表男
    }else if(gender==2){ // 筛选女性
        conditions["gender"] = "0"; // 数据库中那个false或者"0"代表女
    }else{
        //gender==0或者其他时不进行性别筛选
    }
    if(email != "" && !email.empty()){
        conditions["email"] = email;
    }
    if(telephone != "" && !telephone.empty()){
        conditions["telephone"] = telephone;
    }
    if(address != "" && !address.empty()){
        conditions["address"] = address;
    }
    if(userType != "" && !userType.empty()){
        conditions["usertype"] = userType;
    }

    if(currentUserInfo->userType == SystemAdmin || currentUserInfo->userType == UserAdmin || currentUserInfo->userType == LibraryAdmin) {
        usersInfo = userDAO.getAllUsers(conditions, orderBy);
        if(!usersInfo.empty()){
            result = static_cast<int>(usersInfo.size());
        }else{
            result = 0;
        }
        msg = "完成！执行查询操作完成，查询到" + std::to_string(result) + "条用户信息";
    }else if(currentUserInfo->userType == FacultyBorrower || currentUserInfo->userType == OrdinaryBorrower) {
        msg = "失败！" + currentUserInfo->userType + "只能查询个人用户信息，无权查询其他用户";
        result = 0;
    }else if (currentUserInfo->userType == DisabledStatus) {
        msg = "失败！已冻结用户无权查询用户信息！";
        result = 0;
    }else{
        msg = "失败！当前用户未登录或不合法";
        result = 0;
    }
    return result; // 返回查询到的用户总数
}

bool UserService::validateUser(std::string& msg, const std::string& userId, const std::string& password) {
    bool result; // 返回值
    DAO::UserDAO userDAO(BookMSEnv->getDbPath());
    const UserInfo userInfo = userDAO.getUserById(userId); // 获取用户信息

    const std::string hashedPassword = PwdUtils::digestPassword(password, userInfo.salt);
    // 使用用户输入的密码和数据库中保存的盐值进行hash计算
    if (hashedPassword == userInfo.password) {
        msg = "验证通过";
        result = true;
    }else {
        msg = "验证失败";
        result = false;
    }
    return result;
}


bool UserService::validateCurrentUser(std::string& msg, const std::string& password) {
    bool result; // 返回值
    DAO::UserDAO userDAO(BookMSEnv->getDbPath());
    const UserInfo userInfo = userDAO.getUserById(currentUserInfo->id); // 获取用户信息

    const std::string hashedPassword = PwdUtils::digestPassword(password, userInfo.salt);
    // 使用用户输入的密码和数据库中保存的盐值进行hash计算
    if (hashedPassword == userInfo.password) {
        msg = "验证通过";
        result = true;
    }else {
        msg = "验证失败";
        result = false;
    }
    return result;
}

bool UserService::login(std::string& msg, const std::string& userId, const std::string& password, UserInfo& userInfo){
    bool result;
    DAO::UserDAO userDAO(BookMSEnv->getDbPath());
    userInfo = userDAO.getUserById(userId); // 获取用户信息

    const std::string hashedPassword = PwdUtils::digestPassword(password, userInfo.salt);
    // 使用用户输入的密码和数据库中保存的盐值进行hash计算
    if (hashedPassword == userInfo.password) {
        if (userInfo.userType == SystemAdmin || userInfo.userType ==  UserAdmin || userInfo.userType == LibraryAdmin) {
            msg = "验证成功！" + userInfo.userType + "管理员";
            result = true;
        }else if (userInfo.userType == FacultyBorrower || userInfo.userType ==  OrdinaryBorrower) {
            msg = "验证成功！" + userInfo.userType + "读者";
            result = true;
        }else if(userInfo.userType == DisabledStatus){
            msg = "此账号已冻结/自主注销";
            result = false;
        }else{
            msg = "警告！其他不合法用户标识";
            result = true;
        }
    }else{
        msg = "失败！请检查账号和密码是否正确";
        result = false;
    }
    return result;
}

bool UserService::changePassword(std::string& msg, const std::string& userId, const std::string& oldPassword,
                                 const std::string& newPassword) {
    bool result; // 返回值
    DAO::UserDAO userDAO(BookMSEnv->getDbPath());
    UserInfo userInfo = userDAO.getUserById(userId); // 获取用户信息

    if (!validateUser(msg, userId, oldPassword)) {
        msg = "合法性校验失败！请检查账号和密码";
        result = false;
    }
    else {
        const std::string newSalt = PwdUtils::generateRandomString(); // 设置密码时生成一个新盐值
        const std::string newHashedPassword = PwdUtils::digestPassword(newPassword, newSalt); // 根据新盐值生成一个新密码密文
        // 更新新盐值和新密码
        userInfo.password = newHashedPassword;
        userInfo.salt = newSalt;
        if (!userDAO.updateUser(userInfo)) {
            msg = "错误！操作数据库时密码更新失败";
            result = false;
        }
        else {
            msg = "成功！已修改密码";
            result = true;
        }
    }

    return result;
}

bool UserService::resetPassword(std::string& msg, const std::string& userId, const std::string& newPassword) {
    bool result; // 返回值
    DAO::UserDAO userDAO(BookMSEnv->getDbPath());
    UserInfo userInfo = userDAO.getUserById(userId); // 获取用户信息
    if (userInfo.id.empty()) {
        msg = "失败！未查询到目标用户";
        result = false;
        return result;
    }

    if (currentUserInfo->userType == SystemAdmin) {
        const std::string newSalt = PwdUtils::generateRandomString(); // 设置密码时生成一个新盐值
        const std::string newHashedPassword = PwdUtils::digestPassword(newPassword, newSalt); // 根据新盐值生成一个新密码密文
        userInfo.password = newHashedPassword;
        userInfo.salt = newSalt;
        if (!userDAO.updateUser(userInfo)) {
            msg = "错误！操作数据库时密码重置失败";
            result = false;
        }
        else {
            msg = "成功！已重置密码";
            result = true;
        }
    }
    else if (currentUserInfo->userType == UserAdmin) {
        if (userInfo.userType == SystemAdmin || userInfo.userType == UserAdmin || userInfo.userType == LibraryAdmin) {
            msg = "失败！UA无权限重置管理员类型用户的密码";
            result = false;
        }
        else {
            const std::string newSalt = PwdUtils::generateRandomString(); // 设置密码时生成一个新盐值
            const std::string newHashedPassword = PwdUtils::digestPassword(newPassword, newSalt); // 根据新盐值生成一个新密码密文
            userInfo.password = newHashedPassword;
            userInfo.salt = newSalt;
            if (!userDAO.updateUser(userInfo)) {
                msg = "错误！操作数据库时密码重置失败";
                result = false;
            }
            else {
                msg = "成功！已重置密码";
                result = true;
            }
        }
    }
    else {
        msg = "失败！当前用户或状态无权限重置用户密码";
        result = false;
    }

    return result;
}

bool UserService::registerUser(std::string& msg, UserInfo& userInfo, const std::string& userType) {
    bool result; // 返回值
    const DAO::UserDAO userDAO(BookMSEnv->getDbPath());
    // 先检查userId是否已经存在了
    bool isExisted = userDAO.userExists(userInfo.id);
    if(isExisted){
        msg = "失败！用户ID已存在";
        result = false;
        return result;
    }

    userInfo.userType = userType;
    userInfo.salt = PwdUtils::generateRandomString(); // 新生成一个salt
    const std::string newHashedPassword = PwdUtils::digestPassword(userInfo.password, userInfo.salt);
    // 根据盐值生成一个新密码密文
    if (!newHashedPassword.empty()) { // 不为空即代表成功生成新密文
        userInfo.password = newHashedPassword;
    }
    result = userDAO.addUser(userInfo);
    if (result) {
        msg = "成功！注册用户完成";
    }
    else {
        msg = "错误！操作数据库新增用户时失败";
    }
    return result;
}

bool UserService::removeUser(std::string& msg, const std::string& userId) {
    bool result; // 返回值
    const DAO::UserDAO userDAO(BookMSEnv->getDbPath());
    // 检查目标ID是否存在
    if(userDAO.userExists(userId) == false){
        msg = "失败！无需删除用户" + userId + "，因为目标用户不存在";
        result = false;
        return result;
    }
    if (currentUserInfo->userType == SystemAdmin) {
        result = userDAO.deleteUser(userId);
        if (result) {
            msg = "成功！删除用户" + userId + "操作完成";
        }
        else {
            msg = "错误！操作数据库时删除用户" + userId + "失败";
        }
    }
    else if (currentUserInfo->userType == UserAdmin) {
        msg = "失败！" + currentUserInfo->userType + "只有冻结借阅者的权限，无删除用户的权限"; // 仅SA用户有权物理删除用户信息
        result = false;
    }
    else if (currentUserInfo->userType == LibraryAdmin || currentUserInfo->userType == FacultyBorrower
             ||
             currentUserInfo->userType == OrdinaryBorrower || currentUserInfo->userType == DisabledStatus) {
        msg = "失败！" + currentUserInfo->userType + "无删除用户的权限"; // 仅SA用户有权物理删除用户信息
        result = false;
    }
    else {
        msg = "失败！当前用户未登录或不合法";
        result = false;
    }
    return result;
}

bool UserService::setUserType(std::string& msg, const std::string& userId, const std::string& userType) {
    bool result; // 返回值
    DAO::UserDAO userDAO(BookMSEnv->getDbPath());
    if (currentUserInfo->userType == SystemAdmin) { // SA修改其他用户的用户类型时
        if (userType == SystemAdmin) { // 修改成SA时
            msg = "失败！系统只允许存在一个SA用户";
            result = false;
        }
        else if (userType == UserAdmin || userType == LibraryAdmin || userType == FacultyBorrower || userType ==
                                                                                                         OrdinaryBorrower || userType == DisabledStatus) {
            UserInfo userInfo = userDAO.getUserById(userId); // 先获取用户信息
            userInfo.userType = userType;
            if (!userDAO.updateUser(userInfo)) {
                msg = "错误！操作数据库时设置" + userType + "类型失败";
                result = false;
            }
            else {
                msg = "成功！已修改用户" + userId + "的用户类型为" + userType;
                result = true;
            }
        }
        else {
            msg = "错误！目标用户类型" + userType + "不合法";
            result = false;
        }
    }
    else if (currentUserInfo->userType == UserAdmin) { // UA修改其他用户的用户类型时
        if (userType == SystemAdmin || userType == UserAdmin || userType == LibraryAdmin) {
            msg = "失败！当前UA无权限设置" + userType + "类型";
            result = false;
        }
        else if (userType == FacultyBorrower || userType == OrdinaryBorrower || userType == DisabledStatus) {
            UserInfo userInfo = userDAO.getUserById(userId); // 先获取用户信息
            userInfo.userType = userType;
            if (!userDAO.updateUser(userInfo)) {
                msg = "错误！操作数据库时设置" + userType + "类型失败";
                result = false;
            }
            else {
                msg = "成功！已修改用户" + userId + "的用户类型为" + userType;
                result = true;
            }
        }
        else {
            msg = "错误！目标用户类型" + userType + "不合法";
            result = false;
        }
    }
    else if (currentUserInfo->userType == LibraryAdmin || currentUserInfo->userType == FacultyBorrower
             ||
             currentUserInfo->userType == OrdinaryBorrower || currentUserInfo->userType == DisabledStatus) {
        // 非SA且非UA修改其他用户的用户类型时
        msg = "失败！" + currentUserInfo->userType + "无权限设置用户类型"; // LA、FB、OB、DS
        result = false;
    }
    else {
        msg = "失败！当前用户未登录或不合法";
        result = false;
    }
    return result; // 修改用户类型结果
}

bool UserService::updateUser(std::string& msg, const std::string& userId, const std::string& username, const std::string& email, const std::string& telephone, const std::string& address, const bool gender) {
    bool result; // 返回值
    DAO::UserDAO userDAO(BookMSEnv->getDbPath());
    // 检查用户是否存在
    if (userDAO.userExists(userId) == false) {
        msg = "失败！用户" + userId + "不存在";
        result = false;
        return result;
    }
    UserInfo userInfo = userDAO.getUserById(userId); // 先获取原用户信息

    // 处理已冻结用户
    if (currentUserInfo->userType == DisabledStatus) {
        msg = "失败！已冻结用户无权修改任何信息";
        result = false;
        return result;
    }
    // 处理未登录或不合法身份
    if (currentUserInfo->userType == SystemAdmin || currentUserInfo->userType == UserAdmin ||
        currentUserInfo->userType == LibraryAdmin || currentUserInfo->userType == FacultyBorrower ||
        currentUserInfo->userType == OrdinaryBorrower) {
        // 处理修改他人信息的权限问题
        if (currentUserInfo->id!= userId) {
            if (currentUserInfo->userType == UserAdmin) {
                if (userInfo.userType == SystemAdmin || userInfo.userType == UserAdmin || userInfo.userType == LibraryAdmin) {
                    msg = "失败！UA用户只能修改 FB、OB、DS 用户以及自己的信息，无权修改其他 " + userInfo.userType + " 用户的信息";
                    result = false;
                    return result;
                }
            } else if (currentUserInfo->userType == LibraryAdmin || currentUserInfo->userType == FacultyBorrower || currentUserInfo->userType == OrdinaryBorrower) {
                msg = "失败！你无权修改其他用户的信息";
                result = false;
                return result;
            }
        }
        // 进行信息修改
        userInfo.username = username;
        userInfo.email = email;
        userInfo.telephone = telephone;
        userInfo.address = address;
        userInfo.gender = gender;
        if (!userDAO.updateUser(userInfo)) {
            msg = "错误！操作数据库时变更用户信息失败";
            result = false; // 修改用户类型失败
        } else {
            msg = "成功！已变更用户" + userId + "的基本信息";
            result = true;
        }
    } else {
        msg = "失败！用户未登录或不合法身份";
        result = false;
    }
    return result;
}

// 尽量不要用这个版本，后续会删除这个函数，使用上面那个版本替代
bool UserService::updateUser(std::string& msg, const std::string& userId, const std::string& email, const std::string& telephone, const std::string& address) {
    bool result; // 返回值
    DAO::UserDAO userDAO(BookMSEnv->getDbPath());
    // 检查用户是否存在
    if(userDAO.userExists(userId) == false){
        msg = "失败！用户" + userId + "不存在";
        result = false;
        return result;
    }

    UserInfo userInfo = userDAO.getUserById(userId); // 先获取原用户信息
    userInfo.email = email;
    userInfo.telephone = telephone;
    userInfo.address = address;
    if (!userDAO.updateUser(userInfo)) {
        msg = "错误！操作数据库时变更用户信息失败";
        result = false; // 修改用户类型失败
    }else {
        msg = "成功！已变更用户" + userId + "的基本信息";
        result = true;
    }
    return result; // 修改用户类型成功
}
