#ifndef TIMERS_H
#define TIMERS_H


// micros instead of millis
#include <Arduino.h>

namespace mytimer {
	typedef void (*timerFuncPtr)(void);

	static void emptyFunc(void) { return; };
} // namespace mytimer

struct thread {
	mytimer::timerFuncPtr funcPtr   = mytimer::emptyFunc;
	unsigned long long    interval  = 0;
	unsigned long long    beginTime = 0;
};

template<int n>
class Timers {
public:
	void attach(int t, mytimer::timerFuncPtr funcPtr, unsigned long long interval) {
		_threads[t].funcPtr   = funcPtr;
		_threads[t].interval  = interval;
		_threads[t].beginTime = micros();
	};

	void detach(int t) {
		if (t < 0 || t >= n)
			return;

		_threads[t].interval = 0;
		_threads[t].funcPtr  = mytimer::emptyFunc;
	}

	void setInterval(int t, unsigned long long interval) {
		_threads[t].interval  = interval;
		_threads[t].beginTime = micros();
	};

	void changeInterval(int t, unsigned long long interval) {
		_threads[t].interval = interval;
	}

	void proceed() {
		unsigned long long time = micros();
		for (int i = 0; i < n; ++i) {
			if (_threads[i].interval <= 0) {
				continue;
			}
			if (time - _threads[i].beginTime >= _threads[i].interval) {
				_threads[i].funcPtr();
				_threads[i].beginTime += time;
			}
		}
	}

private:
	thread _threads[n];
};

#endif
