#include "pch.h"
#include "CppUnitTest.h"
#include "../GenericList/list.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace GenericListTest
{
	TEST_CLASS(GenericListTests)
	{
	public:

		//Constructors

		TEST_METHOD(AddAccess_FundamentalTypes) {
			List<int> list;
			int value = 5;
			int value_copy = value;

			list.Add(value);
			Assert::IsTrue(list[0] == value);
			Assert::IsFalse(&list[0] == &value);

			list.Add(std::move(value_copy));
			Assert::IsTrue(list[1] == value);
		}

		TEST_METHOD(AddAccess_ClassTypes) {
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

		TEST_METHOD(CapacityShrink_Write) {
			List<int> list;

			size_t addedAmount = 10;
			for (size_t i = 0; i < addedAmount; i++) {
				list.Add(0);
			}

			size_t oldCapacity, newCapacity;

			//newCapacity < oldCapacity
			oldCapacity = list.Capacity();
			newCapacity = addedAmount;
			list.Capacity(newCapacity);
			Assert::IsTrue(list.Capacity() == oldCapacity);
			Assert::IsFalse(list.Capacity() == newCapacity);

			//newCapacity < count
			oldCapacity = list.Capacity();
			newCapacity = addedAmount - 10;
			list.Capacity(newCapacity);
			Assert::IsFalse(list.Capacity() == newCapacity);
			Assert::IsTrue(list.Capacity() == oldCapacity);

			//newCapacity > oldCapacity
			oldCapacity = list.Capacity();
			newCapacity = oldCapacity + 10;
			list.Capacity(newCapacity);
			Assert::IsFalse(list.Capacity() == oldCapacity);
			Assert::IsTrue(list.Capacity() == newCapacity);

			// oldCapacity > count
			oldCapacity = list.Capacity();
			list.ShrinkToFit();
			Assert::IsFalse(list.Capacity() == oldCapacity);
			Assert::IsTrue(list.Capacity() == list.Count());
		}

		TEST_METHOD(RemoveAt_FundamentalTypes) {
			List<int> list;

			const auto addedAmount = 10;

			for (size_t i = 0; i < addedAmount; i++) {
				list.Add(i);
			}

			size_t removedIndex;
			size_t oldCount;

			//removedIndex is within list
			removedIndex = 5;
			int nextElement = list[removedIndex + 1];
			oldCount = list.Count();
			list.RemoveAt(removedIndex);
			Assert::IsTrue(removedIndex == list.Count() || list[removedIndex] > (int)removedIndex);
			Assert::IsTrue(oldCount == list.Count() + 1);

			//removedIndex is not within list
			removedIndex = addedAmount - 1;
			Assert::ExpectException<std::out_of_range>([&]() { list.RemoveAt(removedIndex); });
		}

		TEST_METHOD(RemoveAt_ClassTypes) {
			List<string> list;
			string e = "test";

			const auto addedAmount = 10;

			for (size_t i = 0; i < addedAmount; i++) {
				list.Add(e);
			}

			size_t removedIndex;
			size_t oldCount;

			//removedIndex is within list
			removedIndex = 5;
			oldCount = list.Count();
			list.RemoveAt(removedIndex);
			Assert::IsTrue(oldCount == list.Count() + 1);

			//removedIndex is not within list
			removedIndex = addedAmount - 1;
			Assert::ExpectException<std::out_of_range>([&]() { list.RemoveAt(removedIndex); });
		}

		


		TEST_METHOD(Find_FundamentalTypes) {
			List<int> list;

			size_t addedAmount = 10;

			for (size_t i = 0; i < addedAmount; i++) {
				list.Add(i * 2);
			}

			size_t indexedElement;
			size_t index;

			indexedElement = 5 * 2;
			index = list.Find(indexedElement) - list.begin();
			Assert::IsTrue(index == indexedElement / 2);

			indexedElement = addedAmount * 2;
			Assert::IsTrue(list.Find(indexedElement) == nullptr);
		}

		TEST_METHOD(Find_ClassTypes) {
			List<string> list;

			string addedElements[] = { "hi", ",", "bob" };

			string notAddedElements[] = { "by", "!", "tot" };

			for (size_t i = 0; i < 3; i++) {
				list.Add(addedElements[i]);
			}

			for (size_t i = 0; i < 3; i++) {
				Assert::IsTrue(list.Find(addedElements[i]) - list.begin() == i);
			}


			for (size_t i = 0; i < 3; i++) {
				Assert::IsTrue(list.Find(notAddedElements[i]) == nullptr);
			}
		}


		TEST_METHOD(FindIf_FundamentalTypes) {
			List<int> list;

			size_t addedAmount = 10;

			for (size_t i = 0; i < addedAmount; i++) {
				list.Add(i * 2);
			}

			size_t indexedElement;
			size_t index;

			indexedElement = 5 * 2;
			index = list.FindIf([&](const auto& e) { return e == indexedElement; }) - list.begin();
			Assert::IsTrue(index == indexedElement / 2);

			Assert::IsTrue(list.FindIf([&](const auto& e) { return e >= int(list.Count()) * 2; }) == nullptr);
		}

		TEST_METHOD(FindIf_ClassTypes) {
			List<string> list;

			string addedElements[] = { "hi", ",", "bob" };

			string notAddedElements[] = { "by", "!", "tot" };

			for (size_t i = 0; i < 3; i++) {
				list.Add(addedElements[i]);
			}

			for (size_t i = 0; i < 3; i++) {
				Assert::IsTrue(list.FindIf([&](const auto& e) { return e == addedElements[i]; }) - list.begin() == i);
			}


			for (size_t i = 0; i < 3; i++) {
				Assert::IsTrue(list.FindIf([&](const auto& e) { return e == notAddedElements[i]; }) == nullptr);
			}
		}


		TEST_METHOD(Remove_FundamentalTypes) {
			List<int> list;

			size_t addedAmount = 10;

			for (size_t i = 0; i < addedAmount; i++) {
				list.Add(i * 2);
			}

			Assert::IsFalse(list.Remove(addedAmount * 2 + 5) > 0);

			for (size_t i = 0; i < addedAmount; i++) {
				Assert::IsTrue(list.Remove(i * 2) == 1);
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
				Assert::IsFalse(list.Remove(notAddedElements[i]) > 0);
			}

			for (size_t i = 0; i < 3; i++) {
				Assert::IsTrue(list.Remove(addedElements[i]) == 1);
			}
			Assert::IsTrue(list.Count() == 0);
		}

		TEST_METHOD(RemoveIf_FundamentalTypes) {
			List<int> list;

			size_t addedAmount = 10;

			for (size_t i = 0; i < addedAmount; i++) {
				list.Add(i * 2);
			}

			Assert::IsFalse(list.Remove(addedAmount * 2 + 5) > 0);

			size_t oldCount = list.Count();
			Assert::IsTrue(
				list.RemoveIf(
					[&](const auto& e) { 
						return e % 2 == 0 && e / 2 < int(list.Count());  
					}) 
					== oldCount
				);
			Assert::IsTrue(list.Count() == 0);
		}


		TEST_METHOD(RemoveIf_ClassTypes) {
			List<string> list;

			string addedElements[] = { "hi", ",", "bob" };

			string notAddedElements[] = { "by", "!", "tot" };

			for (size_t i = 0; i < 3; i++) {
				list.Add(addedElements[i]);
			}

			//Remove non-added elements
			Assert::IsTrue(
				list.RemoveIf(
					[&](const auto& e) {
						return std::find(std::begin(notAddedElements), std::end(notAddedElements), e) != std::end(notAddedElements);
					})
				== 0
			);

			//Remove all added elements
			size_t oldCount = list.Count();
			Assert::IsTrue(
				list.RemoveIf(
					[&](const auto& e) {
						return std::find(std::begin(addedElements), std::end(addedElements), e) != std::end(addedElements);
					})
				== oldCount
			);
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