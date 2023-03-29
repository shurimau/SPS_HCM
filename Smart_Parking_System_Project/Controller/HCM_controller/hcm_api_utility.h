#ifndef HCM_API_UTILITY_H
#define HCM_API_UTILITY_H

#include <QObject>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QAuthenticator>
#include <QNetworkProxy>
#include <QFile>

bool writeInJson(const QString &fileName, const QJsonObject &object);
void deleteLinesInJson(const QString &filename, int firstLines, int lastLines);
void addCharactersToLinesInJson(const QString &filename, const QString &firstLinePrefix, const QString &lastLineSuffix);
void convertJsonToTable(const QString& filename,const QString& tablename);
bool Check_API(const QString& filePath);
QString getStringFromJson(const QString& filePath, const QString& key);

#endif // HCM_API_UTILITY_H
