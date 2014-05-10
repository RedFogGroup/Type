namespace Types{
	template<typename Expression>class ExpIter{
		Expression handle;
		size_t index;
	public:
		typedef std::random_access_iterator_tag iterator_category;
		typedef int difference_type;
		typedef difference_type distance_type;	// retained
		typedef char* pointer;
		typedef char& reference;
		typedef char value_type;
		ExpIter() :handle(), index(index){};
		ExpIter(Expression m, size_t index = 0) :handle(m), index(index){};
		char& operator[](int plus){ return handle[index + plus]; };
		char const& operator[](int plus)const{ return handle[index + plus]; };
		char& operator*(){ return handle[index]; };
		char const& operator*()const{ return handle[index]; };
		char* operator->(){ return &(handle[index]); };
		char const* operator->()const{ return &(handle[index]); };
		ExpIter operator+()const{ return *this; };
		ExpIter operator-()const{ return ExpIter(handle, -index); };
		ExpIter& operator++(){ ++index; return *this; };
		ExpIter operator++(int){ ++index; return ExpIter(handle, index - 1); };
		ExpIter& operator--(){ --index; return *this; };
		ExpIter operator--(int){ --index; return ExpIter(handle, index + 1); };
		ExpIter& operator+=(int step){ index += step; return *this; };
		ExpIter& operator-=(int step){ index -= step; return *this; };
		friend ExpIter operator+(ExpIter const& handle, int step){
			return ExpIter(handle.handle, handle.index + step);
		};
		friend ExpIter operator+(int step, ExpIter const& handle){
			return ExpIter(handle.handle, handle.index + step);
		};
		friend ExpIter operator-(ExpIter const& handle, int step){
			return ExpIter(handle.handle, handle.index - step);
		};
		friend ExpIter operator-(int step, ExpIter const& handle){
			return ExpIter(handle.handle, step - handle.index);
		};
		friend int operator-(ExpIter const& left, ExpIter const& right){
			return left.index - right.index;
		};
		friend bool operator==(ExpIter const& left, ExpIter const& right){
			return left.handle == right.handle && left.index == right.index;
		};
		friend bool operator!=(ExpIter const& left, ExpIter const& right){
			return left.handle == right.handle && left.index != right.index;
		};
		friend bool operator>=(ExpIter const& left, ExpIter const& right){
			return left.handle == right.handle && left.index >= right.index;
		};
		friend bool operator<=(ExpIter const& left, ExpIter const& right){
			return left.handle == right.handle && left.index <= right.index;
		};
		friend bool operator>(ExpIter const& left, ExpIter const& right){
			return left.handle == right.handle && left.index > right.index;
		};
		friend bool operator<(ExpIter const& left, ExpIter const& right){
			return left.handle == right.handle && left.index < right.index;
		};
	};
	template<typename Expression>class ReverseExpIter{
		Expression handle;
		size_t index;
	public:
		typedef char value_type;
		ReverseExpIter() :handle(), index(index){};
		ReverseExpIter(Expression m, size_t index = 0) :handle(m), index(index){};
		char& operator[](int plus){ return handle[m.size() - index - plus]; };
		char const& operator[](int plus)const{ return handle[m.size() - index - plus]; };
		char& operator*(){ return handle[m.size() - index]; };
		char const& operator*()const{ return handle[m.size() - index]; };
		char* operator->(){ return &(handle[m.size() - index]); };
		char const* operator->()const{ return &(handle[m.size() - index]); };
		ReverseExpIter operator+()const{ return *this; };
		ReverseExpIter operator-()const{ return ReverseExpIter(handle, -index); };
		ReverseExpIter& operator++(){ ++index; return *this; };
		ReverseExpIter operator++(int){
			++index;
			return ReverseExpIter(handle, index - 1);
		};
		ReverseExpIter& operator--(){ --index; return *this; };
		ReverseExpIter operator--(int){
			--index;
			return ReverseExpIter(handle, index + 1);
		};
		ReverseExpIter& operator+=(int step){ index += step; return *this; };
		ReverseExpIter& operator-=(int step){ index -= step; return *this; };
		friend ReverseExpIter operator+(ReverseExpIter const& handle, int step){
			return ReverseExpIter(handle.handle, handle.index + step);
		};
		friend ReverseExpIter operator+(int step, ReverseExpIter const& handle){
			return ReverseExpIter(handle.handle, handle.index + step);
		};
		friend ReverseExpIter operator-(ReverseExpIter const& handle, int step){
			return ReverseExpIter(handle.handle, handle.index - step);
		};
		friend ReverseExpIter operator-(int step, ReverseExpIter const& handle){
			return ReverseExpIter(handle.handle, step - handle.index);
		};
		friend int operator-(ReverseExpIter const& left, ReverseExpIter const& right){
			return left.index - right.index;
		};
		friend bool operator==(ReverseExpIter const& left, ReverseExpIter const& right){
			return left.handle == right.handle && left.index == right.index;
		};
		friend bool operator!=(ReverseExpIter const& left, ReverseExpIter const& right){
			return left.handle == right.handle && left.index != right.index;
		};
		friend bool operator>=(ReverseExpIter const& left, ReverseExpIter const& right){
			return left.handle == right.handle && left.index >= right.index;
		};
		friend bool operator<=(ReverseExpIter const& left, ReverseExpIter const& right){
			return left.handle == right.handle && left.index <= right.index;
		};
		friend bool operator>(ReverseExpIter const& left, ReverseExpIter const& right){
			return left.handle == right.handle && left.index > right.index;
		};
		friend bool operator<(ReverseExpIter const& left, ReverseExpIter const& right){
			return left.handle == right.handle && left.index < right.index;
		};
	};
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
		typedef ExpIter<TypeString> iterator;
		typedef const ExpIter<TypeString> const_iterator;
		typedef ReverseExpIter<TypeString> reverse_iterator;
		typedef const ReverseExpIter<TypeString> const_reverse_iterator;
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
		bool empty()const{ return data->empty(); };
		char& operator[](size_t index){
			return data->at(index);
		};
		char const& operator[](size_t index)const{
			return data->at(index);
		};
		string* operator->(){ return data; };
		string const* operator->()const{ return data; };
		explicit operator string()const{ return *data; };
		explicit operator const char*()const{ return data->c_str(); };
		const char* c_str()const{ return data->c_str(); };
		iterator begin(){ return iterator(*this, 0); };
		const_iterator begin()const{ return const_iterator(*this, 0); };
		const_iterator cbegin()const{ return const_iterator(*this, 0); };
		iterator end(){ return iterator(*this, size()); };
		const_iterator end()const{ return const_iterator(*this, size()); };
		const_iterator cend()const{ return const_iterator(*this, size()); };
		reverse_iterator rbegin(){ return reverse_iterator(*this, 0); };
		const_reverse_iterator rbegin()const{ return const_reverse_iterator(*this, 0); };
		const_reverse_iterator crbegin()const{ return const_reverse_iterator(*this, 0); };
		reverse_iterator rend(){ return reverse_iterator(*this, size()); };
		const_reverse_iterator rend()const{ return const_reverse_iterator(*this, size()); };
		const_reverse_iterator crend()const{ return const_reverse_iterator(*this, size()); };
		friend bool operator==(TypeString const& left, TypeString const& right){
			return left.data == right.data;
		};
		friend bool operator!=(TypeString const& left, TypeString const& right){
			return left.data != right.data;
		};
		friend std::ostream& operator<<(std::ostream& cout, TypeString const& h){
			return cout << (const char *)h;
		};
	};
	template<typename, typename>class TypeExpression;
	template<>class TypeExpression<TypeString, TypeString>{
		typedef TypeString Member;
		typedef TypeExpression Self;
		Member left;
		Member right;
		bool _empty;
		mutable Member result;
	public:
		typedef ExpIter<TypeExpression> iterator;
		typedef const ExpIter<TypeExpression> const_iterator;
		typedef ReverseExpIter<TypeExpression> reverse_iterator;
		typedef const ReverseExpIter<TypeExpression> const_reverse_iterator;
		typedef char value_type;
		TypeExpression() :_empty(true){};
		TypeExpression(Member left, Member right)
			:left(left), right(right), result(), _empty(false){};
		size_t size()const{ return left.size() + right.size(); };
		bool empty()const{ return _empty; };
		char& operator[](size_t index){
			return (index < left.size()) ? left[index] : right[index - (left.size())];
		};
		char const& operator[](size_t index)const{
			return (index < left.size()) ? left[index] : right[index - (left.size())];
		};
		explicit operator TypeString()const{
			if (result->empty()){
				size_t Max = left.size() + right.size();
				char* cache = new char[Max + 1];
				for (size_t i = 0; i != Max; ++i)
					cache[i] = operator[](i);
				cache[Max] = '\0';
				result = cache;
			};
			return result;
		};
		explicit operator const char*()const{
			return TypeString(*this).c_str();
		};
		const char * c_str()const{
			return TypeString(*this).c_str();
		};
		iterator begin(){ return iterator(*this, 0); };
		const_iterator begin()const{ return const_iterator(*this, 0); };
		const_iterator cbegin()const{ return const_iterator(*this, 0); };
		iterator end(){ return iterator(*this, size()); };
		const_iterator end()const{ return const_iterator(*this, size()); };
		const_iterator cend()const{ return const_iterator(*this, size()); };
		reverse_iterator rbegin(){ return reverse_iterator(*this, 0); };
		const_reverse_iterator rbegin()const{ return const_reverse_iterator(*this, 0); };
		const_reverse_iterator crbegin()const{ return const_reverse_iterator(*this, 0); };
		reverse_iterator rend(){ return reverse_iterator(*this, size()); };
		const_reverse_iterator rend()const{ return const_reverse_iterator(*this, size()); };
		const_reverse_iterator crend()const{ return const_reverse_iterator(*this, size()); };
		friend bool operator==(TypeExpression const& left, TypeExpression const& right){
			return left.left == right.left && left.right == right.right;
		};
		friend bool operator!=(TypeExpression const& left, TypeExpression const& right){
			return left.left != right.left || left.right != right.right;
		};
		friend std::ostream& operator<<(std::ostream& cout, TypeExpression const& h){
			return cout << (const char *)h;
		};
	};
	template<typename A, typename B>
	class TypeExpression<TypeExpression<A, B>, TypeString>{
		TypeExpression<A, B> left;
		TypeString right;
		bool _empty;
		mutable TypeString result;
	public:
		typedef ExpIter<TypeExpression> iterator;
		typedef const ExpIter<TypeExpression> const_iterator;
		typedef ReverseExpIter<TypeExpression> reverse_iterator;
		typedef const ReverseExpIter<TypeExpression> const_reverse_iterator;
		typedef char value_type;
		TypeExpression() :_empty(true){};
		TypeExpression(TypeExpression<A, B> left, TypeString right)
			:left(left), right(right), result(), _empty(false){};
		size_t size()const{ return left.size() + right.size(); };
		bool empty()const{ return _empty; };
		char& operator[](size_t index){
			return (index < left.size()) ? left[index] : right[index - (left.size())];
		};
		char const& operator[](size_t index)const{
			return (index < left.size()) ? left[index] : right[index - (left.size())];
		};
		explicit operator TypeString()const{
			if (result->empty()){
				size_t Max = left.size() + right.size();
				char* cache = new char[Max + 1];
				for (size_t i = 0; i != Max; ++i)
					cache[i] = operator[](i);
				cache[Max] = '\0';
				result = cache;
			};
			return result;
		};
		explicit operator const char*()const{
			return TypeString(*this).c_str();
		};
		const char * c_str()const{
			return TypeString(*this).c_str();
		};
		iterator begin(){ return iterator(*this, 0); };
		const_iterator begin()const{ return const_iterator(*this, 0); };
		const_iterator cbegin()const{ return const_iterator(*this, 0); };
		iterator end(){ return iterator(*this, size()); };
		const_iterator end()const{ return const_iterator(*this, size()); };
		const_iterator cend()const{ return const_iterator(*this, size()); };
		reverse_iterator rbegin(){ return reverse_iterator(*this, 0); };
		const_reverse_iterator rbegin()const{ return const_reverse_iterator(*this, 0); };
		const_reverse_iterator crbegin()const{ return const_reverse_iterator(*this, 0); };
		reverse_iterator rend(){ return reverse_iterator(*this, size()); };
		const_reverse_iterator rend()const{ return const_reverse_iterator(*this, size()); };
		const_reverse_iterator crend()const{ return const_reverse_iterator(*this, size()); };
		friend bool operator==(TypeExpression const& left, TypeExpression const& right){
			return left.left == right.left && left.right == right.right;
		};
		friend bool operator!=(TypeExpression const& left, TypeExpression const& right){
			return left.left != right.left || left.right != right.right;
		};
		friend std::ostream& operator<<(std::ostream& cout, TypeExpression const& h){
			return cout << (const char *)h;
		};
	};
	template<typename A, typename B>
	class TypeExpression<TypeString, TypeExpression<A, B>>{
		TypeString left;
		TypeExpression<A, B> right;
		bool _empty;
		mutable TypeString result;
	public:
		typedef ExpIter<TypeExpression> iterator;
		typedef const ExpIter<TypeExpression> const_iterator;
		typedef ReverseExpIter<TypeExpression> reverse_iterator;
		typedef const ReverseExpIter<TypeExpression> const_reverse_iterator;
		typedef char value_type;
		TypeExpression() :_empty(true){};
		TypeExpression(TypeString left, TypeExpression<A, B> right)
			:left(left), right(right), result(), _empty(false){};
		size_t size()const{ return left.size() + right.size(); };
		bool empty()const{ return _empty; };
		char& operator[](size_t index){
			return (index < left.size()) ? left[index] : right[index - (left.size())];
		};
		char const& operator[](size_t index)const{
			return (index < left.size()) ? left[index] : right[index - (left.size())];
		};
		explicit operator TypeString()const{
			if (result->empty()){
				size_t Max = left.size() + right.size();
				char* cache = new char[Max + 1];
				for (size_t i = 0; i != Max; ++i)
					cache[i] = operator[](i);
				cache[Max] = '\0';
				result = cache;
			};
			return result;
		};
		explicit operator const char*()const{
			return TypeString(*this).c_str();
		};
		const char * c_str()const{
			return TypeString(*this).c_str();
		};
		iterator begin(){ return iterator(*this, 0); };
		const_iterator begin()const{ return const_iterator(*this, 0); };
		const_iterator cbegin()const{ return const_iterator(*this, 0); };
		iterator end(){ return iterator(*this, size()); };
		const_iterator end()const{ return const_iterator(*this, size()); };
		const_iterator cend()const{ return const_iterator(*this, size()); };
		reverse_iterator rbegin(){ return reverse_iterator(*this, 0); };
		const_reverse_iterator rbegin()const{ return const_reverse_iterator(*this, 0); };
		const_reverse_iterator crbegin()const{ return const_reverse_iterator(*this, 0); };
		reverse_iterator rend(){ return reverse_iterator(*this, size()); };
		const_reverse_iterator rend()const{ return const_reverse_iterator(*this, size()); };
		const_reverse_iterator crend()const{ return const_reverse_iterator(*this, size()); };
		friend bool operator==(TypeExpression const& left, TypeExpression const& right){
			return left.left == right.left && left.right == right.right;
		};
		friend bool operator!=(TypeExpression const& left, TypeExpression const& right){
			return left.left != right.left || left.right != right.right;
		};
		friend std::ostream& operator<<(std::ostream& cout, TypeExpression const& h){
			return cout << (const char *)h;
		};
	};
	template<typename A, typename B, typename C, typename D>
	class TypeExpression<TypeExpression<A, B>, TypeExpression<C, D>>{
		TypeExpression<A, B> left;
		TypeExpression<C, D> right;
		bool _empty;
		mutable TypeString result;
	public:
		typedef ExpIter<TypeExpression> iterator;
		typedef const ExpIter<TypeExpression> const_iterator;
		typedef ReverseExpIter<TypeExpression> reverse_iterator;
		typedef const ReverseExpIter<TypeExpression> const_reverse_iterator;
		typedef char value_type;
		TypeExpression() :_empty(true){};
		TypeExpression(TypeExpression<A, B> left, TypeExpression<C, D> right)
			:left(left), right(right), result(), _empty(false){};
		size_t size()const{ return left.size() + right.size(); };
		bool empty()const{ return _empty; };
		char& operator[](size_t index){
			return (index < left.size()) ? left[index] : right[index - (left.size())];
		};
		char const& operator[](size_t index)const{
			return (index < left.size()) ? left[index] : right[index - (left.size())];
		};
		explicit operator TypeString()const{
			if (result->empty()){
				size_t Max = left.size() + right.size();
				char* cache = new char[Max + 1];
				for (size_t i = 0; i != Max; ++i)
					cache[i] = operator[](i);
				cache[Max] = '\0';
				result = cache;
			};
			return result;
		};
		explicit operator const char*()const{
			return TypeString(*this).c_str();
		};
		const char * c_str()const{
			return TypeString(*this).c_str();
		};
		iterator begin(){ return iterator(*this, 0); };
		const_iterator begin()const{ return const_iterator(*this, 0); };
		const_iterator cbegin()const{ return const_iterator(*this, 0); };
		iterator end(){ return iterator(*this, size()); };
		const_iterator end()const{ return const_iterator(*this, size()); };
		const_iterator cend()const{ return const_iterator(*this, size()); };
		reverse_iterator rbegin(){ return reverse_iterator(*this, 0); };
		const_reverse_iterator rbegin()const{ return const_reverse_iterator(*this, 0); };
		const_reverse_iterator crbegin()const{ return const_reverse_iterator(*this, 0); };
		reverse_iterator rend(){ return reverse_iterator(*this, size()); };
		const_reverse_iterator rend()const{ return const_reverse_iterator(*this, size()); };
		const_reverse_iterator crend()const{ return const_reverse_iterator(*this, size()); };
		friend bool operator==(TypeExpression const& left, TypeExpression const& right){
			return left.left == right.left && left.right == right.right;
		};
		friend bool operator!=(TypeExpression const& left, TypeExpression const& right){
			return left.left != right.left || left.right != right.right;
		};
		friend std::ostream& operator<<(std::ostream& cout, TypeExpression const& h){
			return cout << (const char *)h;
		};
	};
	template<>class TypeExpression<TypeString, size_t>{
		TypeString left;
		size_t right;
		bool _empty;
		mutable TypeString result;
	public:
		typedef ExpIter<TypeExpression> iterator;
		typedef const ExpIter<TypeExpression> const_iterator;
		typedef ReverseExpIter<TypeExpression> reverse_iterator;
		typedef const ReverseExpIter<TypeExpression> const_reverse_iterator;
		typedef char value_type;
		TypeExpression() :_empty(true){};
		TypeExpression(TypeString left, size_t right) :left(left), right(right),
			result(), _empty(false){};
		TypeExpression(size_t right, TypeString left) :left(left), right(right),
			result(), _empty(false){};
		size_t size()const{ return left.size()*right; };
		bool empty()const{ return _empty; };
		char& operator[](size_t index){
			return left[index % (left.size())];
		};
		char const& operator[](size_t index)const{
			return left[index % (left.size())];
		};
		explicit operator TypeString()const{
			if (result->empty()){
				size_t Max = left.size() * right;
				char* cache = new char[Max + 1];
				for (size_t i = 0; i != Max; ++i)
					cache[i] = operator[](i);
				cache[Max] = '\0';
				result = cache;
			};
			return result;
		};
		explicit operator const char*()const{
			return TypeString(*this).c_str();
		};
		const char * c_str()const{
			return TypeString(*this).c_str();
		};
		iterator begin(){ return iterator(*this, 0); };
		const_iterator begin()const{ return const_iterator(*this, 0); };
		const_iterator cbegin()const{ return const_iterator(*this, 0); };
		iterator end(){ return iterator(*this, size()); };
		const_iterator end()const{ return const_iterator(*this, size()); };
		const_iterator cend()const{ return const_iterator(*this, size()); };
		reverse_iterator rbegin(){ return reverse_iterator(*this, 0); };
		const_reverse_iterator rbegin()const{ return const_reverse_iterator(*this, 0); };
		const_reverse_iterator crbegin()const{ return const_reverse_iterator(*this, 0); };
		reverse_iterator rend(){ return reverse_iterator(*this, size()); };
		const_reverse_iterator rend()const{ return const_reverse_iterator(*this, size()); };
		const_reverse_iterator crend()const{ return const_reverse_iterator(*this, size()); };
		friend bool operator==(TypeExpression const& left, TypeExpression const& right){
			return left.left == right.left && left.right == right.right;
		};
		friend bool operator!=(TypeExpression const& left, TypeExpression const& right){
			return left.left != right.left || left.right != right.right;
		};
		friend std::ostream& operator<<(std::ostream& cout, TypeExpression const& h){
			return cout << (const char *)h;
		};
	};
	template<typename A, typename B>
	class TypeExpression<TypeExpression<A, B>, size_t>{
		TypeExpression<A, B> left;
		size_t right;
		bool _empty;
		mutable TypeString result;
	public:
		typedef ExpIter<TypeExpression> iterator;
		typedef const ExpIter<TypeExpression> const_iterator;
		typedef ReverseExpIter<TypeExpression> reverse_iterator;
		typedef const ReverseExpIter<TypeExpression> const_reverse_iterator;
		typedef char value_type;
		TypeExpression() :_empty(true){};
		TypeExpression(TypeExpression<A, B> left, size_t right)
			:left(left), right(right), result(), _empty(false){};
		TypeExpression(size_t right, TypeExpression<A, B> left)
			:left(left), right(right), result(), _empty(false){};
		size_t size()const{ return left.size()*right; };
		bool empty()const{ return _empty; };
		char& operator[](size_t index){
			return left[index % (left.size())];
		};
		char const& operator[](size_t index)const{
			return left[index % (left.size())];
		};
		explicit operator TypeString()const{
			if (result->empty()){
				size_t Max = left.size() * right;
				char* cache = new char[Max + 1];
				for (size_t i = 0; i != Max; ++i)
					cache[i] = operator[](i);
				cache[Max] = '\0';
				result = cache;
			};
			return result;
		};
		explicit operator const char*()const{
			return TypeString(*this).c_str();
		};
		const char * c_str()const{
			return TypeString(*this).c_str();
		};
		iterator begin(){ return iterator(*this, 0); };
		const_iterator begin()const{ return const_iterator(*this, 0); };
		const_iterator cbegin()const{ return const_iterator(*this, 0); };
		iterator end(){ return iterator(*this, size()); };
		const_iterator end()const{ return const_iterator(*this, size()); };
		const_iterator cend()const{ return const_iterator(*this, size()); };
		reverse_iterator rbegin(){ return reverse_iterator(*this, 0); };
		const_reverse_iterator rbegin()const{ return const_reverse_iterator(*this, 0); };
		const_reverse_iterator crbegin()const{ return const_reverse_iterator(*this, 0); };
		reverse_iterator rend(){ return reverse_iterator(*this, size()); };
		const_reverse_iterator rend()const{ return const_reverse_iterator(*this, size()); };
		const_reverse_iterator crend()const{ return const_reverse_iterator(*this, size()); };
		friend bool operator==(TypeExpression const& left, TypeExpression const& right){
			return left.left == right.left && left.right == right.right;
		};
		friend bool operator!=(TypeExpression const& left, TypeExpression const& right){
			return left.left != right.left || left.right != right.right;
		};
		friend std::ostream& operator<<(std::ostream& cout, TypeExpression const& h){
			return cout << (const char *)h;
		};
	};
	template<>class TypeExpression<TypeString, std::pair<size_t, size_t>>{
		TypeString str;
		size_t start, xend;
		bool _empty;
		mutable TypeString result;
	public:
		typedef ExpIter<TypeExpression> iterator;
		typedef const ExpIter<TypeExpression> const_iterator;
		typedef ReverseExpIter<TypeExpression> reverse_iterator;
		typedef const ReverseExpIter<TypeExpression> const_reverse_iterator;
		typedef char value_type;
		TypeExpression() :_empty(true){};
		TypeExpression(TypeString left, size_t start, size_t end)
			:str(left), start(start > str.size() ? str.size() : start),
			xend(end > str.size() ? str.size() : end), result(), _empty(false){};
		size_t size()const{ return xend - start; };
		bool empty()const{ return _empty; };
		char& operator[](size_t index){
			return str[index + start];
		};
		char const& operator[](size_t index)const{
			return str[index + start];
		};
		explicit operator TypeString()const{
			if (result->empty()){
				size_t Max = size();
				char* cache = new char[Max + 1];
				for (size_t i = 0; i != Max; ++i)
					cache[i] = operator[](i);
				cache[Max] = '\0';
				result = cache;
			};
			return result;
		};
		explicit operator const char*()const{
			return TypeString(*this).c_str();
		};
		const char * c_str()const{
			return TypeString(*this).c_str();
		};
		iterator begin(){ return iterator(*this, 0); };
		const_iterator begin()const{ return const_iterator(*this, 0); };
		const_iterator cbegin()const{ return const_iterator(*this, 0); };
		iterator end(){ return iterator(*this, size()); };
		const_iterator end()const{ return const_iterator(*this, size()); };
		const_iterator cend()const{ return const_iterator(*this, size()); };
		reverse_iterator rbegin(){ return reverse_iterator(*this, 0); };
		const_reverse_iterator rbegin()const{ return const_reverse_iterator(*this, 0); };
		const_reverse_iterator crbegin()const{ return const_reverse_iterator(*this, 0); };
		reverse_iterator rend(){ return reverse_iterator(*this, size()); };
		const_reverse_iterator rend()const{ return const_reverse_iterator(*this, size()); };
		const_reverse_iterator crend()const{ return const_reverse_iterator(*this, size()); };
		friend bool operator==(TypeExpression const& left, TypeExpression const& right){
			return left.str == right.str && left.start == right.start && left.xend == right.xend;
		};
		friend bool operator!=(TypeExpression const& left, TypeExpression const& right){
			return left.str != right.str && left.start != right.start && left.xend != right.xend;
		};
		friend std::ostream& operator<<(std::ostream& cout, TypeExpression const& h){
			return cout << (const char *)h;
		};
	};
	template<typename A, typename B>
	class TypeExpression<TypeExpression<A, B>, std::pair<size_t, size_t>>{
		TypeExpression<A, B> str;
		size_t start, xend;
		bool _empty;
		mutable TypeString result;
	public:
		typedef ExpIter<TypeExpression> iterator;
		typedef const ExpIter<TypeExpression> const_iterator;
		typedef ReverseExpIter<TypeExpression> reverse_iterator;
		typedef const ReverseExpIter<TypeExpression> const_reverse_iterator;
		typedef char value_type;
		TypeExpression() :_empty(true){};
		TypeExpression(TypeExpression<A, B> left, size_t start, size_t end)
			:str(left), start(start > str.size() ? str.size() : start),
			xend(end > str.size() ? str.size() : end), result(), _empty(false){};
		size_t size()const{ return xend - start; };
		bool empty()const{ return _empty; };
		char& operator[](size_t index){
			return str[index + start];
		};
		char const& operator[](size_t index)const{
			return str[index + start];
		};
		explicit operator TypeString()const{
			if (result->empty()){
				size_t Max = size();
				char* cache = new char[Max + 1];
				for (size_t i = 0; i != Max; ++i)
					cache[i] = operator[](i);
				cache[Max] = '\0';
				result = cache;
			};
			return result;
		};
		explicit operator const char*()const{
			return TypeString(*this).c_str();
		};
		const char * c_str()const{
			return TypeString(*this).c_str();
		};
		iterator begin(){ return iterator(*this, 0); };
		const_iterator begin()const{ return const_iterator(*this, 0); };
		const_iterator cbegin()const{ return const_iterator(*this, 0); };
		iterator end(){ return iterator(*this, size()); };
		const_iterator end()const{ return const_iterator(*this, size()); };
		const_iterator cend()const{ return const_iterator(*this, size()); };
		reverse_iterator rbegin(){ return reverse_iterator(*this, 0); };
		const_reverse_iterator rbegin()const{ return const_reverse_iterator(*this, 0); };
		const_reverse_iterator crbegin()const{ return const_reverse_iterator(*this, 0); };
		reverse_iterator rend(){ return reverse_iterator(*this, size()); };
		const_reverse_iterator rend()const{ return const_reverse_iterator(*this, size()); };
		const_reverse_iterator crend()const{ return const_reverse_iterator(*this, size()); };
		friend bool operator==(TypeExpression const& left, TypeExpression const& right){
			return left.str == right.str && left.start == right.start && left.xend == right.xend;
		};
		friend bool operator!=(TypeExpression const& left, TypeExpression const& right){
			return left.str != right.str && left.start != right.start && left.xend != right.xend;
		};
		friend std::ostream& operator<<(std::ostream& cout, TypeExpression const& h){
			return cout << (const char *)h;
		};
	};
	template<>class TypeExpression<TypeString, std::tuple<size_t, size_t, TypeString>>{
		TypeString source;
		TypeString temp;
		size_t start, xend;
		bool _empty;
		mutable TypeString result;
	public:
		typedef ExpIter<TypeExpression> iterator;
		typedef const ExpIter<TypeExpression> const_iterator;
		typedef ReverseExpIter<TypeExpression> reverse_iterator;
		typedef const ReverseExpIter<TypeExpression> const_reverse_iterator;
		typedef char value_type;
		TypeExpression() :_empty(true){};
		TypeExpression(TypeString source, TypeString temp, size_t start, size_t end)
			:source(source), temp(temp), _empty(false),
			start(start > source.size() ? source.size() : start),
			xend(start > source.size() ? source.size() : end){};
		TypeExpression(TypeString source, size_t start, size_t end, TypeString temp)
			:source(source), temp(temp), _empty(false),
			start(start > source.size() ? source.size() : start),
			xend(start > source.size() ? source.size() : end){};
		size_t size()const{ return source.size() + temp.size() + start - xend; };
		bool empty()const{ return _empty; };
		char& operator[](size_t index){
			if (index < start) return source[index];
			if (index < start + temp.size()) return temp[index - start];
			return source[xend + index - start - temp.size()];
		};
		char const& operator[](size_t index)const{
			if (index < start) return source[index];
			if (index < start + temp.size()) return temp[index - start];
			return source[xend + index - start - temp.size()];
		};
		explicit operator TypeString()const{
			if (result->empty()){
				size_t Max = size();
				char* cache = new char[Max + 1];
				for (size_t i = 0; i != Max; ++i)
					cache[i] = operator[](i);
				cache[Max] = '\0';
				result = cache;
			};
			return result;
		};
		explicit operator const char*()const{
			return TypeString(*this).c_str();
		};
		const char * c_str()const{
			return TypeString(*this).c_str();
		};
		iterator begin(){ return iterator(*this, 0); };
		const_iterator begin()const{ return const_iterator(*this, 0); };
		const_iterator cbegin()const{ return const_iterator(*this, 0); };
		iterator end(){ return iterator(*this, size()); };
		const_iterator end()const{ return const_iterator(*this, size()); };
		const_iterator cend()const{ return const_iterator(*this, size()); };
		reverse_iterator rbegin(){ return reverse_iterator(*this, 0); };
		const_reverse_iterator rbegin()const{ return const_reverse_iterator(*this, 0); };
		const_reverse_iterator crbegin()const{ return const_reverse_iterator(*this, 0); };
		reverse_iterator rend(){ return reverse_iterator(*this, size()); };
		const_reverse_iterator rend()const{ return const_reverse_iterator(*this, size()); };
		const_reverse_iterator crend()const{ return const_reverse_iterator(*this, size()); };
		friend bool operator==(TypeExpression const& left, TypeExpression const& right){
			return left.source == right.source && left.temp == right.temp
				&& left.start == right.start && left.xend == right.xend;
		};
		friend bool operator!=(TypeExpression const& left, TypeExpression const& right){
			return left.source != right.source && left.temp != right.temp
				&& left.start != right.start && left.xend != right.xend;
		};
		friend std::ostream& operator<<(std::ostream& cout, TypeExpression const& h){
			return cout << (const char *)h;
		};
	};
	template<typename A, typename B>
	class TypeExpression<TypeExpression<A, B>, std::tuple<size_t, size_t, TypeString>>{
		TypeExpression<A, B> source;
		TypeString temp;
		size_t start, xend;
		bool _empty;
		mutable TypeString result;
	public:
		typedef ExpIter<TypeExpression> iterator;
		typedef const ExpIter<TypeExpression> const_iterator;
		typedef ReverseExpIter<TypeExpression> reverse_iterator;
		typedef const ReverseExpIter<TypeExpression> const_reverse_iterator;
		typedef char value_type;
		TypeExpression() :_empty(true){};
		TypeExpression(TypeExpression<A, B> source, TypeString temp,
			size_t start, size_t end) :source(source), temp(temp), _empty(false),
			start(start > source.size() ? source.size() : start),
			xend(start > source.size() ? source.size() : end){};
		TypeExpression(TypeExpression<A, B> source, size_t start, size_t end,
			TypeString temp) :source(source), temp(temp), _empty(false),
			start(start > source.size() ? source.size() : start),
			xend(start > source.size() ? source.size() : end){};
		size_t size()const{ return source.size() + temp.size() + start - xend; };
		bool empty()const{ return _empty; };
		char& operator[](size_t index){
			if (index < start) return source[index];
			if (index < start + temp.size()) return temp[index - start];
			return source[xend + index - start - temp.size()];
		};
		char const& operator[](size_t index)const{
			if (index < start) return source[index];
			if (index < start + temp.size()) return temp[index - start];
			return source[xend + index - start - temp.size()];
		};
		explicit operator TypeString()const{
			if (result->empty()){
				size_t Max = size();
				char* cache = new char[Max + 1];
				for (size_t i = 0; i != Max; ++i)
					cache[i] = operator[](i);
				cache[Max] = '\0';
				result = cache;
			};
			return result;
		};
		explicit operator const char*()const{
			return TypeString(*this).c_str();
		};
		const char * c_str()const{
			return TypeString(*this).c_str();
		};
		iterator begin(){ return iterator(*this, 0); };
		const_iterator begin()const{ return const_iterator(*this, 0); };
		const_iterator cbegin()const{ return const_iterator(*this, 0); };
		iterator end(){ return iterator(*this, size()); };
		const_iterator end()const{ return const_iterator(*this, size()); };
		const_iterator cend()const{ return const_iterator(*this, size()); };
		reverse_iterator rbegin(){ return reverse_iterator(*this, 0); };
		const_reverse_iterator rbegin()const{ return const_reverse_iterator(*this, 0); };
		const_reverse_iterator crbegin()const{ return const_reverse_iterator(*this, 0); };
		reverse_iterator rend(){ return reverse_iterator(*this, size()); };
		const_reverse_iterator rend()const{ return const_reverse_iterator(*this, size()); };
		const_reverse_iterator crend()const{ return const_reverse_iterator(*this, size()); };
		friend bool operator==(TypeExpression const& left, TypeExpression const& right){
			return left.source == right.source && left.temp == right.temp
				&& left.start == right.start && left.xend == right.xend;
		};
		friend bool operator!=(TypeExpression const& left, TypeExpression const& right){
			return left.source != right.source && left.temp != right.temp
				&& left.start != right.start && left.xend != right.xend;
		};
		friend std::ostream& operator<<(std::ostream& cout, TypeExpression const& h){
			return cout << (const char *)h;
		};
	};
	template<typename A, typename B>
	class TypeExpression<TypeString, std::tuple<size_t, size_t, TypeExpression<A, B>>>{
		TypeString source;
		TypeExpression<A, B> temp;
		size_t start, xend;
		bool _empty;
		mutable TypeString result;
	public:
		typedef ExpIter<TypeExpression> iterator;
		typedef const ExpIter<TypeExpression> const_iterator;
		typedef ReverseExpIter<TypeExpression> reverse_iterator;
		typedef const ReverseExpIter<TypeExpression> const_reverse_iterator;
		typedef char value_type;
		TypeExpression() :_empty(true){};
		TypeExpression(TypeString source, TypeExpression<A, B> temp,
			size_t start, size_t end) :source(source), temp(temp), _empty(false),
			start(start > source.size() ? source.size() : start),
			xend(start > source.size() ? source.size() : end){};
		TypeExpression(TypeString source, size_t start, size_t end,
			TypeExpression<A, B> temp) :source(source), temp(temp), _empty(false),
			start(start > source.size() ? source.size() : start),
			xend(start > source.size() ? source.size() : end){};
		size_t size()const{ return source.size() + temp.size() + start - xend; };
		bool empty()const{ return _empty; };
		char& operator[](size_t index){
			if (index < start) return source[index];
			if (index < start + temp.size()) return temp[index - start];
			return source[xend + index - start - temp.size()];
		};
		char const& operator[](size_t index)const{
			if (index < start) return source[index];
			if (index < start + temp.size()) return temp[index - start];
			return source[xend + index - start - temp.size()];
		};
		explicit operator TypeString()const{
			if (result->empty()){
				size_t Max = size();
				char* cache = new char[Max + 1];
				for (size_t i = 0; i != Max; ++i)
					cache[i] = operator[](i);
				cache[Max] = '\0';
				result = cache;
			};
			return result;
		};
		explicit operator const char*()const{
			return TypeString(*this).c_str();
		};
		const char * c_str()const{
			return TypeString(*this).c_str();
		};
		iterator begin(){ return iterator(*this, 0); };
		const_iterator begin()const{ return const_iterator(*this, 0); };
		const_iterator cbegin()const{ return const_iterator(*this, 0); };
		iterator end(){ return iterator(*this, size()); };
		const_iterator end()const{ return const_iterator(*this, size()); };
		const_iterator cend()const{ return const_iterator(*this, size()); };
		reverse_iterator rbegin(){ return reverse_iterator(*this, 0); };
		const_reverse_iterator rbegin()const{ return const_reverse_iterator(*this, 0); };
		const_reverse_iterator crbegin()const{ return const_reverse_iterator(*this, 0); };
		reverse_iterator rend(){ return reverse_iterator(*this, size()); };
		const_reverse_iterator rend()const{ return const_reverse_iterator(*this, size()); };
		const_reverse_iterator crend()const{ return const_reverse_iterator(*this, size()); };
		friend bool operator==(TypeExpression const& left, TypeExpression const& right){
			return left.source == right.source && left.temp == right.temp
				&& left.start == right.start && left.xend == right.xend;
		};
		friend bool operator!=(TypeExpression const& left, TypeExpression const& right){
			return left.source != right.source && left.temp != right.temp
				&& left.start != right.start && left.xend != right.xend;
		};
		friend std::ostream& operator<<(std::ostream& cout, TypeExpression const& h){
			return cout << (const char *)h;
		};
	};
	template<typename A, typename B, typename C, typename D>class TypeExpression
		<TypeExpression<A, B>, std::tuple<size_t, size_t, TypeExpression<C, D>>>{
		TypeExpression<A, B> source;
		TypeExpression<C, D> temp;
		size_t start, xend;
		bool _empty;
		mutable TypeString result;
	public:
		typedef ExpIter<TypeExpression> iterator;
		typedef const ExpIter<TypeExpression> const_iterator;
		typedef ReverseExpIter<TypeExpression> reverse_iterator;
		typedef const ReverseExpIter<TypeExpression> const_reverse_iterator;
		typedef char value_type;
		TypeExpression() :_empty(true){};
		TypeExpression(TypeExpression<A, B> source, TypeExpression<C, D> temp,
			size_t start, size_t end) :source(source), temp(temp), _empty(false),
			start(start > source.size() ? source.size() : start),
			xend(start > source.size() ? source.size() : end){};
		TypeExpression(TypeExpression<A, B> source, size_t start, size_t end,
			TypeExpression<C, D> temp) :source(source), temp(temp), _empty(false),
			start(start > source.size() ? source.size() : start),
			xend(start > source.size() ? source.size() : end){};
		size_t size()const{ return source.size() + temp.size() + start - xend; };
		bool empty()const{ return _empty; };
		char& operator[](size_t index){
			if (index < start) return source[index];
			if (index < start + temp.size()) return temp[index - start];
			return source[xend + index - start - temp.size()];
		};
		char const& operator[](size_t index)const{
			if (index < start) return source[index];
			if (index < start + temp.size()) return temp[index - start];
			return source[xend + index - start - temp.size()];
		};
		explicit operator TypeString()const{
			if (result->empty()){
				size_t Max = size();
				char* cache = new char[Max + 1];
				for (size_t i = 0; i != Max; ++i)
					cache[i] = operator[](i);
				cache[Max] = '\0';
				result = cache;
			};
			return result;
		};
		explicit operator const char*()const{
			return TypeString(*this).c_str();
		};
		const char * c_str()const{
			return TypeString(*this).c_str();
		};
		iterator begin(){ return iterator(*this, 0); };
		const_iterator begin()const{ return const_iterator(*this, 0); };
		const_iterator cbegin()const{ return const_iterator(*this, 0); };
		iterator end(){ return iterator(*this, size()); };
		const_iterator end()const{ return const_iterator(*this, size()); };
		const_iterator cend()const{ return const_iterator(*this, size()); };
		reverse_iterator rbegin(){ return reverse_iterator(*this, 0); };
		const_reverse_iterator rbegin()const{ return const_reverse_iterator(*this, 0); };
		const_reverse_iterator crbegin()const{ return const_reverse_iterator(*this, 0); };
		reverse_iterator rend(){ return reverse_iterator(*this, size()); };
		const_reverse_iterator rend()const{ return const_reverse_iterator(*this, size()); };
		const_reverse_iterator crend()const{ return const_reverse_iterator(*this, size()); };
		friend bool operator==(TypeExpression const& left, TypeExpression const& right){
			return left.source == right.source && left.temp == right.temp
				&& left.start == right.start && left.xend == right.xend;
		};
		friend bool operator!=(TypeExpression const& left, TypeExpression const& right){
			return left.source != right.source && left.temp != right.temp
				&& left.start != right.start && left.xend != right.xend;
		};
		friend std::ostream& operator<<(std::ostream& cout, TypeExpression const& h){
			return cout << (const char *)h;
		};
	};
	template<typename A = TypeString, typename B = TypeString>
	using Concat = TypeExpression<A, B>;
	template<typename A = TypeString>using Multiply = TypeExpression<A, size_t>;
	template<typename A = TypeString>using Slice = TypeExpression<A, std::pair<size_t, size_t>>;
	template<typename A = TypeString, typename B = TypeString>
	using Replace = TypeExpression<A, std::tuple<size_t, size_t, B>>;

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
	TypeExpression<TypeString, std::tuple<size_t, size_t, TypeString>> replace
		(TypeString source, TypeString temp, size_t start, size_t end){
		return TypeExpression<TypeString, std::tuple<size_t, size_t, TypeString>>
			(source, temp, start, end);
	};
	TypeExpression<TypeString, std::tuple<size_t, size_t, TypeString>> replace
		(TypeString source, size_t start, size_t end, TypeString temp){
		return TypeExpression<TypeString, std::tuple<size_t, size_t, TypeString>>
			(source, temp, start, end);
	};
	template<typename A, typename B>
	TypeExpression<TypeExpression<A, B>, std::tuple<size_t, size_t, TypeString>>
		replace(TypeExpression<A, B> source, TypeString temp, size_t start, size_t end){
			return TypeExpression<TypeExpression<A, B>,
				std::tuple<size_t, size_t, TypeString >> (source, temp, start, end);
		};
	template<typename A, typename B>
	TypeExpression<TypeExpression<A, B>, std::tuple<size_t, size_t, TypeString>> replace
		(TypeExpression<A, B> source, size_t start, size_t end, TypeString temp){
		return TypeExpression<TypeExpression<A, B>,
			std::tuple<size_t, size_t, TypeString >> (source, temp, start, end);
	};
	template<typename A, typename B>
	TypeExpression<TypeString, std::tuple<size_t, size_t, TypeExpression<A, B>>>replace
		(TypeString source, TypeExpression<A, B> temp, size_t start, size_t end){
		return TypeExpression<TypeString, std::tuple<size_t, size_t,
			TypeExpression<A, B >> >(source, temp, start, end);
	};
	template<typename A, typename B>
	TypeExpression<TypeString, std::tuple<size_t, size_t, TypeExpression<A, B>>> replace
		(TypeString source, size_t start, size_t end, TypeExpression<A, B> temp){
		return TypeExpression<TypeString,
			std::tuple<size_t, size_t, TypeExpression<A, B>>>(source, temp, start, end);
	};
};