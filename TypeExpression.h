#include <iostream>
using std::string;
namespace Types{
	class TypeString{
		string* data;
		class Count{
		private:
			int *count;
			Count& operator=(const Count&c);
		public:
			Count() :count(new int(1)){};
			Count(const Count &c) :count(c.count){ ++*count; };
			~Count(){ if (!(--*count)) delete count; };
			bool only()const{ return *count == 1; };
			bool reattach(const Count&c){
				++*c.count;
				if (!(--*count)){
					delete count;
					count = c.count;
					return true;
				};
				count = c.count;
				return false;
			};
		} count;
	public:
		typedef char value_type;
		TypeString() :data(new string()), count(){};
		TypeString(string const& str) :data(new string(str)), count(){};
		TypeString(const char* str) :data(new string(str)), count(){};
		~TypeString(){ if (count.only()) delete data; };
		TypeString& operator=(const TypeString& handle){
			if (count.reattach(handle.count))
				delete data;
			data = handle.data;
			return *this;
		};
		TypeString& operator=(const char* str){
			if (count.reattach(Count()))
				delete data;
			data = new string(str);
			return *this;
		};
		TypeString& operator=(string const& str){
			if (count.reattach(Count()))
				delete data;
			data = new string(str);
			return *this;
		};
		size_t size()const{ return data->size(); };
		char& operator[](size_t index){
			return data->at(index);
		};
		char const& operator[](size_t index)const{
			return ((const string*)data)->at(index);
		};
		string* operator->(){ return data; };
		string const* operator->()const{ return data; };
		operator const char*(){ return data->c_str(); };
	};
	template<typename, typename>class TypeExpression;
	template<>class TypeExpression<TypeString, TypeString>{
		typedef TypeString Member;
		typedef TypeExpression Self;
		Member left;
		Member right;
		mutable Member result;
	public:
		TypeExpression(Member left, Member right) :left(left), right(right), result(){};
		size_t size()const{ return left.size() + right.size(); };
		char& operator[](size_t index){
			return (index < left.size()) ? left[index] : right[index - (left.size())];
		};
		char const& operator[](size_t index)const{
			return (index < left.size()) ? left[index] : right[index - (left.size())];
		};
		operator TypeString()const{
			if (result->empty()){
				size_t Max = left.size() + right.size();
				char* cache = new char[Max + 1];
				for (size_t i = 0; i != Max; ++i)
					cache[i] = operator[](i);
				cache[Max] = '\0';
				std::cout << "make a string" << std::endl;
				result = cache;
			};
			return result;
		};
		operator const char*()const{
			return TypeString(*this);
		};
	};
	template<typename A, typename B>
	class TypeExpression<TypeExpression<A, B>, TypeString>{
		TypeExpression<A, B> left;
		TypeString right;
		mutable TypeString result;
	public:
		TypeExpression(TypeExpression<A, B> left, TypeString right)
			:left(left), right(right), result(){};
		size_t size()const{ return left.size() + right.size(); };
		char& operator[](size_t index){
			return (index < left.size()) ? left[index] : right[index - (left.size())];
		};
		char const& operator[](size_t index)const{
			return (index < left.size()) ? left[index] : right[index - (left.size())];
		};
		operator TypeString()const{
			if (result->empty()){
				size_t Max = left.size() + right.size();
				char* cache = new char[Max + 1];
				for (size_t i = 0; i != Max; ++i)
					cache[i] = operator[](i);
				cache[Max] = '\0';
				std::cout << "make a string" << std::endl;
				result = cache;
			};
			return result;
		};
		operator const char*()const{
			return TypeString(*this);
		};
	};
	template<typename A, typename B>
	class TypeExpression<TypeString, TypeExpression<A, B>>{
		TypeString left;
		TypeExpression<A, B> right;
		mutable TypeString result;
	public:
		TypeExpression(TypeString left, TypeExpression<A, B> right)
			:left(left), right(right), result(){};
		size_t size()const{ return left.size() + right.size(); };
		char& operator[](size_t index){
			return (index < left.size()) ? left[index] : right[index - (left.size())];
		};
		char const& operator[](size_t index)const{
			return (index < left.size()) ? left[index] : right[index - (left.size())];
		};
		operator TypeString()const{
			if (result->empty()){
				size_t Max = left.size() + right.size();
				char* cache = new char[Max + 1];
				for (size_t i = 0; i != Max; ++i)
					cache[i] = operator[](i);
				cache[Max] = '\0';
				std::cout << "make a string" << std::endl;
				result = cache;
			};
			return result;
		};
		operator const char*()const{
			return TypeString(*this);
		};
	};
	template<typename A, typename B, typename C, typename D>
	class TypeExpression<TypeExpression<A, B>, TypeExpression<C, D>>{
		TypeExpression<A, B> left;
		TypeExpression<C, D> right;
		mutable TypeString result;
	public:
		TypeExpression(TypeExpression<A, B> left, TypeExpression<C, D> right)
			:left(left), right(right), result(){};
		size_t size()const{ return left.size() + right.size(); };
		char& operator[](size_t index){
			return (index < left.size()) ? left[index] : right[index - (left.size())];
		};
		char const& operator[](size_t index)const{
			return (index < left.size()) ? left[index] : right[index - (left.size())];
		};
		operator TypeString()const{
			if (result->empty()){
				size_t Max = left.size() + right.size();
				char* cache = new char[Max + 1];
				for (size_t i = 0; i != Max; ++i)
					cache[i] = operator[](i);
				cache[Max] = '\0';
				std::cout << "make a string" << std::endl;
				result = cache;
			};
			return result;
		};
		operator const char*()const{
			return TypeString(*this);
		};
	};
	template<>class TypeExpression<TypeString, size_t>{
		TypeString left;
		size_t right;
		mutable TypeString result;
	public:
		TypeExpression(TypeString left, size_t right) :left(left), right(right),
			result(){};
		TypeExpression(size_t right, TypeString left) :left(left), right(right),
			result(){};
		size_t size()const{ return left.size()*right; };
		char& operator[](size_t index){
			return left[index % (left.size())];
		};
		char const& operator[](size_t index)const{
			return left[index % (left.size())];
		};
		operator TypeString()const{
			if (result->empty()){
				size_t Max = left.size() * right;
				char* cache = new char[Max + 1];
				for (size_t i = 0; i != Max; ++i)
					cache[i] = operator[](i);
				cache[Max] = '\0';
				std::cout << "make a string" << std::endl;
				result = cache;
			};
			return result;
		};
		operator const char*()const{
			return TypeString(*this);
		};
	};
	template<typename A, typename B>
	class TypeExpression<TypeExpression<A, B>, size_t>{
		TypeExpression<A, B> left;
		size_t right;
		mutable TypeString result;
	public:
		TypeExpression(TypeExpression<A, B> left, size_t right)
			:left(left), right(right), result(){};
		TypeExpression(size_t right, TypeExpression<A, B> left)
			:left(left), right(right), result(){};
		size_t size()const{ return left.size()*right; };
		char& operator[](size_t index){
			return left[index % (left.size())];
		};
		char const& operator[](size_t index)const{
			return left[index % (left.size())];
		};
		operator TypeString()const{
			if (result->empty()){
				size_t Max = left.size() * right;
				char* cache = new char[Max + 1];
				for (size_t i = 0; i != Max; ++i)
					cache[i] = operator[](i);
				cache[Max] = '\0';
				std::cout << "make a string" << std::endl;
				result = cache;
			};
			return result;
		};
		operator const char*()const{
			return TypeString(*this);
		};
	};
	template<>class TypeExpression<TypeString, std::pair<size_t, size_t>>{
		TypeString str;
		size_t start, end;
		mutable TypeString result;
	public:
		TypeExpression(TypeString left, size_t start, size_t end)
			:str(left), start(start >= str.size() ? str.size() - 1 : start),
			end(end >= str.size() ? str.size() - 1 : end), result(){};
		size_t size()const{ return end - start; };
		char& operator[](size_t index){
			return str[index + start];
		};
		char const& operator[](size_t index)const{
			return str[index + start];
		};
		operator TypeString()const{
			if (result->empty()){
				size_t Max = size();
				char* cache = new char[Max + 1];
				for (size_t i = 0; i != Max; ++i)
					cache[i] = operator[](i);
				cache[Max] = '\0';
				std::cout << "make a string" << std::endl;
				result = cache;
			};
			return result;
		};
		operator const char*()const{
			return TypeString(*this);
		};
	};
	template<typename A, typename B>
	class TypeExpression<TypeExpression<A, B>, std::pair<size_t, size_t>>{
		TypeExpression<A, B> str;
		size_t start, end;
		mutable TypeString result;
	public:
		TypeExpression(TypeExpression<A, B> left, size_t start, size_t end)
			:str(left), start(start >= str.size() ? str.size() - 1 : start),
			end(end >= str.size() ? str.size() - 1 : end), result(){};
		size_t size()const{ return end - start; };
		char& operator[](size_t index){
			return str[index + start];
		};
		char const& operator[](size_t index)const{
			return str[index + start];
		};
		operator TypeString()const{
			if (result->empty()){
				size_t Max = size();
				char* cache = new char[Max + 1];
				for (size_t i = 0; i != Max; ++i)
					cache[i] = operator[](i);
				cache[Max] = '\0';
				std::cout << "make a string" << std::endl;
				result = cache;
			};
			return result;
		};
		operator const char*()const{
			return TypeString(*this);
		};
	};

	TypeExpression<TypeString, TypeString> operator+(TypeString a, TypeString b){
		return TypeExpression<TypeString, TypeString>(a, b);
	};
	template<typename A, typename B>
	TypeExpression<TypeExpression<A, B>, TypeString> operator+
		(TypeExpression<A, B> a, TypeString b){
		return TypeExpression<TypeExpression<A, B>, TypeString>(a, b);
	};
	template<typename A, typename B>
	TypeExpression<TypeString, TypeExpression<A, B>> operator+
		(TypeString a, TypeExpression<A, B> b){
		return TypeExpression<TypeString, TypeExpression<A, B>>(a, b);
	};
	template<typename A, typename B, typename C, typename D>
	TypeExpression<TypeExpression<A, B>, TypeExpression<C, D>> operator+
		(TypeExpression<A, B> a, TypeExpression<C, D> b){
		return TypeExpression<TypeExpression<A, B>, TypeExpression<C, D>>(a, b);
	};
	TypeExpression<TypeString, size_t> operator*(TypeString a, size_t b){
		return TypeExpression<TypeString, size_t>(a, b);
	};
	TypeExpression<TypeString, size_t> operator*(size_t a, TypeString b){
		return TypeExpression<TypeString, size_t>(a, b);
	};
	template<typename A, typename B>
	TypeExpression<TypeExpression<A, B>, size_t> operator*
		(TypeExpression<A, B> a, size_t b){
		return TypeExpression<TypeExpression<A, B>, size_t>(a, b);
	};
	template<typename A, typename B>
	TypeExpression<TypeExpression<A, B>, size_t> operator*
		(size_t a, TypeExpression<A, B> b){
		return TypeExpression<TypeExpression<A, B>, size_t>(a, b);
	};
	TypeExpression<TypeString, std::pair<size_t, size_t>> slice
		(TypeString str, size_t start, size_t end){
		return TypeExpression<TypeString, std::pair<size_t, size_t>>(str, start, end);
	};
	template<typename A, typename B>
	TypeExpression<TypeExpression<A, B>, std::pair<size_t, size_t>> slice
		(TypeExpression<A, B> str, size_t start, size_t end){
		return TypeExpression<TypeExpression<A, B>, std::pair<size_t, size_t>>
			(str, start, end);
	};
};