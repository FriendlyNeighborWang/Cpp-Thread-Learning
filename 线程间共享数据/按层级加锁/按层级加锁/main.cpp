#include "hierarchical_mutex.h"

hierarchical_mutex high_level_mutex(10000);
hierarchical_mutex low_level_mutex(5000);
hierarchical_mutex other_mutex(6000);

// 低层次操作，设定低层次互斥锁 
int do_low_level_stuff();
int low_level_func() {
	std::lock_guard<hierarchical_mutex> lk(low_level_mutex);
	return do_low_level_stuff();
}

// 高层次操作，设定高层次互斥锁 
void do_high_level_stuff(int);
void high_level_func() {
	std::lock_guard<hierarchical_mutex> lk(high_level_mutex);
	do_high_level_stuff(low_level_func());
}

// 上述调用操作遵循先对高层次上锁，在对低层次上锁的规则
void thread_a() {
	high_level_func();
}

void do_other_stuff();
void other_stuff() {
	high_level_func();
	do_other_stuff();
}

// 该操作会抛出异常，因为先对低层次上锁后才对高层次上锁
void thread_b() {
	std::lock_guard<hierarchical_mutex> lk(other_mutex);
	other_stuff();
}