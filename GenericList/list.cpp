// StringList.cpp : This file contains the 'main_StringList' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

#include "list.h"


template <typename T>
List<T>::List() : data(nullptr), capacity(0), count(0) {

}


template <typename T>
void swap(List<T>& first, List<T>& second) {
	using std::swap; //fall back to std::swap if swap(T,T) isn't defined
	//explanation: calling swap makes it unqualified, meaning if swap(T,T) is specially defined, it is called, otherwise, call std::swap(T,T)
	swap(first.capacity, second.capacity);
	swap(first.count, second.count);
	swap(first.data, second.data);
}
//Rule of 3

//Destructor
template <typename T>
List<T>::~List() {
	delete[] data;
}

//Copy constructor
template <typename T>
List<T>::List(const List<T>& other) : capacity(other.capacity), count(other.count) {
	data = CreateDeepCopy(other.data, other.capacity);
}

template <typename T>
List<T>& List<T>::operator=(List<T> other) {
	//Note: other is not a const reference but a copy of the value, allowing for use of swap logic (since "other" will be destroyed after
	swap(*this, other);
	return *this;
}


template <typename T>
int List<T>::Capacity() const { return capacity; }

template <typename T>
bool List<T>::Capacity(const int& new_capacity) {
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

template <typename T>
int List<T>::Count() const { return count; }

template <typename T>
void List<T>::Print() const {
	if (count == 0) {
		std::cout << ""; return;
	}
	std::cout << "(";
	for (int i = 0; i < count - 1; i++) {
		std::cout << data[i] << ", ";
	}
	std::cout << data[count - 1] << ")\n";
}

template <typename T>
bool List<T>::Add(const T& new_val) {
	if (capacity < count + 1) {
		Capacity(capacity == 0 ? 1 : capacity * 2); //double array size
	}
	data[count++] = new_val;
	return true;
}

template <typename T>
bool List<T>::Contains(const T& val) const {
	return IndexOf(val) != -1;
}

template <typename T>
bool List<T>::RemoveAt(const int& index) {
	if (index < 0) return false;
	if (index >= count) return false;
	//allowed setting: index = [0, count-1], count > 0 (data is initialized)

	memmove(data + index, data + index + 1, (count - index - 1) * sizeof(T)); //shallow shuffle left
	count--;
	return true;
}

template <typename T>
int List<T>::IndexOf(const T& val) const {
	for (int i = 0; i < count; i++) {
		if (data[i] == val) return i;
	}
	return -1;
}

template <typename T>
bool List<T>::Remove(const T& val) {
	int index = IndexOf(val);
	return RemoveAt(index);
}

template <typename T>
T List<T>::operator[](const int& index) const {
	return Get(index);
}

template <typename T>
T List<T>::Get(const int& index) const {
	if (index >= count || index < 0) throw std::out_of_range(string("Cannot get element at out_of_range index: ") + to_string(index) + string(")"));

	return data[index];
}


template <typename T>
T* List<T>::CreateDeepCopy(T* const& data, size_t const& data_size, size_t const& copy_size) {
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

template <typename T>
T* List<T>::CreateDeepCopy(T* const& data, size_t const& data_size) {
	return CreateDeepCopy(data, data_size, data_size);
}



void Main_Test_List() {
	List<std::string> string_list;
	std::cout << "List count: " << string_list.Count() << "\n";

	std::string s1 = "List";
	string_list.Add(s1);
	std::cout << "Added element: " << s1 << "\n";
	std::cout << "List count: " << string_list.Count() << "\n";

	std::cout << "\n";

	s1 = "<string>";
	string_list.Add(s1);
	std::cout << "Added element: " << s1 << ".\n";
	std::cout << "List count: " << string_list.Count() << "\n";

	std::cout << "\n";

	string_list.Print();

	std::cout << "\n\n";

	s1 = "<string>";
	string_list.Remove(s1);
	std::cout << "Removed element: " << s1 << ".\n";
	std::cout << "List count: " << string_list.Count() << "\n";

	std::cout << "\n";

	s1 = "<T>";
	string_list.Add(s1);
	std::cout << "Added element: " << s1 << "\n";
	std::cout << "List count: " << string_list.Count() << "\n";

	string_list.Print();

	std::cout << "Element at index 0: " << string_list.Get(0) << ".\n";
	std::cout << "Element at index 1: " << string_list[1] << ".\n";

	std::cout << "\n\n";

	List<std::string> copy1(string_list);
	std::cout << "Copied list using copy constructor.\n";
	copy1.Print();

	std::cout << "\n\n";

	List<std::string> copy2 = string_list;
	std::cout << "Copied list using copy assignement.\n";
	copy2.Print();


	List<int>test;
	test.Add(5);
	test.Print();



}