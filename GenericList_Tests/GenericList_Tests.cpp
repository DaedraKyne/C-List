#include "pch.h"
#include "CppUnitTest.h"
#include "../GenericList/list.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GenericListTests
{
	TEST_CLASS(GenericListTests)
	{
	public:
		
		TEST_METHOD(TestIntList)
		{
			List<int> intList;
			for (int i = 0; i < 10; i++) {
				intList.Add(i);
			}

			for (int i = 0; i < 10; i++) {
				Assert::AreEqual(intList[i], i);
			}
		}
	};
}
