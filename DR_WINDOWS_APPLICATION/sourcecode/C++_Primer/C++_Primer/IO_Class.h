#pragma once
/*
------------------------------------------------------------------------------------------------------
										信息点
------------------------------------------------------------------------------------------------------
	1. IO库类型和头文件
		头文件						类型
		iostream					istream, wistream从流读取数据
									ostream, wostream向流写入数据
									iostream, wiostream读写流
		fstream						ifstream, wifstream从文件读取数据
									ofstream, wofstream向文件写入数据
									fstream, wfstream读写文件
		sstream						istringstream, wistringstream从string读取数据
									ostringstream, wostringstream向string写入数据
									stringstream, wstringstream读写string
	2. 不能拷贝或对IO对象赋值
		由于不能拷贝IO对象，因此我们也不能将形参或返回类型设置为流类型。进行IO操作的函数通常以引用方式
		传递和返回流。读写一个IO对象会改变其状态，因此传递和返回的引用不能是const的。
	3. 条件状态
									     IO库条件状态
		strm::iostate		strm是一种IO类型，iostate是一种机器相关的类型，提供了表达条件状态的完整功能
		strm::badbit		strm::badbit用来指出流已崩溃
		strm::failbit		strm::failbit用来指出一个IO操作失败了
		strm::eofbit		strm::eofbit用来指出流达到了文件结束
		strm::goodbit		strm::goodbit用来指出流未处于错误状态。此值保证为零
		s.eof()				若流s的eofbit置位，则返回true
		s.fail()			若流s的failbit或badbit置位，则返回true
		s.bad()				若流s的badbit置位，则返回true
		s.good()			若流s处于有效状态，则返回true
		s.clear()			若流s中所有条件状态位复位，将流的状态设置为有效。返回void
		s.clear(flags)		根据给定的flags标志位，将流s中对应条件状态位复位。flags的类型为strm::iostate
							。返回void
		s.setstate(flags)	根据给定的flags标志位，将流s中对应条件状态位置位。flags的类型为strm::iostate
							。返回void
		s.rdstate()			返回流s的当前条件状态，返回值类型为strm::iostate
	4. 由于流可能处于错误状态，以此代码通常应该在使用一个流之前检查它是否处于良好状态。确定一个流对象
		的状态的最简单的方法是将它当作一个条件来使用：while (cin >> word)。
	5. 每个输出流都管理一个缓冲区，用来保存程序读写的数据。导致缓冲刷新（即，数据真正写到输出设备或文件）
		的原因有很多：
		（1）程序正常结束，作为main函数的return操作的一部分，缓冲刷新被执行。
		（2）缓冲区满时，需要刷新缓冲，而后新的数据才能继续写入缓冲区。
		（3）我们可以使用操纵符如endl来显式刷新缓冲区。
		（4）在每个输出操作之后，我们可以用操纵符unitbuf设置流的内部状态，来清空缓冲区。默认情况下，对cerr
		是设置unitbuf的，因此写到cerr的内容都是立即刷新的。
		（5）一个输出流可能被关联到另一个流。在这种情况下，当读写被关联到流时，关联到的流的缓冲区会被刷新。
	6. 刷新输出缓冲区
		cout << "hi!" << endl;    // 输出hi和一个换行，然后刷新缓冲区
		cout << "hi!" << flush;   // 输出hi，然后刷新缓冲区，不附加任何额外字符
		cout << "hi!" << ends;    // 输出hi和一个空字符，然后刷新缓冲区
	7. 如果想在每次输出操作后都刷新缓冲区，我们可以使用unitbuf操纵符。它告诉流在接下来的每次写操作之后都
		进行一次flush操作。而nounitbuf操纵符则重置流，使其恢复使用正常的系统管理的缓冲区刷新机制。
		cout << unitbuf;   // 所有输出操作后都立即刷新缓冲区
		cout << nounitbuf;  // 回到正常的缓冲方式
	8. 关联输入和输出流
		（1）当一个输入流被关联到一个输出流时，任何试图从输入流读取数据的操作都会先刷新关联的输出流。标准库将
		cout和cin关联在一起，因此下面语句cin >> ival；导致cout的缓冲区被刷新。
		（2）tie有两个重载的版本：一个版本不带参数，返回指向输出流的指针。如果本对象当前关联到一个输出流，则
		返回的就是指向这个流的指针，如果对象未关联到流，则返回空指针。tie的第二个版本接受一个指向ostream的指针
		，将自己关联到此ostream。即，x.tie(&o)将流x关联到输出流o。
	9. fstream特有的操作
		fstream fstrm;  // 创建一个未绑定的文件流。fstream是头文件fstream中定义的一个类型
		fstream fstrm(s);  // 创建一个fstream，并打开名为s的文件。s可以是string类型，或者是一个指向C
			风格字符串的指针。这些构造函数都是explicit的。默认的文件模式mode依赖于fstream的类型。
		fstream fstrm(s, mode);  // 与前一个构造函数类似，但按指定mode打开文件
		fstrm.open(s)		// 打开名为s的文件，并将文件与fstrm绑定。s可以是一个string或一个指向C风格
						字符串的指针。默认的文件mode依赖于fstream的类型。返回void
		fstrm.close()	// 关闭与fstrm绑定的文件。返回void
		fstrm.is_open()  // 返回一个bool值，指出与fstrm关联的文件是否成功打开且尚未关闭
	10. 当一个fstream对象离开其作用域时，与之关联的文件会自动关闭。即当一个fstream对象被销毁时，close
		会自动被调用。
	11. 文件模式，每个流都有一个关联的文件模式(file mode)，用来指出如何使用文件：
		in    以读方式打开
		out	  以写方式打开
		app   每次写操作前均定位到文件末尾
		ate   打开文件后立即定位到文件末尾
		trunc 截断文件
		binary 以二进制方式进行IO
	12. stringstream特有的操作
		sstream strm;    // strm是一个未绑定的stringstream对象。sstream是头文件sstream中定义的一个类型
		sstream strm(s);    // strm是一个sstream对象，保存string s的一个拷贝。此构造函数是explicit的
		strm.str();		// 返回strm所保存的string的拷贝
		strm.str(s);	// string s拷贝到strm中。返回void
		
------------------------------------------------------------------------------------------------------
*/