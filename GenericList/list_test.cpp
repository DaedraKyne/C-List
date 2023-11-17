// StringList.cpp : This file contains the 'main_StringList' function. Program execution begins and ends there.
//
#ifndef _LIST_IMPL_H_
#define _LIST_IMPL_H_

#include <iostream>
#include <stdexcept>
#include <string>

#include "list.h"

using namespace std;

//TODO: https://stackoverflow.com/questions/495021/why-can-templates-only-be-implemented-in-the-header-file
//https://isocpp.org/wiki/faq/templates#class-templates


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


	//List<int>test;
	//test.Add(5);
	//test.Print();



}


#endif

