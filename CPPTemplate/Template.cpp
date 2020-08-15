/*
ѧϰC++ģ��
������ת�Ľ̳̣�д�úܺ���⣬��ûд�꣺https://github.com/wuye9036/CppTemplateTutorial

*/

// ������ ����������׳�
template<int N>
struct Foo {
	static const int value = N * Foo<N - 1>::value;
};

// �ػ���0!=1����Ϊ��ֹ����
template<>
struct Foo<0> {
	static const int value = 1;
};

constexpr int a = Foo<10>::value;

#include<algorithm>
#include<iostream>
#include<bitset>
#include<vector>
#include<tuple>
using namespace std;

#define WRONG_CODE_ENABLED 0

// ---------------
// �ɱ�������ģ����� 
// ---------------

// print����������������
namespace _1_ {
// �ػ�
void print() {}

template<class T, class... Ts>
void print(const T& args1st, const Ts&... args)
{
	cout << args1st << endl;
	cout << (sizeof...(args)) << " params left:" << endl;
	print(args...);
}

// ����
template<class... Ts>
void print(const Ts&... args)
{
	cout << "!" << endl;
}

void test_print()
{
	print(1, 2, "helo", bitset<16>(0101), 42, 9.30f);
}

}

// ------------------Tuple------------------
// .	 .	  .	   .	.	 Head
// .	 .	  .	   .	Head  
// .	 .	  .	   Head	  
// .	 .	  Head	
// .	 Head
// Head

// �����������͵�����
namespace _2_ 
{
using namespace _1_;

template<class... Ts> class Tuple;

template<> class Tuple<> {};

template<class Head, class... Ts>
class Tuple<Head, Ts...> : private Tuple<Ts...>
{
	typedef Tuple<Ts...> inherited;
public:
	Tuple() {}
	Tuple(const Head& h, const Ts&... ts)
		: head_(h), inherited(ts...) {}

	const Head& head() const { return head_; }
	const inherited& tail() const { return *this; }

	/*
	��ô�����ڲ�get��auto�Ƶ�
	template<size_t I>
	const auto& get() const { return tail().get<I - 1>;	}

	template<0>
	const auto& get() const { return head(); }
	*/

	template<size_t I>
	const auto& get() const;

protected:
	Head head_;
};

template<size_t N, typename Head> class TupleElem;

template<size_t N, typename Head, typename... Ts>
class TupleElem<N, Tuple<Head, Ts...>> : public TupleElem <N - 1, Tuple<Ts...>>
{
public:
	typedef typename TupleElem<N, Tuple<Head, Ts...>>::Type type;
	static const type& Get(const Tuple<Head, Ts...>& tuple)
	{
		return TupleElem<N - 1, Tuple<Ts...>>::Get(tuple.tail());
	}
};

template<typename Head, typename... Ts>
class TupleElem<0, Tuple<Head, Ts...>>
{
public:
	typedef Head Type;
	static const Type& Get(const Tuple<Head, Ts...>& tuple)
	{
		return tuple.head();
	}
};

template<size_t I, typename Head, typename... Ts>
typename const TupleElem<I, Tuple<Head, Ts...>>::Type& Get(const Tuple<Head, Ts...>& t)
{
	return TupleElem<I, Tuple<Head, Ts...>>::Get(t);
}

template<class Head, class... Ts>
template<size_t I>
const auto& Tuple<Head, Ts...>::get() const
{
	return Get<I>(*this);
}

// -------------- std tuple --------------
void test_tuple()
{
	Tuple<int, float, string> t1(1, 0.1f, "h1");
	tuple<int, float, string> t2(2, 0.2f, "h2");

	bool b = is_same<decltype(t1), decltype(t2)>();

	cout << Get<0>(t1) << endl;
	cout << Get<1>(t1) << endl;
	cout << Get<2>(t1) << endl;

	cout << t1.get<0>() << endl;
	cout << t1.get<1>() << endl;
	cout << t1.get<2>() << endl;

	print(t1.get<0>(), t1.get<1>(), t1.get<2>());

	cout << get<0>(t2) << endl;
	cout << get<1>(t2) << endl;
	cout << get<2>(t2) << endl;
}

}

// SFINAE �滻ʧ�ܲ��Ǵ��󣬻���������滻����
namespace _3_ {

template <typename A>
struct B { using type = typename A::type; };

template <
	class T,
	class U = typename T::type,      // �� T �޳�Ա type ��Ϊ SFINAE ʧ��
	class V = typename B<T>::type    // �� T �޳�Ա type ��ΪӲ���� C++14 ��֤�����֣���Ϊ�� U ��Ĭ��ģ��ʵ���е��滻������ʧ�ܣ�
> void foo(int) {};

struct C { using type = int; };
struct D { using type2 = int; };

#define explicit_instantiate 0
#if explicit_instantiate

template<> void foo<B<C>>(int) {} // ����ģ����ʽʵ����

#if WRONG_CODE_ENABLED
	template<> void foo<B<D>>(int) {}
#endif

#else

void test_sfinae1()
{
	foo<B<C>>(0);
#if WRONG_CODE_ENABLED
	foo<B<D>>(0); // ����ģ����ʽʵ����
#endif
}

#endif // explicit_instantiate

}

namespace _4_{

template <int I> struct X { };
template <template <class T> class> struct Z { };
template <class T> void f(typename T::Y*) {}
template <class T> void g(X<T::N>*) {}
template <class T> void h(Z<T::template TT>*) {}
struct A {};
struct B { int Y; };
struct C { typedef int N; };
struct D { typedef int TT; };
struct B1 { typedef int Y; };
struct C1 { static const int N = 0; };
struct D1 {
	template <typename T>
	struct TT
	{
	};
};

void test_sfinae() {

#if WRONG_CODE_ENABLED
	// ���и�������Ƶ�ʧ�ܣ�
	f<A>(0); // ������Ա Y
	f<B>(0); // B �� Y ��Ա��������
	g<C>(0); // C �� N ��Ա���Ƿ�����
	h<D>(0); // D �� TT ��Ա����ģ��
#endif

	// ���и�������Ƶ��ɹ���
	f<B1>(0);
	g<C1>(0);
	h<D1>(0);
}

}

// std::is_enum
namespace _5_{

/*
https://zh.cppreference.com/w/cpp/types/is_enum
https://cloud.tencent.com/developer/ask/120661
https://stackoverflow.com/questions/13082635/boostis-enum-how-it-works
https://stackoverflow.com/questions/11316912/is-enum-implementation
*/

#include <type_traits>

class AA {};

enum E {};

enum class Ec : int {};

template<class T>
struct my_is_enum { static const bool value = true; };

template<>
struct my_is_enum<enum T> { static const bool value = false; };


void test_is_enum()
{
	cout << my_is_enum<AA>::value << '\n';
	cout << my_is_enum<E>::value << '\n';
	cout << my_is_enum<Ec>::value << '\n';
	cout << my_is_enum<int>::value << '\n';

	bool a = is_enum<AA>::value;

	system( "pause" );
}

namespace _0_ {

// ������ħ��
// STRUCT TEMPLATE is_enum
#if 0
template <class _Ty>
struct is_enum : bool_constant<__is_enum(_Ty)> {}; // determine whether _Ty is an enumerated type
#endif

// �ų���
template <class _Tp>
struct libc_is_enum : public integral_constant<bool,	!is_void<_Tp>::value &&
														!is_integral<_Tp>::value &&
														!is_floating_point<_Tp>::value &&
														!is_array<_Tp>::value &&
														!is_pointer<_Tp>::value &&
														!is_reference<_Tp>::value &&
														!is_member_pointer<_Tp>::value &&
														!is_union<_Tp>::value &&
														!is_class<_Tp>::value &&
														!is_function<_Tp>::value > 
{
	// 
};

class AA {};

enum E {};

enum class Ec : int {};

constexpr bool v0 = libc_is_enum<AA>::value;
constexpr bool v1 = libc_is_enum<E>::value;
constexpr bool v2 = libc_is_enum<Ec>::value;
constexpr bool v3 = libc_is_enum<int>::value;

}

// ����һ����򵥵İɣ�����ж�����(int��long��char....)

// ��һ�γ��ԣ�ʹ���ػ�
namespace _1_ {

template<class T>
struct is_int { static const bool value = false; };

template<>
struct is_int<int> { static const bool value = true; };
template<>
struct is_int<long> { static const bool value = true; };

//...������һ��Ҫд�ģ������뵽������define

#define MAKETRAIT(is_t, t, v) \
template<> \
struct is_t<t> { static const bool value = v; };

MAKETRAIT(is_int, unsigned, true)
MAKETRAIT(is_int, char, true)

#if WRONG_CODE_ENABLED
MAKETRAIT(is_float, float, true)
#endif

template<class T>
struct is_float { static const bool value = false; };

MAKETRAIT(is_float, float, true)
MAKETRAIT(is_float, double, true)

#define MAKETRAIT0(is_t) \
template<class T> \
struct is_t { static const bool value = false; };

MAKETRAIT0(is_ptr)
MAKETRAIT(is_ptr, nullptr_t, true)
MAKETRAIT(is_ptr, void*, true)

//...��һ��һ��д��һЩ�����ǿ�������̫�ÿ����ܲ�����vairiadic
//�ڴ���д����is_int<int, long, char, ...>

}

// ��һ�γ���
namespace _2_ {
	
template<class... T> struct is_int;
template<> struct is_int<> {};

template<class T>
struct is_int<T> {
	static const bool value = true;
};

template<class T0, class... T >
struct is_int<T0, T...> {
	static const bool value = is_int<T0>::value || is_int<T...>::value;
};

constexpr int v = is_int<int, float, char>::value;

// �������е�����ȫ��int�ˣ���ô�������أ�������
namespace stdimp{

constexpr int v = std::is_integral<int>::value;

template<class T>
constexpr bool stdisint = std::_Is_any_of_v<T, int, bool, unsigned>;

constexpr int v1 = stdisint<int>;
constexpr int v2 = std::_Is_any_of_v<int, int>;

}

}

// ��ô�����أ�
// �ٳ���
namespace _3_ {

//��ʵ��is_same���ػ�����������ɣ���ͬ���͸����⣩
template<class T, class Tp>
struct is_same { static const bool value = false; };

template<class T>
struct is_same<T, T> { static const bool value = true; };

#if 1
template<class... T> struct is_int { static const bool value = false; };
#else
template<class... T> struct is_int;
template<> struct is_int<> { static const bool value = false; };
template<class T> struct is_int<T> { static const bool value = false; };
#endif

template<class T, class T0>
struct is_int<T, T0> {
	static const bool value = is_same<T, T0>::value;
};

template<class T, class T0, class... Ts>
struct is_int<T, T0, Ts...> {
	static const bool value = is_same<T, T0>::value || is_int<T, Ts...>::value;
};

// �ص����ڷ���T�ͺ����Ts���ú���Ŀ��Ե���
constexpr int v1 = is_int<float, int, float, char>::value;
constexpr int v2 = is_int<float>::value;
constexpr int v3 = is_int<>::value;

// ���Զ�����
template<class T>
using my_is_integral = is_int<T, bool, char, signed char, unsigned char, wchar_t, char16_t, char32_t,
								 short, unsigned short, int, unsigned int, long, unsigned long, 
								 long long, unsigned long long>;

constexpr int v4 = my_is_integral<int>::value;
constexpr int v5 = my_is_integral<const int>::value; // remove_cv�Ȳ����ˣ�˼·���ػ�ָ��ȥ��ָ��һ�� template<T*> + using type = T;
constexpr int v6 = my_is_integral<unsigned>::value;
constexpr int v7 = my_is_integral<double>::value;

// ���������ж�ԭ������

namespace _4_{

template <class _Tp>
struct libc_is_enum : public integral_constant<bool, 
	!is_void<_Tp>::value &&
	!my_is_integral<_Tp>::value &&
	!is_floating_point<_Tp>::value &&
	!is_array<_Tp>::value &&
	!is_pointer<_Tp>::value &&
	!is_reference<_Tp>::value &&
	!is_member_pointer<_Tp>::value &&
	!is_union<_Tp>::value && // ���Ҳ�Ǳ������ɵ� ������
	!is_class<_Tp>::value && // ���Ҳ�Ǳ������ɵ� ������!std::is_union<T>
	!is_function<_Tp>::value > {}; // https://en.cppreference.com/w/cpp/types/is_function �����ʵ�֣�һ��

class AA {};

enum E {};

enum class Ec : int {}; 

constexpr bool v0 = libc_is_enum<AA>::value;
constexpr bool v1 = libc_is_enum<E>::value;
constexpr bool v2 = libc_is_enum<Ec>::value;
constexpr bool v3 = libc_is_enum<int>::value;

}

}

}

namespace _5_{
// ģ���е�if else-
// ��sizeС�ڵ�int��int���ɣ�����aglist
#include<stdarg.h>

template<class T, int n>
void print_aglist(T x, ...)
{
#if 0
	using UpType = conditional_t<is_integral_v<T>,
		conditional_t<sizeof(T) <= sizeof(int), int, T>,
		conditional_t<is_floating_point_v<T> && sizeof(T) <= sizeof(double), double, T>>;
#else
	using UpType = T;
#endif
	va_list args;
	va_start(args, x);

	cout << x << endl;
	for (size_t i = 1; i < n; i++)
		cout << T(va_arg(args, UpType)) << endl;

	va_end(args);
}

void test_conditional()
{
	print_aglist<int, 5>(1, 2, 3, 4, 5);
	print_aglist<char, 5>('a', 'b', 'c', 'd', 'e');
	print_aglist<float, 5>(0.1, 1.2, 5.1, 55.1, 32.0);
}

}

int main()
{
	//_2_::test_tuple();
	//_5_::test_conditional();

	return 0;
}
