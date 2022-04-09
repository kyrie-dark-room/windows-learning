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
	2）默认情况下，regex使用的正则表达式语言是ECMAScript。在ECMAScript中，模式[[::alpha:]]匹配任意字母，符号+和*分别表示我们希望“一个或多个”或“零个或多个”匹配。
	3）当我们定义一个regex或是对一个regex调用assign为其赋予新值时，可以指定一些标志来影响regex如何操作。这些标志控制regex对象的处理过程。下表列出的最后6个标志指出
	  编写正则表达式所用的语言。对这6个标志，我们必须设置其中之一，且只能设置一个。默认情况下，ECMAScript标志被设置，从而regex会使用ECMA-262规范，这也是很多Web浏览器所使用的正则表达式语言。
	  ------------------------------------------------------------------------------------------------------------------------------------------------------
																	regex(和wregex)选项
	  ------------------------------------------------------------------------------------------------------------------------------------------------------
			regex r(re)				re表示一个正则表达式，它可以是一个string、一个表示字符范围的迭代器对、一个指向空字符结尾的字符数组的指针、一个字符指针
			regex r(re, f)			和一个计数器或是一个花括号包围的字符列表。f是指出对象如何处理的标志。f通过下面列出的值来设置。如果未指定f，其默认值为
									ECMAScript
			
			r1 = re					将r1中的正则表达式替换为re。re表示一个正则表达式，它可以是另一个regex对象、一个string、一个指向空字符结尾的字符数组的指针
									或是一个花括号包围的字符列表

			r1.assign(re, f)		与使用赋值运算符（=）效果相同；可选的标志f也与regex的构造函数中对应的参数含义相同
			r.mark_count()			r中子表达式的数目
			r.flags()				返回r的标志集

		注：构造函数和赋值操作可能抛出类型为regex_error的异常。
																	定义regex时指定的标志
			定义在regex和regex_constants::syntax_option_type中
				icase				在匹配过程中忽略大小写
				nosubs				不保存匹配的子表达式
				optimize			执行速度优先于构造速度
				ECMAScript			使用ECMA-262指定的语法
				basic				使用POSIX基本的正则表达式语法
				extended			使用POSIX扩展的正则表达式语法
				awk					使用POSIX版本的awk语言的语法
				grep				使用POSIX版本的grep的语法
				egrep				使用POSIX版本的egrep的语法
	  ------------------------------------------------------------------------------------------------------------------------------------------------------
	4）需要意识到的非常重要的一点是，一个正则表达式的语法是否正确是在运行时解析的。如果我们编写的正则表达式存在错误，则在运行时标准库会抛出一个类型为regex_error
	  的异常。类似标准异常类型，regex_error有一个what操作来表述发生了什么错误。regex_error还有一个名为code的成员，用来返回某个错误类型对应的数值编码。code返回的值
	  是由具体实现定义的。RE库能抛出的标准错误如下表所示：
	  ------------------------------------------------------------------------------------------------------------------------------------------------------
															正则表达式错误类型
	  ------------------------------------------------------------------------------------------------------------------------------------------------------
			定义在regex和regex_constants::error_type中
			error_collate					无效的元素校对请求
			error_ctype						无效的字符类
			error_escape					无效的转义字符或无效的尾置转义
			error_backref					无效的向后引用
			error_brack						不匹配的方括号（[或]）
			error_paren						不匹配的小括号（（或））
			error_brace						不匹配的花括号（{或}）
			error_badbrace					{}中无效的范围						
			error_range						无效的字符范围（如[z-a]）
			error_space						内存不足，无法处理此正则表达式
			error_badrepeat					重复字符（*、？、+或{）之前没有有效的正则表达式
			error_complexity				要求的匹配过于复杂
			error_stack						栈空间不足，无法处理匹配
	  ------------------------------------------------------------------------------------------------------------------------------------------------------
	5）建议：避免创建不必要的正则表达式
		如我们所见，一个正则表达式所表示的“程序”是在运行时而非编译时编译的。正则表达式的编译是一个非常慢的操作，特别是在你使用了扩展的正则表达式语法或是复杂的正则表达式时。
	  因此，构造一个regex对象以及向一个已存在的regex赋予一个新的正则表达式可能是非常耗时的。为了最小化这种开销，你应该努力避免创建很多不必要的regex。特别是，如果你在一个循环
	  中使用正则表达式，应该在循环外创建它，而不是在每步迭代时都编译它。
	6）正则表达式类和输入序列类型
	  我们可以搜索多种类型的输入序列。输入可以是普通char数据或wchar_t数据，字符可以保存在标准库string中或是char数组中（或是宽字符版本，wstring或wchar_t数组中）。RE为这些不同
	  的输入序列类型都定义了对应的类型。
	  ------------------------------------------------------------------------------------------------------------------------------------------------------
																	正则表达式库类
	  ------------------------------------------------------------------------------------------------------------------------------------------------------
		如果输入序列类型					则使用正则表达式类
		string								regex、smatch、ssub_match和sregex_iterator
		const char*							regex、cmatch、csub_match和cregex_iterator
		wstring								wregex、wsmatch、wssub_match和wsregex_iterator
		const wchar_t*						wregex、wcmatch、wcsub_match和wcregex_iterator
	  ------------------------------------------------------------------------------------------------------------------------------------------------------
	7）匹配与Regex迭代器类型
	  ------------------------------------------------------------------------------------------------------------------------------------------------------
																sregex_iterator
	  ------------------------------------------------------------------------------------------------------------------------------------------------------
	    这些操作也适用于cregex_iterator、wsregex_iterator和wcregex_iterator
		  sregex_iterator			一个sregex_iterator，遍历迭代器b和e表示的string。
		  it(b, e, r);				它调用sregex_search(b, e, r)将it定位到输入中第一个匹配的位置
		  sregex_iterator			sregex_iterator的尾后迭代器
		  end;
		  *it						根据最后一个调用regex_search的结果，返回一个smatch对象的引用或一个指向smatch对象的指针
		  it->						
		  ++it						从输入序列当前匹配位置开始调用regex_search。前置版本返回递增后迭代器；后置版本返回旧值
		  it++
		  it1 == it2				如果两个sregex_iterator都是尾后迭代器，则它们相等两个非尾后迭代器是从相同的输入序列和regex对象构造，则它们相等
		  it1 != it2
	  ------------------------------------------------------------------------------------------------------------------------------------------------------
	      当我们将一个sregex_iterator绑定到一个string和一个regex对象时，迭代器自动定位到给定string中第一个匹配位置。即，sregex_iterator构造函数对给定string和
		regex调用regex_search。当我们解引用迭代器时，会得到一个对应最近一次搜索结果的smatch对象。当我们递增迭代器时，它调用regex_search在输入string中查找下一个匹配。
	8）------------------------------------------------------------------------------------------------------------------------------------------------------
																		smatch操作
	   ------------------------------------------------------------------------------------------------------------------------------------------------------
	     这些操作也适用于cmatch、wsmatch、wcmatch和对应的csub_match、wssub_match和wcsub_match。
		 m.ready()				如果已经通过调用regex_search或regex_match设置了m，则返回true；否则返回false。如果ready返回false，则对m进行操作是未定义的
		 m.size()				如果匹配失败，则返回0；否则返回最近一次匹配的正则表达式中子表达式的数目。
		 m.empty()				若m.size()为0，则返回true
		 m.prefix()				一个ssub_match对象，表示当前匹配之前的序列
		 m.suffix()				一个ssub_match对象，表示当前匹配之后的部分
		 m.format(...)			

		 在接受一个索引的操作中，n的默认值为0且必须小于m.size()。
		 第一个子匹配（索引为0）表示整个匹配
		 m.length(n)			第n个匹配的子表达式的大小
		 m.position(n)			第n个子表达式距序列开始的距离
		 m.str(n)				第n个子表达式匹配的string
		 m[n]					对应第n个子表达式的ssub_match对象
		 m.begin(), m.end()		表示m中sub_match元素范围的迭代器。与往常一样，cbegin和
		 m.cbegin(),m.cend()	cend返回const_iterator
	   ------------------------------------------------------------------------------------------------------------------------------------------------------
	9）使用子表达式
	     正则表达式中的模式通常包含一个或多个子表达式（subexpression）。一个子表达式是模式的一部分，本身也具有意义。正则表达式语法通常用括号表示子表达式。
	     匹配对象除了提供匹配整体的相关信息外，还提供访问模式中每个子表达式的能力。子匹配是按位置来访问的。第一个子匹配位置为0，表示整个模式对应的匹配，随后是
	   每个子表达式对应的匹配。因此，本例模式中第一个子表达式，即表示文件名的子表达式，其位置为1，而文件扩展名对应的子表达式位置为2。
	     介绍一下ECMAScript正则表达式语言的一些特性：
		 ·\{d}表示单个数字而\{d}{n}则表示一个n个数字的序列。（如，\{d}{3}匹配三个数字的序列。）
		 ·在方括号中的字符集合表示匹配这些字符中任意一个。（如，[-. ]匹配一个短横线或一个点或一个空格。注意，点在括号中没有特殊含义。）
		 ·后接'?'的组件是可选的。（如，\{d}{3}[-. ]?\{d}{4}匹配这样的序列：开始是三个数字，后接一个可选的短横线或点或空格，然后是四个数字。此模式可以匹配555-0132或
		   555.0132或555 0132或5550132。）
		 ·类似C++，ECMAScript使用反斜线表示一个字符本身而不是其特殊含义。由于我们的模式包含括号，而括号是ECMAScript中特殊字符，因此我们必须用\(和\)来表示括号是我们
		   模式的一部分而不是特殊字符。
	10）------------------------------------------------------------------------------------------------------------------------------------------------------
																			子匹配操作
		------------------------------------------------------------------------------------------------------------------------------------------------------
		  注意：这些操作适用于ssub_match、csub_match、wssub_match、wcsub_match。
		    matched				一个public bool 数据成员，指出此ssub_match是否匹配了
			first				public数据成员，指出匹配序列首元素和尾后位置的迭代器。
			second				如果未匹配，则first和second是相等的
			length()			匹配的大小。如果matched为false，则返回0
			str()				返回一个包含输入中匹配部分的string。如果matched为false，则返回空string
			s = ssub			将ssub_match对象ssub转化为string对象s。等价于s = ssub.str()。转换运算符不是explicit的。
		------------------------------------------------------------------------------------------------------------------------------------------------------
	11）实例：
		  下面的代码读取一个文件，并用此模式查找与完整的电话号码模式匹配的数据。它会调用一个名为valid的函数来检查号码格式是否合法：
		  string phone = "(\\()?(\\d{3})(\\))?([-. ])?(\\d{3})([-. ]?)(\\d{4})";
		  regex r(phone); // regex对象，用于查找我们的模式
		  smatch m;
		  string s;
		  // 从输入文件中读取每条记录
		  while (getlien(cin, s)){
			// 对每个匹配的电话号码
			for (sregex_iterator it(s.begin(), s.end(), r), end_it; it != end_it; ++it)
			{
				// 检查号码的格式是否合法
				if (valid(*it))
					cout << "Valid: " << it->str() << endl;
				else
					cout << "not valid: " << it->str() << endl;
			}
		  }

		  bool valid(const smatch &m)
		  {
			  // 如果区号前有一个左括号
			  if (m[1].matched)
				  // 则区号后必须有一个右括号，之后紧跟剩余号码或一个空格
				  return m[3].matched && (m[4].matched == 0 || m[4].str() == " ");
			  else
			      // 否则，区号后不能有右括号
				  // 另两个组成部分间的分隔符必须匹配
				  return !m[3].matched && m[4].str() == m[6].str();
		  }
	12）当我们希望在输入序列中查找并替换一个正则表达式时，可以调用regex_replace。类似搜索函数，它接受一个输入字符序列和一个regex对象
	  ，不同的是，它还接受一个描述我们想要的输出形式的字符串。
	    ------------------------------------------------------------------------------------------------------------------------------------------------------
															正则表达式替换操作
		------------------------------------------------------------------------------------------------------------------------------------------------------
		  m.format(dest, fmt, mft)					使用格式字符串fmt生成格式化输出，匹配在m中，可选的match_flag_type标志在mft中。第一个版本写入迭代器dest指向
		  m.format(fmt, mft)						的目的位置并接受fmt参数，可以是一个string，也可以是表示字符数组中范围的一对指针。第二个版本返回一个string，
													保存输出，并接受fmt参数，可以是一个string，也可以是一个指向空字符结尾的字符数组的指针。mft的默认值为format
										            _default

		  regex_replace(dest, seq, r, fmt, mft)		遍历seq，用regex_search查找与regex对象r匹配的子串。使用格式字符串fmt和可选的match_flag_type标志来生成输出。
		  regex_replace(seq, r, fmt, mft)			第一个版本将输出写入到迭代器dest指定的位置，并接受一对迭代器seq表示范围。第二个版本返回一个string，保存输出，
													且seq既可以是一个string也可以是一个指向空字符结尾的字符数组的指针。在所有情况下，fmt既可以是一个string也可以
													是一个指向空字符结尾的字符数组的指针，且mft的默认值为match_default
		------------------------------------------------------------------------------------------------------------------------------------------------------
	13）就像标准库定义标志来指导如何处理正则表达式一样，标准库还定义了用来在替换过程中控制匹配或格式的标志。这些标志可以传递给函数regex_search或regex_match或
	   是类smatch的format成员。
	    匹配和格式化标志的类型为match_flag_type。这些值都定义在名为regex_constants的命名空间中。
	    ------------------------------------------------------------------------------------------------------------------------------------------------------
																	匹配标志
		------------------------------------------------------------------------------------------------------------------------------------------------------
		  定义在regex_constants::match_flag_type中
		  match_default					等价于format_default
		  match_not_bol					不将首字符作为首行处理
		  match_not_eol					不将尾字符作为行尾处理
		  match_not_bow					不将首字符作为单词首处理
		  match_not_eow					不将尾字符作为单词尾处理
		  match_any						如果存在多于一个匹配，则可返回任意一个匹配
		  match_not_null				不匹配任何空序列
		  match_continuous				匹配必须从输入的首字符开始
		  match_prev_avail				输入序列包含第一个匹配之前的内容
		  format_default				用ECMAScript规则替换字符串
		  format_sed					用POSIX sed规则替换字符串
		  format_no_copy				不输出输入序列中未匹配的部分
		  format_first_only				只替换子表达式的第一次出现
		------------------------------------------------------------------------------------------------------------------------------------------------------
4.随机数
	1）程序通常需要一个随机数源。在新标准出现之前，C和C++都依赖于一个简单的C库函数rand来生成随机数。此函数生成均匀分布的伪随机数，每个随机数的范围在0和一个系统相关的最大值（至少为32767）之间。
	   rand函数有一些问题：即使不是大多数，也有很多程序需要不同范围的随机数。一些应用需要随机浮点数。一些程序需要非均匀分布的数。而程序员为了解决这些问题而试图转换rand生成的随机数的范围、类型
	   或分布时，常常会引入非随机性。
	2）定义在头文件random中的随机数库通过一组协作的类来解决这些问题：随机数引擎类（random-number engines）和随机数分布类（random-number distribution）。一个引擎类可以生成unsigned随机数序列，
	   一个分布类使用一个引擎类生成指定类型的、在给定范围内的、服从特定概率分布的随机数。
	   ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
																					随机数库的组成
	   ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			引擎								类型，生成随机unsigned整数序列
			分布								类型，使用引擎返回服从特定概率分布的随机数
	   ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	   C++程序不应该使用库函数rand，而应该使用default_random_engine类和恰当的分布类对象。
	3）随机数引擎是函数对象类，它们定义了一个调用运算符，该运算符不接受参数返回一个随机unsigned整数。
		example：
		default_random_engine e;	// 生成随机无符号数
		for (size_t i = 0; i < 10; ++i)
		{
			// e() “调用”对象来生成下一个随机数
			cout << e() << "";
		}
		标准库定义了多个随机数引擎类，区别在于性能和随机性质量不同。每个编译器都会指定其中一个作为default_random_engine类型。
		---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
																					随机数引擎操作
		---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			Engine e;					默认构造函数；使用该引擎类型默认的种子
			Engine e(s);				使用整形值s作为种子
			e.seed(s)					使用种子s重置引擎的状态
			e.min()						此引擎可生成的最小值和最大值
			e.max()						
			Engine::result_type			此引擎生成的unsigned整形类型
			e.discard(u)				将引擎推进u步；u的类型为unsigned long long
		---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		对于大多数场合，随机数引擎的输出是不能直接使用的，这也是为什么早先我们称之为原始随机数。问题出在生成的随机数的值范围通常与我们需要的不符，而正确转换随机数的范围是及其困难的。
	4）分布类型和引擎
		为了得到一个指定范围内的数，我们使用一个分布类型的对象：
		// 生成0到9之间（包含）均匀分布的随机数
		uniform_int_distribution<unsigned> u(0, 9);     // 均匀整形分布
		default_random_engine e;						// 生成无符号随机整数
		for (size_t i = 0; i < 10; ++i)
		{
			// 将u作为随机数源
			// 每个调用返回在指定范围内并服从均匀分布的值
			cout << u(e) << " ";
		}
		类似引擎类型，分布类型也是函数对象类。分布类型定义了一个调用运算符，它接受一个随机数引擎作为参数。分布对象使用它的引擎参数生成随机数，并将其映射到指定的分布。
	5）当我们说随机数发生器时，是指分布对象和引擎对象的组合。
	6）随机数发生器有一个特性经常会使新手迷惑：即使生成的数看起来是随机的，但对一个给定的发生器，每次运行程序它都会返回相同的数值序列。序列不变这一事实在调试时非常有用。但另一方面，使用随机数
	  发生器的程序也必须考虑这一特性。
	   所以，一个给定的随机数发生器一直会生成相同的随机数序列。一个函数如果定义了局部的随机数发生器，应该将其（包括引擎和分布对象）定义为static的。否则，每次调用函数都会生成相同的序列。
	   另外，我们可以通过提供一个种子（seed）来达到这一目的。种子就是一个数值，引擎可以利用它从序列中一个新位置重新开始生成随机数。
	   选择一个好的种子，与生成好的随机数所涉及的其他大多数事情相同，是极其困难的。可能最常用的方法是调用系统函数time。
	7）其它随机数分布
	   生成随机实数
			使用新标准库设施，可以很容易地获得随机浮点数。我们可以定义一个uniform_real_distribution类型的对象，并让标准库来处理从随机整数到随机浮点数的映射。
			example:
			default_random_engine e;	// 生成无符号随机整数
			// 从0到1（包含）的均匀分布
			uniform_real_distribution<double> u(0,1);
			for (size_t i = 0; i < 10; i++)
			{
				cout << u(e) << " ";
			}
	   ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
																					分布类型的操作
	   ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			Dist d;					默认构造函数；使d准备好被使用。
									其他构造函数依赖于Dist的类型；
									分布类型的构造函数是explicit的
			d(e)					用相同的e连续调用d的话，会根据d的分布式类型生成一个随机数序列；e是一个随机数引擎对象
			d.min()					返回d(e)能生成的最小值和最大值
			d.max()	
			d.reset()				重建d的状态，使得随后对d的使用不依赖于d已经生成的值
	   ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			每个分布模板都有一个默认模板实参。生成浮点值的分布类型默认生成double值，而生成整形值的分布默认生成int值。
	   生成非均匀分布的随机数
			除了正确生成在指定范围内的数之外，新标准库的另一个优势是可以生成非均匀分布的随机数。实际上，新标准库定义了20种分布类型。
			均匀分布：2种
			伯努利分布：4种
			泊松分布：5种
			正态分布：6种
			抽样分布：4种
5.IO库再探
	1）除了条件状态外，每个iostream对象还维护一个格式状态来控制IO如何格式化的细节。格式状态控制格式化的某些方面，如整形值是几进制、浮点值的精度、一个输出元素的宽度等。
	   标准库定义了一组操纵符（manipulator）来修改流的格式状态，一个操纵符是一个函数或是一个对象，会影响流的状态，并能用作输入或输出运算符的运算对象。类似输入和输出运算符，
	  操作符也返回它所处理的流的对象，因此我们可以在一条语句中组合操纵符和数据。
	2）操纵符用于两大类输出控制：控制数值的输出形式以及控制补白的数量和位置。大多数改变格式状态的操纵符都是设置/复原成对的；一个操纵符用来将格式状态设置为一个新值，而另一个
	  用来将其复原，恢复为正常的默认格式。
	3）当操纵符改变流的格式状态时，通常改变后的状态对所有后续IO都生效。
	4）控制布尔值的格式
	   操纵符改变对象的格式状态的一个例子是boolalpha操纵符。默认情况下，bool值打印为1或0。一个true值输出为整数1，而false输出为0。一旦向cout“写入”了boolalpha，我们就改变了cout
      打印bool值的方式。后续打印bool值的操作就会打印true或false而非1或0.
	   为了取消cout格式状态的改变，我们使用noboolalpha。
	5）指定整形值的进制
	   默认情况下，整形值的输入输出使用十进制。我们可以使用操纵符hex、oct和dec将其改为十六进制、八进制或是改回十进制。
	6）在输出中指出进制
	   默认情况下，当我们打印出数值时，没有可见的线索指出使用的是几进制。如果需要打印八进制值或十六进制值，应该使用showbase操纵符。当对流应用showbase操纵符时，会在输出结果中
	  显式进制，它遵循与整形常量中指定进制相同的规范：
	   ·前导0x表示十六进制。
	   ·前导0表示八进制。
	   ·无前导字符串表示十进制。
	   操纵符noshowbase恢复cout的状态，从而不再显式整形值的进制。
	   默认情况下，十六进制值会以小写打印，前导字符也会小写的x。我们可以通过使用uppercase操纵符来输出大写的X并将十六进制数字a-f以大写输出。
	7）控制浮点数格式
	   我们可以控制浮点数输出三个种格式：
	   ·以多高精度（多少个数字）打印浮点值。
	   ·数值时打印为十六进制、定点十进制还是科学计数法形式。
	   ·对于没有小数点部分的浮点值是否打印小数点。
	   默认情况下，浮点值按六位数字精度打印；如果浮点值没有小数部分，则不会打印小数点；根据浮点数的值选择打印成定点十进制或科学计数法形式。标准库会选择一种可读性更好的格式；
	  非常大和非常小的值打印为科学计数法形式，其他值打印为定点十进制形式。
	8）指定打印精度
	   我们可以通过调用IO对象的precision成员或使用setprecision操纵符来改变精度。precision成员是重载的。一个版本接受一个int值，将精度设置为此值，并返回旧精度值。另一个版本不接受参数，
	  返回当前精度值。setpricision操纵符接受一个参数，用来设置精度。
	   操纵符setpricesion和其他接受参数的操纵符都定义在头文件iomanip中。
	9）定义在iostream中的操纵符
	   ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
																				定义在iostream中的操纵符
	   ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			  boolalpha									将true和false输出为字符串
			* noboolalpha								将true和false输出为1,0
			  showbase									对整形值输出表示进制的前缀
			* noshowbase								不生成表示进制的前缀
			  showpoint									对浮点值总是显示小数点
			* noshowpoint								只有当浮点值包含小数部分时才显示小数点
			  showpos									对非负数显式+
			* noshowpos									对非负数不显式+
			  uppercase									在十六进制值中打印0X，在科学计数法中打印E
			* nouppercase								在十六进制值中打印0x，在科学计数法中打印e
			* dec										整形值显示为十进制
			  hex										整形值显示为十六进制
			  oct										整形值显示为八进制
			  left										在值的右侧添加填充字符
			  right										在值的左侧添加填充字符
			  internal									在符号和值之间添加填充字符
			  fixed										浮点值显示为定点十进制
			  scientific								浮点值显示为科学计数法
			  hexfloat									浮点值显示为十六进制（C++11 新特性）
			  defaultfloat								重置浮点数格式为十进制（C++11 新特性）
			  unitbuf									每次输出操作后都刷新缓冲区
			* nounitbuf								    恢复正常的缓冲区刷新方式
			* skipws									输入运算符跳过空白符
			  noskipws									输入运算符不跳过空白符
			  flush										刷新ostream缓冲区
			  ends										插入空字符，然后刷新ostream缓冲区
			  endl										插入换行符，然后刷新ostream缓冲区
		* 表示默认流状态
	   ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	10）指定浮点数计数法
		除非你需要控制浮点数的表示形式（如，按列打印数据或打印表示金额或百分比的数据），否则由标准库选择计数法是最好的方式。
		这些操纵符也会改变流的精度的默认含义。在执行scientific、fixed或hexfloat后，精度值控制的是小数点后面的数字位数，而默认情况下精度值指定的是数字的总位数--既包括小数点之后的数字也包括
	  小数点之前的数字。
	11）当按列打印数据时，我们常常需要非常精细地控制数据格式。标准库提供了一些操纵符帮助我们完成所需的控制：
		·setw指定下一个数字或字符串值的最小空间。
		·left表示左对齐输出。
		·right表示右对齐输出，右对齐是默认格式。
		·internal控制负数的符号的位置，它左对齐符号，右对齐值，用空格填满所有中间空间。
		·setfill允许指定一个字符代替默认的空格来补白输出。
	   ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
																				定义在iomanip中的操纵符
	   ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			setfill(ch)				用ch填充空白
			setprecision(n)			将浮点精度设置为n
			setw(w)					读或写值的宽度为w个字符
			setbase(b)				将整数输出为b进制
	   ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	12）标准库还提供了一组底层操作，支持未格式化IO（unformatted IO）。这些操作允许我们将一个流当作一个无解释的字节序列来处理。
	13）单字节操作
		有几个未格式化操作每次一个字节地处理流。它们会读取而不是忽略空白符。
		---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
																			单字节底层IO操作
		---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			is.get(ch)		从istream is 读取下一个字节存入字符ch中。返回is
			os.put(ch)		将字符ch输出到ostream os。返回os
			is.get()		将is的下一个字节作为int返回
			is.putback(ch)	将字符ch放回is。返回is
			is.unget()		将is向后移动一个字节。返回is
			is.peek()		将下一个字节作为int返回，但不从流中删除它
		---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		一般情况下，在读取下一个值之前，标准库保证我们可以退回最多一个值。即，标准库不保证在中间不进行读取操作的情况下能连续调用putback或unget。
		有些函数返回一个int的原因是：可以返回文件尾标记。我们使用char返回中的每个值来表示一个真实字符，因此，取值范围中没有额外的值可以用来表示文件尾。
	   返回int的函数将它们要返回的字符先转换为unsigned char，然后再将结果提升到int。因此，即使字符集中有字符映射到负值，这些操作返回的int也是正值。而
	   标准库使用负值表示文件尾，这样就可以保证与任何合法字符的值都不同。头文件cstdio定义了一个名为EOF的const，我们可以用它来检测从get返回的值是否是
	   文件尾，而不必记忆表示文件尾的实际数值。
	14）多字节操作
		一些未格式化IO操作一次处理大块数据。如果速度是要考虑的重点问题的话，这些操作是很重要的，但类似其它底层操作，这些操作也容易出错。特别是，这些操作
	   要求我们自己分配并管理用来保存和提取数据的字符数组。
	   ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
																			多字节底层IO操作
		---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			is.get(sink, size, delim)
				从is中读取最多size个字节，并保存在字符数组中，字符数组的起始地址由sink给出。读取过程直到遇到字符delim或读取了size个字节或遇到文件尾时停止。如果遇到了delim，则将其留在输入流中，
				不读取出来存入sink
			
			is.getline(sink, size, delim)
				与接受三个参数的get版本类似，但会读取并丢弃delim

			is.read(sink, size)
				读取最多size个字节，存入字符数组sink中。返回is

			is.gcount()
				返回上一个未格式化读取操作从is读取的字节数

			os.write(source, size)
				将字符数组source中的size个字节写入os。返回os

			is.ignore(size, delim)
				读取并忽略最多size个字符，包括delim。与其他未格式化函数不同，ignore有默认参数：size的默认值为1，delim的默认值为文件尾
		 
		 * 一个常见的错误是本想从流中删除分隔符，但却忘了做。
		---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	15）流随机访问
		各种流类型通常都支持对流中数据的随机访问。标准库提供了一对函数，来定位(seek)到流中给定的位置，以及告诉（tell）我们当前位置。
		随机IO本质上是依赖于系统的。为了理解如何使用这些特性，你必须查询系统文档。
		虽然标准库为所有流类型都定义了seek和tell函数，但它们是否会做有意义的事情依赖于流绑定到哪个设备。在大多数系统中，绑定到cin、cout、cerr和clog的流不支持随机访问——毕竟，
	   当我们向cout直接输出数据时，类似向回跳十个位置这种操作是没有意义的。对这些流我们可以调用seek和tell函数，但是运行时会报错，将流置于一个无效状态。
	    由于istream和ostream类型通常不支持随机访问，所以本节剩余内容只适用于fstream和sstream类型。
		为了支持随机访问，IO类型维护一个标记来确定下一个读写操作要在哪里进行。它们还提供了两个函数：一个函数通过将标记seek到一个给定位置来重定位它；另一个函数tell我们标记的当前位置。
	   标准库实际上定义了两对seek和tell函数，一对用于输入流，另一对用于输出流。输入和输出版本的差别在于名字的后缀是g还是p。g版本表示我们正在“获得”（读取）数据，而p版本表示我们正在
	   “放置（写入）数据”。
	   ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
																				seek 和 tell 函数
	   ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			tellg()					返回一个输入流中（tellg）或输出流中（tellp）标记的当前位置
			tellp()

			seekg(pos)				在一个输入流或输出流中将标记重定位到给定的绝对地址。pos通常是前一个tellg或tellp返回的值
			seekp(pos)

			seekp(off, from)		在一个输入流或输出流中将标记定位到from之前或之后off个字符，from可以是下列值之一
			seekg(off, from)			·beg，偏移量相对于流开始位置
										·cur，偏移量相对于流当前位置
										·end，偏移量相对于流结尾位置
	   ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		流中只维护单一的标记
			由于只有单一的标记，因此只要我们在读写操作间切换，就必须进行seek操作来重定位标记。
*/