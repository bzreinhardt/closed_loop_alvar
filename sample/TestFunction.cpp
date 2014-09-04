#include "CvTestbed.h"
#include "MarkerDetector.h"
#include "MultiMarker.h"
#include "GlutViewer.h"
//#include "Shared.h"
#include "FFKalmanFilter.h"
#include "ControlClass.h"
#include <fstream>
#include <string>
#include <sys/time.h>
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

int main(int argc, char *argv[])
{
	std::cout << "hello world" << std::endl;
	int speed = 100;
	speed = ControlClass::Instance().GetSpeed(1);
	char key1 = ControlClass::Instance().GetCmdKey();
	ControlClass::Instance().SetCmdKey('a');
	char key2 = ControlClass::Instance().GetCmdKey();
	std::cout << "speed: "<<speed<<" key1 " << key1 <<" key2 "<<key2<< std::endl;
	ControlClass::Instance().SendCmd();
	return 0;
	
}
