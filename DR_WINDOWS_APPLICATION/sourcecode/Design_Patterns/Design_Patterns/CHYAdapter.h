#pragma once
#include <iostream>
#include <numeric>

using namespace std;

class FibonacciGenerator
{
	int n;
	int val[2];
public:
	FibonacciGenerator() : n(0) { val[0] = val[1] = 0; }
	int operator()()
	{
		int result = n > 2 ? val[0] + val[1] : n > 0 ? 1 : 0;
		++n;
		val[0] = val[1];
		val[1] = result;
		return result;
	}
	int count() { return n; }
};

class CHYFibonacciAdapter
{
	FibonacciGenerator f;
	int length;

public:
	CHYFibonacciAdapter(int size) : length(0) {}
	class iterator;
	friend class iterator;
	class iterator : public std::iterator<std::input_iterator_tag, CHYFibonacciAdapter, ptrdiff_t>
	{
		CHYFibonacciAdapter& ap;
	public:
		typedef int value_type;
		iterator(CHYFibonacciAdapter& a) : ap(a) {}
		bool operator==(const iterator&) const
		{
			return ap.f.count() == ap.length;
		}

		bool operator!=(const iterator& x) const
		{
			return !(*this == x);
		}

		int operator*() const { return ap.f(); }
		iterator& operator++() { return *this; }
		iterator operator++(int) { return *this; }
	};

	iterator begin() { return iterator(*this); }
	iterator end() { return iterator(*this); }
};

/*
int main()
{
	const int SZ = 20;
	CHYFibonacciAdapter a1(SZ);
	cout << "accumulate: " << accumulate(a1.begin(), a1.end(), 0) << endl;
	return 0;
}*/

