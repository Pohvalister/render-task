#ifndef INT_TO_BIN_H
#define INT_TO_BIN_H


using namespace std;

template<typename T>
int checkSize();

template<>
int checkSize<int>() {
    return 32;
}

template <>
int checkSize<char>(){
    return 8;
}

template<>
int checkSize<long long int>() {
    return 64;
}

template<>
int checkSize<short>() {
    return 16;
}

template<typename T>
string toBinary(T value) {
    string answer;

    bool add = false, neg = false;
    if (value < 0) {
        value *= -1;
        add = neg = true;
    }

    for (int i = 0; i < checkSize<T>(); i++) {
        bool new_byte = (value % 2) xor neg xor add;
        add = not new_byte and add;

        answer = to_string(new_byte) + answer;
        value /= 2;
    }
    
    return answer;
}
#endif