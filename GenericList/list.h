#pragma once

#ifndef _LIST_H_
#define _LIST_H_

#include <iostream>
#include <stdexcept>
#include <string>

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
        using std::swap; //fall back to std::swap if swap(T,T) isn't defined
        //explanation: calling swap makes it unqualified, meaning if swap(T,T) is specially defined, it is called, otherwise, call std::swap(T,T)
        swap(first.capacity, second.capacity);
        swap(first.count, second.count);
        swap(first.data, second.data);
    }

    //Rule of 3
 
    //Destructor
    ~List() {
        delete[] data;
    }
    //Copy Constructor
    List(const List& other) : capacity(other.capacity), count(other.count) {
        data = CreateDeepCopy(other.data, other.capacity, other.count);
    }
    //Copy assignement
    List& operator=(List other) {
        //Note: other is not a const reference but a copy of the value, allowing for use of swap logic (since "other" will be destroyed after
        swap(*this, other);
        return *this;
    }


    int Capacity() const { return capacity; }
    bool Capacity(const int& new_capacity) {
        if (new_capacity < 0) return false; //allowed values must be >= 0
        if (new_capacity < count) return false; //can't have shorter capacity than element count
        if (new_capacity == capacity) return true; //no change
        if (new_capacity == 0) { //empty non-empty array
            delete[] data;
            data = nullptr;
            return true;
        };

        T* new_data = CreateDeepCopy(data, new_capacity, count);

        delete[] data;

        data = new_data;
        capacity = new_capacity;
        return true;
    }

    int Count() const { return count; }

    void Print() const {
        if (count == 0) {
            std::cout << ""; return;
        }
        std::cout << "(";
        for (int i = 0; i < count - 1; i++) {
            std::cout << data[i] << ", ";
        }
        std::cout << data[count - 1] << ")\n";
    }

    bool Add(const T& new_val) {
        if (capacity < count + 1) {
            Capacity(capacity == 0 ? 1 : capacity * 2); //double array size
        }
        data[count++] = new_val;
        return true;
    }

    bool Contains(const T& val) const {
        return IndexOf(val) != -1;
    }

    bool RemoveAt(const int& index) {
        if (index < 0) return false;
        if (index >= count) return false;
        //allowed setting: index = [0, count-1], count > 0 (data is initialized)

        memmove(data + index, data + index + 1, (count - index - 1) * sizeof(T)); //shallow shuffle left
        count--;
        return true;
    }

    int IndexOf(const T& val) const {
        for (int i = 0; i < count; i++) {
            if (data[i] == val) return i;
        }
        return -1;
    }

    bool Remove(const T& val) {
        int index = IndexOf(val);
        return RemoveAt(index);
    }

    T operator[](const int& index) const {
        return Get(index);
    }

    T Get(const int& index) const {
        if (index >= count || index < 0) throw std::out_of_range(string("Cannot get element at out_of_range index: ") + to_string(index) + string(")"));

        return data[index];
    }


private:
    T* data;
    int capacity;
    int count;

    static T* CreateDeepCopy(T* const& data, size_t const& data_size, size_t const& copy_size) {
        if (copy_size > data_size || copy_size < 0) {
            throw std::out_of_range(string("Cannot copy array of size ") + to_string(copy_size) + string(" onto array of size ") + to_string(copy_size) + string("."));
        }
        T* new_data = data_size > 0 ? new T[data_size] : nullptr;

        //For improved performance, replace copy by memcpy+fill (no deep copy of non-POD objects) or by using a swap method
        if (copy_size > 0) { //data != nullptr
            copy(data, data + copy_size, new_data);
        }

        return new_data;
    }

};

//void Main_Test_List();



#endif // !_LIST_H
