#ifndef CONTROLCLASS_H
#define CONTROLCLASS_H

#include "Alvar.h"
#include <vector>
#include <string>
#include "cv.h"
#include "highgui.h"
#include "CaptureFactory.h"
#include <sys/time.h>

using namespace alvar;


class ControlClass {
protected:
	ControlClass();
        /** \brief Hidden copy constructor for Singleton */
        ControlClass(const ControlClass&);
        /** \brief Hidden copy operator for Singleton */
        ControlClass& operator=(const ControlClass&);
        /** \brief Hidden destructor for Singleton */
        ~ControlClass();
        /** \brief Boolean indicating are we still running. We exit from the \e WaitKeys when this is false. */
	/** Generates command to be send w no delimiters*/
private:
	char cmdKey;
	/** A char to keep track of keypresses**/
        int cmdMode;
	/** controls what kind of command is generated in GenCmd**/
        double speed1;
	/** speed of motor 1**/
        double speed2;
	/** speed of motor2 **/
        std::string GenCmd();
	/**Generates a string with command info for an external embedded system**/

public:
	static ControlClass& Instance();
	/** single static instance of this class
	use ControlClass::Instance.foo() to call methods or fields **/	
	char GetCmdKey();
	/**Returns command key character **/
	double GetSpeed(int spd);
	/**Gets the speed of one motor **/
	void SetCmdKey(char key);
	/**Sets the command key field **/
	void SendCmd();
	/** Generates and sends a command via USB serial **/
	void SetCmdMode();
	/**Sets the command mode of the instance**/
};
#endif
	
	
