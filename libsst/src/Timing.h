#ifndef _TIMING_H_
#define _TIMING_H_
namespace sst {
	class Timing {

public:

		Timing();

		void start();
		void split();
		void stop();

		void reset();

		double getTotalElapsedTime() const;
		double getSplitElapsedTime() const;

		double getCurrentTime() const;

private:

		bool _isRunning;

		double _startingTime;
		double _elapsedTime;

		void init();
	};
}
;

#endif
