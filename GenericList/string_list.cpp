// StringList.cpp : This file contains the 'main_StringList' function. Program execution begins and ends there.
//

#include <iostream>

#include "string_list.h"

List_String::List_String() : data(nullptr), capacity(0), count(0) {

}

List_String::~List_String() {
	delete[] data;
}

int List_String::Capacity() { return capacity; }

bool List_String::Capacity(int new_capacity) {
	if (new_capacity < 0) return false; //allowed values must be >= 0
	if (new_capacity < count) return false; //can't have shorter capacity than element count
	if (new_capacity == capacity) return true; //no change
	if (new_capacity == 0) { //empty non-empty array
		delete data;
		return true;
	};
	
	// general case: dynamic resizing
	std::string* new_data = new std::string[new_capacity]; 
	for (int i = 0; i < capacity; i++) {
		new_data[i] = data[i];
	}
	//switch data
	delete[] data;
	data = new_data;
	capacity = new_capacity;
	return true;
}

int List_String::Count() {
	return count;
}

std::string List_String::ToString() {
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


bool List_String::Add(std::string new_val) {
	if (capacity < count + 1) {
		Capacity(capacity == 0 ? 1 : capacity * 2); //double array size
	}
	data[count++] = new_val;
	return true;
}

bool List_String::Contains(std::string val) {
	return IndexOf(val) != -1;
}

bool List_String::RemoveAt(int index) {
	if (index < 0) return false;
	if (index >= count) return false;
	//allowed setting: index = [0, count-1], count > 0 (data is initialized)

	std::memmove(data + index, data + index + 1, count - index - 1);
	count--;
	return true;
}


int List_String::IndexOf(std::string val) {
	for (int i = 0; i < count; i++) {
		if (data[i] == val) return i;
	}
	return -1;
}


bool List_String::Remove(std::string val) {
	int index = IndexOf(val);
	return RemoveAt(index);
}



void Main_Test_List_String() {
	List_String string_list;
	std::cout << "List count: " << string_list.Count() << "\n";
	
	std::string s1 = "List";
	string_list.Add(s1);
	std::cout << "Added element: " << s1 << "\n";
	std::cout << "List count: " << string_list.Count() << "\n";

	s1 = "<string>";
	string_list.Add(s1);
	std::cout << "Added element: " << s1 << ".\n";
	std::cout << "List count: " << string_list.Count() << "\n";

	std::cout << "Current list: " << string_list.ToString() << ".\n";
	

	s1 = "<string>";
	string_list.Remove(s1);
	std::cout << "Removed element: " << s1 << ".\n";
	std::cout << "List count: " << string_list.Count() << "\n";

	s1 = "<T>";
	string_list.Add(s1);
	std::cout << "Added element: " << s1 << "\n";
	std::cout << "List count: " << string_list.Count() << "\n";
	
	std::cout << "Current list: " << string_list.ToString() << ".\n";


}