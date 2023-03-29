#ifndef HCM_HASHTABLE_H
#define HCM_HASHTABLE_H

#include<QString>
#include<QVector>
#include<QList>

class hcm_hashtable
{

public:
    hcm_hashtable() : size(0), capacity(1003)
    {
        table.resize(capacity);
    }

    ~hcm_hashtable()
    {
        for (int i = 0; i < capacity; i++)
        {
            delete table[i];
        }
    }

private:
    class HashNode
    {
        public:
            int key;
            QString value;

            HashNode(int key, const QString& value) : key(key), value(value) {}
    };

        QVector<HashNode*> table;
        int size;
        int capacity;

        int hash(int key) {
            return key % capacity;
        }

        void resize(int newCapacity)
        {
            QVector<HashNode*> oldTable = table;
            int oldCapacity = capacity;

            table.resize(newCapacity);
            capacity = newCapacity;
            size = 0;

            // Rehash the elements in the old table
            for (int i = 0; i < oldCapacity; i++) {
                if (oldTable[i] != nullptr) {
                    insert(oldTable[i]->key, oldTable[i]->value);
                    delete oldTable[i];
                }
            }
        }

public:
    void insert(const int key, const QString& value);
    QString search(const int key);
    void remove(const int key);
};

#endif // HCM_HASHTABLE_H
