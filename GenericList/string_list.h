#pragma once

class List_String {
//note: elements are value copies of original objects (copy-by-value, not copy-by-reference)
public:
    List_String();

    friend void swap(List_String& first, List_String& second);

    //Rule of 3
    ~List_String(); //Destructor
    List_String(const List_String& other); //Copy Constructor
    List_String& operator=(List_String other); //Copy assignement


    int Capacity() const;
    bool Capacity(const int &new_capacity);
    int Count() const;

    std::string ToString() const;

    bool Add(const std::string& new_val);
    bool Contains(const std::string& val) const;
    bool RemoveAt(const int& index);
    
    int IndexOf(const std::string& val) const;
    bool Remove(const std::string& val);

    std::string operator[](const int& index) const;
    std::string Get(const int& index) const;


private:
    std::string* data;
    int capacity;
    int count;

    static std::string* CreateDeepCopy(std::string* const& data, size_t const &data_size, size_t const &copy_size);
    static std::string* CreateDeepCopy(std::string* const &data, size_t const &data_size);


};

void Main_Test_List_String();