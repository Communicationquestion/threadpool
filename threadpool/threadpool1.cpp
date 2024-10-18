#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>
#include <barrier>
std::mutex cout_mutex;
std::mutex qmutex;
std::mutex over;
std::barrier b(26);
class Task {
public:
	Task(std::string url, int a) : url(url), a(a) {}

	int foo() const {
		try {
			std::this_thread::sleep_for(std::chrono::seconds(2));
			std::lock_guard<std::mutex> lock(cout_mutex);
			std::cout << url << " " << a << std::endl;
			return 0;
		} catch(const std::exception& e) {
			std::cerr << "Exception in thread: " << e.what() << std::endl;
			return -1;
		}
	}
private:

	std::string url;
	int a;
};

class threadpool {
public:
	threadpool(int n) : n(n) {};
	void push_task(Task task) {
		qtasks.push(task);
	};
	void start() {
		for(int i = 0; i < n; i++) {
			threads.emplace_back([&] {
				Task* s = NULL;
				Task a("NULL", 27);
				while(true) {
					{
						if(isnotask) {
							std::lock_guard<std::mutex> lock(qmutex);
							if(!qtasks.empty()) {
								s = NULL;
								a = qtasks.front();
								s = &a;
								qtasks.pop();
								isnotask = 1;
							} else {
								continue;
							}
						}
						if(s != NULL) {
							s->foo();
							s = NULL;
						}
						b.arrive_and_wait();
						if(qtasks.empty()) {
							if(isnotask == 1) {
								if(s != NULL) {
									s->foo();
									isnotask = 0;
									s = NULL;
								}
							}
						}
					}
				}
			});
		}
	}

	void join() {
		for(std::thread& t : threads) {
			if(t.joinable()) {
				t.join();
			}
		}
	}
private:
	bool isnotask = 1;
	int n;
	std::queue<Task> qtasks;
	std::vector<std::thread> threads;
};

int main() {


	std::string strs[100] = {"a", "b", "c", "d", "e", "f", "g", "h", "i",
							 "j", "k", "l", "m", "n", "o", "p", "q", "r",
							 "s", "t", "u", "v", "w", "x", "y", "z"};
	threadpool tp(26);
	tp.start();
	int j = 0;
	while(true) {
		std::string s = strs[j];
		Task task(s, j);
		tp.push_task(task);
		if(j == 25) {
			break;
		}
		if(j == 13) {
			std::this_thread::sleep_for(std::chrono::seconds(5));
		}
		j++;
	}

	tp.join();

	return 0;
}