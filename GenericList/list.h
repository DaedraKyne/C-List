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
        return *this = List(other);
    }

    //Move Constructor
    List(List&& other) : data(nullptr), capacity(0), count(0) {
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
        if (new_capacity == 0) { //empty non-empty array with no initialized elements
            dataAllocator.deallocate(data, capacity);
            data = nullptr;
            return;
        };

        T* new_data = dataAllocator.allocate(new_capacity); //allocate new memory

        for (size_t i = 0; i < count; i++) {
            new (new_data + i) T(std::move(data[i])); //transfer data without temporary copies
        }

        dataAllocator.deallocate(data, capacity); //deallocate old memory

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
        new (data + count++) T(std::forward<const T>(new_val)); //construct new data as copy of old data
    }

    void Add(T&& new_val) {
        if (capacity < count + 1) {
            Capacity(capacity == 0 ? 1 : capacity * 2); //double array size
        }
        new (data + count++) T(std::forward<T>(new_val)); //construct new data by moving old data
    }

    bool Contains(const T& val) const {
        return IndexOf(val) != -1;
    }

    bool RemoveAt(size_t index) {
        if (index >= count) return false;
        //allowed setting: index = [0, count-1], count > 0 (data is initialized)
        data[index].~T(); //destruct element
        std::move(data + index + 1, data + count, data + index); //shuffle-move data left by 1
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
        if (index >= count) throw std::out_of_range(""); //do we remove this line and let user deal with errors from incorrect indexing?
        return data[index];
    }
    T& Get(size_t index) {
        if (index >= count) throw std::out_of_range(""); //do we remove this line and let user deal with errors from incorrect indexing?
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

    static inline std::allocator<T> dataAllocator; //any allocator can allocate/deallocate any data, so not necessary to keep an allocator specifically for List, but avoids having to instantiate one each time

    static T* CreateDeepCopy(T* const& data, size_t data_size, size_t copy_size) {
        if (copy_size > data_size) {
            return nullptr;
        }
        T* new_data = data_size > 0 ? dataAllocator.allocate(data_size) : nullptr; //allocate new data

        //For improved performance, replace copy by memcpy+fill (no deep copy of non-POD objects) or by using a swap method
        for (size_t i = 0; i < copy_size; i++) {
            new (new_data + i) T(data[i]); //create new copy of old data's element
        }

        return new_data;
    }

};


#endif // !_LIST_H
