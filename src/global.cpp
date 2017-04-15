#include <cstdio>
#include <ctime>

#include "global.h"

using namespace std;

timespec operator-(const timespec &a, const timespec &b) {
	timespec temp;

	if ((a.tv_nsec - b.tv_nsec) < 0) {
		temp.tv_sec = a.tv_sec - b.tv_sec - 1;
		temp.tv_nsec = 1000000000 + a.tv_nsec - b.tv_nsec;
	} else {
		temp.tv_sec = a.tv_sec - b.tv_sec;
		temp.tv_nsec = a.tv_nsec - b.tv_nsec;
	}

	return temp;
}

timespec operator+(const timespec &a, const timespec &b) {
	timespec temp;

	if ((a.tv_nsec + b.tv_nsec) >= 1000000000) {
		temp.tv_sec = a.tv_sec + b.tv_sec + 1;
		temp.tv_nsec = (a.tv_nsec + b.tv_nsec) - 1000000000;
	} else {
		temp.tv_sec = a.tv_sec + b.tv_sec;
		temp.tv_nsec = a.tv_nsec + b.tv_nsec;
	}

	return temp;
}

bool operator>(const timespec &a, const timespec &b) {
	timespec temp;

	temp = a - b;

	if ((temp.tv_sec > 0) or ((temp.tv_sec >= 0) and (temp.tv_nsec > 0))) {
		return true;
	} else {
		return false;
	}
}
