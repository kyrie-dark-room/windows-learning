#pragma once
/*
1.tuple类型
	1）不同tuple类型的成员类型也不相同，但一个tuple可以有任意数量的成员。每个确定的tuple类型的成员数目是固定的，但一个tuple类型的成员数目可以与另一个tuple类型不同。
	  当我们希望将一些数据组合成单一对象，但又不想麻烦地定义一个新数据结构来表示这些数据时，tuple是非常有用的。tuple类型及其伴随类型和函数定义在tuple头文件中。
	2）-------------------------------------------------------------------------------------------------------------------------------------------------------
																tuple支持的操作
	   -------------------------------------------------------------------------------------------------------------------------------------------------------
	     tuple<T1, T2, ..., Tn> t;						t是一个tuple，成员数为n，第i个成员的类型为Ti。所有成员都进行值初始化
		 tuple<T1, T2, ..., Tn> t(v1, v2, ..., vn);     t是一个tuple，成员类型为T1...Tn，每个成员用对应初始值vi进行初始化。此构造函数是explicit的
		 make_tuple(v1, v2, ..., vn)					返回一个用给定初始值初始化的tuple。tuple的类型从初始值的类型推断
		 t1 == t2										当两个tuple具有相同数量的成员且成员对应相等时，两个tuple相等。这两个操作使用成员的==运算符
		 t1 != t2										来完成。一旦发现某对成员不等，接下来的成员就不用比较了。
		 t1 relop t2									tuple的关系运算使用字典序。两个tuple必须具有相同数量的成员。使用<运算符比较t1的成员和t2的对应
														成员。
		 get<i>(t)										返回t的第i个数据成员的引用；如果t是一个左值，结果是一个左值引用；否则，结果是一个右值引用。
														tuple的所有成员都是public的。
		 tuple_size<tupleType>::value					一个类模板，可以通过一个tuple类型来初始化。它有一个名为value的public constexpr static数据成员
														，类型为size_t，表示给定tuple类型中成员的数量。
         tupel_element<i, tupleType>::type				一个类模板，可以通过一个整形变量和一个tuple类型来初始化。它有一个名为type的public成员，表示
														给定tuple类型中指定成员的类型。
	   -------------------------------------------------------------------------------------------------------------------------------------------------------
	3）定义和初始化tuple
	  当我们定义一个tuple时，需要指出每个成员的类型：
	  tuple<size_t, size_t, size_t> threeD;    // 三个成员都设置为0
	  tuple<>string, vector<double>, int, list<int>> someVal("constants", {3.14, 2.718}, 42, {0,1,2,3,4,5});
	  当我们创建一个tuple对象时，可以使用tuple的默认构造函数，它会对每个成员进行值初始化；也可以像本例中初始化someVal一样，为每个成员提供一个初始值。tuple
	  的这个构造函数是explicit的，因此我们必须使用直接初始化语法。
	  类似make_pair函数，标准库定义了make_tuple函数，我们还可以用它来生成tuple对象：
	  auto item = make_tuple("0-999-78345-X", 3, 20.00);
	4）访问tuple的成员
	  要访问一个tuple的成员，就要使用一个名为get的标准库函数模板。为了使用get，我们必须指定一个显式模板实参，它指出我们想要访问第几个成员。我们传递给get
	  一个tuple对象，它返回指定成员的引用：
	    auto book = get<0>(item);    // 返回item的第一个成员
	  如果不知道一个tuple准确的类型细节信息，可以用两个辅助类模板来查询tuple成员的数量和类型：
	    typedef decltype(item) trans;  // trans是item的类型
		// 返回trans类型对象中成员的数量
		size_t sz = tuple_size<trans>::value;  // 返回3
		// cnt的类型与item中第二个成员相同
		tuple_element<1, trans>::type cnt = get<1>(item); // cnt是一个int
	5）tuple的一个常见用途是从一个函数返回多个值。
2.bitset 类型
	1）标准库还定义了bitset类，使得位运算的使用更为容易，并且能够处理超过最长整形类型大小的为集合。bitset类定义在头文件bitset中。
	2）bitset类是一个类模板，具有固定的大小。当我们定义一个bitset时，需要声明它包含多少个二进制位：
	  bitset<32> bitvec(1U);  // 32位；低位为1，其他位为0
	  大小必须为一个常量表达式。bitset中的二进制位也是未命名的，我们通过位置来访问它们。二进制位的位置是从0开始编号的。编号从0开始的二进制位被称为低位(low-order)，编号到31结束的二进制位被称为高位(high-order)。
	3）初始化bitset的方法
	  ------------------------------------------------------------------------------------------------------------------------------------------------------
																初始化bitset的方法
	  ------------------------------------------------------------------------------------------------------------------------------------------------------
	    bitset<n> b;									b有n位；每一位均为0。此构造函数是一个constexpr
		bitset<n> b(u);									b是一个unsigned long long值u的低n位的拷贝。如果n大于unsigned long long的大小，则b中超过unsigned long
														long 的高位被置位0。此构造函数是一个constexpr
		bitset<n> b(s, pos, m, zero, one);				b是string s从位置pos开始m个字符的拷贝。s只能包含字符zero或one; 如果s包含任何其他字符，构造函数会抛出
														invalid_argument异常。字符在b中分别保存为zero和One。pos默认为0，m默认为string::npos，zero默认为'0',
														one默认为'1'
		bitset<n> b(cp, pos, m, zero, one);				与上一个构造函数相同，但从cp指向的字符数组中拷贝字符。如果未提供m，则cp必须指向一个C风格字符串。如果
														提供了m，则从cp开始必须至少有m个zero或one字符
	  接受一个string或一个字符指针的构造函数是explicit的。在新标准中增加了为0和1指定其他字符的功能。
	  ------------------------------------------------------------------------------------------------------------------------------------------------------
	4）当我们使用一个整形值来初始化bitset时，此值将被转换为unsigned long long类型并被当作位模式来处理。
	5）string的下标编号习惯与bitset恰好相反：string中下标最大的字符（最右字符）用来初始化bitset中的低位（下标为0的二进制位）。当你用一个string初始化一个bitset
	  时，要记住这个差别。
	6）bit操作
	  bitset操作定义了多种检测或设置一个或多个二进制位的方法。bitset类还支持位运算符。这些运算符用于bitset对象的含义与内置运算符用于unsigned运算对象相同。
	  ------------------------------------------------------------------------------------------------------------------------------------------------------
																bitset操作
	  ------------------------------------------------------------------------------------------------------------------------------------------------------
	    b.any()									b中是否存在置位的二进制位
		b.all()									b中所有位都置位了吗
		b.none()								b中不存在置位的二进制位吗
		b.count()								b中置位的位数
		b.size()								一个constexpr函数，返回b中的位数
		b.test(pos)								若pos位置的位是置位的，则返回true，否则返回false
		b.set(pos, v)							将位置pos处的位设置为bool值v。v默认为true。
		b.set()									如果未传递实参，则将b中所有位置位
		b.reset(pos)							将位置pos处的位复位或将b中的所有位复位
		b.reset()
		b.flip(pos)								改变位置pos处的位的状态或改变b中每一位的状态
		b.flip()
		b[pos]									访问b中位置pos处的位；如果b是const的，则当该位置位时b[pos]返回一个bool值true，否则返回false
		b.to_ulong()							返回一个unsigned long或一个unsigned long long值，其位模式与b相同。如果b中位模式不能放入指定的结果类型，
		b.to_ullong()							则抛出一个overflow_error异常
		b.to_string(zero, one)					返回一个string，表示b中的位模式。zero和one的默认为分别是0和1，用来表示b中的0和1
		os << b									将b中二进制位打印为字符1和0，打印到流os
		is >> b									从is读取字符存入b。当下一个字符不是1或0时，或是已经读入b.size()个位时，读取过程停止。
	  ------------------------------------------------------------------------------------------------------------------------------------------------------
3.正则表达式
	1）正则表达式（regular expression）是一种描述字符序列的方法，是一种极其强大的计算工具。C++正则表达式库（RE库），它是新标准的一部分。RE库定义在头文件regex中，它包含多个组件。
	  ------------------------------------------------------------------------------------------------------------------------------------------------------
																正则表达式库组件
	  ------------------------------------------------------------------------------------------------------------------------------------------------------
	    regex							表示有一个正则表达式的类
		regex_match						将一个字符序列与一个正则表达式匹配
		regex_search					寻找第一个与正则表达式匹配的子序列
		regex_replace					使用给定格式替换一个正则表达式
		sregex_iterator					迭代器适配器，调用regex_search来遍历一个string中所有匹配的子串
		smatch							容器类，保存在string中搜索的结果
		ssub_match						string中匹配的子表达式的结果
	  ------------------------------------------------------------------------------------------------------------------------------------------------------

	  列出regex的函数的参数。这些函数都返回bool值，且都被重载了：其中一个版本接受一个类型为smatch的附加参数。如果匹配成功，这些函数将成功匹配的相关信息保存在
	  给定的smatch对象中。
	  ------------------------------------------------------------------------------------------------------------------------------------------------------
																regex_search和regex_match的参数
	  ------------------------------------------------------------------------------------------------------------------------------------------------------
	  注意：这些操作返回bool值，指出是否找到匹配。
		(seq, m, r, mft)	在字符序列seq中查找regex对象r中的正则表达式。seq可以是一个string、表示范围的一对迭代器以及一个指向空字符串结尾的字符数组的指针
		(seq, r, mft)		m是一个match对象，用来保存匹配结果的相关细节。m和seq必须具有兼容的类型
							mft是一个可选的regex_constants::match_flag_type值。这些值会影响匹配过程
	  ------------------------------------------------------------------------------------------------------------------------------------------------------
*/