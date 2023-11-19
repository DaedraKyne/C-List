#pragma once

#ifndef _LIST_H_
#define _LIST_H_

#include <iostream>
#include <stdexcept>
#include <string>
#include <memory>

using namespace std;

//TODO: https://stackoverflow.com/questions/495021/why-can-templates-only-be-implemented-in-the-header-file
//https://isocpp.org/wiki/faq/templates#class-templates


template <typename T>
class List {
    //note: elements are value copies of original objects (copy-by-value, not copy-by-reference)
public:
    List() : data(nullptr), capacity(0), count(0) {

    }

    friend void swap(List<T>& first, List<T>& second) {
        //explanation: calling swap makes it unqualified, meaning if swap(T,T) is specially defined, it is called, otherwise, call std::swap(T,T)
        swap(first.capacity, second.capacity);
        swap(first.count, second.count);
        swap(first.data, second.data);
    }

    //Rule of 3
 
    //Destructor
    ~List() {
        for (size_t i = 0; i < count; i++) {
            data[i].~T();
        }
        dataAllocator.deallocate(data, capacity);
    }
    //Copy Constructor
    List(const List& other) : capacity(other.capacity), count(other.count) {
        data = CreateDeepCopy(other.data, other.capacity, other.count);
    }
    //Copy assignement
    List& operator=(const List& other) {
        //Note: other is not a const reference but a copy of the value, allowing for use of swap logic (since "other" will be destroyed after
        return *this = List(other);
    }

    //Move Constructor
    List(List&& other) {
        swap(*this, other);
    }
    //Move Assignement
    List& operator=(List&& other) {
        swap(*this, other);
        return *this;
    }


    size_t Capacity() const { return capacity; }
    size_t Count() const { return count; }

    void Capacity(size_t new_capacity) {
        if (new_capacity < count) return; //can't have shorter capacity than element count
        if (new_capacity == capacity) return; //no change
        if (new_capacity == 0) { //empty non-empty array
            dataAllocator.deallocate(data, capacity);
            data = nullptr;
            return;
        };

        T* new_data = dataAllocator.allocate(new_capacity);

        for (size_t i = 0; i < count; i++) {
            new (new_data + i) T(std::move(data[i]));
        }

        dataAllocator.deallocate(data, capacity);

        data = new_data;
        capacity = new_capacity;
    }


    void Print() const {
        if (count == 0) {
            std::cout << ""; return;
        }
        std::cout << "(";
        for (size_t i = 0; i < count - 1; i++) {
            std::cout << data[i] << ", ";
        }
        std::cout << data[count - 1] << ")\n";
    }

    void Add(const T& new_val) {
        if (capacity < count + 1) {
            Capacity(capacity == 0 ? 1 : capacity * 2); //double array size
        }
        new (data + count++) T(std::forward<const T>(new_val));
    }

    void Add(T&& new_val) {
        if (capacity < count + 1) {
            Capacity(capacity == 0 ? 1 : capacity * 2); //double array size
        }
        new (data + count++) T(std::forward<T>(new_val));
    }

    bool Contains(const T& val) const {
        return IndexOf(val) != -1;
    }

    bool RemoveAt(size_t index) {
        if (index >= count) return false;
        //allowed setting: index = [0, count-1], count > 0 (data is initialized)
        data[index].~string();
        std::move(data + index + 1, data + count, data + index);
        count--;
        return true;
    }

    size_t IndexOf(const T& val) const {
        for (size_t i = 0; i < count; i++) {
            if (data[i] == val) return i;
        }
        return -1;
    }

    bool Remove(const T& val) {
        size_t index = IndexOf(val);
        return RemoveAt(index);
    }

    const T& operator[](size_t index) const { return Get(index); }
    T& operator[](size_t index) { return Get(index); }
    
    const T& Get(size_t index) const {
        if (index >= count) throw std::out_of_range("");
        return data[index];
    }
    T& Get(size_t index) {
        if (index >= count) throw std::out_of_range("");
        return data[index];
    }


    T* begin() { return data; }
    const T* begin() const { return data; }
    const T* cbegin() const { return data; }

    T* end() { return data + count; }
    const T* end() const { return data + count; }
    const T* cend() const { return data + count; }


private:
    T* data;
    size_t capacity;
    size_t count;

    static inline std::allocator<T> dataAllocator;

    static T* CreateDeepCopy(T* const& data, size_t data_size, size_t copy_size) {
        if (copy_size > data_size) {
            return nullptr;
        }
        T* new_data = data_size > 0 ? dataAllocator.allocate(data_size) : nullptr;

        //For improved performance, replace copy by memcpy+fill (no deep copy of non-POD objects) or by using a swap method
        for (size_t i = 0; i < copy_size; i++) {
            new (new_data + i) T(data[i]);
        }

        return new_data;
    }

};


#endif // !_LIST_H
