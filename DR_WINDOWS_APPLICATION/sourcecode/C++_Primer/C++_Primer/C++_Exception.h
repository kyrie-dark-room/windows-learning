#pragma once
/*
1. try语句块的通用语法形式是
try{
	program-statements
}catch(exception-declaration){
	handler-statements
}catch(exception-declaration){
	handler-statements
}

2. throw表达式用来抛出异常
   实例：throw runtime_error("Data must refer to same ISBN");

3. 如果没有try语句块定义的异常，或者没有找到任何匹配的catch子句，程序转到名为terminate的标准库函数。
该函数的行为与系统有关，一般情况下，执行该函数将导致程序非正常退出。

4. C++标准库定义了一组类，用于报告标准库函数遇到的问题。这些异常类也可以在用户编写的程序中使用，它们
分别定义在4个头文件中：
  （1）exception头文件定义了最通用的异常类exception。它只报告异常的发生，不提供任何额外信息。
  （2）stdexcept头文件定义了几种常用的异常类。
  （3）new头文件定义了bad_alloc异常类型。
  （4）type_info头文件定义了bad_cast异常类型。
------------------------------------------------------------------------------------------------------
										<stdexcept>定义的异常类
------------------------------------------------------------------------------------------------------
  exception                                         最常见的问题
  runtime_error                                     只有在运行时才能检测到的问题
  range_error                                       运行时错误：生成的结果超出了有意义的值域范围
  overflow_error                                    运行时错误：计算上溢
  underflow_error                                   运行时错误：计算下溢
  logic_error                                       程序逻辑错误
  domain_error                                      逻辑错误：参数对应的结果值不存在
  invalid_argument                                  逻辑错误：无效参数
  length_error                                      逻辑错误：试图创建一个超出该类型最大程度的对象
  out_of_range                                      逻辑错误：使用一个超出有效范围的值
------------------------------------------------------------------------------------------------------
*/
