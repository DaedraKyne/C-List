// StringList.cpp : This file contains the 'main_StringList' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdexcept>
#include <string>
#include <memory>
#include <cassert>


#include "string_list.h"



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

	string_list.RemoveIf([](const auto& val) {return val.find("[processed]") == std::string::npos;  });
	std::cout << string_list.ToString() << "\n";

}