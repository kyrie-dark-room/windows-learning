// C++_Primer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>

/*
 *  简单主函数
 *  读取两个函数，求它们的和
 */
void example_add_two_number(void)
{
	std::cout << "Enter two numbers:" << std::endl;
	int v1 = 0, v2 = 0;
	std::cin >> v1 >> v2;
	std::cout << "The sum of " << v1 << " and " << v2
		<< " is " << v1 + v2 << std::endl;
}

void example_while_add_one2ten(void)
{
	int sum = 0, val = 1;
	// 只要val的值小于等于10，while循环就会持续进行
	while (val <= 10)
	{
		sum += val;   // 将sum + val 赋予sum
		++val;        // 将val加1
	}

	std::cout << "Sum of 1 to 10 inclusive is " << sum << std::endl;
}

void example_for_add_one2ten(void)
{
	int sum = 0;
	// 从1加到10
	for (int val = 1; val <= 10; val++)
		sum += val;   // 等价于sum = sum + val
	std::cout << "Sum of 1 to 10 inclusive is " << sum << std::endl;
}

void example_for_add_somenumber(void)
{
	int sum = 0, value = 0;
	// 从1加到10
	while (std::cin >> value)
	{
		sum += value; // 等价于sum = sum + value
	}
	std::cout << "Sum is " << sum << std::endl;
}

void example_count_number_occurs_times(void)
{
	// currVal 是我们正在统计的数；我们将读入的新值存入val
	int currVal = 0, val = 0;
	// 读取第一个数，并确保真实有数据可以处理
	if (std::cin >> currVal) {
		int cnt = 1;
		while (std::cin >> val){
			if (val == currVal)
				++cnt;
			else {
				std::cout << currVal << " occurs " << cnt << " times" << std::endl;
				currVal = val;
				cnt = 1;
			}
		}

		// 记住打印文件中最后一个值的个数
		std::cout << currVal << " occurs " << cnt << " times" << std::endl;
	}
}

int main()
{
	// call example_add_two_number
	//example_add_two_number();

	// call example_while_add_one2ten
	//example_while_add_one2ten();

	// call example_for_add_one2ten
	//example_for_add_one2ten();

	// call example_for_add_somenumber
	//example_for_add_somenumber();

	// call example_count_number_occurs_times
	example_count_number_occurs_times();
	return 0;
}
