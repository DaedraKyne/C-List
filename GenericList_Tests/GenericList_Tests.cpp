#include <string>

#include "pch.h"
#include "CppUnitTest.h"
#include "../GenericList/list.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

//Unit tests development: https://alexott.net/en/cpp/CppTestingIntro.html

namespace GenericListTests
{
	TEST_CLASS(GenericListTests)
	{
	public:
		
		//Constructors

		TEST_METHOD(AddGet_FundamentalTypes) {
			List<int> list;
			int value = 5;
			int value_copy = value;

			list.Add(value);
			Assert::IsTrue(list[0] == value);
			Assert::IsFalse(&list[0] == &value);

			list.Add(std::move(value_copy));
			Assert::IsTrue(list[1] == value);
		}

		TEST_METHOD(AddGet_ClassTypes) {
			List<string> list;
			string value = "data";
			string value_copy = value;

			list.Add(value);
			Assert::IsTrue(list[0] == value);
			Assert::IsFalse(&list[0] == &value);

			list.Add(std::move(value_copy));
			Assert::IsTrue(list[1] == value);
		}

		TEST_METHOD(ConstructorsAssignements_FundamentalTypes) {
			//default constructor
			List<int> list; 
			list.Add(5); //fill with info
			Assert::AreEqual(list[0], 5);

			//copy constructor
			List<int> list2(list);
			Assert::AreEqual(list2[0], 5);
			Assert::AreNotEqual(&list2[0], &list[0]);

			//copy assignement
			List<int> list22 = list2;
			Assert::AreEqual(list22[0], 5);
			Assert::AreNotEqual(&list22[0], &list2[0]);

			//move constructor
			List<int> list3(std::move(list));
			Assert::AreEqual(list3[0], 5);

			//move assignement
			List<int> list33 = std::move(list3);
			Assert::AreEqual(list33[0], 5);
		}

		TEST_METHOD(ConstructorsAssignements_ClassTypes) {
			//default constructor
			List<string> list;
			string data = "test";
			list.Add(data); //fill with info
			Assert::IsTrue(list[0] == data);
			Assert::IsFalse(&list[0] == &data);

			//copy constructor
			List<string> list2(list);
			Assert::IsTrue(list2[0] == data);
			Assert::IsFalse(&list2[0] == &list[0]);

			//copy assignement
			List<string> list22 = list2;
			Assert::IsTrue(list22[0] == data);
			Assert::IsFalse(&list22[0] == &list2[0]);

			//move constructor
			List<string> list3(std::move(list));
			Assert::IsTrue(list3[0] == data);

			//move assignement
			List<string> list33 = std::move(list3);
			Assert::IsTrue(list33[0] == data);
		}

		TEST_METHOD(CapacityCount_Read) {
			List<int> list;
			Assert::IsTrue(list.Capacity() == 0);
			Assert::IsTrue(list.Count() == 0);

			int addedAmount = 10;
			for (int i = 0; i < addedAmount; i++) {
				list.Add(0);
			}

			Assert::IsTrue(list.Count() == addedAmount);
			Assert::IsTrue(list.Capacity() >= list.Count());

		}

		TEST_METHOD(Capacity_Write) {
			List<int> list;

			size_t addedAmount = 10;
			for (size_t i = 0; i < addedAmount; i++) {
				list.Add(0);
			}

			size_t oldCapacity, newCapacity;

			oldCapacity = list.Capacity();
			newCapacity = addedAmount;
			list.Capacity(newCapacity);
			Assert::IsFalse(list.Capacity() == oldCapacity);
			Assert::IsTrue(list.Capacity() == newCapacity);

			oldCapacity = list.Capacity();
			newCapacity = addedAmount - 10;
			list.Capacity(newCapacity);
			Assert::IsFalse(list.Capacity() == newCapacity);
			Assert::IsTrue(list.Capacity() == oldCapacity);

			oldCapacity = list.Capacity();
			newCapacity = addedAmount + 10;
			list.Capacity(newCapacity);
			Assert::IsFalse(list.Capacity() == oldCapacity);
			Assert::IsTrue(list.Capacity() == newCapacity);
		}

		TEST_METHOD(Contains_FundamentalTypes) {
			List<int> list;
			int addedValue = 5;
			int notAddedValue = 6;
			list.Add(addedValue);
			Assert::IsTrue(list.Contains(addedValue));
			Assert::IsFalse(list.Contains(notAddedValue));
		}
		TEST_METHOD(Contains_ClassTypes) {
			List<string> list;
			string addedValue = "a";
			string notAddedValue = "b";
			list.Add(addedValue);
			Assert::IsTrue(list.Contains(addedValue));
			Assert::IsFalse(list.Contains(notAddedValue));
		}

		TEST_METHOD(RemoveAt_FundamentalTypes) {
			List<int> list;

			size_t addedAmount = 10;

			for (size_t i = 0; i < addedAmount; i++) {
				list.Add(i);
			}

			size_t removedIndex;
			bool removed;

			removedIndex = 5;
			removed = list.RemoveAt(removedIndex);
			Assert::IsTrue(removed);
			Assert::IsTrue(removedIndex == list.Count() || list[removedIndex] > (int)removedIndex);

			removedIndex = addedAmount - 1;
			removed = list.RemoveAt(removedIndex);
			Assert::IsFalse(removed);
		}

		TEST_METHOD(RemoveAt_ClassTypes) {
			List<string> list;

			size_t addedAmount = 10;
			string addedValue = "5";

			for (size_t i = 0; i < addedAmount; i++) {
				list.Add(addedValue);
			}

			size_t removedIndex;
			bool removed;

			removedIndex = 5;
			removed = list.RemoveAt(removedIndex);
			Assert::IsTrue(removed);

			removedIndex = addedAmount - 1;
			removed = list.RemoveAt(removedIndex);
			Assert::IsFalse(removed);
		}


		TEST_METHOD(IndexOf_FundamentalTypes) {
			List<int> list;

			size_t addedAmount = 10;

			for (size_t i = 0; i < addedAmount; i++) {
				list.Add(i * 2);
			}

			size_t indexedElement;
			size_t index;

			indexedElement = 5 * 2;
			index = list.IndexOf(indexedElement);
			Assert::IsTrue(index == indexedElement / 2);

			indexedElement = addedAmount * 2;
			index = list.IndexOf(indexedElement);
			Assert::IsFalse(index == indexedElement / 2);
		}

		TEST_METHOD(IndexOf_ClassTypes) {
			List<string> list;

			string addedElements[] = { "hi", ",", "bob" };

			string notAddedElements[] = { "by", "!", "tot" };

			for (size_t i = 0; i < 3; i++) {
				list.Add(addedElements[i]);
			}

			for (size_t i = 0; i < 3; i++) {
				Assert::IsTrue(list.IndexOf(addedElements[i]) == i);
			}


			for (size_t i = 0; i < 3; i++) {
				Assert::IsTrue(list.IndexOf(notAddedElements[i]) == -1);
			}
		}


		TEST_METHOD(Remove_FundamentalTypes) {
			List<int> list;

			size_t addedAmount = 10;

			for (size_t i = 0; i < addedAmount; i++) {
				list.Add(i * 2);
			}

			Assert::IsFalse(list.Remove(addedAmount * 2 + 5));

			for (size_t i = 0; i < addedAmount; i++) {
				Assert::IsTrue(list.Remove(i * 2));
			}
			Assert::IsTrue(list.Count() == 0);

		}

		TEST_METHOD(Remove_ClassTypes) {
			List<string> list;

			string addedElements[] = { "hi", ",", "bob" };

			string notAddedElements[] = { "by", "!", "tot" };

			for (size_t i = 0; i < 3; i++) {
				list.Add(addedElements[i]);
			}

			for (size_t i = 0; i < 3; i++) {
				Assert::IsFalse(list.Remove(notAddedElements[i]));
			}

			for (size_t i = 0; i < 3; i++) {
				Assert::IsTrue(list.Remove(addedElements[i]));
			}
			Assert::IsTrue(list.Count() == 0);


		}

		TEST_METHOD(Iterator) {
			List<int> list;
			
			size_t addedAmount = 10;
			for (size_t i = 0; i < addedAmount; i++) {
				list.Add(i * 2);
			}

			int curr = 0;
			for (int e : list) {
				Assert::IsTrue(e == (curr++) * 2);
			}

		}

	};
}
