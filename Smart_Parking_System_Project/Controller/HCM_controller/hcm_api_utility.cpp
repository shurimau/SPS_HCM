#include "hcm_api_utility.h"

#include <QJsonDocument>
#include <QFile>
#include <QJsonObject>
#include <QTextStream>
#include <QStringList>
#include <QMap>
#include <QJsonArray>

typedef enum {
    E_NOT_OK,
    E_OK
} ReturnType;

/***********************************************************************************************************************
** Function Name         : writeInJson()
**
** Description           : Convert data received from server to file JSON
**
** Input Parameters      : QString &fileName, QJsonObject &object
**
** InOut Parameters      : N/A
**
** Output Parameters     : N/A
**
** Return parameter      : ReturnType(E_OK / E_NOT_OK)
**
** Preconditions         : Must have path of file JSON
**
** Global Variables Used : N/A
**
** Function(s) invoked   : N/A
**
***********************************************************************************************************************/


bool writeInJson(const QString &fileName, const QJsonObject &object)
{
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Fail to open";
        return E_NOT_OK;
    }

    QJsonDocument doc = QJsonDocument(object);

    qInfo() << doc.toJson();
    file.write(doc.toJson());
    file.close();

    return E_OK;
}

/***********************************************************************************************************************
** Function Name         : deleteLinesInJson()
**
** Description           : Remove the number of rows at the beginning and the end of file JSON
**
** Input Parameters      : QString &filename, int firstLines, int lastLines
**
** InOut Parameters      : N/A
**
** Output Parameters     : N/A
**
** Return parameter      : N/A
**
** Preconditions         : Must have path of file JSON
**
** Global Variables Used : N/A
**
** Function(s) invoked   : N/A
**
***********************************************************************************************************************/

void deleteLinesInJson(const QString &filename, int firstLines, int lastLines)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    QStringList lines;
    while (!in.atEnd()) {
        lines << in.readLine();
    }
    file.close();

    lines = lines.mid(firstLines, lines.length() - firstLines - lastLines);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    for (const QString &line : qAsConst(lines)) {
        out << line << '\n';
    }
    file.close();
}

/***********************************************************************************************************************
** Function Name         : addCharactersToLinesInJson()
**
** Description           : Add specific character at the beginning and the end of file JSON
**
** Input Parameters      : QString &filename, QString &firstLinePrefix, QString &lastLineSuffix
**
** InOut Parameters      : N/A
**
** Output Parameters     : N/A
**
** Return parameter      : N/A
**
** Preconditions         : Must have path of file JSON
**
** Global Variables Used : N/A
**
** Function(s) invoked   : N/A
**
***********************************************************************************************************************/

void addCharactersToLinesInJson(const QString &filename, const QString &firstLinePrefix, const QString &lastLineSuffix)
{
    // Load the contents of the JSON file into memory
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream stream(&file);
    QStringList lines;
    while (!stream.atEnd()) {
        lines << stream.readLine();
    }
    file.close();

    // Add characters to the first line
    lines[0].prepend(firstLinePrefix);

    // Add characters to the last line
    lines[lines.length() - 1].append(lastLineSuffix);

    // Write the modified data back to the file
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream outStream(&file);
    for (const QString &line : qAsConst(lines)) {
        outStream << line << '\n';
    }

//    QTextStream outStream(&file);
//    for (const QString &line : lines) {
//        outStream << line << endl;
//    }

    file.close();
}

/***********************************************************************************************************************
** Function Name         : convertJsonToTable()
**
** Description           : Convert file JSON to table (.txt)
**
** Input Parameters      : QString& jsonFileName, QString& tableFileName
**
** InOut Parameters      : N/A
**
** Output Parameters     : N/A
**
** Return parameter      : N/A
**
** Preconditions         : Must have path of file JSON
**
** Global Variables Used : N/A
**
** Function(s) invoked   : deleteLinesInJson(), addCharactersToLinesInJson()
**
***********************************************************************************************************************/

void convertJsonToTable(const QString& jsonFileName, const QString& tableFileName)
{
    // Load the JSON file
    QFile file(jsonFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<< "Can not open file JSON";
        return;
    }

    QByteArray jsonData = file.readAll();
    file.close();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    //qDebug() << jsonDoc;

    // Get the headers
    QJsonArray jsonArray = jsonDoc.array();
    qDebug() << jsonArray;
    QJsonObject firstObject = jsonArray[0].toObject();
    QStringList headers = firstObject.keys();

    // Find the maximum width for each header
    QMap<QString, int> widths;
    foreach (const QString &header, headers)
        widths[header] = header.length();

    // Find the maximum width for each column
    for (int i = 0; i < jsonArray.count(); ++i) {
        QJsonObject obj = jsonArray[i].toObject();
        foreach (const QString &header, headers) {
            int current_width = obj[header].toString().length();
            if (current_width > widths[header])
                widths[header] = current_width;
        }
    }

    // Build the table header
    QString header_row = "|";
    QString separator_row = "|";
    foreach (const QString &header, headers) {
       header_row += " " + header.leftJustified(widths[header]) + " |";
       separator_row += " " + QString("").leftJustified(widths[header], '-') + " |";
    }

    // Build the table data
    QStringList data_rows;
    for (int i = 0; i < jsonArray.count(); ++i) {
        QJsonObject obj = jsonArray[i].toObject();
        int column = 1;
        QString data_row = "|";
        foreach (const QString &header, headers)
        {
            QJsonValueRef jsonOj = obj[header];
            if(column == 4)
            {
                QJsonArray strJSON = jsonOj.toArray();

                QString str = QString(QJsonDocument(strJSON).toJson(QJsonDocument::Compact));

                str.remove(str.indexOf('['),1);
                str.remove(str.indexOf(']'),1);


//                str.remove(str.indexOf('{'),1);
//                str.remove(str.indexOf('}'),1);

                data_row += " " + str.leftJustified(widths[header]) + " |";
                column++;
                continue;
            }
            else if(column > 9) column = 0;
            data_row += " " + obj[header].toString().leftJustified(widths[header]) + " |";
            column++;
        }
        data_rows << data_row;
    }

    // Save the table to a file
    QFile table_file(tableFileName);
    if (table_file.open(QIODevice::WriteOnly | QIODevice::Text)) {
       QTextStream stream(&table_file);
       stream << header_row << Qt::endl;
       stream << separator_row << Qt::endl;
       foreach (const QString &data_row, data_rows)
           stream << data_row << Qt::endl;
       table_file.close();
    }
}

QString getStringFromJson(const QString& filePath, const QString& key)
{
    QFile jsonFile(filePath);

    if (!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // Handle the case where the file could not be opened
        return QString();
    }

    QByteArray jsonDocument = jsonFile.readAll();
    jsonFile.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonDocument);

    if (jsonDoc.isObject()) {
        QJsonObject jsonObj = jsonDoc.object();

        if (jsonObj.contains(key)) {
            QJsonValue jsonVal = jsonObj.value(key);

            if (jsonVal.isString()) {
                QString result = jsonVal.toString();
                return result;
            }
        }
    }

    return QString(); // return an empty QString if the key is not found or the value is not a string
}

bool Check_API(const QString& filePath)
{
    qDebug() << "code: " << getStringFromJson(filePath,"code");
    if(getStringFromJson(filePath,"code") == "CODE_SUCCESS")    return true;
    else return false;
}




































