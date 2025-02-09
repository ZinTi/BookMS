#ifndef PWDUTILS_H
#define PWDUTILS_H

#include <vector>
#include <string>
#include <sstream>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/err.h>

class PwdUtils
{
public:
    PwdUtils();

    /**
     * @brief 将 const std::vector<unsigned char> 类型的 key 转换为十六进制字符串
     * @param key 密钥
     * @return 十六进制字符串
     */
    static std::string keyToHexString(const std::vector<unsigned char>& key);

    /**
     * @brief 将十六进制字符串转换为 const std::vector<unsigned char> 类型
     * @param hexString 十六进制字符串
     * @return 密钥
     */
    static std::vector<unsigned char> hexStringToKey(const std::string& hexString);

    /**
     * @brief 计算MD5摘要
     * @param plaintext 原文
     * @return MD5 digest
     */
    static std::string calculateMD5(const std::string& plaintext);

    /**
     * @brief 计算SHA256摘要
     * @param plaintext 原文
     * @return SHA256 digest
     */
    static std::string calculateSHA256(const std::string& plaintext);

    /**
     * @brief 生成一个随机字符串
     * @param length 随机字符串长度，默认长度12
     * @return 返回一个随机字符串
     */
    static std::string generateRandomString(int length = 12);

    /**
     * @brief 通过对原密码加盐后再进行散列操作以获取新的密文密码
     * @param password 原明文密码
     * @param salt 盐值
     * @return 返回新的密文密码
     */
    static std::string digestPassword(const std::string& password, const std::string& salt);


    /**
     * @brief 生成一个初始化向量iv
     * @param ivLength 长度，以字节为单位
     * @return 返回一个随机字符串
     */
    static std::vector<unsigned char> generateIV(size_t ivLength);

    /**
     * @brief 加密，AES-128-CTR模式
     * @param plaintext 原文
     * @param key 密钥
     * @param ciphertext 密文
     * @param iv 初始化向量
     * @return 成功与否
     */
    static bool encryptTextByAES128CTR(const std::string& plaintext, const std::vector<unsigned char>& key, std::vector<unsigned char>& ciphertext, std::vector<unsigned char>& iv);

    /**
     * @brief 解密，AES-128-CTR模式
     * @param ciphertext 密文
     * @param iv 初始化向量
     * @param key 密钥
     * @param plaintext 原文
     * @return 成功与否
     */
    static bool decryptTextByAES128CTR(const std::vector<unsigned char>& ciphertext, const std::vector<unsigned char>& iv, const std::vector<unsigned char>& key, std::string& plaintext);

private:
    /**
     * @brief 迭代合并字符串
     * @param str1 字符串1
     * @param str2 字符串2
     * @return 返回处理后的新字符串
     */
    static std::string mergeStringsIteratively(const std::string& str1, const std::string& str2);

    /**
     * @brief 计算摘要
     * @param plaintext 明文
     * @param hashType 散列类型
     * @return 计算结果
     */
    static std::string calculateHash(const std::string& plaintext, const EVP_MD* hashType);
};

#endif // PWDUTILS_H
