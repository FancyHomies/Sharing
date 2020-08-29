/* 
学习C++2.0新特性
参考:

侯捷——C++11新特性:
视频：https://www.bilibili.com/video/av51863195?from=search&seid=3610634846288253061
侯捷C++课程PPT及代码：https://github.com/harvestlamb/Cpp_houjie
相似教程：https://light-city.club/sc/c%2B%2B2.0/c%2B%2B11/c%2B%2B11/

C++学习网站：

C++手册，有中文，可以中英对照加深理解：
https://zh.cppreference.com/w/cpp/11

C++发明人博客：
http://www.stroustrup.com/C++.html

另一个查资料网站，感觉不如第一个好用：
http://www.cplusplus.com/

有好的学习资料请补充
*/

#include<iostream>
#include<vector>
using namespace std;

namespace Cpp2
{

// C++11 新特性

// initializer list
namespace _1 {

	struct Foo {
		Foo() {}
		Foo(int a, int b) {}
	};

	Foo f1(1, 2);
	Foo f2 = Foo(1, 2);
	Foo f3{ 1, 2 };  // C++11

}


// nullptr
// C++11之前NULL无法匹配指针类型
namespace _2 {

	void f(int) { cout << "int" << endl; }
	void f(void*) { cout << "ptr" << endl; }

	void test_nullptr()
	{

		f(0);
		f(NULL);
		f(nullptr);

		cout << typeid(nullptr).name() << endl;
		nullptr_t a;
		cout << typeid(nullptr_t).name() << endl;

		system("pause");
	}

}

// alias
// using xxx = int; 
// typedef int xxx;
namespace _3 {

	template<typename T>
	struct myalloc : std::allocator<T> {
	};

	template<typename T>
	using Vec = std::vector<T, myalloc<T>>;

	Vec<int> vi;

}

// lambda
// [ captures ] <tparams>(optional)(c++20) ( params ) specifiers exception attr -> ret requires(optional)(c++20) { body }
namespace _4 {

	template<class T>
	void printaddress(const T& v)
	{
		cout << "v address:" << reinterpret_cast<int>(&v) << endl;
	}

	void test_lambda()
	{
		int id = 0;
		printaddress(id);
		auto f = [id]() mutable -> int { printaddress(id); std::cout << id; return ++id; };
		cout << typeid(f).name() << endl; // lambda是一个functor

#if 0 // 无法被构造
		decltype(f) f1(); 
		cout << f1()() << endl; 
#endif

		id = 42;
		cout << f() << endl;
		cout << f() << endl;
		cout << id << endl;
	}
}

}

void main()
{
	Cpp2::_4::test_lambda();
	system( "pause" );
}

