#ifndef HASH_TABLE_H
#define HASH_TABLE_H
#include<QString>
#include<QVector>
#include<QList>
#include<QJsonObject>
#include<QJsonArray>
#include <QtCore/QHash>
#include <QtCore/QList>

#define PATH_GET "json/get.js";
#define NAME_FILE "hash_table.h"
class HashTable {
public:
    HashTable(int size) : m_size(size), m_table(size) {
       filePath = __FILE__;
       QString fileName= NAME_FILE;
       filePath = filePath.remove(filePath.length() - fileName.length(),fileName.length() )+ PATH_GET;
    }
    ~HashTable() {
        // Clear all the chains in the hash table
        for (auto& chain : m_table) {
            chain.clear();
        }
    }


private:
    int m_size;
    QVector<QList<QPair<int, QJsonObject>>> m_table;
    QString filePath;

    int hash(int key);
    void insert(int key, QJsonObject value);
    QJsonArray readDataFile();
    void build();
    bool remove(int key);

public:
    QJsonObject get(int key);
 };


#endif // HASH_TABLE_H








