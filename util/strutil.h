#ifndef STRUTIL_H
#define STRUTIL_H

#include <QString>
#include <QStringList>
#include <ocilib.h>

QString toQString(const dtext *ociString);
dtext* toOciString(const QString &qString);


//QMultiMap<QString, unsigned int> getNamedParametersWithPositions(const QString &query);

int getEditorIntValue(const QString &value);
QString getEditorStrValue(int value);

QStringList splitTrimmed(const QString &text, bool convertToUpperCase=true);

QString humanize(const QString &input);

QString humanizeSize(int size);

QString optionText(int option,
                   const QString &option1,
                   const QString &option2);

QString optionText(int option, const QString &option1,
                   const QString &option2,
                   const QString &option3);

int numberAfterLastUnderscore(const QString &s);

bool existsInCollection(const QString &commaSeparatedString, const QStringList &collection);

bool allExistInCollection(const QString &commaSeparatedString, const QStringList &collection);

bool hasMatch(const QString &str, const QStringList &collection);

void addEOL(QString &str, const QString &terminator=QString());

QString formatSecs(int secs);

QString formatMsecs(int msecs);

QString joinEnclosed(const QStringList &list, const QString &join, const QString &enclose);

QString ensureStartsWith(QString str, const QString &startWith);

void print(const char *msg);

bool caseInsensitiveLessThan(const QString &s1, const QString &s2);

void setDbLinkName(QString &str, const QString &dbLinkName);

QString md5(const QString &str);

#endif // STRUTIL_H
