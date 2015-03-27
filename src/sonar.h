/*
 * sonar.h
 *
 *  Created on: May 12, 2013
 *      Author: daniel
 */

#ifndef _SONAR_H
#define _SONAR_H

#include <poll.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <vector>

#define SONAR_1_PW_PIN		25
#define SONAR_1_RX_PIN		26

#define	SONAR_2_PW_PIN		18

#define SONAR_LOOP_SEC		0
#define SONAR_LOOP_NSEC		100E6

#define SONAR_POLL_TIMEOUT	50

#define Elements_in(arrayname) (sizeof arrayname/sizeof *arrayname)

//void sonar_init();
//void sonar_timer_init();
//void sonar_run(union sigval);
//void *sonar_ping(void *arg);
//void sonar_fsm(SonarState *current_state, timespec *time, const bool value);

class SonarSystem {
private:
	enum SonarState {
		STATE_HIGH,
		STATE_LOW,
	};

public:
	// Classes
	class SonarSensor {
	private:
		timespec	_previousTime;
		SonarState	_currentState;

	public:
		SonarSensor (int pwFileDesc, int rxFileDesc, int groupNumber = 0) : groupNumber( groupNumber ), pwFileDesc( pwFileDesc ), rxFileDesc( rxFileDesc ){};

		int			pwFileDesc;
		int			rxFileDesc;
		int			groupNumber;

		friend class SonarSystem;
	};

	// Constructor and Destructor
	SonarSystem(SonarSensor *, int , int , void (*)(int, float), timespec);
	~SonarSystem();

//	void enableSensor(int sensorNumber);
//	void disableSensor(int sensorNumber);

	void enableRanging();
	void disableRanging();

	void setRangingPeriod();
	void getRangingPeriod();

	void setCallback();

	void setGroupEnable(int, bool);
	bool getGroupEnable(int);

private:
	timespec		_rangingPeriod;

	int				_groupCount;
	int				_sensorCount;

	std::vector<SonarSensor>	_sensors;

	timer_t			_timerID;
	pthread_t		_threadID;

	void		(*_sonarCallbackFunction)(int, float);

	void		initTimer();
	void		initThread();

	static void sonarPing(union sigval);
	static void *sonarISR(void *);

	static void	sonarFSM(SonarState *, timespec *, const bool);
};

SonarSystem::SonarSystem(SonarSensor *sensors, int sensorCount, int groupCount, void (*sonarCallbackFunction)(int, float), timespec rangingPeriod) {
	_sensorCount = sensorCount;
	_groupCount = groupCount;

	_rangingPeriod = rangingPeriod;
	_sonarCallbackFunction = sonarCallbackFunction;

	_sensors = sensors;

	initTimer();
	initThread();
}

SonarSystem::~SonarSystem() {
	timer_delete(_timerID);
	pthread_cancel(_threadID);
}

void SonarSystem::initTimer() {
	int rc;
	struct sigevent svnt;
	struct itimerspec its;

	// Set timer settings
	svnt.sigev_notify = SIGEV_THREAD;
	svnt.sigev_notify_function = &SonarSystem::sonarPing;
	svnt.sigev_notify_attributes = NULL;

	// Pass value and pointers
	svnt.sigev_value.sival_int = _groupCount;
	svnt.sigev_value.sival_ptr = _sensors;

	if ((rc = timer_create(CLOCK_MONOTONIC, &svnt, &_timerID)) < 0) {
		perror("Sonar Create Timer");
	}

	its.it_interval = _rangingPeriod;
	its.it_value = _rangingPeriod;

	// Set timer to ping sonar sensors
	if ((rc = timer_settime(_timerID, 0, &its, NULL)) < 0) {
		perror("Sonar Set Timer");
	}
}

void SonarSystem::initThread() {
	int rc;

	// Create thread for encoder interrupts
	if ((rc = pthread_create(&_threadID, NULL, SonarSystem::sonarISR, (void *)_sensors)) < 0 ) {
		perror("Sonar");
	}
}

void SonarSystem::sonarPing(union sigval arg) {
	SonarSensor *sensors = (SonarSensor *)arg.sival_ptr;

	int rc;
	int groupCount = arg.sival_int;
	int sensorCount = Elements_in(sensors);
	static int lastGroup = 0;

	// Increment last group
	++lastGroup %= groupCount;

	// Ping all the sensors in the group
	for (int i = 0; i < sensorCount; i++) {
		if (sensors[i].groupNumber == lastGroup) {
			// Write "1" to file
			if ((rc = write(sensors[i].fileDesc, "1", 2)) < 0) {
				perror("Sonar");
			}

			// Write "0" to file
			if ((rc = write(sensors[i].fileDesc, "0", 2)) < 0) {
				perror("Sonar");
			}
		}
	}
}

void *SonarSystem::sonarISR(void *arg) {
	int rc;
	bool sonar_value;
	char tempBuff[255];

	timespec start_time;

	SonarSensor *sensors = (SonarSensor *)arg;
	int sensorCount = Elements_in(sensors);

	SonarState current_state = STATE_LOW;

	struct pollfd sonar_int[sensorCount];

	for (int i = 0; i < sensorCount; i++) {
		sonar_int[0].fd = sensors[0].fileDesc;
		sonar_int[0].events = POLLPRI;
	}

	while (true) {
		// Count number of nsec PW pin is high
		if ((rc = poll(sonar_int, 1, SONAR_POLL_TIMEOUT)) < 0) {
			perror("Sonar ISR");
		} else {
			for (int j = 0; j < sensorCount; j++) {
				if (sonar_int[j].revents & POLLPRI) {
					lseek(sonar_int[j].fd, 0, SEEK_SET);

					if ((rc = read(sonar_int[j].fd, tempBuff, 1)) < 0) {
						perror("Sonar");
					} else {
						sonar_value = strtol(tempBuff, NULL, 10);
					}

					sonarFSM(&sensors[j]._currentState, &sensors[j]._previousTime, sonar_value);
				}
			}

			if (sonar_int[0].revents & POLLHUP) {
				// A hangup has occurred
			}
		}
	}

	return NULL;
}

void SonarSystem::sonarFSM(SonarState *current_state, timespec *time, const bool value) {
	SonarState next_state;

	timespec tmp_time;
	timespec dif_time;

	switch (*current_state) {
	case STATE_HIGH:
		if (value == 0) {
			clock_gettime(CLOCK_BOOTTIME, &tmp_time);

			dif_time = tmp_time - *time;

			printf("d:%i\n", (int)(dif_time.tv_nsec / 147000));

			next_state = STATE_LOW;
		}

		break;
	case STATE_LOW:
		if (value == 1) {
			clock_gettime(CLOCK_BOOTTIME, time);

			next_state = STATE_HIGH;
		}
		break;
	default:
		next_state = STATE_HIGH;

		break;
	}

	*current_state = next_state;

}

#endif /* SONAR_H_ */
