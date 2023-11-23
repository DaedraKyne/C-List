// StringList.cpp : This file contains the 'main_StringList' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdexcept>
#include <string>
#include <memory>
#include <cassert>


#include "list.h"


template <typename T>
List<T> foo(List<T> org) {
	List<T> tmp(org);
	return tmp;
}

template<typename U>
bool eq(U v) { return v == "List[processed]"; }



void Main_Test_List_String() {
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

	std::cout << "\n\n";

	List<std::string> copy3(foo(string_list));
	std::cout << "Copied list using move constructor.\n";
	copy3.Print();

	std::cout << "\n\n";

	List<std::string> copy4 = foo(string_list);
	std::cout << "Copied list using move assignement.\n";
	copy4.Print();

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
	string_list.Print();

	string_list[0] = "hi";
	string_list.Print();

	string_list.Add("bob");
	string_list.Print();

	string_list.RemoveIf([](const auto& val) {return val.find("[processed]") == std::string::npos;  });
	string_list.Print();

}