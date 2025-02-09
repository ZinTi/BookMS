#include "PwdUtils.h"
#include <random>
#include <chrono>
#include <iomanip>
#include <sstream>

PwdUtils::PwdUtils() = default;

// 将 const std::vector<unsigned char> 类型的 key 转换为十六进制字符串
std::string PwdUtils::keyToHexString(const std::vector<unsigned char>& key) {
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');

    // 遍历 key 中的每个字节
    for (const unsigned char c : key) {
        // 将每个字节转换为两位十六进制字符串
        oss << std::setw(2) << static_cast<int>(c);
    }

    // 返回拼接好的十六进制字符串
    return oss.str();
}

// 将十六进制字符串转换为 const std::vector<unsigned char> 类型
std::vector<unsigned char> PwdUtils::hexStringToKey(const std::string& hexString) {
    std::vector<unsigned char> key;
    if (hexString.length() % 2 != 0) {
        // "Invalid hex string"
        return key;
    }

    for (size_t i = 0; i < hexString.length(); i += 2) {
        std::string byteString = hexString.substr(i, 2);
        unsigned int byte;
        std::istringstream iss(byteString);
        iss >> std::hex >> byte;
        key.push_back(static_cast<unsigned char>(byte));
    }

    return key;
}

std::string PwdUtils::calculateMD5(const std::string& plaintext) {
    return calculateHash(plaintext, EVP_md5());
}

std::string PwdUtils::calculateSHA256(const std::string& plaintext) {
    return calculateHash(plaintext, EVP_sha256());
}

std::string PwdUtils::calculateHash(const std::string& plaintext, const EVP_MD* hashType) {
    // 初始化 OpenSSL 库（线程安全，多次调用无害）
    OPENSSL_init_crypto(OPENSSL_INIT_ADD_ALL_DIGESTS, nullptr);

    EVP_MD_CTX* context = EVP_MD_CTX_new();
    if (!context) return {};

    // 初始化摘要上下文
    if (1 != EVP_DigestInit_ex(context, hashType, nullptr)) {
        EVP_MD_CTX_free(context);
        return {};
    }

    // 更新摘要计算
    if (1 != EVP_DigestUpdate(context, plaintext.data(), plaintext.size())) {
        EVP_MD_CTX_free(context);
        return {};
    }

    // 获取摘要结果
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hashLength;
    if (1 != EVP_DigestFinal_ex(context, hash, &hashLength)) {
        EVP_MD_CTX_free(context);
        return {};
    }

    EVP_MD_CTX_free(context);

    // 将二进制哈希值转换为十六进制字符串
    std::string result;
    result.reserve(hashLength * 2);
    for (unsigned int i = 0; i < hashLength; ++i) {
        char hex[3];
        snprintf(hex, sizeof(hex), "%02x", hash[i]);
        result.append(hex);
    }

    return result;
}

std::string PwdUtils::generateRandomString(const int length) {
    // 定义可用于生成随机字符串的字符集合，这里包含大小写字母和数字
    const std::string charset = "!#$%&()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[]^_abcdefghijklmnopqrstuvwxyz{|}~";
    const int charsetSize = static_cast<int>(charset.length());

    // 使用当前时间作为随机数生成器的种子
    const unsigned seed = static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count());
    std::mt19937 gen(seed); // 使用Mersenne Twister算法作为随机数生成器
    std::uniform_int_distribution<> dis(0, charsetSize - 1); // 定义均匀分布的随机数范围

    std::string result;
    result.reserve(length); // 预分配字符串空间以提高性能

    for (int i = 0; i < length; ++i) {
        // 随机选取字符集中的一个字符添加到结果字符串中
        result += charset[dis(gen)];
    }

    return result;
}

std::string PwdUtils::digestPassword(const std::string& password, const std::string& salt) {
    // 迭代合并字符串，一旦确定处理方式就不要随意更改，否则在此之前的所有用户密码都将失效
    const std::string newStr = mergeStringsIteratively(password, salt);
    return calculateMD5(newStr); // 返回加密密文
}

std::string PwdUtils::mergeStringsIteratively(const std::string& str1, const std::string& str2) {
    constexpr short iterations = 1000; // 迭代次数
    std::string combined = str1 + str2;
    for (int i = 0; i < iterations; ++i) {
        combined += str2;
    }
    return combined;
}

std::vector<unsigned char> PwdUtils::generateIV(const size_t ivLength) {
    // 使用当前时间作为随机数生成器的种子
    const unsigned seed = static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count());
    std::mt19937 gen(seed); // 使用Mersenne Twister算法作为随机数生成器
    std::uniform_int_distribution<> dis(0, 255); // 生成0到255之间的随机数（一个字节的范围）

    std::vector<unsigned char> iv(ivLength); // 创建一个长度为ivLength的向量
    for (size_t i = 0; i < ivLength; ++i) {
        iv[i] = static_cast<unsigned char>(dis(gen)); // 填充随机字节
    }

    return iv;
}

bool PwdUtils::encryptTextByAES128CTR(const std::string& plaintext, const std::vector<unsigned char>& key, std::vector<unsigned char>& ciphertext, std::vector<unsigned char>& iv) {
    iv = generateIV(16);

    // 检查密钥长度是否为 16 字节（AES-128）
    if (key.size() != 16) {
        return false;
    }

    // 创建并初始化加密上下文
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        return false;
    }

    // 使用AES-128-CTR模式初始化加密操作
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_ctr(), nullptr, key.data(), iv.data())) {
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    // 禁用填充，CTR模式不需要填充
    EVP_CIPHER_CTX_set_padding(ctx, 0);

    // 准备存储密文的缓冲区
    ciphertext.resize(plaintext.size());
    int len = 0;

    // 对明文进行加密
    if (1 != EVP_EncryptUpdate(ctx, ciphertext.data(), &len,
                               reinterpret_cast<const unsigned char*>(plaintext.data()), plaintext.size())) {
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    // 完成加密过程，处理可能的剩余数据（CTR模式通常无剩余）
    int final_len = 0;
    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &final_len)) {
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    // 验证加密后的总长度是否正确
    if (len + final_len != static_cast<int>(plaintext.size())) {
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    // 释放上下文资源
    EVP_CIPHER_CTX_free(ctx);
    return true;
}

bool PwdUtils::decryptTextByAES128CTR(const std::vector<unsigned char>& ciphertext, const std::vector<unsigned char>& iv, const std::vector<unsigned char>& key, std::string& plaintext) {
    // 检查密钥长度是否为 16 字节（AES-128）
    if (key.size() != 16) {
        return false;
    }

    // 检查 IV 长度是否为 16 字节（AES-128-CTR 的 IV 长度通常为 16 字节）
    if (iv.size() != 16) {
        return false;
    }

    // 创建并初始化解密上下文
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        return false;
    }

    // 使用 AES-128-CTR 模式初始化解密操作
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_ctr(), nullptr, key.data(), iv.data())) {
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    // 禁用填充，CTR 模式不需要填充
    EVP_CIPHER_CTX_set_padding(ctx, 0);

    // 准备存储明文的缓冲区
    plaintext.resize(ciphertext.size());
    int len = 0;

    // 对密文进行解密
    if (1 != EVP_DecryptUpdate(ctx, reinterpret_cast<unsigned char*>(plaintext.data()), &len, ciphertext.data(), ciphertext.size())) {
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    // 完成解密过程，处理可能的剩余数据（CTR 模式通常无剩余）
    int final_len = 0;
    if (1 != EVP_DecryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(plaintext.data()) + len, &final_len)) {
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    // 验证解密后的总长度是否正确
    if (len + final_len != static_cast<int>(ciphertext.size())) {
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    // 释放上下文资源
    EVP_CIPHER_CTX_free(ctx);
    return true;
}
