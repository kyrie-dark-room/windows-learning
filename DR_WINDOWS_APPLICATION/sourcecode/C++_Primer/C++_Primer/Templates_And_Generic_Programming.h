#pragma once
/*
1.模板是C++中泛型编程的基础。一个模板就是一个创建类或函数的蓝图或者说公式。
2.函数模板
   1）函数模板定义：
	template <typename T>   // 模板类型参数T之前必须使用关键字class或typename，两个关键字在此没有什么不同
	int compare(const T &v1, const T &v2)
	{
		if (v1 < v2) return -1;
		if (v2 < v1) return 1;
		return 0;
	}
	模板定义以关键字template开始，后跟一个模板参数列表，这是一个逗号分隔的一个或多个模板参数的列表，
	用小于号（<）和大于号（>）包围起来。在模板定义中，模板参数列表不能为空。
   2）实例化函数模板
    当我们调用一个函数模板时，编译器（通常）用函数实参来为我们推断模板实参。编译器用推断出的模板参数
	来为我们实例化一个特定版本的函数。当编译器实例化一个模板时，它使用实际的模板实参代替对应的模板参数
	来创建出模板的一个新“实例”。
   3）非类型模板参数
    除了定义类型参数，还可以在模板中定义非类型参数。一个非类型参数表示一个值而非一个类型。我们通过一个
	特定的类型名而非关键字class或typename来指定非类型参数。
	当一个模板被实例化时，非类型参数被一个用户提供的或编译器推断出的值所代替。这些值必须是常量表达式，
	从而允许编译器在编译时实例化模板。
	template<unsigned N, unsigned M>
	int compare(const char (&p1)[N], const char(&p2)[M])
	{
		return strcmp(p1, p2);
	}
	当我们调用这个版本的compare时：
	  compare("hi", "mom")
	编译器会使用字面常量的大小来代替N和M，从而实例化模板。记住，编译器会在一个字符串字面常量的末尾插入
	一个空字符作为终结符，因此编译器会实例化出如下版本：
	  int compare(const char (&p1)[3], const char (&p2)[4])
	  一个非类型参数可以是一个整形，或者是一个指向对象或函数类型的指针或（左值）引用。绑定到非类型整形参数
	  的实参必须是一个常量表达式。绑定到指针或引用非类型参数的实参必须具有静态的生存期。我们不能用一个普通
	  （非static）局部变量或动态对象作为指针或引用非类型模板参数的实参。指针参数也可以用nullptr或一个值为
	  0的常量表达式来实例化。
   4）函数模板可以声明为inline或constexpr的，如同非模板函数一样。inline或constexpr说明符放在模板参数列表
      之后，返回类型之前：
	  template <typename T> inline T min(const T&, const T&);
   5）我们最初的compare函数虽然简单，但它说明了编写泛型代码的两个重要原则：
      ·模板中的函数参数都是const的引用。
	  ·函数体中的条件判断仅使用<比较运算。
	  模板程序应该尽量减少对实参类型的要求。
   6）通常，当我们调用一个函数时，编译器只需要掌握函数的声明。类似的，当我们使用一个类类型的对象时，
      类定义必须是可用的，但成员函数的定义不必已经出现。因此，我们将类定义和函数声明放在头文件中，而
	  普通函数和类的成员函数的定义放在源文件中。
	  模板则不同：为了生成一个实例化版本，编译器需要掌握函数模板或类模板成员函数的定义。因此，与非模板
	  代码不同，模板的头文件通常既包括声明也包括定义。
   7）大多数编译错误在实例化期间报告
      这一特性影响了我们何时才会获知模板内代码的编译错误。通常，编译器会在三个阶段报告错误。
	    第一个阶段是编译模板本身时。在这个阶段，编译器通常不会发现很多错误。编译器可以检查语法错误，例如
	  忘记分号或者变量名拼错等，但也就这么多了。
	    第二个阶段是编译器遇到模板使用时。在此阶段，编译器仍然没有很多可检查。对于函数模板调用，编译器
	  通常会检查实参数目是否正确。它还能检查参数类型是否匹配。对于类模板，编译器可以检查用户是否提供了
	  正确数目的目标实参，但也仅限于此了。
	    第三个阶段是模板实例化时，只有这个阶段才能发现类型相关的错误。依赖于编译器如何管理实例化，这类
	  错误可能在链接是才报告。
3.类模板
   1）类模板是用来生成类的蓝图的。与函数模板的不同之处是，编译器不能为类模板推断模板参数类型。为了使用
     类模板，我们必须在模板名后的尖括号中提供额外信息--用来代替模板参数的模板实参列表。
	 template <typename T> class Blob {
	 public:
	     typedef T value_type;
		 typedef typename std::vecctor<T>::size_type size_type;
		 // 构造函数
		 Blob();
		 Blob(std::initializer_list<T> il);
		 // Blob中的元素数目
		 size_type size() const { return data->size(); }
		 bool empty() const { return data->empty(); }
		 // 添加和删除元素
		 void push_back(const T &t){ data->push_back(t); }
		 // 移动版本
		 void push_back(T &&t){ data->push_back(std::move(t)); }
		 void pop_back();
		 // 元素访问
		 T& back();
		 T& operator[](size_type i); 
	 private:
	     std::shared_pter<std::vector<T>> data;
		 // 若data[i]无效，则抛出msg
		 void check(size_type i, const std::string &msg) const;
	 };
   2）我们已经多次见到，当使用一个类模板时，我们必须提供额外信息。我们现在知道这些额外信息是显示模板实参
      列表，它们被绑定到模板参数。编译器使用这些模板实参来实例化出特定的类。
	  例如，为了用我们的Blob模板定义一个类型，必须提供元素类型：
	  Blob<int> ia;					// 空Blob<int>
	  Blob<int> ia2 = {0,1,2,3,4};  // 有5个元素的Blob<int>
	  从这两个定义，编译器会实例化出一个与下面定义等价的类：
	  template <> class Blob<int> {
		typedef typename std::vector<int>::size_type size_type;
		Blob();
		Blob(std::initializer_list<int> il);
		// ...
		int& operator[](size_type i);
		private:
			std::shared_ptr<std::vector<int>> data;
			void check(size_type i, const std::string &msg) const;
	  };
   3）在模板作用域中引用模板类型：可能令人迷惑的是，一个类模板中的代码如果使用了另外一个模板，通常
      不将一个实际类型（或值）的名字用作其模板实参。相反的，我们通常将模板自己的参数当做被使用模板的实参。
   4）类模板的成员函数
        与其他任何类相同，我们既可以在类模板内部，也可以在类模板外部为其定义成员函数，且定义在类模板内
	  的成员函数被隐式声明为内联函数。
	    类模板在成员函数本身是一个普通函数。但是，类模板的每个实例都有其自己版本的成员函数。因此，类模板
	  的成员函数具有和模板相同的模板参数。因而，定义在类模板之外的成员函数就必须以关键字template开始，后
	  接类模板参数列表。
	    与往常一样，当我们在类外定义一个成员时，必须说明成员属于哪个类。而且，从一个模板生成的类的名字中必须
	  包含其模板实参。当我们定义一个成员函数时，模板实参与模板形参相同。
   5）如果一个成员函数没有被使用，则它不会被实例化。成员函数只有在被用到时才进行实例化，这一特性使得
      即使某种类型不能完全符合模板操作的要求，我们仍然能用该类型实例化类。
   6）当我们使用一个类模板类型时必须提供模板实参，但这一规则有一个例外。在类模板自己的作用域中，我们可以
      直接使用模板名而不提供实参。
   7）类模板和友元
      当一个类包含一个友元声明时，类与友元各自是否是模板是相互无关的。如果一个类模板包含一个非模板友元，
	  则友元被授权可以访问所有模板实例。如果友元自身是模板，类可以授权给所有友元模板实例，也可以只授权
	  给特定实例。
	  示例：
	  // 前置声明，在将模板的一个特定实例声明为友元时要用到
	  template <typename T> class Pal;
	  class C{ // C是一个普通的非模板类
		friend class Pal<C>; // 用类C实例化的Pal是C的一个友元
		// Pal2的所有实例都是C的友元；这种情况无需前置声明
		template <typename T> friend class Pal2;
	  };
	  template <typename T> class C2 { // C2本身是一个类模板
		// C2的每个实例将相同实例化的Pal声明为友元
		friend class Pal<T>; // Pal的模板声明必须在作用域内
		// Pal2的所有实例都是C2的每个实例的友元，不需要前置声明
		template <typename X> friend class Pal2;
		// Pal3是一个非模板类，它是C2所有实例的友元
		friend class Pal3; // 不需要Pal3的前置声明
	  };
   8）在新标准中，我们可以将模板类型参数声明为友元：
	  template <typename Type> class Bar{
		friend Type; // 将访问权限授予用来实例化Bar的类型
		// ...
	  };
   9）新标准允许我们为类模板定义一个类型别名：
	  template<typename T> using twin = pair<T, T>;
	  twin<string> authors; // authors是一个pair<string, string>
	  当我们定义一个模板类类型别名时，可以固定一个或多个模板参数：
	  template <typename T> using partNo = pair<T, unsigned>;
	  partNo<string> books; // books是一个pair<string, unsigned>
	  partNo<Vehicle> cars; // cars是一个pair<Vehicle, unsigned>
   10）类似任何其他成员函数，一个static成员函数只有在使用时才会实例化。
4.模板参数
   1）类似函数参数的名字，一个模板参数的名字也没有什么内在含义。我们通常将类型参数命名为T，但实际上
      我们可以使用任何名字。
   2）模板参数遵循普通的作用域规则。一个模板参数名的可用范围是在其声明之后，至模板声明或定义结束之前。
   3）模板声明必须包括模板参数，与函数参数相同，声明中的模板参数的名字不必与定义中相同。
   4）默认情况下，C++语言假定通过作用域运算符访问的名字不是类型。因此，如果我们希望使用一个模板类型参数
      的类型成员，就必须显式告诉编译器改名字是一个类型。我们通过使用关键字typename来实现这一点。当我们
	  希望通知编译器一个名字表示类型时，必须使用关键字typename，而不能使用class。
   5）在新标准中，我们可以为函数和类模板提供默认实参。
      示例：
	  // compare有一个默认模板实参less<T>和一个默认函数实参F()
	  template <typename T, typename F = less<T>>
	  int compare(const T &v1, const T &v2, F f = F())
	  {
		if (f(v1, v2)) return -1;
		if (f(v2, v1)) return 1;
		return 0;
	  }
	  与函数默认实参一样，对于一个模板参数，只有当它右侧的所有参数都有默认实参时，它才可以有默认实参。
   6）无论何时使用一个类模板，我们都必须在模板名之后接上尖括号。尖括号指出类必须从一个模板实例化而来。
      特别的，如果一个类模板为其所有模板参数都提供了默认实参，且我们希望使用这些默认实参，就必须在模板
	  名之后跟一个空尖括号对：
	  template <class T = int> class Numbers{
		public:
			Numbers(T v = 0):val(v){}
			// 对数值的各种操作
		private:
			T val;
	  };
	  Numbers<long double> lots_of_precision;
	  Numbers<> average_precision;
   7）一个类（无论是普通类还是类模板）可以包含本身是模板的成员函数。这种成员被称为成员模板。成员模板
      不能是虚函数。
   8）与类模板的普通函数成员不同，成员模板是函数模板。当我们在类模板外定义一个成员模板时，必须同时
      为类模板和成员模板提供模板参数列表。类模板的参数列表在前，后跟成员自己的模板参数列表：
	  template <typename T>  // 类的类型参数
	  template <typename It>  // 构造函数的类型参数
	  Blob<T>::Blob(It b, It e):data(std::make_shared<std::vector<T>>(b, e)){ }
	  为了实例化一个类模板的成员模板，我们必须同时提供类和函数模板的实参。
   9）控制实例化，在大系统中，在多个文件中实例化相同模板的额外开销可能非常严重。在新标准中，我们可以
      通过显示实例化来避免这种开销。一个显示实例化有如下形式：
	  extern template declaration;   // 实例化声明
	  template declaration;          // 实例化定义
	  declaration是一个类或函数声明，其中所有模板参数已被替换为模板参数。例如，
	  extern template class Blob<string>;
	  template int compare(const int&, const int&);
   10）一个类模板的实例化定义会实例化该模板的所有成员，包括内联的成员函数。因此，我们用来显示实例化一个
      类模板的类型，必须能用于模板的所有成员。
   11）通过在编译时绑定删除器，unique_ptr避免了间接调用删除器的运行时开销。通过在运行时绑定删除器，
      shared_pter使用户重载删除器更为方便。
*/