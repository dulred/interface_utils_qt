#ifndef UTILS_H
#define UTILS_H
#include <QString>
class utils
{
public:
    utils();
    static QString generateRandomString(int length);
    static QString sha1(const QString &input);
    static QString md5(const QString &input);
};

#endif // UTILS_H
