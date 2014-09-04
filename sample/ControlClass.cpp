#include "ControlClass.h"
//#include "Shared.h"
#include <fstream>
#include <string>
#include <sys/time.h>
#include <pthread.h>

#include <sys/stat.h>
#include <ctime>

#include <zmq.h>
//These additional includes are needed to send serial comands
#include <stdio.h>      // standard input / output functions
#include <stdlib.h>
#include <unistd.h>     // UNIX standard function definitions 
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <termios.h>    // POSIX terminal control definitions
#include <iostream>
using namespace alvar;
using namespace std;


ControlClass::ControlClass() {
	cmdKey = '0';
	cmdMode = 1;
	speed1 = 0;
	speed2 = 0;
}
ControlClass::~ControlClass(){
}

ControlClass& ControlClass::Instance(){
	static ControlClass obj;
	return obj;
}

char ControlClass::GetCmdKey(){
	return cmdKey;
}

void ControlClass::SetCmdKey(char key){
	//more generally this function can be used to control settings based on a keyboard input
	cmdKey = key;
	switch(key){
		case 'w':{
		//motor 1 forward
		speed1 = speed1 + 100;
		if(speed1 > 255){speed1=255;};
		break;
	}
		case 's':{
				 //motor 1 back
				 speed1 = speed1-100;
				 if(speed1<-255){speed1 = -255;};
				 break;
			 }
		case 'a':{
				 //motor 2 forward
				 speed2 = speed2+100;
				 if(speed2>255){speed2=255;}
				 break;
			 }
		case 'd':{
				 //motor 2 back
				 speed2 = speed2-100;
				 if(speed2<-255){speed2=-255;};
				 break;
			 }
		case 'z':{
				 //both motors forward
				 speed1 = 255;
				 speed2 = 255;
				 break;
			 }
		case 'x':{
				 //both motors backwards
				 speed1 = -255;
				 speed2 = -255;
				 break;
			 }
		case 'c':{
				 //both motors spinning in (motor 2 right motor spinning positive, motor 1 left motor spinning negative)
				 speed1 = -255;
				 speed2 = 255;
				 break;
			 }
		case 'v':{
				 //both motors spinning out (motor 2 right motor spinning negative, motor 1 left motor spinning positive)
				 speed1 = 255;
				 speed2 = -255;
				 break;
			 }
		case 't':{
				 // t kills all motors
				 speed1 = 0;
				 speed2 = 0;
				 break;
			 }


	}
}
double ControlClass::GetSpeed(int spd){
	if (spd == 1){return speed1;}
	if (spd == 2){return speed2;}
	else{return 0;}
} 

void ControlClass::SendCmd(){
	

	int USB; 
//TODO make it not hard coded to use ttyUSB0
        if ((USB = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY)) < 0) {
                perror("Cannot open USB device");
                exit(-1);
        }

        struct termios tty;
        struct termios tty_old;
        memset(&tty, 0, sizeof tty);
    
        /* Error Handling */ 
        if (tcgetattr(USB, &tty) != 0) {
                cout << "Error " << errno << " from tcgetattr: " << strerror(errno)
                                << endl;
        }

        /* Save old tty parameters */
        tty_old = tty;

        /* Set Baud Rate */
        cfsetospeed(&tty, (speed_t) B9600);
        cfsetispeed(&tty, (speed_t) B9600);

        /* Setting other Port Stuff */
        tty.c_cflag &= ~PARENB; // Make 8n1
        tty.c_cflag &= ~CSTOPB;
        tty.c_cflag &= ~CSIZE;
        tty.c_cflag |= CS8;

        tty.c_cflag &= ~CRTSCTS; // no flow control
        tty.c_cc[VMIN] = 1; // read doesn't block
        tty.c_cc[VTIME] = 5; // 0.5 seconds read timeout
        tty.c_cflag |= CREAD | CLOCAL; // turn on READ & ignore ctrl lines

        /* Make raw */
        cfmakeraw(&tty);
        /* Flush Port, then applies attributes */
        tcflush(USB, TCIFLUSH);
        if (tcsetattr(USB, TCSANOW, &tty) != 0) {
                cout << "Error " << errno << " from tcsetattr" << endl;
        }


//      Generate the command and add the delimiters
//	TODO These delimiters should be made more generic
	std::string command = GenCmd();
	command.insert(0,"{");
	command.append("}");
	//convert command string into char array
	const char *charCommand = command.c_str();
	write(USB, charCommand,strlen(charCommand));
//	std::cout << "CmdKey: " << cmdKey << std::endl;
//	std::cout << "Command: " << charCommand << std::endl;
	//alvar::sleep(1);
	close(USB);

}

std::string ControlClass::GenCmd(){
	std::string cmd;
	//generate the correct type of command based on the command mode
	if (cmdMode == 1){
		cmd = string(1,cmdKey);
	}
/*      Put closed loop control command generation here
	else if (cmdMode == 2){
	//make sure to keep track of speeds
	}
*/
	else{cmd = string("0\n");};
	return cmd;
}
