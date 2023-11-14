#pragma once

class List_String {
public:
    List_String();

    friend void swap(List_String& first, List_String& second);

    //Rule of 3
    ~List_String(); //Destructor
    List_String(const List_String& other); //Copy Constructor
    List_String& operator=(List_String other); //Copy assignement


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

    std::string* DeepCopyData(std::string* const& data, size_t const &data_size, size_t const &copy_size);
    std::string* DeepCopyData(std::string* const &data, size_t const &data_size);


};

void Main_Test_List_String();