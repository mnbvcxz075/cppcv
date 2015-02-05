#include"TimeCounter.h"

TimeCounter::TimeCounter(){
	times = std::map<std::string,timer>();
}
TimeCounter::~TimeCounter(){

}

bool TimeCounter::add(std::string str){
	if (times.find(str) != times.end()){
		std::cout << str << " has already existed" << std::endl;
		return false;
	}
	times[str] = timer();
	return true;
}

bool TimeCounter::start(std::string str){
	if (times.find(str) == times.end()){
		add(str);
	}
	if (times[str].start != 0){
		std::cout << str << " is already started" << std::endl;
		return false;
	}

	times[str].times++;
	times[str].start = timeGetTime();
	return true;
}

double TimeCounter::stop(std::string str){
	if (times.find(str) == times.end()){
		std::cout << str << " isn't defined" << std::endl;
		return false;
	}
	if (times[str].start == 0){
		std::cout << str << " isn't started" << std::endl;
		return false;
	}
	double thisTime = timeGetTime() - times[str].start;
	times[str].time += thisTime;
	times[str].start = 0;
	return thisTime;
}

double TimeCounter::get(std::string str,bool mean){
	if (times.find(str) == times.end()){
		std::cout << str << " isn't defined" << std::endl;
		return -1;
	}
	if (mean)
		return times[str].time / times[str].times;
	else
		return times[str].time;
}

