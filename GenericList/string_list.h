#pragma once

class List_String {
public:
    List_String();
    ~List_String();

    int Capacity();
    bool Capacity(int new_capacity);
    int Count();

    std::string ToString();

    bool Add(std::string new_val);
    bool Contains(std::string val);
    bool RemoveAt(int index);
    
    int IndexOf(std::string val);
    bool Remove(std::string val);

    std::string Get(int index);


private:
    std::string* data;
    int capacity;
    int count;

};

void Main_Test_List_String();