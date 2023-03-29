#include "hcm_hashtable.h"

void hcm_hashtable:: insert(int key, const QString& value) {
    int index = hash(key);

    // Linear probing for collision resolution
    while (table[index] != nullptr) {
        index = (index + 1) % capacity;
    }

    table[index] = new HashNode(key, value);
    size++;

    // Resize the table if it's more than 70% full
    if (size > 0.7 * capacity) {
        resize(capacity * 2);
    }
}
QString hcm_hashtable::search(int key) {
    int index = hash(key);

    // Linear probing for collision resolution
    while (table[index] != nullptr && table[index]->key != key) {
        index = (index + 1) % capacity;
    }

    if (table[index] != nullptr && table[index]->key == key) {
        return table[index]->value;
    } else {
        return "";
    }
}

void hcm_hashtable::remove(int key) {
    int index = hash(key);

    // Linear probing for collision resolution
    while (table[index] != nullptr && table[index]->key != key) {
        index = (index + 1) % capacity;
    }

    if (table[index] != nullptr && table[index]->key == key) {
        delete table[index];
        table[index] = nullptr;
        size--;

        // Resize the table if it's less than 30% full
        if (size < 0.3 * capacity) {
            resize(capacity / 2);
        }
    }
}
