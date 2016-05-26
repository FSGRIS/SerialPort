#include <stdio.h>      // standard input / output functions
#include <stdlib.h>
#include <string.h>     // string function definitions
#include <unistd.h>     // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <termios.h>    // POSIX terminal control definitions
#include <iostream>
#include "ros/ros.h"
#include "std_msgs/String.h"

using namespace std;

int sendDegree(const std_msgs::String::ConstPtr& msg);

int USB;

int main(int argc, char** argv) {
	USB = open("/dev/ttyACM1", O_RDWR| O_NOCTTY);
	if (USB < 0) {
		cout << "Error, couldn't open USB /dev/ttyACM0, " << errno << endl;
	}
	struct termios tty;
	struct termios tty_old;
	memset(&tty, 0, sizeof(tty));

	if (tcgetattr(USB, &tty) != 0) {
	   cout << "Error " << errno << " from tcgetattr: " << strerror(errno) << std::endl;
	}
	tty_old = tty;
	cfsetospeed(&tty, (speed_t)B9600);
	cfsetispeed (&tty, (speed_t)B9600);
	tty.c_cflag     &=  ~PARENB;            // Make 8n1
	tty.c_cflag     &=  ~CSTOPB;
	tty.c_cflag     &=  ~CSIZE;
	tty.c_cflag     |=  CS8;

	tty.c_cflag     &=  ~CRTSCTS;           // no flow control
	tty.c_cc[VMIN]   =  1;                  // read doesn't block
	tty.c_cc[VTIME]  =  5;                  // 0.5 seconds read timeout
	tty.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines

	/* Make raw */
	cfmakeraw(&tty);

	/* Flush Port, then applies attributes */
	tcflush( USB, TCIFLUSH );
	if ( tcsetattr ( USB, TCSANOW, &tty ) != 0) {
	   std::cout << "Error " << errno << " from tcsetattr" << std::endl;
	}

	while (1) {
		int degree;
		cin >> degree;
		cout << "Degree was " << degree << endl;
		if (degree < 0) {
			close(USB);
			return 0;
		}
		sendDegree(degree, USB);
	}
}

// message format should be "servoNum:degree:"
int sendDegree(const std_msgs::String::ConstPtr& msg) {//int degree, int USB) {
	string servoNum = strtok(msg.data, ":");
	string degree = strtok(NULL, ":");
	ROS_INFO("Sending...");
	int n_written = 0;
	do {
	    n_written = write( USB, &degree, 1 );
	    cout << "n_written: " << n_written << endl;
	} while (n_written == 0);
	cout << "sent" << endl;
}