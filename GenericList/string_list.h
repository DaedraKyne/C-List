#pragma once

class List_String {
//note: elements are value copies of original objects (copy-by-value, not copy-by-reference)
public:
    List_String();

    friend void swap(List_String& first, List_String& second);

    //Rule of 3
    ~List_String(); //Destructor
    List_String(const List_String& other); //Copy Constructor
    List_String& operator=(const List_String& other); //Copy assignement

    //Rule of 5
    //R-value references (&&) explained: http://thbecker.net/articles/rvalue_references/section_01.html 
    //basic explanation: if a function argument is &&, whatever it references will stop existing at the end of the function
    List_String(List_String&& other);
    List_String& operator=(List_String&& other);




    int Capacity() const;
    bool Capacity(int new_capacity);
    int Count() const;

    std::string ToString() const;

    bool Add(const std::string& new_val);
    bool Contains(const std::string& val) const;
    bool RemoveAt(int index);
    
    int IndexOf(const std::string& val) const;
    bool Remove(const std::string& val);

    std::string operator[](int index) const;
    std::string Get(int index) const;

    //can iterate through list without using iterators if internal data is contiguous
    std::string* begin() const;
    std::string* end() const;


private:
    std::string* data;
    int capacity;
    int count;

    static std::string* CreateDeepCopy(std::string* data, size_t data_size, size_t copy_size);
    static std::string* CreateDeepCopy(std::string* data, size_t data_size);


};

void Main_Test_List_String();