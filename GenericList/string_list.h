#pragma once

#include <iostream>
#include <stdexcept>
#include <string>
#include <memory>
#include <cassert>
#include <functional>

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


    //Remove all elements - maintain capacity
    void Clear();

    //Shrink array capacity down to count
    void ShrinkToFit();

    int Capacity() const;
    int Count() const;

    //Sets the capacity of the internal array to new_capacity. If new_capacity is smaller than Count, do nothing.
    void Capacity(int new_capacity);

    std::string ToString() const;

    //Take in any amount of arguments of any type, then unpack on element construction - allows for creating new data without checking for logic errors (the compiler and the element's constructor will take care of that)
    template<typename... Args>
    void Add(Args&&... args) {
        if (capacity == count) Capacity(std::max(2 * capacity, 1));
        new (data + count++) std::string(std::forward<Args>(args)...);
    }

    std::string* Find(const std::string& val);
    const std::string* Find(const std::string& val) const;

    template <typename Predicate>
    std::string* FindIf(Predicate&& pred) {
        for (auto ptr = begin(); ptr < end(); ptr++) {
            if (pred(*ptr)) return ptr; //TODO: research why &data[i] might get overloaded
        }
        return nullptr;
    }

    template <typename Predicate>
    const std::string* FindIf(Predicate&& pred) const {
        for (auto ptr = begin(); ptr < end(); ptr++) {
            if (pred(*ptr)) return ptr; //TODO: research why &data[i] might get overloaded
        }
        return nullptr;
    }

    bool Contains(const std::string& val) const;
    void RemoveAt(int index);

    int IndexOf(const std::string& val) const;

    //Returns true if a relevant element exists, and removes it.
    size_t Remove(const std::string& val);
    
    template <typename Predicate>
    size_t RemoveIf(Predicate&& pred) {
        //idea: double pointers, picker points to next element to check,
        //                       placer points to next available space

        auto placer = FindIf(pred);
        if (placer == nullptr) return 0;

        for (auto picker = placer + 1; picker < end(); ++picker) {
            if (!pred(*picker)) {
                *placer = std::move(*picker); //*picker now contains irrelevant instantiated data
                ++placer;
            }
        }

        for (auto k = placer; k < end(); ++k) {
            k->~string(); //destructs meaningless data from matched indexes moved to end
        }

        const auto removed = end() - placer;
        count -= removed;

        return removed;
    }

    const std::string& operator[](int index) const { return data[index]; } //read-only
    std::string& operator[](int index) { return data[index]; } //read+(later)write
    const std::string& Get(int index) const;
    std::string& Get(int index);


    //can iterate through list without using iterators if internal data is contiguous
    std::string* begin() { return data; }
    const std::string* begin() const { return data; } //non-copy version for read-only
    const std::string* cbegin() const { return data; } //same as above, allows for direct call to cbegin for user who knows they want constant iterators
    
    std::string* end() { return data + count; }
    const std::string* end() const { return data + count; }
    const std::string* cend() const { return data + count; }



private:
    std::string* data;
    int capacity;
    int count;

    static std::allocator<std::string> dataAllocator; //not actually necessary to maintain same allocator throughout program as allocators can allocate/deallocate any data

    static std::string* CreateDeepCopy(std::string* data, size_t data_size, size_t copy_size);

    void Resize(int new_capacity);


};

void Main_Test_List_String();