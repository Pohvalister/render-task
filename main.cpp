#include <gtest/gtest.h>
#include "cstdlib"
#include <bitset>

#include "int_to_bin.h"
#include "list_serialize.h"
//#include "calc_mesh_normals.h"

using namespace std;

TEST(int_to_bin, int_positive) {

    for (int i = 0; i < 20; i++) {
        int test_val = rand();
        string check = bitset<sizeof(int) * 8>(test_val).to_string();
        string answ = toBinary(test_val);
        EXPECT_EQ(check, answ);
    }
}

TEST(int_to_bin, int_negative) {

    for (int i = 0; i < 20; i++) {
        int test_val = rand() * (-1);
        string check = bitset<sizeof(int) * 8>(test_val).to_string();
        string answ = toBinary(test_val);
        EXPECT_EQ(check, answ);
    }
}

TEST(int_to_bin, different_values) {

    for (int i = 0; i < 20; i++) {
        short test_val = rand()%1000 - 500;
        string check = bitset<sizeof(short) * 8>(test_val).to_string();
        string answ = toBinary(test_val);
        EXPECT_EQ(check, answ);
    }
    for (int i = 0; i < 20; i++) {
        long long test_val = rand()%1000 - 500;
        string check = bitset<sizeof(long long) * 8>(test_val).to_string();
        string answ = toBinary(test_val);
        EXPECT_EQ(check, answ);
    }
    for (int i = 0; i < 20; i++) {
        char test_val = rand()%1000 - 500;
        string check = bitset<sizeof(char) * 8>(test_val).to_string();
        string answ = toBinary(test_val);
        EXPECT_EQ(check, answ);
    }
}


TEST(list_serialize, empty_list){
    List test;

    FILE* tmp_file = tmpfile();
    EXPECT_NO_THROW(test.Serialize(tmp_file));
    rewind(tmp_file);

    EXPECT_NO_THROW(test.Deserialize(tmp_file));
}

TEST(list_serialize, data_transfer){

    List input_list;
    input_list.head  =  new ListNode();
    input_list.head->data = "Hello";
    input_list.tail = input_list.head;
    input_list.count = 1;

    FILE* tmp_file = tmpfile();

    EXPECT_NO_THROW(input_list.Serialize(tmp_file));
    rewind(tmp_file);

    List output_list;
    EXPECT_NO_THROW(output_list.Deserialize(tmp_file));

    EXPECT_EQ(output_list.head, output_list.tail);
    EXPECT_EQ(output_list.count,1);
    EXPECT_EQ(output_list.head->data, output_list.tail->data);

    delete input_list.head;
    delete output_list.head;
}


TEST(list_serialize, big_list_transfer){

    List input_list;
    List output_list;
    const int cap = 1000;

    vector<ListNode*> nodes;
    ListNode * pointer = new ListNode();
    pointer -> data = "0";
    input_list.head = pointer;
    for (int i =1 ; i < cap; i++){
        ListNode * tmp  = new ListNode();
        tmp->data = std::to_string(i);
        tmp->prev = pointer;
        pointer->next = tmp;
        nodes.push_back(pointer);
        pointer = tmp;
    }

    nodes.push_back(pointer);
    input_list.tail= pointer;
    input_list.count = cap;

    for (int i = 0; i < cap/2; i++){
        int from_node = rand()%cap;
        int to_node = rand()%cap;
        nodes[from_node]->rand = nodes[to_node];
    }
    nodes[333]->rand = nodes[333];

    FILE* tmp_file = tmpfile();
    EXPECT_NO_THROW(input_list.Serialize(tmp_file));
    rewind(tmp_file);

    EXPECT_NO_THROW(output_list.Deserialize(tmp_file));

    EXPECT_EQ(input_list.count,output_list.count);

    //check data
    ListNode * p1 = input_list.head;
    ListNode * p2 = output_list.head;
    for (int i = 0; i < output_list.count - 1; i++){
        EXPECT_EQ(p1->data,p2->data);
        EXPECT_EQ(p1->next->data, p2->next->data);
        EXPECT_TRUE(p1->rand == NULL && p2->rand == NULL || p1->rand->data == p2->rand->data);
        p1 = p1->next;
        p2 = p2->next;
    }

    EXPECT_EQ(p1, input_list.tail);
    EXPECT_EQ(p2, output_list.tail);
    EXPECT_TRUE(p1->rand == NULL && p2->rand == NULL || p1->rand->data == p2->rand->data);

    p1 = input_list.tail;
    p2 = output_list.tail;
    for (int i = 0; i< cap; i++){
        EXPECT_EQ(p1->data,p2->data);
        ListNode* tmp = p1;
        EXPECT_NO_THROW(p1 = tmp->prev);
        delete tmp;
        tmp = p2;
        EXPECT_NO_THROW(p2 = tmp->prev);
        delete tmp;
    }
}
