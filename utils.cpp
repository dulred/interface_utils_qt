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

QString utils::sha1(const QString &input)
{
    QByteArray byteArray = input.toUtf8();
    QByteArray hash = QCryptographicHash::hash(byteArray, QCryptographicHash::Sha1);
    return hash.toHex(); // 将哈希值转换为十六进制字符串
}

QString utils::md5(const QString &input)
{
    QByteArray byteArray = input.toUtf8();
    QByteArray hash = QCryptographicHash::hash(byteArray, QCryptographicHash::Md5);
    return hash.toHex(); // 将哈希值转换为十六进制字符串
}

