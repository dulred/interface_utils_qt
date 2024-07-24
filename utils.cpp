#include "utils.h"

#include <QRandomGenerator>

#include <QCryptographicHash>

utils::utils() {}

QString utils::generateRandomString(int length)
{
    const QString characters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
    QString randomString;
    QRandomGenerator randomGenerator;

    for (int i = 0; i < length; ++i) {
        int index = randomGenerator.bounded(characters.size());
        randomString += characters[index];
    }

    return randomString;
}

QString utils::sign(const QString &input)
{
    // 计算 SHA-1 哈希
    QByteArray sha1Hash = QCryptographicHash::hash(input.toUtf8(), QCryptographicHash::Sha1);
    QString sha1HashString = sha1Hash.toHex();


    // 计算 MD5 哈希
    QByteArray md5Hash = QCryptographicHash::hash(sha1Hash, QCryptographicHash::Md5);
    QString md5HashString = md5Hash.toHex();

    return md5HashString; // 将哈希值转换为十六进制字符串
}

