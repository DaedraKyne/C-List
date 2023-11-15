#pragma once
#pragma once
template <typename T>
class List {
    //note: elements are value copies of original objects (copy-by-value, not copy-by-reference)
public:
    List();

    friend void swap(List<T>& first, List<T>& second);

    //Rule of 3
    ~List(); //Destructor
    List(const List& other); //Copy Constructor
    List& operator=(List other); //Copy assignement


    int Capacity() const;
    bool Capacity(const int& new_capacity);
    int Count() const;

    void Print() const;

    bool Add(const T& new_val);
    bool Contains(const T& val) const;
    bool RemoveAt(const int& index);

    int IndexOf(const T& val) const;
    bool Remove(const T& val);

    T operator[](const int& index) const;
    T Get(const int& index) const;


private:
    T* data;
    int capacity;
    int count;

    static T* CreateDeepCopy(T* const& data, size_t const& data_size, size_t const& copy_size);
    static T* CreateDeepCopy(T* const& data, size_t const& data_size);


};

void Main_Test_List();