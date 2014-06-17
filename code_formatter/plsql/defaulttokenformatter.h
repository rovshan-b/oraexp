#ifndef DEFAULTTOKENFORMATTER_H
#define DEFAULTTOKENFORMATTER_H

#include <QString>

class CodeScanner;

class DefaultTokenFormatter
{
public:
    static QString format(CodeScanner *scanner);

private:
    DefaultTokenFormatter();
};

#endif // DEFAULTTOKENFORMATTER_H
