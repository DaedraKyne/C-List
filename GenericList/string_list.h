#pragma once

#include <iostream>
#include <stdexcept>
#include <string>
#include <memory>
#include <cassert>
#include <functional>

using namespace std;


class List_String {
    //note: elements are value copies of original objects (copy-by-value, not copy-by-reference)
public:
    List_String() : data(nullptr), capacity(0), count(0) {

    }

    friend void swap(List_String& first, List_String& second) noexcept {
        //explanation: calling swap makes it unqualified, meaning if swap(T,T) is specially defined, it is called, otherwise, call std::swap(T,T)
        swap(first.capacity, second.capacity);
        swap(first.count, second.count);
        swap(first.data, second.data);
    }

    //Rule of 3
    ~List_String() { //Destructor
        Clear();
        dataAllocator.deallocate(data, capacity);
    }

    //Copy Constructor
    List_String(const List_String& other) : data(CreateDeepCopy(other.data, other.capacity, other.count)),
                                            capacity(other.capacity), count(other.count) {}
    
    List_String& operator=(const List_String& other) { //Copy assignement
        if (this != &other) {
            Clear();
            Capacity(other.capacity);
            const auto end = other.data + other.count;
            //double-pointer progression
            for (auto dst = data, src = other.data; src != end; ++src, ++dst) {
                new (dst) std::string(*src);
            }
            count = other.count;
        }
        return *this;
    } 

    //Rule of 5
    //R-value references (&&) explained: http://thbecker.net/articles/rvalue_references/section_01.html 
    //basic explanation: if a function argument is &&, whatever it references will stop existing at the end of the function
    List_String(List_String&& other) noexcept : data(other.data), capacity(other.capacity), count(other.count) {
        other.data = nullptr;
        other.capacity = 0;
        other.count = 0;
    }
    List_String& operator=(List_String&& other) noexcept {
        List_String tmp(std::move(other));
        swap(*this, tmp);
        return *this;
        //old " *this "-related data is deleted automatically at function end (through temp destruction)
    }


    //Remove all elements - maintain capacity
    void Clear() {
        for (int i = 0; i < count; i++) {
            data[i].~string();
        }
        count = 0;
    }

    //Shrink array capacity down to count
    void ShrinkToFit() {
        if (capacity == count) return;
        //sadly, allocators don't support partial deallocation - need to fully resize
        Resize(count);
    }

    int Capacity() const { return capacity; }
    int Count() const { return count; }

    //Sets the capacity of the internal array to new_capacity. If new_capacity is smaller than Count, do nothing.
    void Capacity(int new_capacity) {
        if (new_capacity <= capacity) return; //no change
        Resize(new_capacity);
    }


    std::string ToString() const {
        if (count == 0) return "";
        std::string result = "(";
        for (int i = 0; i < count - 1; i++) {
            result += data[i];
            result += ", ";
        }
        result += data[count - 1];
        result += ")";
        return result;
    }

    //Take in any amount of arguments of any type, then unpack on element construction - allows for creating new data without checking for logic errors (the compiler and the element's constructor will take care of that)
    template<typename... Args>
    void Add(Args&&... args) {
        if (capacity == count) Capacity(std::max(2 * capacity, 1));
        new (data + count++) std::string(std::forward<Args>(args)...);
    }

    std::string* Find(const std::string& val) {
        for (int i = 0; i < count; i++) {
            if (data[i] == val) return data + i; //TODO: research why &data[i] might get overloaded
        }
        return nullptr;
    }
    const std::string* Find(const std::string& val) const {
        for (int i = 0; i < count; i++) {
            if (data[i] == val) return data + i; //TODO: research why &data[i] might get overloaded
        }
        return nullptr;
    }

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

    void RemoveAt(int index) {
        if (index < 0 || index >= count) throw std::out_of_range(string("Cannot remove element at out_of_range index: ") + to_string(index) + string("."));

        std::move(data + index + 1, data + count, data + index);
        data[--count].~string();
    }


    //Returns true if a relevant element exists, and removes it.
    size_t Remove(const std::string& val) {
        //Lazy, simpler way - when changing behaviour, just change it in RemoveIf:
        /*
         * return RemoveIf([](const auto& e) { return e == val; } );
         */

        size_t new_count = 0;
        auto picker = begin(), placer = begin();
        for (; picker < end(); picker++) {
            if (!(*picker == val)) {
                std::swap(*placer, *picker);
                placer++;
                new_count++;
            }
        }
        //destruct afterwards to ensure std::swap operates on instantiated objects
        for (; placer < end(); placer++) {
            placer->~basic_string();
        }

        size_t removed = count - new_count;
        count = new_count;

        return removed;
    }
    
    template <typename Predicate>
    size_t RemoveIf(Predicate&& pred) {
        //Dumb version - O(n^2) time, O(1) space (if removal is O(1) space)
        /*
         *  std::string* ptr;
         *  int deletions = 0;
         *  while ((ptr = FindIf(pred)) != nullptr) {
         *      RemoveAt(ptr - data);
         *      deletions++;
         *  }
         *  return deletions;
         */
        //idea: double pointers, picker points to next element to check,
        //                       placer points to next available space
        size_t new_count = 0;
        auto picker = begin(), placer = begin();
        for (; picker < end(); picker++) {
            if (!pred(*picker)) {
                std::swap(*placer, *picker);
                placer++;
                new_count++;
            }
        }
        //destruct afterwards to ensure std::swap operates on instantiated objects
        for (; placer < end(); placer++) {
            placer->~basic_string();
        }

        size_t removed = count - new_count;
        count = new_count;

        return removed;
    }

    const std::string& operator[](int index) const { return data[index]; } //read-only
    std::string& operator[](int index) { return data[index]; } //read+(later)write
    const std::string& Get(int index) const {
        if (index >= count || index < 0) throw std::out_of_range(string("Cannot get element at out_of_range index: ") + to_string(index) + string(")"));

        return data[index];
    }
    std::string& Get(int index) {
        if (index >= count || index < 0) throw std::out_of_range(string("Cannot get element at out_of_range index: ") + to_string(index) + string(")"));

        return data[index];
    }


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

    static inline std::allocator<std::string> dataAllocator; //not actually necessary to maintain same allocator throughout program as allocators can allocate/deallocate any data

    static std::string* CreateDeepCopy(std::string* data, size_t data_size, size_t copy_size) {
        assert(copy_size <= data_size && data_size >= 0);

        std::string* new_data = data_size > 0 ? dataAllocator.allocate(data_size) : nullptr;

        for (size_t i = 0; i < copy_size; i++) {
            new (new_data + i) std::string(data[i]);
        }

        return new_data;
    }

    void Resize(int new_capacity) {
        assert(new_capacity >= count); //asserts get removed in release builds

        std::string* new_data = dataAllocator.allocate(new_capacity);
        const auto end = data + count;
        for (auto dest = new_data, src = data; src != end; ++src, ++dest) {
            new (dest) std::string(*src); //move data to new location (also clears data from old location)
            src->~string(); //delete invalid data at old location (does not own any relevant data anymore so can destruct safely)
        }

        dataAllocator.deallocate(data, capacity);

        data = new_data;
        capacity = new_capacity;
    }


};

void Main_Test_List_String();