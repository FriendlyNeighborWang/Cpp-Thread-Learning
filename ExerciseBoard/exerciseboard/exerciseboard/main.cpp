#include <chrono>
#include <future>
#include <thread>

int some_task() {
	return 0;
}
void do_something(int) {

}

int main() {
	std::future<int> f = std::async(some_task);
	if (f.wait_for(std::chrono::milliseconds(35)) == std::future_status::ready)
		do_something(f.get());
}