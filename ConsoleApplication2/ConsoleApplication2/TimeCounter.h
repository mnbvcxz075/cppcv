#include"Setting.h"

class TimeCounter{
	typedef struct timer{
		double start=0;
		double time=0;
	};

public:
	TimeCounter();
	~TimeCounter();
	bool add(std::string);
	bool start(std::string);
	double stop(std::string);
	double get(std::string);
private:
	std::map<std::string,timer> times;
};