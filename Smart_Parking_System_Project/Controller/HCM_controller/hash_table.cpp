#include <QJsonArray>
#include "qjsondocument.h"
#include "qjsonobject.h"
#include <QJsonValue>
#include <QJsonDocument>
#include <QFile>
#include <QJsonObject>

#include "hash_table.h"
/***********************************************************************************************************************
** Function Name         : readDataFile()
**
** Description           : Read data from rjson file
**
** Input Parameters      : QString filePath
**
** InOut Parameters      : N/A
**
** Output Parameters     : N/A
**
** Return parameter      : QJsonArray list_employee
**
** Preconditions         : N/A
**
** Global Variables Used : N/A
**
** Function(s) invoked   : N/A
***********************************************************************************************************************/
QJsonArray HashTable::readDataFile() {
    QFile file(this->filePath);
    // Open the file for reading
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file for reading";
    }

    // Parse the file contents as a JSON document
        QByteArray jsonData = file.readAll();
        QJsonParseError jsonError;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &jsonError);

        if (jsonError.error != QJsonParseError::NoError) {
            qDebug() << "Failed to parse JSON data:" << jsonError.errorString();

        }
    // Parse the JSON object from the document
        if (!jsonDoc.isObject()) {
            qDebug() << "JSON data is not an object";

        }

        QJsonObject jsonObj = jsonDoc.object();

        QJsonObject data_object= jsonObj["data"].toObject();

        QJsonArray list_employee=data_object["items"].toArray();

    file.close();

    return list_employee;
}

int HashTable::hash(int key) {
    // Calculate the hash
    return key % m_size;
}
/***********************************************************************************************************************
** Function Name         : insert()
**
** Description           : Insert an element into Hash Table (key & value)
**
** Input Parameters      : int key, QJsonObject& value
**
** InOut Parameters      : N/A
**
** Output Parameters     : N/A
**
** Return parameter      : N/A
**
** Preconditions         : N/A
**
** Global Variables Used : N/A
**
** Function(s) invoked   : int hash(int key)
**
***********************************************************************************************************************/

void HashTable:: insert(int key, QJsonObject value) {
    int index = hash(key);
    QList<QPair<int, QJsonObject>>& list = m_table[index];

    // Check if the key already exists in the list
    for (auto& pair : list) {
        if (pair.first == key) {
            pair.second = value;
            return;
        }
    }

    // If the key does not exist in the list, add it
    list.append(qMakePair(key, value));
}

/***********************************************************************************************************************
** Function Name         : remove()
**
** Description           : Remove an elements from Hash Table
**
** Input Parameters      : int key
**
** InOut Parameters      : N/A
**
** Output Parameters     : N/A
**
** Return parameter      : E_OK/NOT_OK
**
** Preconditions         : N/A
**
** Global Variables Used : N/A
**
** Function(s) invoked   : HashTable::hash(int key)
**
***********************************************************************************************************************/
bool HashTable::remove(int key) {
    int index = hash(key);
    QList<QPair<int, QJsonObject>>& list = m_table[index];

    // Check if the key exists in the list
    for (auto it = list.begin(); it != list.end(); ++it) {
        if (it->first == key) {
            list.erase(it);
            return true;
        }
    }

    // If the key does not exist in the list, return false
    return false;
}

/***********************************************************************************************************************
** Function Name         : get()
**
** Description           : Get an elements from Hash Table by key (from file path)
**
** Input Parameters      : int key, QString filePath
**
** InOut Parameters      : N/A
**
** Output Parameters     : N/A
**
** Return parameter      : QJsonObject value
**
** Preconditions         : N/A
**
** Global Variables Used : N/A
**
** Function(s) invoked   :  HashTable::(readDataFile(filePath)),
**                          HashTable::insert(const int key, const QJsonObject& value),
**                          HashTable::hash(int key)
**
***********************************************************************************************************************/
QJsonObject HashTable::get(int key) {

    //read data from file json
    QJsonArray list_employee=readDataFile();

    //insert data
    for(int i=0;i<list_employee.count();i++){
        QString str=list_employee[i].toObject().value("cif").toString();
        insert(str.toInt(),list_employee[i].toObject());
    }

    //get data
    QJsonObject value;
    int index = hash(key);
    QList<QPair<int, QJsonObject>>& list = m_table[index];

    // Check if the key exists in the list
    for (auto& pair : list) {
        if (pair.first == key) {
            value= pair.second;
        }
    }
    // If the key does not exist in the list, return some other default value
    return value;
}

/***********************************************************************************************************************
** Function Name         : buildData()
**
** Description           : Build data by hash table algorithm
**
** Input Parameters      : QString filePath
**
** InOut Parameters      : N/A
**
** Output Parameters     : N/A
**
** Return parameter      : N/A
**
** Preconditions         : N/A
**
** Global Variables Used : N/A
**
** Function(s) invoked   : HashTable::insert(const int key, const QJsonObject& value)
**
***********************************************************************************************************************/
void HashTable::build(){
    QJsonArray list_employee=readDataFile();
    //insert data
    for(int i=0;i<list_employee.count();i++){
        QString str=list_employee[i].toObject().value("cif").toString();
        insert(str.toInt(),list_employee[i].toObject());
    }
}






