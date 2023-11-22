// StringList.cpp : This file contains the 'main_StringList' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdexcept>
#include <string>
#include <memory>
#include <cassert>

using namespace std;

#include "string_list.h"

std::allocator<std::string> List_String::dataAllocator;

List_String::List_String() : data(nullptr), capacity(0), count(0) {

}


void swap(List_String& first, List_String& second) {
	//explanation: calling swap makes it unqualified, meaning if swap(T,T) is specially defined, it is called, otherwise, call std::swap(T,T)
	swap(first.capacity, second.capacity);
	swap(first.count, second.count);
	swap(first.data, second.data);
}
//Rule of 3

//Destructor
List_String::~List_String() {
	Clear();
	dataAllocator.deallocate(data, capacity);
}

//Copy constructor
List_String::List_String(const List_String& other) : 	data(CreateDeepCopy(other.data, other.capacity, other.count)), capacity(other.capacity), count(other.count) {}

//Copy assignement
List_String& List_String::operator=(const List_String& other) {
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
List_String::List_String(List_String&& other) : data(other.data), capacity(other.capacity), count(other.count) {
    other.data = nullptr;
    other.capacity = 0;
    other.count = 0;
}

List_String& List_String::operator=(List_String&& other) {
	List_String tmp(std::move(other));
	swap(*this, tmp);
	return *this;
	//old " *this "-related data is deleted automatically at function end (through temp destruction)
}


void List_String::Clear() {
	for (int i = 0; i < count; i++) {
		data[i].~string();
	}
	count = 0;
}

void List_String::ShrinkToFit() {
	if (capacity == count) return;
	//sadly, allocators don't support partial deallocation - need to fully resize
	Resize(count);
}


int List_String::Capacity() const { return capacity; }

void List_String::Capacity(int new_capacity) {
	if (new_capacity <= capacity) return; //no change
	Resize(new_capacity);
}

int List_String::Count() const { return count; }

std::string List_String::ToString() const {
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

std::string* List_String::Find(const std::string& val) {
	for (int i = 0; i < count; i++) {
		if (data[i] == val) return data+i; //TODO: research why &data[i] might get overloaded
	}
	return nullptr;
}


const std::string* List_String::Find(const std::string& val) const {
	for (int i = 0; i < count; i++) {
		if (data[i] == val) return data + i; //TODO: research why &data[i] might get overloaded
	}
	return nullptr;
}


void List_String::RemoveAt(int index) {
	if (index < 0 || index >= count) throw std::out_of_range(string("Cannot remove element at out_of_range index: ") + to_string(index) + string("."));
	
	std::move(data + index + 1, data + count, data + index);
	data[--count].~string();
}



bool List_String::Remove(const std::string& val) {
	std::string* ptr = Find(val);
	if (ptr!=nullptr) RemoveAt(ptr-data);
	return ptr!=nullptr;
}



const std::string& List_String::Get(int index) const {
	if (index >= count || index < 0) throw std::out_of_range(string("Cannot get element at out_of_range index: ") + to_string(index) + string(")"));

	return data[index];
}

std::string& List_String::Get(int index) {
	if (index >= count || index < 0) throw std::out_of_range(string("Cannot get element at out_of_range index: ") + to_string(index) + string(")"));

	return data[index];
}


std::string* List_String::CreateDeepCopy(std::string* data, size_t data_size, size_t copy_size) {
	if (copy_size > data_size || copy_size < 0) {
		throw std::out_of_range(string("Cannot copy array of size ") + to_string(copy_size) + string(" onto array of size ") + to_string(copy_size) + string("."));
	}
	std::string* new_data = data_size > 0 ? dataAllocator.allocate(data_size) : nullptr;


	//For improved performance, replace copy by memcpy+fill (no deep copy of non-POD objects) or by using a swap method
	for (size_t i = 0; i < copy_size; i++) {
		new (new_data + i) std::string(data[i]);
	}

	return new_data;
}

void List_String::Resize(int new_capacity) {
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


List_String foo(List_String org) {
	List_String tmp(org);
	return tmp;
}

template<typename U>
bool eq(U v) { return v == "List[processed]"; }



void Main_Test_List_String() {
	List_String string_list;
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

	std::cout << "Current list: " << string_list.ToString() << ".\n";
	
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
	
	std::cout << "Current list: " << string_list.ToString() << ".\n";

	std::cout << "Element at index 0: " << string_list.Get(0) << ".\n";
	std::cout << "Element at index 1: " << string_list[1] << ".\n";

	std::cout << "\n\n";

	List_String copy1(string_list);
	std::cout << "Copied list using copy constructor.\n";
	std::cout << "Copied list: " << copy1.ToString() << ".\n";

	std::cout << "\n\n";

	List_String copy2 = string_list;
	std::cout << "Copied list using copy assignement.\n";
	std::cout << "Copied list: " << copy2.ToString() << ".\n";

	std::cout << "\n\n";

	List_String copy3(foo(string_list));
	std::cout << "Copied list using move constructor.\n";
	std::cout << "Copied list: " << copy3.ToString() << ".\n";

	std::cout << "\n\n";

	List_String copy4 = foo(string_list);
	std::cout << "Copied list using move assignement.\n";
	std::cout << "Copied list: " << copy4.ToString() << ".\n";

	std::cout << "\n\n";

	std::cout << "Looping through data...\n";
	for (auto e : string_list) {
		std::cout << e << "\n";
	}

	std::cout << "\n\n";

	std::cout << "Testing iteration logic..\n";
	for (auto& str : string_list) {
		str += "[processed]";
	}

	for (const auto& str : string_list) {
		assert(str.find("[processed]") != std::string::npos);
	}

	std::cout << string_list.FindIf([](const auto& v) {return v == "List[processed]"; }) << "\n";
	std::cout << string_list.FindIf([](const auto& v) {return v == "<T>[processed]"; }) << "\n";
	std::cout << string_list.FindIf([](const auto& v) {return v == "F[processed]"; }) << "\n";
	std::cout << string_list.FindIf(eq<string>) << "\n";
	std::cout << string_list.FindIf([](const auto& v) {return eq(v); }) << "\n";
	std::cout << string_list.ToString() << "\n";

	string_list[0] = "hi";
	std::cout << string_list.ToString() << "\n";

	string_list.Add("bob");
	std::cout << string_list.ToString() << "\n";

}