If you use directly the ARTe application from the work folder you can run arduino.exe

If you want to install ARTe from the ARTe-1.0.rar file you must follow these instructions:
1 - Download and unzip the file  ARTe-1.0.rar
2 - Download the library DigitalToggle.zip from Arduino site
	2.1 - Unzip the library
	2.2 - Modify the file DigitalToggle.h and replace the line 	#include <pins_arduino.h>	with 	#include "Arduino.h"
3 - After modifying the library create another .zip file and import it from Arduino IDE.


ARTe works only in the Windows 8.1 platform. In other Windows version, the IDE is not guaranteed. 