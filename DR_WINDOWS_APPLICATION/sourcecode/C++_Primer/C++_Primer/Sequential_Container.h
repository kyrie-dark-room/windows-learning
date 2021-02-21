#pragma once

/*
1. 顺序容器类型
------------------------------------------------------------------------------------------------------
										顺序容器类型
------------------------------------------------------------------------------------------------------
vector					可变大小数组。支持快速随机访问。在尾部之外的位置插入或删除元素可能很慢

deque					双端队列。支持快速随机访问。在头尾位置插入/删除速度很快

list					双向链表。只支持双向顺序访问。在list中任何位置进行插入/删除操作速度都很快

forward_list			单向链表。只支持单向顺序访问。在链表任何位置进行插入/删除操作速度都很快

array					固定大小数组。支持快速随机访问。不能添加或删除元素

string					与vector相似的容器，但专门用于保存字符。随机访问快。在尾部插入/删除速度快
-----------------------------------------------------------------------------------------------------

2. 通常，使用vector是最好的选择，除非你有很好的理由选择其他容器。
3. 较旧的编译器可能需要在两个尖括号之间插入空格，例如，vector<vector<string> >。
4. 容器操作
-----------------------------------------------------------------------------------------------------
												容器操作
-----------------------------------------------------------------------------------------------------
类型别名
iterator				此容器类型的迭代器类型
const_iterator			可以读取元素，但不能修改元素的迭代器类型
size_type				无符号整形类型，足够保存此种容器类型最大可能容器的大小
difference_type			带符号整形类型，足够保存两个迭代器之间的距离
value_type				元素类型
reference				元素的左值类型：与value_type&含义相同
const_reference			元素的const左值类型（即，const_value_type&）

构造函数
C c;					默认构造函数。如果C是一个array，则c中元素按默认方式初始化；否则c为空。

C c1(c2);				c1初始化为c2的拷贝。c1和c2必须是相同类型（即，它们必须是相同的容器类型，且
C c1=c2					保存的是相同的元素类型；对于array类型，两者还必须具有相同的大小）

C c(b, e);				c初始化为迭代器b和e指定范围中的元素的拷贝。范围中元素的类型必须与C元素类型相容

C c(a,b,c...)			c初始化为初始化列表中元素的拷贝。列表中元素的类型必须与c的元素类型相容。对于
C c=(a,b,c...)			array类型，列表中元素数目必须等于或小于array的大小，任何遗漏的元素都进行值初始化

C seq(n)				只有顺序容器(不包括array)的构造函数才能接受大小参数。seq包含n个元素，这些元素
						进行了值初始化；此构造函数时explicit
C seq(n, t)				seq包含n个初始化为值t的元素

赋值与swap
c1=c2					将c1中的元素替换为c2中元素
c1={a, b, c...}			将c1中的元素替换为列表中元素(不适用于array)
a.swap(b)				交换a和b的元素，元素本身并未交换，只是交换了两个容器的内部数据结构，因此可以保证
swap(a, b)				在常数时间内完成。与a.swap(b)等价

seq.assign(b,e)			将seq中的元素替换为迭代器b和e所表示的范围中的元素。迭代器b和e不能指向seq中的元素
seq.assign(il)			将seq中的元素替换为初始值列表il中的元素
seq.assign(n,t)			将seq中的元素替换为n个值为t的元素

大小
c.size()				c中元素的数目(不支持forward_list)
c.max_size()			c可保持的最大元素数目
c.empty()				若c中存储了元素，返回false，否则返回true

添加/删除元素（不适用于array）
注：在不同容器中，这些操作的接口都不同
c.insert(args)			将args中元素拷贝进c
c.emplace(inits)		使用inits构造c中的一个元素
c.erase(args)			删除args指定的元素
c.clear()				删除c中的所有元素，返回void

关系运算符
==, !=					所有容器都支持相等(不等)运算符
<, <=, >, >=			关系运算符（无须关联容器不支持）

获取迭代器
c.begin(), c.end()		返回指向c的首元素和尾元素之后位置的迭代器
c.cbegin(), c.cend()	返回const_iterator

反向容器的额外成员（不支持forward_list）
reverse_iterator		按逆序寻址元素的迭代器
const_reverse_iterator	不能修改元素的逆序迭代器
c.rbegin(), c.rend()	返回指向c的尾元素和首元素之间位置的迭代器
c.crbegin(), c.crend()	返回const_reverse_iterator
-----------------------------------------------------------------------------------------------------

5. 当不需要写访问时，应使用cbegin和cend。
6. 为了创建一个容器为另一个容器的拷贝，两个容器的类型及其元素类型必须匹配。不过，当传递迭代器参数来拷贝
一个范围时，就不要求容器类型是相同的了。而且，新容器和原容器中的元素类型也可以不同，只要能将要拷贝的元素
转换为要初始化的容器的元素类型即可。
7. 与内置数组一样，标准库array的大小也是类型的一部分。当定义一个array时，除了指定元素类型，还要指定容器大小
8. 赋值相关运算会导致指向左边容器内部的迭代器、引用和指针失效。而swap操作将容器内容交换不会导致指向容器的
迭代器、引用和指针失效（容器类型为array和string的情况除外）。
9. 非成员版本的swap在泛型编程中是非常重要的。统一使用非成员版本的swap是一个好习惯。

10.向顺序容器添加元素的操作
-----------------------------------------------------------------------------------------------------
										向顺序容器添加元素的操作
-----------------------------------------------------------------------------------------------------
这些操作会改变容器的大小；array不支持这些操作。
forward_list 有自己专有版本的insert和emplace；
forward_list 不支持push_back和emplace_back。
vector和string不支持push_front和emplace_front

c.push_back(t)			在c的尾部创建一个值为t或由args创建的元素。返回void
c.emplace_back(args)

c.push_front(t)			在c的头部创建一个值为t或由args创建的元素。返回void
c.emplace_front(args)	

c.insert(p,t)			在迭代器p指向的元素之前创建一个值为t或由args创建的元素。返回指向新添加的元素
c.emplace(p, args)		的迭代器

c.insert(p,n,t)			在迭代器p指向的元素之前插入n个值为t的元素。返回指向新添加的第一个元素的迭代器；
						若n为0，则返回p
c.insert(p,b,e)			将迭代器b和e指定范围内的元素插入到迭代器p指向的元素之前。b和e不能指向c中的元素。
						返回指向新添加的第一个元素的迭代器；若范围为空，则返回p
c.insert(p, il)			il是一个花括号包围的元素值列表。将这些给定值插入到迭代器p指向的元素之前。返回指向
						新添加的第一个元素的迭代器；若列表为空，则返回p
-----------------------------------------------------------------------------------------------------

11. 当调用push或insert成员函数时，我们将元素类型的对象传递给它们，这些对象被拷贝到容器中。而当我们调用
一个emplace成员函数时，则是将参数传递给元素类型的构造函数。emplace成员使用这些参数在容器管理的内存空间
中直接构造元素。

12. 在顺序容器中访问元素的操作
	at和下标操作只适用于string、vector、deque和array。
	back不适用于forward_list。
	c.back()		返回c中尾元素的引用。若c为空，函数行为未定义
	c.front()		返回c中首元素的引用。若c为空，函数行为未定义
	c[n]			返回c中下标为n的元素的引用，n是一个无符号整数。若n>=c.size()，则函数行为未定义
	c.at(n)			返回下标为n的元素的引用。如果下标越界，则抛出一out_of_range异常

13. 顺序容器的删除操作
	这些操作会改变容器的大小，所以不适用于array。
	forward_list有特殊版本的erase。
	forward_list不支持pop_back；vector和string不支持pop_front。
	c.pop_back()	删除c中尾元素。若c为空，则函数行为未定义。函数返回void
	c.pop_front()	删除c中首元素。若c为空，则函数行为未定义。函数返回void
	c.erase(p)		删除迭代器p所指定的元素，返回一个指向被删元素之后元素的迭代器，若p指向尾元素，则
					返回尾后(off-the-end)迭代器。若p是尾后迭代器，则函数行为未定义。
	c.erase(b, e)	删除迭代器b和e所指定范围内的元素。返回一个指向最后一个被删元素之后元素的迭代器，若
					e本身就是尾后迭代器，则函数也返回尾后迭代器。
	c.clear()		删除c中的所有元素。返回void
	WARNING：		删除deque中除首尾位置之外的任何元素都会使所有迭代器、引用和指针失效。指向vector或
					string中删除点之后位置的迭代器、引用和指针都会失效。

14. 在forward_list中插入或删除元素的操作
	lst.before_begin()		返回指向链表首元素之前不存在的元素的迭代器。此迭代器不能解引用。
	lst.cbefore_begin()		cbefore_begin()返回一个const_iterator。

	lst.insert_after(p,t)   在迭代器p之后的位置插入元素。t是一个对象，n是数量，
	lst.insert_after(p,n,t) b和e是表示范围的一对迭代器(b和e不能指向lst内)，
	lst.insert_after(p,b,e) il是一个花括号列表。返回一个指向最后一个插入元素的迭代器。如果
	lst.insert_after(p,il)  范围为空，则返回p。若p为尾后迭代器，则函数行为未定义。

	emplace_after(p, args)  使用args在p指定的位置之后创建一个元素。返回一个指向这个新元素的迭代器。若p
							为尾后迭代器，则函数行为未定义
	lst.erase_after(p)		删除p指向的位置之后的元素，或删除从b之后直到（但不包含）e之间的元素。返回一个
	lst.erase_after(b,e)	指向被删元素之后元素的迭代器，若不存在这样的元素，则返回尾后迭代器。如果p指向
							lst的尾元素或者是一个尾后迭代器，则函数行为未定义。

15. 顺序容器大小操作
	resize 不适用于array
	c.resize(n)		调整c的大小为n个元素。若n<c.size()，则多出的元素被丢弃。若必须添加新元素，对新元素进行值初始化
	c.resize(n,t)	调整c的大小为n个元素。任何新添加的元素都初始化为值t

	WARNING：如果resize缩小容器，则指向被删除元素的迭代器、引用和指针都会失效；对vector、string或deque进行
			resize可能导致迭代器、指针和引用失效。

16. 容器大小管理操作
	shrink_to_fit只适用于vector、string和deque。
	capacity和reserve只适用于vector和string。
	c.shrink_to_fit()		请将capacity()减少为与size()相同大小
	c.capacity()			不重新分配内存空间的话，c可以保存多少个元素
	c.reserve()				分配至少能容纳n个元素的内存空间

17. 构造string的其他方法
	-------------------------------------------------------------------------------------------------
	n、len2和pos2都是无符号值
	string s(cp, n)		s是cp指向的数组中前n个字符的拷贝。此数组至少应该包含n个字符
	string s(s2, pos2)  s是string s2从下标pos2开始的字符的拷贝。若pos2>s2.size(), 构造函数的行为未定义
	string s(s2, pos2, len2)  s是string s2从下标pos2开始len2个字符的拷贝。若pos2>s2.size()，构造函数
							的行为未定义。不管len2的值是多少，构造函数至多拷贝s2.size()-pos2个字符。
	-------------------------------------------------------------------------------------------------
18. 子字符串操作
	s.substr(pos, n)   返回一个string，包含s中从pos开始的n个字符的拷贝。pos的默认值为0。n的默认值为
			s.size()-pos，即拷贝从pos开始的所有字符。
19. 修改string的操作
	------------------------------------------------------------------------------------------------
	s.insert(pos, args)  在pos之前插入args指定的字符。pos可以是一个下标或一个迭代器。接受下标的版本
			返回一个指向s的引用；接受迭代器的版本返回指定第一个插入字符的迭代器
	s.erase(pos, len)  删除从位置pos开始的len个字符。如果len被省略，则删除从pos开始直至s末尾的所有字符
			。返回一个指向s的引用
	s.assign(args)  将s中的字符替换为args指定的字符。返回一个指向s的引用
	s.append(args)  将args追加到s。返回一个指向s的引用
	s.replace(range, args)  删除s中范围range内的字符，替换为args指定的字符。range或者是一个下标和一个
			长度，或者是一对指向s的迭代器。返回一个指向s的引用

	args可以是下列形式之一；append和assign可以使用所有形式。
	str不能与s相同，迭代器b和e不能指向s。
	str			字符串str
	str,pos,len str中从pos开始最多len个字符
	cp,len		从cp指向的字符数组的前（最多）len个字符
	cp			cp指向的以空字符结尾的字符数组
	n,c			n个字符c
	b,e			迭代器b和e指定的范围内的字符
	初始化列表	花括号包围的，以逗号分隔的字符列表
	replace和insert所允许的args形式依赖于range和pos是如何指定的。
	replace			replace		insert		insert		args可以是
	(pos,len,args)  (b,e,args)  (pos,args)  (iter,args)
	是				是			是			否			str
	是				否			是			否			str,pos,len
	是				是			是			否			cp,len
	是				是			否			否			cp
	是				是			是			是			n,c
	否				是			否			是			b2,e2
	否				是			否			是			初始化列表
	------------------------------------------------------------------------------------------------
20. string搜索操作
	------------------------------------------------------------------------------------------------
	搜索操作返回指定字符出现的下标，如果未找到则返回npos。
	s.find(args)				查找s中args第一次出现的位置
	s.rfind(args)				查找s中args最后一次出现的位置
	s.find_first_of(args)		在s中查找args中任何一个字符第一次出现的位置。
	s.find_last_of(args)		在s中查找args中任何一个字符最后一次出现的位置。
	s.find_first_not_of(args)	在s中查找第一个不在args中的字符
	s.find_last_not_of(args)	在s中查找最后一个不在args中的字符

	args必须是以下形式之一
	c,pos		从s中位置pos开始查找字符c。pos默认为0
	s2,pos		从s中位置pos开始查找字符串s2。pos默认为0
	cp,pos		从s中位置pos开始查找指针cp指向的以空字符结尾的C风格字符串。pos默认为0
	cp,pos,n	从s中位置pos开始查找指针cp指向的数组的前n个字符。pos和n无默认值
	------------------------------------------------------------------------------------------------
21. s.compare的几种参数形式
	------------------------------------------------------------------------------------------------
	s2						比较s和s2
	pos1,n1,s2				将2中从pos1开始的n1个字符与s2进行比较
	pos1,n1,s2,pos2,n2		将s中从pos1开始的n1个字符与s2中从pos2开始的n2个字符进行比较
	cp						比较s与cp指向的以空字符结尾的字符数组
	pos1,n1,cp				将s中从pos1开始的n1个字符与cp指向的以空字符结尾的字符数组进行比较
	pos1,n1,cp,n2			将s中从pos1开始的n1个字符与指针cp指向的地址开始的n2个字符进行比较
	------------------------------------------------------------------------------------------------
22. string和数值之间的转换
	------------------------------------------------------------------------------------------------
	to_string(val)			一组重载函数，返回数值val的string表示。val可以是任何算数类型。对每个浮点
						  类型和int或更大的整形，都有相应版本的to_string。与往常一样，小整形会被提升
	stoi(s,p,b)			    返回s的起始子串（表示整数内容）的数值，返回值类型分别是int、long、unsigned
	stol(s,p,b)				long、long long、unsigned long long。b表示转换所用的基数，默认为10。p是
	stol(s,p,b)				size_t指针，用来保存s中第一个非数值字符的下标，p默认为0，即，函数不保存
	stol(s,p,b)				下标
	stol(s,p,b)

	stof(s,p)				返回s的起始子串（表示浮点数内容）的数值，返回值类型分别是float、double或
	stod(s,p)				long double。参数p的作用与整数转换函数中一样
	stold(s,p)
	------------------------------------------------------------------------------------------------
23. 除了顺序容器外，标准库还定义了三个顺序容器适配器：stack、queue和priority_queue。默认情况下，stack
	和queue是基于deque实现的，priority_queue是在vector之上实现的。
	------------------------------------------------------------------------------------------------
	size_type				一种类型，足以保存当前类型的最大对象的大小
	value_type				元素类型
	container_type			实现适配器的底层容器类型
	A a;					创建一个名为a的空适配器
	A a(c);					创建一个名为a的适配器，带有容器c的一个拷贝
	关系运算符				每个适配器都支持所有关系运算符：==、!=、<、<=、>和>=这些运算符返回底层容器
							的比较结果
	a.empty()				若a包含任何元素，返回false，否则返回true
	a.size()				返回a中的元素数目
	swap(a,b)				交换a和b的内容，a和b必须有相同类型，包括底层容器类型也必须相同
	a.swap(b)
	-------------------------------------------------------------------------------------------------
24. 栈(stack)操作补充
	------------------------------------------------------------------------------------------------
	栈默认基于deque实现，也可以在list或vector之上实现。
	s.pop()				删除栈顶元素，但不返回该元素值
	s.push(item)	    创建一个新元素压入栈顶，该元素通过拷贝或移动item而来，或者由args构造
	s.emplace(args)		
	s.top()				返回栈顶元素，但不将元素弹出栈
	------------------------------------------------------------------------------------------------
25. 队列(queue)和优先级队列(priority_queue)操作
	------------------------------------------------------------------------------------------------
	queue默认基于deque实现，prioriry_queue默认基于vector实现；
	queue也可以用list或vector实现，priority_queue也可以用deque实现。
	q.pop()				返回queue的首元素或priority_queue的最高优先级的元素，但不删除此元素
	q.front()			返回首元素或尾元素，但不删除此元素，只适用于queue
	q.back()
	q.top()				返回最高优先级元素，但不删除该元素，只适用于priority_queue
	q.push(item)		在queue末尾或priority_queue中恰当的位置创建一个元素，
	q.emplace(args)		其值为item，或者由args构造
	------------------------------------------------------------------------------------------------
*/
