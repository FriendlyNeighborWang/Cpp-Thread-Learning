#include <list>
#include <algorithm>
#include <future>

// 串行快速排序
template<typename T>
std::list<T> sequential_quick_sort(std::list<T> input) {
	if (input.empty())
		return input;
	std::list<T> result;
	result.splice(result.begin(), input, input.begin());
	T const& pivot = *result.begin();
	auto divide_point = std::partition(input.begin(), input.end(), [&](T const& t) {return t < pivot; });

	std::list<T> lower_part;
	lower_part.splice(lower_part.end(), input, input.begin(), divide_point);
	auto new_lower(sequential_quick_sort(std::move(lower_part)));
	auto new_higher(sequential_quick_sort(std::move(input)));

	result.splice(result.end(), new_higher);
	result.splice(result.begin(), new_lower);
	return result;
}

// 并行快速加速排序
template<typename T>
std::list<T> parallel_quick_sort(std::list<T> input) {
	if (input.empty())
		return input;
	std::list<T> result;
	result.splice(result.begin(), input, input.begin());
	T const& pivot = *result.begin();
	auto divide_point = std::partition(input.begin(), input.end(), [&](T& const t) {return t < pivot; });
	std::list<T> lower_part;
	lower_part.splice(lower_part.end(), input, input.begin(), divide_point);

	// async这种让编译器自主决定究竟是否调用新线程的方式可以在硬件不足时决定不启用新线程，
	// 会尽可能避免造成过饱和线程的负担
	std::future<std::list<T>> new_lower(std::async(&parallel_quick_sort<T>, std::move(lower_part)));
	auto new_higher(parallel_quick_sort(std::move(input)));
	result.splice(result.end(), new_higher);
	result.splice(result.begin(), new_lower.get());
	return result;
}