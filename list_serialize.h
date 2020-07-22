#ifndef LIST_SERIALIZE_H
#define LIST_SERIALIZE_H

#include <iostream>
#include <gtest/gtest.h>


struct ListNode {
    ListNode *      prev = NULL;
    ListNode *      next = NULL;
    ListNode *      rand = NULL; // указатель на произвольный элемент данного списка либо NULL
    std::string     data;
};

class List {
public:
    void Serialize   (FILE * file);  // сохранение в файл (файл открыт с помощью fopen(path, "wb"))
    void Deserialize (FILE * file);  // загрузка из файла (файл открыт с помощью fopen(path, "rb"))

private:
    ListNode *      head = NULL;
    ListNode *      tail = NULL;
    int       count = 0;

    FRIEND_TEST(list_serialize, data_transfer);
    FRIEND_TEST(list_serialize, big_list_transfer);
};

#endif //LIST_SERIALIZE_H
