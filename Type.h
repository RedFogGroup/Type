
namespace Types{
	template<typename...>struct TypeArray;
	template<size_t...>struct NumArray;
	template<char...>struct TMPString;
	template<typename Type, Type...>struct ScalarArray;
	//result: <bool> 类型表层上是否有const标记
	//Result: <type> 表层上去除const标记的类型
	template<typename _1>struct ConstType{
		static const bool result = false;
		typedef _1 Result;
	};
	template<typename _1>struct ConstType<_1 const>{
		static const bool result = true;
		typedef _1 Result;
	};
	//result: <bool> 类型表层上是否有volatile标记
	//Result: <type> 表层上去除volatile标记的类型
	template<typename _1>struct VolatileType{
		static const bool result = false;
		typedef _1 Result;
	};
	template<typename _1>struct VolatileType<_1 volatile>{
		static const bool result = true;
		typedef _1 Result;
	};
	//Result: <type> 去除类型表层上的const和volatile
	template<typename _1>struct RemoveCV{
		typedef typename VolatileType
		<typename ConstType<_1>::Result>::Result Result;
	};
	//result: <bool> 类型是否相等
	//Select: <template<_,_>> 相等则选前者，否则选后者
	template<typename _1, typename _2>struct SameType{
		static const bool result = false;
		template<typename _3, typename _4>struct Select{
			typedef _4 Result;
		};
	};
	template<typename _1>struct SameType<_1, _1>{
		static const bool result = true;
		template<typename _3, typename _4>struct Select{
			typedef _3 Result;
		};
	};
	//result: <bool> 其余类型中是否有一个与_1相等
	template<typename _1, typename _2, typename... _n>struct AnySameType{
		static const bool result = SameType<_1, _2>::result ||
		AnySameType<_1, _n...>::result;
	};
	template<typename _1, typename _2>struct AnySameType<_1, _2>{
		static const bool result = SameType<_1, _2>::result;
	};
	//result: <bool> 其余类型中是否都与_1相等
	template<typename _1, typename _2, typename... _n>struct AllSameType{
		static const bool result = SameType<_1, _2>::result &&
		AllSameType<_1, _n...>::result;
	};
	template<typename _1, typename _2>struct AllSameType<_1, _2>{
		static const bool result = SameType<_1, _2>::result;
	};
	//result: <bool> 是否为有符号整数
	template<typename _1>struct SignedIntegralType{
		typedef typename RemoveCV<_1>::Result once;
		static const bool result = AnySameType
			<once, signed char, short, int, long, long long>::result;
	};
	//result: <bool> 是否为无符号整数
	template<typename _1>struct UnsignedIntegralType{
		typedef typename RemoveCV<_1>::Result once;
		static const bool result = AnySameType
			<once, unsigned char, unsigned short, unsigned,
			unsigned long, unsigned long long>::result;
	};
	//result: <bool> 是否为bool，char，wchar_t
	template<typename _1>struct OtherIntegralType{
		typedef typename RemoveCV<_1>::Result once;
		static const bool result = AnySameType<once, bool, char, wchar_t>::result;
	};
	//result: <bool> 是否为整数
	template<typename _1>struct IntegralType{
		static const bool result = SignedIntegralType<_1>::result ||
		UnsignedIntegralType<_1>::result || OtherIntegralType<_1>::result;
	};
	//result: <bool> 是否为浮点数
	template<typename _1>struct FloatType{
		typedef typename RemoveCV<_1>::Result once;
		static const bool result = AnySameType
			<once, float, double, long double>::result;
	};
	//result: <bool> 是否为void
	template<typename _1>struct VoidType{
		typedef typename RemoveCV<_1>::Result once;
		static const bool result = SameType<once, void>::result;
	};
	//result: <size_t> 数组大小，0表示非数组
	//Result: <type> 数组元素类型
	template<typename _1>struct ArrayType{
		static const size_t result = 0;
		typedef _1 Result;
	};
	template<typename _1, size_t size>struct ArrayType<_1[size]>{
		static const size_t result = size;
		typedef _1 Result;
	};
	//result: <bool> 是否为指针
	//Result: <type> 被指向类型
	template<typename _1>struct PointerType{
		template<typename _2>struct Helper{
			static const bool result = false;
			typedef _2 Result;
		};
		template<typename _2>struct Helper<_2*>{
			static const bool result = true;
			typedef _2 Result;
		};
		typedef typename RemoveCV<_1>::Result once;
		static const bool result = Helper<once>::result;
		typedef typename Helper<once>::Result Result;
	};
	//result: <size_t> 主线指针层数
	template<typename _1>struct PointerDepth{
		template<size_t depth, typename _2>struct Helper{
			static const size_t result = depth;
			typedef _2 Result;
		};
		template<size_t depth, typename _2>struct Helper<depth, _2*>{
			typedef typename RemoveCV<_2>::Result once;
			static const size_t result = Helper<depth + 1, once>::result;
			typedef typename Helper<depth + 1, once>::Result Result;
		};
		typedef typename RemoveCV<_1>::Result once;
		static const size_t result = Helper<0, once>::result;
		typedef typename Helper<0, once>::Result Result;
	};
	//result: <size_t> 1为左值引用，2为右值引用，0为非引用类型
	//Result: <type> 被引用类型
	template<typename _1>struct ReferenceType{
		static const size_t result = 0;
		typedef _1 Result;
	};
	template<typename _1>struct ReferenceType<_1&>{
		static const size_t result = 1;
		typedef _1 Result;
	};
	template<typename _1>struct ReferenceType<_1&&>{
		static const size_t result = 2;
		typedef _1 Result;
	};
	//result: <bool> 是否为成员类型
	//Result: <type> 成员形态
	template<typename _1>struct MemberType{
		static const bool result = false;
		typedef void Result;
		typedef void Class;
	};
	template<typename _1, typename C>struct MemberType<_1 C::*>{
		static const bool result = true;
		typedef _1 Result;
		typedef C Class;
	};
	template<typename Ret, typename C, typename... Args>
	struct MemberType<Ret(C::*)(Args...)>{
		static const bool result = true;
		typedef Ret Result(Args...);
		typedef C Class;
	};
	template<typename Ret, typename C, typename... Args>
	struct MemberType<Ret(C::*)(Args...)const>{
		static const bool result = true;
		typedef Ret Result(Args...);
		typedef C Class;
	};
	template<typename Ret, typename C, typename... Args>
	struct MemberType<Ret(C::*)(Args..., ...)>{
		static const bool result = true;
		typedef Ret Result(Args..., ...);
		typedef C Class;
	};
	template<typename Ret, typename C, typename... Args>
	struct MemberType<Ret(C::*)(Args..., ...)const>{
		static const bool result = true;
		typedef Ret Result(Args..., ...);
		typedef C Class;
	};
	//result: <bool> 是否为数值
	template<typename _1>struct NumberType{
		static const bool result = IntegralType<_1>::result ||
		FloatType<_1>::result;
	};
	//result: <bool> 是否为内建类型
	template<typename _1>struct IntrinsicType{
		static const bool result = NumberType<_1>::result || VoidType<_1>::result;
	};
	//result: <bool> 非内建类型
	template<typename _1>struct NonIntrinsicType{
		static const bool result = !IntrinsicType<_1>::result;
	};
	//result: <size_t> 主线数组深度
	//Result: <type> 数组根元素类型
	//Select: <template<N>> 第N层数组大小【由里到外】
	//SizeArray: <NumArray> 数组大小的整数序列
	template<typename _1>struct ArrayDepth{
		template<typename _A, size_t w>struct Array{
			static const size_t result = w;
			typedef _A Result;
		};

		template<size_t depth, typename _arr, typename _A>struct Helper{
			typedef _arr RawType;
			typedef _A SizeArray;
			static const size_t result = depth;
		};
		template<size_t depth, typename _arr, size_t _size, typename _A>
		struct Helper<depth, _arr[_size], _A>
			:Helper<depth + 1, _arr, Array<_A, _size> >{};

		typedef Helper<0, _1, int > Next;
		typedef typename Next::RawType Result;
		typedef typename Next::SizeArray _SizeArray;
		static const size_t result = Next::result;

		template<size_t index>struct Select{
			template<size_t index, typename _A>
			struct Helper :Helper<index - 1, typename _A::Result >{};
			template<typename _A>struct Helper<0, _A>{
				static const size_t result = _A::result;
			};

			static const size_t result = Helper<index, SizeArray>::result;
		};

		template<typename NA1, typename NA2>struct MakeArray;
		template<size_t... nums, typename NA2>
		struct MakeArray<NumArray<nums...>, NA2>
			:MakeArray<NumArray<nums..., NA2::result>, typename NA2::Result>{};
		template<size_t... nums>struct MakeArray<NumArray<nums...>, int>{
			typedef NumArray<nums...> Result;
		};

		typedef typename MakeArray<NumArray<>, _SizeArray>::Result SizeArray;

	};
	//result: <size_t> 字节对齐所需要的个数
	template<typename _1>struct AlignmentType{
		static const size_t result = std::alignment_of<_1>::value;
	};
	//result: <bool> 是否为Enum类型
	template<typename _1>struct EnumType{
		static const bool result = std::is_enum<_1>::value;
	};
	//result: <bool> 是否为Union类型
	template<typename _1>struct UnionType{
		static const bool result = std::is_union<_1>::value;
	};
	//result: <bool> 是否为class类型，而不是union类型
	template<typename _1>struct ClassType{
		template<typename _2>static char TypeTester(void (_2::*)()){};
		template<typename _2>static long TypeTester(...){};
		static const bool result = sizeof(TypeTester<_1>(nullptr)) == sizeof(char)
			&& !UnionType<_1>::result;
	};
	//result: <bool> 是否是对象类型（数值或类）
	template<typename _1>struct ObjectType{
		static const bool result = !PointerType<_1>::result &&
		!ReferenceType<_1>::result && !VoidType<_1>::result;
	};
	//result: <bool> 是否为标量类型
	template<typename _1>struct ScalarType{
		static const bool result = NumberType<_1>::result || VoidType<_1>::result
		|| EnumType<_1>::result || MemberType<_1>::result;
	};
	//Result: <type> 是则为前者，否则为后者
	template<bool check, typename _1, typename _2>struct SelectType{
		typedef _2 Result;
	};
	template<typename _1, typename _2>struct SelectType<true, _1, _2>{
		typedef _1 Result;
	};
	//result: <bool> 是否为函数或仿函数类型
	//Result: <type> 函数形态
	template<typename _1>struct FunctorType{
		template<typename _2>struct Helper{
			static const bool result = false;
			typedef void Result;
		};
		template<typename Ret, typename... Args>struct Helper<Ret(Args...)>{
			static const bool result = true;
			typedef Ret Result(Args...);
		};
		template<typename Ret, typename... Args>struct Helper<Ret(Args..., ...)>{
			static const bool result = true;
			typedef Ret Result(Args..., ...);
		};
		template<typename Ret, typename... Args>struct Helper<Ret(*)(Args...)>{
			static const bool result = true;
			typedef Ret Result(Args...);
		};
		template<typename Ret, typename... Args>struct Helper<Ret(&)(Args...)>{
			static const bool result = true;
			typedef Ret Result(Args...);
		};
		template<typename Ret, typename... Args>struct Helper<Ret(*)(Args..., ...)>{
			static const bool result = true;
			typedef Ret Result(Args..., ...);
		};
		template<typename Ret, typename... Args>struct Helper<Ret(&)(Args..., ...)>{
			static const bool result = true;
			typedef Ret Result(Args..., ...);
		};
		template<typename Class, typename Ret, typename... Args>
		struct Helper<Ret(Class::*)(Args...)>{
			static const bool result = true;
			typedef Ret Result(Args...);
		};
		template<typename Class, typename Ret, typename... Args>
		struct Helper<Ret(Class::*)(Args...)const>{
			static const bool result = true;
			typedef Ret Result(Args...);
		};
		template<typename Class, typename Ret, typename... Args>
		struct Helper<Ret(Class::*)(Args..., ...)>{
			static const bool result = true;
			typedef Ret Result(Args..., ...);
		};
		template<typename Class, typename Ret, typename... Args>
		struct Helper<Ret(Class::*)(Args..., ...)const>{
			static const bool result = true;
			typedef Ret Result(Args..., ...);
		};
		typedef typename RemoveCV<_1>::Result once;
		template<bool check, typename _2>struct FunctorHelper{
			static const bool result = false;
			typedef void Result;
		};
		template<typename _2>struct FunctorHelper<true, _2>{
			typedef Helper<decltype(&_2::operator())> Next;
			static const bool result = Next::result;
			typedef typename Next::Result Result;
		};
		template<typename _2>static
			char operatorhelper(decltype(&_2::operator()) w){};
		template<typename _2>static long operatorhelper(...){};
		typedef Helper<once> Xonce;
		typedef FunctorHelper
			<sizeof(operatorhelper<once>(nullptr)) == sizeof(char), _1> Xtwice;
		static const bool result = Xonce::result || Xtwice::result;
		typedef typename SelectType<Xonce::result, typename Xonce::Result,
			typename Xtwice::Result>::Result Result;
	};
	//result: <bool> 是否为空类
	template<typename _1>struct EmptyClassType{
		template<typename _2, bool check>struct Helper{
			static const bool result = false;
		};
		template<typename _2>struct Helper<_2, true>{
			class Derive : _2{
				int a;
			};
			static const bool result = sizeof(Derive) == sizeof(int);
		};
		static const bool result = Helper<_1, ClassType<_1>::result>::result;
	};
	//result: <bool> 是否含虚函数
	template<typename _1>struct VirtualClassType{
		template<typename _2, bool check>struct Helper{
			static const bool result = false;
		};
		template<typename _2>struct Helper<_2, true>{
			class Base : _2{
				int a;
			};
			class Derive : Base{
				virtual ~Derive(){};
			};
			static const bool result = sizeof(Derive) == sizeof(Base);
		};
		static const bool result = Helper<_1, ClassType<_1>::result>::result;
	};
	//result: <bool> 是否是抽象类
	template<typename _1>struct AbstractType{
		template<typename _2>static char helper(_2(*)[1]){};
		template<typename _2>static long helper(...){};
		static const bool result = VirtualClassType<_1>::result &&
			sizeof(helper<_1>(nullptr)) == sizeof(long);
	};
	//result: <bool> 是否是POD类型
	template<typename _1>struct PODType{
		static const bool result = std::is_pod<_1>::value;
	};
	//result: <bool> 两个类型能否进行隐式转换
	template<typename _1, typename _2>struct ImplicitConvertible{
		static char helper(_1){};
		static long helper(...){};
		static const bool result = sizeof(helper(_2())) == sizeof(char);
	};
	//result: <bool> 两个类型能否进行显式转换
	template<typename _1, typename _2>struct ExplicitConvertible{
		template<typename _3>
		static auto helper(_3* w)->decltype(_2(*w), true){};
		template<typename _3>static long helper(...){};
		static const bool result = sizeof(helper<_1>(nullptr)) == sizeof(bool);
	};
	//result: <bool> 是否是继承关系
	template<typename _1, typename _2>struct InheritType{
		template<bool X, typename _3>struct Helper{
			static const bool result = false;
		};
		template<typename _3>struct Helper<true, _3>{
			static char helper(_3*){};
			static long helper(...){};
			static const bool result = sizeof(helper((_2*)0)) == sizeof(char);
		};

		static const bool result =
			Helper<ClassType<_1>::result && ClassType<_2>::result, _1>::result;
	};
	//call  : <template<_,_>> 反向调用元函数
	template<template<typename, typename>class Func>struct ReverseFunc{
		template<typename _1, typename _2>struct call :Func<_2, _1>{};
	};
	//call  : <template<_>> 将类型在二元元函数上调用
	template<template<typename, typename>class Func>struct SelfTwiceFunc{
		template<typename _1>struct call :Func<_1, _1>{};
	};
	//call  : <template<_>> 将函数递归的调用两次
	template<template<typename>class Func>struct RecursionFunc{
		template<typename _1>struct call :Func<typename Func<_1>::Result>{};
	};
	//Result: <type> 去除主线上的引用，指针，数组，CV标记后的类型
	template<typename _1>struct OriginalType{
		typedef typename ReferenceType<_1>::Result _2;
		//typedef typename RemoveCV<_2>::Result _3;
		template<bool a, bool b, typename _3, typename _4>struct Helper;
		template<bool b, typename _3, typename _4>struct Helper<false, b, _3, _4>{
			typedef typename Helper<
			(bool)PointerDepth<_4>::result, (bool)ArrayDepth<_4>::result,
			typename PointerDepth<_4>::Result,
			typename ArrayDepth<_4>::Result>::Result Result;
		};
		template<bool a, typename _3, typename _4>struct Helper<a, false, _3, _4>{
			typedef typename Helper<
			(bool)PointerDepth<_3>::result, (bool)ArrayDepth<_3>::result,
			typename PointerDepth<_3>::Result,
			typename ArrayDepth<_3>::Result>::Result Result;
		};
		template<typename _3, typename _4>struct Helper<false, false, _3, _4>{
			typedef _3 Result;
		};
		typedef typename Helper<(bool)PointerDepth<_2>::result,
			(bool)ArrayDepth<_2>::result,
			typename PointerDepth<_2>::Result, typename ArrayDepth<_2>::Result>
			::Result _5;
		typedef typename RemoveCV<_5>::Result Result;
	};
	//Result: <type> 添加const标记
	template<typename _1>struct AddConst{
		typedef _1 const Result;
	};
	//Result: <type> 添加volatile标记
	template<typename _1>struct AddVolatile{
		typedef _1 volatile Result;
	};
	//Result: <type> 添加CV标记
	template<typename _1>struct AddCV{
		typedef _1 const volatile Result;
	};
	//Result: <type> 添加左值引用标记
	template<typename _1>struct AddReference{
		typedef typename ReferenceType<_1>::Result _2;
		typedef _2& Result;
	};
	//Result: <type> 添加常量左值引用标记
	template<typename _1>struct AddConstReference{
		typedef typename ReferenceType<_1>::Result _2;
		typedef _2 const& Result;
	};
	//Result: <type> 添加右值引用标记
	template<typename _1>struct AddRValueReference{
		typedef typename ReferenceType<_1>::Result _2;
		typedef _2&& Result;
	};
	//Result: <type> 添加指针标记
	template<typename _1>struct AddPointer{
		typedef typename ReferenceType<_1>::Result _2;
		typedef _2* Result;
	};
	//Result: <type> 添加size重指针标记
	template<typename _1, size_t size>struct AddMultiPointer{
		typedef typename ReferenceType<_1>::Result _2;
		typedef typename AddMultiPointer<_2*, size - 1>::Result Result;
	};
	template<typename _1>struct AddMultiPointer<_1, 0>{
		typedef _1 Result;
	};
	//Result: <type> 添加常量指针标记
	template<typename _1>struct AddConstPointer{
		typedef typename ReferenceType<_1>::Result _2;
		typedef _2* const Result;
	};
	//Result: <type> 添加数组标记
	template<typename _1, size_t size>struct AddArray{
		typedef _1 Result[size];
	};
	//result: <size_t> 对应数字
	//Next  : <type> 下一个数字类型
	//Prev  : <type> 上一个数字类型
	template<size_t index>struct TypeForNumber{
		static const size_t result = index;
		typedef TypeForNumber<index + 1> Next;
		typedef TypeForNumber<index - 1> Prev;
	};
	//Result: <type> 对应的类型
	template<size_t index, typename Head, typename... Types>
	struct SelectTypeInGroup : SelectTypeInGroup<index - 1, Types...>{};
	template<typename Head, typename... Types>
	struct SelectTypeInGroup<0, Head, Types...>{
		typedef Head Result;
	};
	//Result: <TypeArray || void> 类型模板参数
	template<typename _1>struct TemplateClassType{
		template<template <typename...> class A, typename... B>
		static TypeArray<B...>* helper(A<B...>*){};
#define TEMPCODE_A(T) \
	template<template <T, typename...> class A, typename... B, \
	T num>static TypeArray<B...>* helper(A<num, B...>*){};
#define TEMPCODE_B(T, Y) \
	template<template <T, Y, typename...> class A, typename... B, T num, \
	Y num2> static TypeArray<B...>* helper(A<num, num2, B...>*){};
#define TEMPCODE_C(T, Y, W) \
	template<template <T, Y, W, typename...> class A, typename... B, \
	T num, Y num2, W num3> \
	static TypeArray<B...>* helper(A<num, num2, num3, B...>*){};
#define TEMPCODE_D(T, Y, W, S) \
	template<template <T, Y, W, S, typename...> class A, typename... B, \
	T num, Y num2, W num3, S num4> \
	static TypeArray<B...>* helper(A<num, num2, num3, num4, B...>*){};
#define TEMPCODE_E(T, Y, W, S, X) \
	template<template <T, Y, W, S, X, typename...> class A, typename... B, \
	T num, Y num2, W num3, S num4, X num5>static \
	TypeArray<B...>* helper(A<num, num2, num3, num4, num5, B...>*){};
		TEMPCODE_A(bool)	TEMPCODE_A(int)		TEMPCODE_A(size_t)
			TEMPCODE_B(bool, bool)	TEMPCODE_B(int, int)	TEMPCODE_B(size_t, size_t)
			TEMPCODE_B(bool, int)	TEMPCODE_B(int, size_t)	TEMPCODE_B(size_t, bool)
			TEMPCODE_B(bool, size_t)	TEMPCODE_B(int, bool)	TEMPCODE_B(size_t, int)
			TEMPCODE_C(bool, bool, bool)		TEMPCODE_C(int, int, int)
			TEMPCODE_C(bool, int, int)			TEMPCODE_C(int, size_t, size_t)
			TEMPCODE_C(bool, size_t, size_t)	TEMPCODE_C(int, int, size_t)
			TEMPCODE_C(bool, size_t, int)		TEMPCODE_C(int, size_t, int)
			TEMPCODE_C(bool, int, size_t)
			TEMPCODE_C(size_t, size_t, int)		TEMPCODE_C(size_t, int, size_t)
			TEMPCODE_C(size_t, size_t, size_t)	TEMPCODE_C(size_t, int, int)
			TEMPCODE_D(bool, bool, bool, bool)	TEMPCODE_D(int, int, int, int)
			TEMPCODE_D(size_t, size_t, size_t, size_t)
			TEMPCODE_D(bool, size_t, size_t, size_t)
			TEMPCODE_D(bool, int, int, int)
			TEMPCODE_E(bool, bool, bool, bool, bool)
			TEMPCODE_E(int, int, int, int, int)
			TEMPCODE_E(size_t, size_t, size_t, size_t, size_t)
			TEMPCODE_E(bool, size_t, size_t, size_t, size_t)
			TEMPCODE_E(bool, int, int, int, int)
			template<template<template<typename>class, typename...> class A,
			template<typename>class C, typename... B>
		static TypeArray<B...>* helper(A<C, B...>*){};
		template<template<template<typename, typename>class, typename...> class A,
			template<typename, typename>class C, typename... B>
		static TypeArray<B...>* helper(A<C, B...>*){};
		template<template<template<typename...>class, typename...> class A,
			template<typename...>class C, typename... B>
		static TypeArray<B...>* helper(A<C, B...>*){};
#undef TEMPCODE_A
#undef TEMPCODE_B
#undef TEMPCODE_C
#undef TEMPCODE_D
#undef TEMPCODE_E
		//static void* helper(...){};
		typedef decltype(helper((_1*)nullptr)) once;
		typedef typename PointerType<once>::Result Result;
		static const bool result = VoidType<once>::result;
	};
	//result: <bool> 可复制的类型
	template<typename _1>struct Replicable{
		static const bool result = ExplicitConvertible<_1, _1>::result;
	};
	//result: <bool> 有默认构造函数的类型
	template<typename _1>struct DefaultInitializable{
		template<typename _2>static auto helper(void*)->decltype(_2(), true){};
		template<typename _2>static long helper(...){};
		static const bool result = sizeof(helper<_1>(nullptr)) == sizeof(bool);
	};
	//result: <bool> 有移动构造函数的类型
	template<typename _1>struct MoveReplicable{
		static const bool result =
		ExplicitConvertible<AddRValueReference<_1>::Result, _1>::result;
	};
	//result: <bool> 可赋值的类型
	template<typename _1>struct CopyAssignable{
		template<typename _2>
		static auto helper(void*)->decltype((*(_2*)0) = (*(_2*)0), true){};
		template<typename _2>static long helper(...){};
		static const bool result = sizeof(helper<_1>(nullptr)) == sizeof(bool);
	};
	//result: <bool> 可赋值的两个类型
	template<typename _1, typename _2>struct Assignable{
		template<typename _3>
		static auto helper(void*)->decltype((*(_3*)0) = (*(_2*)0), true){};
		template<typename _3>static long helper(...){};
		static const bool result = sizeof(helper<_1>(nullptr)) == sizeof(bool);
	};

	template<size_t num>struct NumToStringHelper{
		template<size_t num_2, typename CA>struct Helper;
		template<size_t num_2, char... chs>struct Helper<num_2, TMPString<chs...>>
			:Helper<num_2 / 10, TMPString<num_2 % 10 + '0', chs...>>{};
			template<char... chs>struct Helper<0, TMPString<chs...>>
				:boost::mpl::c_str<boost::mpl::string<chs...>>{
					typedef boost::mpl::string<chs...> Result;
				};
				typedef Helper<num, TMPString<>> Result;
	};
	//value : <const char[]> 编译期数值转字符串
	template<size_t num>struct NumToString :NumToStringHelper<num>::Result{};

	template<size_t num>struct ArraySizeHelperHelper{
		typedef typename boost::mpl::push_front<typename NumToString<num>::Result,
		boost::mpl::integral_c<char, '[' >> ::type once;
		typedef typename boost::mpl::push_back<once, boost::mpl::integral_c
			<char, ']' >> ::type twice;
		typedef boost::mpl::c_str<twice> Result;
	};
	template<size_t num>struct ArraySizeHelper :ArraySizeHelperHelper<num>::Result{};
};


namespace Types{
	//size  : <size_t> 类型数组的大小
	//Result: <type> 首项【空数组无定义】
	//Next  : <TypeArray> 除首项以外的项【空数组无定义】
	//get   : <template<_>> 选出第N个项
	//add   : <template<_>> 在数组末端加上一项
	template<typename... Args>struct TypeArray;
	template<typename Head, typename... Others>struct TypeArray<Head, Others...>{
		typedef TypeArray<Others...> Next;
		typedef Head Result;
		template<int index>struct get{
			typedef typename SelectTypeInGroup<index, Head, Others...>::Result Result;
		};
		template<typename _1>struct add{
			typedef TypeArray<Head, Others..., _1> Result;
		};
		static const size_t size = sizeof...(Others)+1;
	};
	template<>struct TypeArray<>{
		template<int index>struct get;
		template<typename _1>struct add{
			typedef TypeArray<_1> Result;
		};
		static const size_t size = 0;
	};
	//Result: <TypeArray> 连接两个类型数组
	template<typename Array1, typename Array2>struct ArrayConcat;
	template<typename... Array1, typename... Array2>
	struct ArrayConcat<TypeArray<Array1...>, TypeArray<Array2...> >{
		typedef TypeArray<Array1..., Array2...> Result;
	};
	//Array1: <TypeArray> 将第二个类型数组的首项加在第一个类型数组末端
	//Array2: <TypeArray> 第二个类型数组除去首项
	template<typename Array1, typename Array2>struct ArrayTransfer;
	template<typename Array, typename... Types>
	struct ArrayTransfer<TypeArray<Types...>, Array>{
		typedef TypeArray<Types..., typename Array::Result> Array1;
		typedef typename Array::Next Array2;
	};
	//Result: <TypeArray> 在类型数组前端弹出times个项
	template<size_t times, typename Array>struct ArrayShift{
		typedef typename ArrayShift<times - 1, typename Array::Next>::Result Result;
	};
	template<typename Array>struct ArrayShift<0, Array>{
		typedef typename Array Result;
	};
	//Result: <TypeArray> 从类型数组末端弹出times个项
	template<size_t times, typename Array>struct ArrayPop{
		template<size_t index, typename Array_1, typename Array_2>struct ArrayTransport{
			typedef typename ArrayTransfer<Array_1, Array_2>::Array1 Array1;
			typedef typename ArrayTransport<index - 1, Array1,
				typename Array_2::Next>::Result Result;
		};
		template<typename Array_1, typename Array_2>
		struct ArrayTransport<0, Array_1, Array_2>{
			typedef Array_1 Result;
		};
		typedef typename ArrayTransport<Array::size - times,
			TypeArray<>, Array>::Result Result;
	};
	template<typename Array>struct ArrayPop<0, Array>{
		typedef Array Result;
	};
	//Result: <TypeArray> 选出类型数组里面[index1,index2]的项
	template<size_t index1, size_t index2, typename Array>struct ArraySelect{
		typedef typename ArrayShift<index1,
		typename ArrayPop<Array::size - index2 - 1, Array>::Result>::Result Result;
	};
	//Result: <TypeArray> 删除类型数组里面[index1,index2]的项
	template<size_t index1, size_t index2, typename Array>struct ArrayDelete{
		typedef typename ArrayConcat<typename ArrayPop
		<Array::size - index1, Array>::Result, typename ArrayShift
		<index2 + 1, Array>::Result>::Result Result;
	};
	//result: <bool> 类型数组存在一个项经过函数得到true
	template<typename Array, template<typename> class Func>struct ArrayAny{
		static const bool result = Func<typename Array::Result>::result ||
		ArrayAny<typename Array::Next, Func>::result;
	};
	template<template<typename> class Func>
	struct ArrayAny<TypeArray<>, Func>{
		static const bool result = false;
	};
	//result: <bool> 类型数组每一个项经过函数得到true
	template<typename Array, template<typename> class Func>
	struct ArrayAll{
		static const bool result = Func<typename Array::Result>::result &&
		ArrayAny<typename Array::Next, Func>::result;
	};
	template<template<typename> class Func>
	struct ArrayAll<TypeArray<>, Func>{
		static const bool result = true;
	};
	//Result: <TypeArray> 将每个类型进行迭代转化为新类型
	template<typename Array, template<typename> class Func>struct ArrayMap;
	template<typename... Types, template<typename> class Func>
	struct ArrayMap<TypeArray<Types...>, Func>{
		typedef TypeArray<typename Func<Types>::Result...> Result;
	};
	//Result: <TypeArray> 将每个类型进行迭代转化为新类型
	template<typename Array, template<typename> class Func>
	using ArrayCollect = ArrayMap<Array, Func>;
	//Result: <TypeArray> 选出符合要求的类型
	template<typename Array, template<typename> class Func>
	struct ArraySelectIf{
		template<typename Array1, typename Array2>struct Transport{
			typedef typename Transport<typename SelectType<
			Func<typename Array2::Result>::result,
			typename ArrayTransfer<Array1, Array2>::Array1, Array1>::Result,
			typename Array2::Next>::Result Result;
		};
		template<typename Array1>struct Transport<Array1, TypeArray<> >{
			typedef Array1 Result;
		};
		typedef typename Transport<TypeArray<>, Array>::Result Result;
	};
	//Result: <TypeArray> 清除符合要求的类型
	template<typename Array, template<typename> class Func>
	struct ArrayDeleteIf{
		template<typename Array1, typename Array2>struct Transport{
			typedef typename Transport<typename SelectType
			<!Func<typename Array2::Result>::result,
			typename ArrayTransfer<Array1, Array2>::Array1, Array1>::Result,
			typename Array2::Next>::Result Result;
		};
		template<typename Array1>struct Transport<Array1, TypeArray<> >{
			typedef Array1 Result;
		};
		typedef typename Transport<TypeArray<>, Array>::Result Result;
	};
	//Result: <TypeArray> 在指定索引前插入类型
	template<typename Array, size_t index, typename Type>
	struct ArrayInsert{
		typedef typename ArrayConcat<typename ArrayConcat<typename ArrayPop
		<Array::size - index, Array>::Result, TypeArray<Type> >::Result,
		typename ArrayShift<index, Array>::Result>::Result Result;
	};
	//Result: <TypeArray> 将类型数组中第index个替换为Type
	template<typename Array, size_t index, typename Type>
	struct ArrayReplace{
		typedef typename ArrayInsert<typename ArrayDelete
		<index, index, Array>::Result, index, Type>::Result Result;
	};
	//result: <bool> 判断类型数组中是否含有该类型
	template<typename Array, typename Type>struct ArrayInclude;
	template<typename... Types, typename Type>
	struct ArrayInclude<TypeArray<Types...>, Type>{
		static const bool result = AnySameType<Type, Types...>::result;
	};
	template<typename Type>struct ArrayInclude<TypeArray<>, Type>{
		static const bool result = false;
	};
	//Result: <TypeArray> 对类型进行排序
	template<typename Array, template<typename, typename> class Func>struct ArraySort{
		typedef typename Array::Result First;
		template<typename Type>struct Compare_1{
			static const bool result = Func<Type, First>::result;
		};
		template<typename Type>struct Compare_2{
			static const bool result = !Func<Type, First>::result;
		};
		typedef typename ArraySelectIf<typename Array::Next, Compare_1>::Result Left;
		typedef typename ArraySelectIf<typename Array::Next, Compare_2>::Result Right;
		typedef typename ArrayConcat<typename ArraySort<Left, Func>::Result,
			typename ArrayConcat<TypeArray<First>,
			typename ArraySort<Right, Func>::Result>::Result>::Result Result;
	};
	template<typename Type1, typename Type2, template<typename, typename> class Func>
	struct ArraySort<TypeArray<Type1, Type2>, Func>{
		typedef typename SelectType<Func<Type1, Type2>::result,
		TypeArray<Type1, Type2>, TypeArray<Type2, Type1> >::Result Result;
	};
	template<typename Type, template<typename, typename> class Func>
	struct ArraySort<TypeArray<Type>, Func>{
		typedef TypeArray<Type> Result;
	};
	template<template<typename, typename> class Func>
	struct ArraySort<TypeArray<>, Func>{
		typedef TypeArray<> Result;
	};
	//Result: <TypeArray> 去除重复的类型
	template<typename Array>struct ArrayUnique{
		template<typename Array1, typename Array2>struct Transport{
			typedef typename Transport<typename SelectType<
			ArrayInclude<Array1, typename Array2::Result>::result, Array1,
			typename ArrayTransfer<Array1, Array2>::Array1
			>::Result, typename Array2::Next>::Result Result;
		};
		template<typename Array1>struct Transport<Array1, TypeArray<> >{
			typedef Array1 Result;
		};
		typedef typename Transport<TypeArray<>, Array>::Result Result;
	};
	//Result: <TypeArray> 按定制的函数去除重复的类型
	template<typename Array, template<typename, typename> class Func = SameType>
	struct ArrayUniqueIf{

		template<typename Array, typename Type>struct Helper{
			template<typename Type_1>struct SameUnary{
				static const bool result = Func<Type, Type_1>::result;
			};
			static const bool result = ArrayAny<Array, SameUnary>::result;
		};

		template<typename Array1, typename Array2>struct Transport{
			typedef typename Transport<typename SelectType<
			Helper<Array1, typename Array2::Result>::result, Array1,
			typename ArrayTransfer<Array1, Array2>::Array1
			>::Result, typename Array2::Next>::Result Result;
		};
		template<typename Array1>struct Transport<Array1, TypeArray<> >{
			typedef Array1 Result;
		};
		typedef typename Transport<TypeArray<>, Array>::Result Result;
	};
	template<typename Array>struct ArrayUniqueIf<Array, SameType>{
		template<typename Array1, typename Array2>struct Transport{
			typedef typename Transport<typename SelectType<
			ArrayInclude<Array1, typename Array2::Result>::result, Array1,
			typename ArrayTransfer<Array1, Array2>::Array1
			>::Result, typename Array2::Next>::Result Result;
		};
		template<typename Array1>struct Transport<Array1, TypeArray<> >{
			typedef Array1 Result;
		};
		typedef typename Transport<TypeArray<>, Array>::Result Result;
	};
};


namespace Types{

#define TEMPCODE_A \
	typedef T Result;	typedef T Member;	typedef TypeTree<Member> SubTree;
#define TEMPCODE_B \
	template<template<typename, typename>class Equal, typename From, typename To> \
	struct DynamicReplace{
	\
	typedef typename SelectType<Equal<Member, From>::result, To, typename SubTree \
	::template DynamicReplace<Equal, From, To>::Result>::Result once; \
	typedef typename StaticReplace<once>::Result Result; \
	};
#define TEMPCODE_C \
	template<template<typename, typename>class Equal, typename From, typename To> \
	struct DynamicReplace{
		\
		typedef typename SelectType<Equal<Ret, From>::result, To, \
		typename TypeTree<Ret>::template DynamicReplace<Equal, From, To>::Result \
		>::Result once; \
		template<typename X>struct WW{
			\
			typedef typename SelectType<Equal<X, From>::result, To, \
			typename TypeTree<X>::template DynamicReplace<Equal, From, To> \
			::Result>::Result Result; \
		}; \
		typedef typename StaticReplace<once, typename WW<Args> \
		::Result...>::Result Result; \
	};
#define TEMPCODE_D \
	template<template<typename, typename>class Equal, typename From, typename To> \
	struct DynamicReplace{
	\
	typedef typename SelectType<Equal<Ret, From>::result, To, \
	typename TypeTree<Ret>::template DynamicReplace<Equal, From, To> \
	::Result>::Result once; \
	typedef typename SelectType<Equal<Class, From>::result, To, \
	typename TypeTree<Class>::template DynamicReplace<Equal, From, To> \
	::Result>::Result twice; \
	template<typename X>struct WW{
		\
		typedef typename SelectType<Equal<X, From>::result, To, \
		typename TypeTree<X>::template DynamicReplace<Equal, From, To> \
		::Result>::Result Result; \
	}; \
	typedef typename StaticReplace<once, twice, typename WW<Args> \
	::Result...>::Result Result; \
	};
	template<typename T>struct TypeTree{
		typedef T Result;
		template<typename To>struct StaticReplace{ typedef To Result; };
		template<template<typename, typename>class Equal, typename From, typename To>
		struct DynamicReplace{
			typedef typename SelectType<Equal<T, From>::result, To, T>::Result Result;
		};
		static const unsigned char flag = 0;
		static Slice<TypeString> front_name(){
			static Slice<TypeString> cache;
			if (cache.empty()){
				TypeString temp = typeid(T).name();
				if (temp.size() > 6 && temp[0] == 'c' && temp[1] == 'l'
					&& temp[2] == 'a' && temp[3] == 's' && temp[4] == 's'
					&& temp[5] == ' ')
					return cache = slice(temp, 6, -1);
				if (temp.size() > 7 && temp[0] == 's' && temp[1] == 't'
					&& temp[2] == 'r' && temp[3] == 'u' && temp[4] == 'c'
					&& temp[5] == 't' && temp[6] == ' ')
					return cache = slice(temp, 7, -1);
				if (temp.size() > 6 && temp[0] == 'u' && temp[1] == 'n'
					&& temp[2] == 'i' && temp[3] == 'o' && temp[4] == 'n'
					&& temp[5] == ' ')
					return cache = slice(temp, 6, -1);
				if (temp.size() > 5 && temp[0] == 'e' && temp[1] == 'n'
					&& temp[2] == 'u' && temp[3] == 'm' && temp[4] == ' ')
					return cache = slice(temp, 5, -1);
				return cache = slice(temp, 0, -1);
			};
			return cache;
		};
		static TypeString back_name(){
			return "";
		};
		static Slice<TypeString> name(){
			return front_name();
		};
	};
	template<typename T>struct TypeTree<T&>{
		TEMPCODE_A
		template<typename To>struct StaticReplace{
			typedef typename AddReference<To>::Result Result;
		};
		TEMPCODE_B
			static const unsigned char flag = 0;
		static auto front_name()
			->decltype(SubTree::front_name() + TypeString()){
			typedef decltype(SubTree::front_name()) L;
			typedef decltype(L() + TypeString()) M;
			static M cache;
			if (cache.empty()){
				auto front = SubTree::front_name();
				cache = front + TypeString(SubTree::flag ? "(&" : " &");
			};
			return cache;
		};
		static auto back_name()
			->decltype(TypeString() + SubTree::back_name()){
			typedef decltype(SubTree::back_name()) L;
			typedef decltype(TypeString() + L()) M;
			static M cache;
			if (cache.empty()){
				auto back = SubTree::back_name();
				cache = TypeString(SubTree::flag ? ")" : "") + back;
			};
			return cache;
		};
		static auto name()
			->decltype(front_name() + back_name()){
			typedef decltype(front_name() + back_name()) M;
			static M cache;
			if (cache.empty())
				cache = front_name() + back_name();
			return cache;
		};
	};
	template<typename T>struct TypeTree<T&&>{
		TEMPCODE_A
		template<typename To>struct StaticReplace{
			typedef typename AddRValueReference<To>::Result Result;
		};
		TEMPCODE_B
			static const unsigned char flag = 0;
		static auto front_name()
			->decltype(SubTree::front_name() + TypeString()){
			typedef decltype(SubTree::front_name()) L;
			typedef decltype(L() + TypeString()) M;
			static M cache;
			if (cache.empty()){
				auto front = SubTree::front_name();
				cache = front + TypeString(SubTree::flag ? "(&&" : " &&");
			};
			return cache;
		};
		static auto back_name()
			->decltype(TypeString() + SubTree::back_name()){
			typedef decltype(SubTree::back_name()) L;
			typedef decltype(TypeString() + L()) M;
			static M cache;
			if (cache.empty()){
				auto back = SubTree::back_name();
				cache = TypeString(SubTree::flag ? ")" : "") + back;
			};
			return cache;
		};
		static auto name()
			->decltype(front_name() + back_name()){
			typedef decltype(front_name() + back_name()) M;
			static M cache;
			if (cache.empty())
				cache = front_name() + back_name();
			return cache;
		};
	};
	template<typename T>struct TypeTree<T*>{
		TEMPCODE_A
		template<typename To>struct StaticReplace{
			typedef typename AddPointer<To>::Result Result;
		};
		TEMPCODE_B
			static const unsigned char flag = 0;
		static auto front_name()
			->decltype(SubTree::front_name() + TypeString()){
			typedef decltype(SubTree::front_name()) L;
			typedef decltype(L() + TypeString()) M;
			static M cache;
			if (cache.empty()){
				auto front = SubTree::front_name();
				cache = front + TypeString(SubTree::flag ? "(*" : " *");
			};
			return cache;
		};
		static auto back_name()
			->decltype(TypeString() + SubTree::back_name()){
			typedef decltype(SubTree::back_name()) L;
			typedef decltype(TypeString() + L()) M;
			static M cache;
			if (cache.empty()){
				auto back = SubTree::back_name();
				cache = TypeString(SubTree::flag ? ")" : "") + back;
			};
			return cache;
		};
		static auto name()
			->decltype(front_name() + back_name()){
			typedef decltype(front_name() + back_name()) M;
			static M cache;
			if (cache.empty())
				cache = front_name() + back_name();
			return cache;
		};
	};
	template<typename T>struct TypeTree<T const>{
		TEMPCODE_A
		template<typename To>struct StaticReplace{
			typedef typename AddConst<To>::Result Result;
		};
		static const unsigned char flag = 0;
		static auto front_name()
			->decltype(SubTree::front_name() + TypeString()){
			typedef decltype(SubTree::front_name()) L;
			typedef decltype(L() + TypeString()) M;
			static M cache;
			if (cache.empty()){
				auto front = SubTree::front_name();
				cache = front + TypeString(" const");
			};
			return cache;
		};
		static auto back_name()->decltype(SubTree::back_name()){
			return SubTree::back_name();
		};
		static auto name()
			->decltype(front_name() + back_name()){
			typedef decltype(front_name() + back_name()) M;
			static M cache;
			if (cache.empty())
				cache = front_name() + back_name();
			return cache;
		};
		TEMPCODE_B
	};
	template<typename T>struct TypeTree<T volatile>{
		TEMPCODE_A
		template<typename To>struct StaticReplace{
			typedef typename AddVolatile<To>::Result Result;
		};
		static const unsigned char flag = 0;
		static auto front_name()
			->decltype(SubTree::front_name() + TypeString()){
			typedef decltype(SubTree::front_name()) L;
			typedef decltype(L() + TypeString()) M;
			static M cache;
			if (cache.empty()){
				auto front = SubTree::front_name();
				cache = front + TypeString(" volatile");
			};
			return cache;
		};
		static auto back_name()->decltype(SubTree::back_name()){
			return SubTree::back_name();
		};
		static auto name()
			->decltype(front_name() + back_name()){
			typedef decltype(front_name() + back_name()) M;
			static M cache;
			if (cache.empty())
				cache = front_name() + back_name();
			return cache;
		};
		TEMPCODE_B
	};
	template<typename T>struct TypeTree<T const volatile>{
		TEMPCODE_A
		template<typename To>struct StaticReplace{
			typedef typename AddCV<To>::Result Result;
		};
		static const unsigned char flag = 0;
		static auto front_name()
			->decltype(SubTree::front_name() + TypeString()){
			typedef decltype(SubTree::front_name()) L;
			typedef decltype(L() + TypeString()) M;
			static M cache;
			if (cache.empty()){
				auto front = SubTree::front_name();
				cache = front + TypeString(" const volatile");
			};
			return cache;
		};
		static auto back_name()->decltype(SubTree::back_name()){
			return SubTree::back_name();
		};
		static auto name()
			->decltype(front_name() + back_name()){
			typedef decltype(front_name() + back_name()) M;
			static M cache;
			if (cache.empty())
				cache = front_name() + back_name();
			return cache;
		};
		TEMPCODE_B
	};
	template<typename T, size_t size>struct TypeTree<T[size]>{
		TEMPCODE_A
		static const size_t result = size;
		template<typename To>struct StaticReplace{
			typedef typename AddArray<To, size>::Result Result;
		};
		static const unsigned char flag = 2;
		static auto front_name()->decltype(SubTree::front_name()){
			return SubTree::front_name();
		};
		static auto back_name()->decltype(TypeString() + SubTree::back_name()){
			typedef decltype(SubTree::back_name()) L;
			typedef decltype(TypeString() + L()) M;
			static M cache;
			if (cache.empty()){
				auto back = SubTree::back_name();
				cache = TypeString(ArraySizeHelper<size>::value) + back;
			};
			return cache;
		};
		static auto name()
			->decltype(front_name() + back_name()){
			typedef decltype(front_name() + back_name()) M;
			static M cache;
			if (cache.empty())
				cache = front_name() + back_name();
			return cache;
		};
		TEMPCODE_B
	};
	template<typename T, class C>struct TypeTree<T C::*>{
		typedef T Result;
		typedef TypeArray<T, C> Member;
		template<typename To1, typename To2>struct StaticReplace{
			typedef To1 To2::* Result;
		};
		template<template<typename, typename>class Equal, typename From, typename To>
		struct DynamicReplace{
			typedef typename SelectType<Equal<T, From>::result, To,
			typename TypeTree<T>::template DynamicReplace<Equal, From, To>::Result
			>::Result once;
			typedef typename SelectType<Equal<C, From>::result, To,
				typename TypeTree<C>::template DynamicReplace<Equal, From, To>::Result
			>::Result twice;
			typedef typename StaticReplace<once, twice>::Result Result;
		};
		static const unsigned char flag = 0;
		static auto front_name()->decltype(TypeTree<T>::front_name() + TypeString()
			+ TypeTree<C>::name() + TypeString()){
			typedef decltype(TypeTree<T>::front_name() + TypeString() +
				TypeTree<C>::name() + TypeString()) M;
			static M cache;
			if (cache.empty()){
				auto a = TypeTree<T>::front_name();
				auto b = TypeTree<C>::name();
				cache = a + TypeString(TypeTree<T>::flag ? " (" : " ") + b + TypeString("::*");
			};
			return cache;
		};
		static auto back_name()->decltype(TypeString() + TypeTree<T>::back_name()){
			typedef decltype(TypeTree<T>::back_name()) L;
			typedef decltype(TypeString() + L()) M;
			static M cache;
			if (cache.empty()){
				auto back = TypeTree<T>::back_name();
				cache = TypeString(TypeTree<T>::flag ? ") " : " ") + back;
			};
			return cache;
		};
		static auto name()
			->decltype(front_name() + back_name()){
			typedef decltype(front_name() + back_name()) M;
			static M cache;
			if (cache.empty())
				cache = front_name() + back_name();
			return cache;
		};
	};
	template<typename Ret, typename... Args>struct TypeTree<Ret(Args...)>{
		typedef Ret Result;
		typedef TypeArray<Ret, Args...> Member;
		template<typename To1, typename... Ton>struct StaticReplace{
			typedef To1 Result(Ton...);
		};
		template<typename, typename...>struct PackHelper;
		template<typename String, typename Head, typename... Other>struct PackHelper
			<String, Head, Other...>{
			static auto StringPack(String text)
			->decltype(PackHelper<decltype(String() + TypeString() +
			TypeTree<Head>::name()), Other...>::StringPack(String() + TypeString() +
			TypeTree<Head>::name())){
				auto a = text + TypeString(",") + TypeTree<Head>::name();
				return PackHelper<decltype(String() + TypeString() +
					TypeTree<Head>::name()), Other...>::StringPack(a);
			};
		};
		template<typename String, typename Head>struct PackHelper<String, Head>{
			static auto StringPack(String text)
			->decltype(String() + TypeString() + TypeTree<Head>::name()){
				return text + TypeString(",") + TypeTree<Head>::name();
			};
		};
		template<typename String>struct PackHelper<String>{
			static TypeString StringPack(String text){
				return text;
			};
		};
		static auto StringPack()
			->decltype(PackHelper<TypeString, Args...>::StringPack(TypeString(""))){
			return PackHelper<TypeString, Args...>::StringPack(TypeString(""));
		};
		static const unsigned char flag = 1;
		static auto front_name()->decltype(TypeTree<Ret>::front_name()){
			return TypeTree<Ret>::front_name();
		};
		static auto back_name()->decltype(TypeString() + slice(StringPack(), 1, -1)
			+ TypeString()){
			typedef decltype(TypeString() + slice(StringPack(), 1, -1)
				+ TypeString()) M;
			static M cache;
			if (cache.empty()){
				auto args = StringPack();
				cache = " (" + slice(args, 1, -1) + TypeString(") ");
			};
			return cache;
		};
		static auto name()
			->decltype(front_name() + back_name()){
			typedef decltype(front_name() + back_name()) M;
			static M cache;
			if (cache.empty())
				cache = front_name() + back_name();
			return cache;
		};
		TEMPCODE_C
	};
	template<typename Ret, typename... Args>struct TypeTree<Ret(Args..., ...)>{
		typedef Ret Result;
		typedef TypeArray<Ret, Args...> Member;
		template<typename To1, typename... Ton>struct StaticReplace{
			typedef To1 Result(Ton..., ...);
		};
		template<typename, typename...>struct PackHelper;
		template<typename String, typename Head, typename... Other>struct PackHelper
			<String, Head, Other...>{
			static auto StringPack(String text)
			->decltype(PackHelper<decltype(String() + TypeString() +
			TypeTree<Head>::name()), Other...>::StringPack(String() + TypeString() +
			TypeTree<Head>::name())){
				auto a = text + TypeString(",") + TypeTree<Head>::name();
				return PackHelper<decltype(String() + TypeString() +
					TypeTree<Head>::name()), Other...>::StringPack(a);
			};
		};
		template<typename String, typename Head>struct PackHelper<String, Head>{
			static auto StringPack(String text)
			->decltype(String() + TypeString() + TypeTree<Head>::name()){
				return text + TypeString(",") + TypeTree<Head>::name();
			};
		};
		template<typename String>struct PackHelper<String>{
			static TypeString StringPack(String text){
				return text;
			};
		};
		static auto StringPack()
			->decltype(PackHelper<TypeString, Args...>::StringPack(TypeString(""))){
			return PackHelper<TypeString, Args...>::StringPack(TypeString(""));
		};
		static const unsigned char flag = 1;
		static auto front_name()->decltype(TypeTree<Ret>::front_name()){
			return TypeTree<Ret>::front_name();
		};
		static auto back_name()->decltype(TypeString() + slice(StringPack(), 1, -1)
			+ TypeString()){
			typedef decltype(TypeString() + slice(StringPack(), 1, -1)
				+ TypeString()) M;
			static M cache;
			if (cache.empty()){
				auto args = StringPack();
				cache = " (" + slice(args, 1, -1) + TypeString(",...) ");
			};
			return cache;
		};
		static auto name()
			->decltype(front_name() + back_name()){
			typedef decltype(front_name() + back_name()) M;
			static M cache;
			if (cache.empty())
				cache = front_name() + back_name();
			return cache;
		};
		TEMPCODE_C;
	};
	template<typename Ret, typename Class, typename... Args>
	struct TypeTree<Ret(Class::*)(Args...)>{
		typedef Ret Result;
		typedef TypeArray<Ret, Class, Args...> Member;
		template<typename To1, typename To2, typename... Ton>struct StaticReplace{
			typedef To1(To2::*Result)(Ton...);
		};
		template<typename, typename...>struct PackHelper;
		template<typename String, typename Head, typename... Other>struct PackHelper
			<String, Head, Other...>{
			static auto StringPack(String text)
			->decltype(PackHelper<decltype(String() + TypeString() +
			TypeTree<Head>::name()), Other...>::StringPack(String() + TypeString() +
			TypeTree<Head>::name())){
				auto a = text + TypeString(",") + TypeTree<Head>::name();
				return PackHelper<decltype(String() + TypeString() +
					TypeTree<Head>::name()), Other...>::StringPack(a);
			};
		};
		template<typename String, typename Head>struct PackHelper<String, Head>{
			static auto StringPack(String text)
			->decltype(String() + TypeString() + TypeTree<Head>::name()){
				return text + TypeString(",") + TypeTree<Head>::name();
			};
		};
		template<typename String>struct PackHelper<String>{
			static TypeString StringPack(String text){
				return text;
			};
		};
		static auto StringPack()
			->decltype(PackHelper<TypeString, Args...>::StringPack(TypeString(""))){
			return PackHelper<TypeString, Args...>::StringPack(TypeString(""));
		};
		static const unsigned char flag = 0;
		static auto front_name()->decltype(TypeTree<Ret>::front_name() + TypeString()
			+ TypeTree<Class>::name() + TypeString()){
			typedef decltype(TypeTree<Ret>::front_name() + TypeString()
				+ TypeTree<Class>::name() + TypeString()) M;
			static M cache;
			if (cache.empty()){
				auto a = TypeTree<Ret>::front_name();
				auto b = TypeTree<Class>::name();
				cache = a + TypeString(" (") + b + "::* ";
			};
			return cache;
		};
		static auto back_name()->decltype(TypeString() + slice(StringPack(), 1, -1)
			+ TypeString()){
			typedef decltype(TypeString() + slice(StringPack(), 1, -1)
				+ TypeString()) M;
			static M cache;
			if (cache.empty()){
				auto args = StringPack();
				cache = ")(" + slice(args, 1, -1) + TypeString(") ");
			};
			return cache;
		};
		static auto name()
			->decltype(front_name() + back_name()){
			typedef decltype(front_name() + back_name()) M;
			static M cache;
			if (cache.empty())
				cache = front_name() + back_name();
			return cache;
		};
		TEMPCODE_D
	};
	template<typename Ret, typename Class, typename... Args>
	struct TypeTree<Ret(Class::*)(Args..., ...)>{
		typedef Ret Result;
		typedef TypeArray<Ret, Class, Args...> Member;
		template<typename To1, typename To2, typename... Ton>struct StaticReplace{
			typedef To1(To2::*Result)(Ton..., ...);
		};
		template<typename, typename...>struct PackHelper;
		template<typename String, typename Head, typename... Other>struct PackHelper
			<String, Head, Other...>{
			static auto StringPack(String text)
			->decltype(PackHelper<decltype(String() + TypeString() +
			TypeTree<Head>::name()), Other...>::StringPack(String() + TypeString() +
			TypeTree<Head>::name())){
				auto a = text + TypeString(",") + TypeTree<Head>::name();
				return PackHelper<decltype(String() + TypeString() +
					TypeTree<Head>::name()), Other...>::StringPack(a);
			};
		};
		template<typename String, typename Head>struct PackHelper<String, Head>{
			static auto StringPack(String text)
			->decltype(String() + TypeString() + TypeTree<Head>::name()){
				return text + TypeString(",") + TypeTree<Head>::name();
			};
		};
		template<typename String>struct PackHelper<String>{
			static TypeString StringPack(String text){
				return text;
			};
		};
		static auto StringPack()
			->decltype(PackHelper<TypeString, Args...>::StringPack(TypeString(""))){
			return PackHelper<TypeString, Args...>::StringPack(TypeString(""));
		};
		static const unsigned char flag = 0;
		static auto front_name()->decltype(TypeTree<Ret>::front_name() + TypeString()
			+ TypeTree<Class>::name() + TypeString()){
			typedef decltype(TypeTree<Ret>::front_name() + TypeString()
				+ TypeTree<Class>::name() + TypeString()) M;
			static M cache;
			if (cache.empty()){
				auto a = TypeTree<Ret>::front_name();
				auto b = TypeTree<Class>::name();
				cache = a + TypeString(" (") + b + "::* ";
			};
			return cache;
		};
		static auto back_name()->decltype(TypeString() + slice(StringPack(), 1, -1)
			+ TypeString()){
			typedef decltype(TypeString() + slice(StringPack(), 1, -1)
				+ TypeString()) M;
			static M cache;
			if (cache.empty()){
				auto args = StringPack();
				cache = ")(" + slice(args, 1, -1) + TypeString(",...) ");
			};
			return cache;
		};
		static auto name()
			->decltype(front_name() + back_name()){
			typedef decltype(front_name() + back_name()) M;
			static M cache;
			if (cache.empty())
				cache = front_name() + back_name();
			return cache;
		};
		TEMPCODE_D
	};
	template<typename Ret, typename Class, typename... Args>
	struct TypeTree<Ret(Class::*)(Args...)const>{
		typedef Ret Result;
		typedef TypeArray<Ret, Class, Args...> Member;
		template<typename To1, typename To2, typename... Ton>struct StaticReplace{
			typedef To1(To2::*Result)(Ton...);
		};
		template<typename, typename...>struct PackHelper;
		template<typename String, typename Head, typename... Other>struct PackHelper
			<String, Head, Other...>{
			static auto StringPack(String text)
			->decltype(PackHelper<decltype(String() + TypeString() +
			TypeTree<Head>::name()), Other...>::StringPack(String() + TypeString() +
			TypeTree<Head>::name())){
				auto a = text + TypeString(",") + TypeTree<Head>::name();
				return PackHelper<decltype(String() + TypeString() +
					TypeTree<Head>::name()), Other...>::StringPack(a);
			};
		};
		template<typename String, typename Head>struct PackHelper<String, Head>{
			static auto StringPack(String text)
			->decltype(String() + TypeString() + TypeTree<Head>::name()){
				return text + TypeString(",") + TypeTree<Head>::name();
			};
		};
		template<typename String>struct PackHelper<String>{
			static TypeString StringPack(String text){
				return text;
			};
		};
		static auto StringPack()
			->decltype(PackHelper<TypeString, Args...>::StringPack(TypeString(""))){
			return PackHelper<TypeString, Args...>::StringPack(TypeString(""));
		};
		static const unsigned char flag = 0;
		static auto front_name()->decltype(TypeTree<Ret>::front_name() + TypeString()
			+ TypeTree<Class>::name() + TypeString()){
			typedef decltype(TypeTree<Ret>::front_name() + TypeString()
				+ TypeTree<Class>::name() + TypeString()) M;
			static M cache;
			if (cache.empty()){
				auto a = TypeTree<Ret>::front_name();
				auto b = TypeTree<Class>::name();
				cache = a + TypeString(" (") + b + "::* ";
			};
			return cache;
		};
		static auto back_name()->decltype(TypeString() + slice(StringPack(), 1, -1)
			+ TypeString()){
			typedef decltype(TypeString() + slice(StringPack(), 1, -1)
				+ TypeString()) M;
			static M cache;
			if (cache.empty()){
				auto args = StringPack();
				cache = ")(" + slice(args, 1, -1) + TypeString(")const ");
			};
			return cache;
		};
		static auto name()
			->decltype(front_name() + back_name()){
			typedef decltype(front_name() + back_name()) M;
			static M cache;
			if (cache.empty())
				cache = front_name() + back_name();
			return cache;
		};
		TEMPCODE_D
	};
	template<typename Ret, typename Class, typename... Args>
	struct TypeTree<Ret(Class::*)(Args..., ...)const>{
		typedef Ret Result;
		typedef TypeArray<Ret, Class, Args...> Member;
		template<typename To1, typename To2, typename... Ton>struct StaticReplace{
			typedef To1(To2::*Result)(Ton..., ...);
		};
		template<typename, typename...>struct PackHelper;
		template<typename String, typename Head, typename... Other>struct PackHelper
			<String, Head, Other...>{
			static auto StringPack(String text)
			->decltype(PackHelper<decltype(String() + TypeString() +
			TypeTree<Head>::name()), Other...>::StringPack(String() + TypeString() +
			TypeTree<Head>::name())){
				auto a = text + TypeString(",") + TypeTree<Head>::name();
				return PackHelper<decltype(String() + TypeString() +
					TypeTree<Head>::name()), Other...>::StringPack(a);
			};
		};
		template<typename String, typename Head>struct PackHelper<String, Head>{
			static auto StringPack(String text)
			->decltype(String() + TypeString() + TypeTree<Head>::name()){
				return text + TypeString(",") + TypeTree<Head>::name();
			};
		};
		template<typename String>struct PackHelper<String>{
			static TypeString StringPack(String text){
				return text;
			};
		};
		static auto StringPack()
			->decltype(PackHelper<TypeString, Args...>::StringPack(TypeString(""))){
			return PackHelper<TypeString, Args...>::StringPack(TypeString(""));
		};
		static const unsigned char flag = 0;
		static auto front_name()->decltype(TypeTree<Ret>::front_name() + TypeString()
			+ TypeTree<Class>::name() + TypeString()){
			typedef decltype(TypeTree<Ret>::front_name() + TypeString()
				+ TypeTree<Class>::name() + TypeString()) M;
			static M cache;
			if (cache.empty()){
				auto a = TypeTree<Ret>::front_name();
				auto b = TypeTree<Class>::name();
				cache = a + TypeString(" (") + b + "::* ";
			};
			return cache;
		};
		static auto back_name()->decltype(TypeString() + slice(StringPack(), 1, -1)
			+ TypeString()){
			typedef decltype(TypeString() + slice(StringPack(), 1, -1)
				+ TypeString()) M;
			static M cache;
			if (cache.empty()){
				auto args = StringPack();
				cache = ")(" + slice(args, 1, -1) + TypeString(",...)const ");
			};
			return cache;
		};
		static auto name()
			->decltype(front_name() + back_name()){
			typedef decltype(front_name() + back_name()) M;
			static M cache;
			if (cache.empty())
				cache = front_name() + back_name();
			return cache;
		};
		TEMPCODE_D
	};
	template<typename Ret, typename Class, typename... Args>
	struct TypeTree<Ret(Class::*)(Args...)volatile>{
		typedef Ret Result;
		typedef TypeArray<Ret, Class, Args...> Member;
		template<typename To1, typename To2, typename... Ton>struct StaticReplace{
			typedef To1(To2::*Result)(Ton...);
		};
		template<typename, typename...>struct PackHelper;
		template<typename String, typename Head, typename... Other>struct PackHelper
			<String, Head, Other...>{
			static auto StringPack(String text)
			->decltype(PackHelper<decltype(String() + TypeString() +
			TypeTree<Head>::name()), Other...>::StringPack(String() + TypeString() +
			TypeTree<Head>::name())){
				auto a = text + TypeString(",") + TypeTree<Head>::name();
				return PackHelper<decltype(String() + TypeString() +
					TypeTree<Head>::name()), Other...>::StringPack(a);
			};
		};
		template<typename String, typename Head>struct PackHelper<String, Head>{
			static auto StringPack(String text)
			->decltype(String() + TypeString() + TypeTree<Head>::name()){
				return text + TypeString(",") + TypeTree<Head>::name();
			};
		};
		template<typename String>struct PackHelper<String>{
			static TypeString StringPack(String text){
				return text;
			};
		};
		static auto StringPack()
			->decltype(PackHelper<TypeString, Args...>::StringPack(TypeString(""))){
			return PackHelper<TypeString, Args...>::StringPack(TypeString(""));
		};
		static const unsigned char flag = 0;
		static auto front_name()->decltype(TypeTree<Ret>::front_name() + TypeString()
			+ TypeTree<Class>::name() + TypeString()){
			typedef decltype(TypeTree<Ret>::front_name() + TypeString()
				+ TypeTree<Class>::name() + TypeString()) M;
			static M cache;
			if (cache.empty()){
				auto a = TypeTree<Ret>::front_name();
				auto b = TypeTree<Class>::name();
				cache = a + TypeString(" (") + b + "::* ";
			};
			return cache;
		};
		static auto back_name()->decltype(TypeString() + slice(StringPack(), 1, -1)
			+ TypeString()){
			typedef decltype(TypeString() + slice(StringPack(), 1, -1)
				+ TypeString()) M;
			static M cache;
			if (cache.empty()){
				auto args = StringPack();
				cache = ")(" + slice(args, 1, -1) + TypeString(")volatile ");
			};
			return cache;
		};
		static auto name()
			->decltype(front_name() + back_name()){
			typedef decltype(front_name() + back_name()) M;
			static M cache;
			if (cache.empty())
				cache = front_name() + back_name();
			return cache;
		};
		TEMPCODE_D
	};
	template<typename Ret, typename Class, typename... Args>
	struct TypeTree<Ret(Class::*)(Args..., ...)volatile>{
		typedef Ret Result;
		typedef TypeArray<Ret, Class, Args...> Member;
		template<typename To1, typename To2, typename... Ton>struct StaticReplace{
			typedef To1(To2::*Result)(Ton..., ...);
		};
		template<typename, typename...>struct PackHelper;
		template<typename String, typename Head, typename... Other>struct PackHelper
			<String, Head, Other...>{
			static auto StringPack(String text)
			->decltype(PackHelper<decltype(String() + TypeString() +
			TypeTree<Head>::name()), Other...>::StringPack(String() + TypeString() +
			TypeTree<Head>::name())){
				auto a = text + TypeString(",") + TypeTree<Head>::name();
				return PackHelper<decltype(String() + TypeString() +
					TypeTree<Head>::name()), Other...>::StringPack(a);
			};
		};
		template<typename String, typename Head>struct PackHelper<String, Head>{
			static auto StringPack(String text)
			->decltype(String() + TypeString() + TypeTree<Head>::name()){
				return text + TypeString(",") + TypeTree<Head>::name();
			};
		};
		template<typename String>struct PackHelper<String>{
			static TypeString StringPack(String text){
				return text;
			};
		};
		static auto StringPack()
			->decltype(PackHelper<TypeString, Args...>::StringPack(TypeString(""))){
			return PackHelper<TypeString, Args...>::StringPack(TypeString(""));
		};
		static const unsigned char flag = 0;
		static auto front_name()->decltype(TypeTree<Ret>::front_name() + TypeString()
			+ TypeTree<Class>::name() + TypeString()){
			typedef decltype(TypeTree<Ret>::front_name() + TypeString()
				+ TypeTree<Class>::name() + TypeString()) M;
			static M cache;
			if (cache.empty()){
				auto a = TypeTree<Ret>::front_name();
				auto b = TypeTree<Class>::name();
				cache = a + TypeString(" (") + b + "::* ";
			};
			return cache;
		};
		static auto back_name()->decltype(TypeString() + slice(StringPack(), 1, -1)
			+ TypeString()){
			typedef decltype(TypeString() + slice(StringPack(), 1, -1)
				+ TypeString()) M;
			static M cache;
			if (cache.empty()){
				auto args = StringPack();
				cache = ")(" + slice(args, 1, -1) + TypeString(",...)volatile ");
			};
			return cache;
		};
		static auto name()
			->decltype(front_name() + back_name()){
			typedef decltype(front_name() + back_name()) M;
			static M cache;
			if (cache.empty())
				cache = front_name() + back_name();
			return cache;
		};
		TEMPCODE_D
	};
	template<typename Ret, typename Class, typename... Args>
	struct TypeTree<Ret(Class::*)(Args...)const volatile>{
		typedef Ret Result;
		typedef TypeArray<Ret, Class, Args...> Member;
		template<typename To1, typename To2, typename... Ton>struct StaticReplace{
			typedef To1(To2::*Result)(Ton...);
		};
		template<typename, typename...>struct PackHelper;
		template<typename String, typename Head, typename... Other>struct PackHelper
			<String, Head, Other...>{
			static auto StringPack(String text)
			->decltype(PackHelper<decltype(String() + TypeString() +
			TypeTree<Head>::name()), Other...>::StringPack(String() + TypeString() +
			TypeTree<Head>::name())){
				auto a = text + TypeString(",") + TypeTree<Head>::name();
				return PackHelper<decltype(String() + TypeString() +
					TypeTree<Head>::name()), Other...>::StringPack(a);
			};
		};
		template<typename String, typename Head>struct PackHelper<String, Head>{
			static auto StringPack(String text)
			->decltype(String() + TypeString() + TypeTree<Head>::name()){
				return text + TypeString(",") + TypeTree<Head>::name();
			};
		};
		template<typename String>struct PackHelper<String>{
			static TypeString StringPack(String text){
				return text;
			};
		};
		static auto StringPack()
			->decltype(PackHelper<TypeString, Args...>::StringPack(TypeString(""))){
			return PackHelper<TypeString, Args...>::StringPack(TypeString(""));
		};
		static const unsigned char flag = 0;
		static auto front_name()->decltype(TypeTree<Ret>::front_name() + TypeString()
			+ TypeTree<Class>::name() + TypeString()){
			typedef decltype(TypeTree<Ret>::front_name() + TypeString()
				+ TypeTree<Class>::name() + TypeString()) M;
			static M cache;
			if (cache.empty()){
				auto a = TypeTree<Ret>::front_name();
				auto b = TypeTree<Class>::name();
				cache = a + TypeString(" (") + b + "::* ";
			};
			return cache;
		};
		static auto back_name()->decltype(TypeString() + slice(StringPack(), 1, -1)
			+ TypeString()){
			typedef decltype(TypeString() + slice(StringPack(), 1, -1)
				+ TypeString()) M;
			static M cache;
			if (cache.empty()){
				auto args = StringPack();
				cache = ")(" + slice(args, 1, -1) + TypeString(")const volatile ");
			};
			return cache;
		};
		static auto name()
			->decltype(front_name() + back_name()){
			typedef decltype(front_name() + back_name()) M;
			static M cache;
			if (cache.empty())
				cache = front_name() + back_name();
			return cache;
		};
		TEMPCODE_D
	};
	template<typename Ret, typename Class, typename... Args>
	struct TypeTree<Ret(Class::*)(Args..., ...)const volatile>{
		typedef Ret Result;
		typedef TypeArray<Ret, Class, Args...> Member;
		template<typename To1, typename To2, typename... Ton>struct StaticReplace{
			typedef To1(To2::*Result)(Ton..., ...);
		};
		template<typename, typename...>struct PackHelper;
		template<typename String, typename Head, typename... Other>struct PackHelper
			<String, Head, Other...>{
			static auto StringPack(String text)
			->decltype(PackHelper<decltype(String() + TypeString() +
			TypeTree<Head>::name()), Other...>::StringPack(String() + TypeString() +
			TypeTree<Head>::name())){
				auto a = text + TypeString(",") + TypeTree<Head>::name();
				return PackHelper<decltype(String() + TypeString() +
					TypeTree<Head>::name()), Other...>::StringPack(a);
			};
		};
		template<typename String, typename Head>struct PackHelper<String, Head>{
			static auto StringPack(String text)
			->decltype(String() + TypeString() + TypeTree<Head>::name()){
				return text + TypeString(",") + TypeTree<Head>::name();
			};
		};
		template<typename String>struct PackHelper<String>{
			static TypeString StringPack(String text){
				return text;
			};
		};
		static auto StringPack()
			->decltype(PackHelper<TypeString, Args...>::StringPack(TypeString(""))){
			return PackHelper<TypeString, Args...>::StringPack(TypeString(""));
		};
		static const unsigned char flag = 0;
		static auto front_name()->decltype(TypeTree<Ret>::front_name() + TypeString()
			+ TypeTree<Class>::name() + TypeString()){
			typedef decltype(TypeTree<Ret>::front_name() + TypeString()
				+ TypeTree<Class>::name() + TypeString()) M;
			static M cache;
			if (cache.empty()){
				auto a = TypeTree<Ret>::front_name();
				auto b = TypeTree<Class>::name();
				cache = a + TypeString(" (") + b + "::* ";
			};
			return cache;
		};
		static auto back_name()->decltype(TypeString() + slice(StringPack(), 1, -1)
			+ TypeString()){
			typedef decltype(TypeString() + slice(StringPack(), 1, -1)
				+ TypeString()) M;
			static M cache;
			if (cache.empty()){
				auto args = StringPack();
				cache = ")(" + slice(args, 1, -1) + TypeString(",...)const volatile ");
			};
			return cache;
		};
		static auto name()
			->decltype(front_name() + back_name()){
			typedef decltype(front_name() + back_name()) M;
			static M cache;
			if (cache.empty())
				cache = front_name() + back_name();
			return cache;
		};
		TEMPCODE_D
	};
	template<template<typename...>class A, typename... Args>
	struct TypeTree<A<Args...>>{
		typedef A<Args...> Result;
		typedef TypeArray<Args...> Member;
		template<typename... To>struct StaticReplace{
			typedef A<To...> Result;
		};
		template<template<typename, typename>class Equal, typename From, typename To>
		struct DynamicReplace{
			template<typename X>struct WW{
				typedef typename SelectType<Equal<X, From>::result, To,
				typename TypeTree<X>::template DynamicReplace<Equal, From, To>
				::Result>::Result Result;
			};
			typedef typename StaticReplace<typename WW<Args>::Result...>::Result Result;
		};
		template<typename, typename...>struct PackHelper;
		template<typename String, typename Head, typename... Other>struct PackHelper
			<String, Head, Other...>{
			static auto StringPack(String text)
			->decltype(PackHelper<decltype(String() + TypeString() +
			TypeTree<Head>::name()), Other...>::StringPack(String() + TypeString() +
			TypeTree<Head>::name())){
				auto a = text + TypeString(",") + TypeTree<Head>::name();
				return PackHelper<decltype(String() + TypeString() +
					TypeTree<Head>::name()), Other...>::StringPack(a);
			};
		};
		template<typename String, typename Head>struct PackHelper<String, Head>{
			static auto StringPack(String text)
			->decltype(String() + TypeString() + TypeTree<Head>::name()){
				return text + TypeString(",") + TypeTree<Head>::name();
			};
		};
		template<typename String>struct PackHelper<String>{
			static TypeString StringPack(String text){
				return text;
			};
		};
		static auto StringPack()
			->decltype(PackHelper<TypeString, Args...>::StringPack(TypeString(""))){
			return PackHelper<TypeString, Args...>::StringPack(TypeString(""));
		};
		static const unsigned char flag = 0;
		static auto front_name()->decltype(slice("", 0, 0) + TypeString() +
			slice(StringPack(), 1, -1) + TypeString()){
			typedef decltype(slice("", 0, 0) + TypeString() +
				slice(StringPack(), 1, -1) + TypeString()) M;
			static M cache;
			if (cache.empty()){
				TypeString a = typeid(A<Args...>).name();
				size_t index = std::find(a.begin(), a.end(), '<') - a.begin();
				size_t start = 0;
				if (a[0] == 'c') start = 6;
				if (a[0] == 's') start = 7;
				if (a[0] == 'u') start = 6;
				auto m = slice(a, start, index);
				auto b = slice(StringPack(), 1, -1);
				cache = m + TypeString("<") + b + TypeString(">");
			};
			return cache;
		};
		static TypeString back_name(){
			return "";
		};
		static auto name()->decltype(front_name()){
			return front_name();
		};
	};
#undef TEMPCODE_A
#undef TEMPCODE_B
#undef TEMPCODE_C
#undef TEMPCODE_D
};