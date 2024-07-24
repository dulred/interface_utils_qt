#ifndef UTILS_H
#define UTILS_H
#include <QString>
class utils
{
public:
    utils();
    static QString generateRandomString(int length);
    static QString sign(const QString &input);
};

#endif // UTILS_H
