#include "list_serialize.h"

#include <unordered_map>
#include <vector>


struct FileHandler {

private:
    FILE *file;

public:
    explicit FileHandler(FILE *f)
            : file(f) {}

    void putString(const std::string& str) {
        std::size_t len = str.size();
        fwrite(&len, sizeof(size_t), 1, file);
        fwrite(str.c_str(), sizeof(char), len, file);
    }

    std::string getString() {
        std::size_t len;
        fread(&len, sizeof(std::size_t), 1, file);

        char *char_str = new char[len + 1];
        fread(char_str, sizeof(char), len, file);
        char_str[len]='\0';

        std::string received_str = char_str;
        delete[] char_str;

        return received_str;
    }

    void putVector(const std::vector<int> &vec) {
        std::size_t len = vec.size();
        fwrite(&len, sizeof(std::size_t), 1, file);
        fwrite((int *) &vec[0], sizeof(int), len, file);
    }

    std::vector<int> *getVector() {
        std::size_t len;
        fread(&len, sizeof(std::size_t), 1, file);
        std::vector<int> * received_vec;
        if (len != 0) {
            int *tmp = new int[len];
            fread(tmp, sizeof(int), len, file);
            received_vec = new std::vector<int>(tmp, tmp + len);
            delete[] tmp;
        } else
            received_vec = new std::vector<int>;

        return received_vec;
    }

    void putEmpty() {
        std::size_t len = 0;
        fwrite(&len, sizeof(std::size_t), 1, file);
    }

    void putBool(bool bl) {
        fwrite(&bl, sizeof(bool), 1, file);
    }

    bool getBool(){
        bool bl;
        fread(&bl,sizeof(bool), 1, file);
        return bl;
    }

    void putInt(int val){
        fwrite(&val, sizeof(int), 1, file);
    }

    int getInt(){
        int val;
        fread(&val, sizeof(int),1, file);
        return val;
    }
};

/*Создадим мапу, которая по адресу ноды будет возвращать её номер п/п в списке,
 * первым проходом заполним мапу, вторым запишем номера всех rand ссылок в вектор (запишем его в файл)
 * третьим проходом запишем все необходимые данные из нод*/

void List::Serialize(FILE *file) {
    std::unordered_map<ListNode*, int> link_map;

    ListNode *pointer = head;

    for (int i=1; i<=count; i++) {
        link_map[pointer] = i;
        pointer = pointer->next;
    }

    std::vector<int> rands_struct;
    pointer = head;

    for (int i=1; i<=count; i++){
        if (pointer->rand != NULL && link_map.find(pointer->rand)!=link_map.end())
            rands_struct.push_back(link_map[pointer->rand]);
        else
            rands_struct.push_back(0);
        pointer = pointer->next;
    }

    FileHandler writer(file);
    writer.putVector(rands_struct);
    pointer = head;

    for (int i=1; i<=count; i++) {
        writer.putString(pointer->data);
        pointer = pointer->next;
    }

}

void List::Deserialize(FILE *file) {

    FileHandler reader(file);

    std::vector<int> rands_struct = *reader.getVector();
    int amount = rands_struct.size();
    std::vector<ListNode*> link_vec(amount + 1);

    List received = List();
    received.count = amount;
    link_vec[0] = NULL;

    if (!rands_struct.empty()) {

        ListNode* pointer = new ListNode();
        pointer->data = reader.getString();
        received.head = pointer;
        link_vec[1] = pointer;

        for (int i = 2; i <= amount; i++) {
            ListNode* tmp = new ListNode();
            tmp->data = reader.getString();
            tmp->prev = pointer;
            pointer->next = tmp;
            pointer = tmp;
            link_vec[i] = pointer;
        }
        received.tail = pointer;

        for (int j = 0; j < rands_struct.size(); j++)
            link_vec[j+1]->rand = link_vec[rands_struct[j]];
    }

    std::swap(this->head, received.head);
    std::swap(this->tail, received.tail);
    std::swap(this->count, received.count);
}