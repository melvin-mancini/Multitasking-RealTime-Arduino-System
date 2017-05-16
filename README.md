# Multitasking-RealTime-Arduino
## Introduction 
This simple project shows how Arduino can be used with a real time operating system (Erika). A special version of Arduino called ARTe was used for software development. For more information on how to use ARTe, visit the official developer site ( http://retis.sssup.it/?q=arte ). 

### ARTe (Arduino Real-Time extension)
ARTe (Arduino Real-Time extension) is an extension to the Arduino framework that supports multitasking and real-time preemptive scheduling. Thanks to ARTe, the user can easily specify and run multiple concurrent loops at differents rates, in addition to the single execution cycle provided by the standard Arduino framework. Today ARTE supports the most popular platforms: Arduino UNO and Arduino DUE. In addition to the single loop present in the standard Arduino approach, the user can easily specify a number of concurrent loops to be executed at specific rates. Concurrency and real-time scheduling is provided by the ERIKA Enterprise ( http://erika.tuxfamily.org/drupal/ ) open-source real-time kernel. Loops can use the standard Arduino libraries, which have been enhanced to guarantee mutual exclusion in the access of shared data structures. The impact of ARTe in terms of footprint and runtime overhead has beed evaluated by extensive tests and resulted to be negligible.
**You can use ARTe and develop Arduino code only with Windows platforms.** The project under consideration was developed on a machine with Windows 8.1.

### About this project
Thanks to ARTe it has been possible to implement a system in which there are 3 periodic processes. Each of them has a given priority and is managed according to a real time scheduler algorithm made available by the ERIKA operating system.
Processes, when they are running, handle different activities. A process for managing every 3 milliseconds the acquisition of the distance through the sensor HC-SR04. Another process for flashing a LED every 7 milliseconds. A final process to read the temperature returned by the LM35 sensor expressed in degrees Celsius. All processes are managed in real time mode.

## How to use
### ARTe installation
In order to develop applications with ARTe you must download the development tool from the site ( http://retis.sssup.it/?q=arte ). The developer site provides all the information on how to use the development tool, providing documentation and examples.
Once installed the tool is quite similar to Arduino's editor but with the ability to use the Art Extension.
You can then enable or disable the ART extension by clicking on the **Tools** item in the menu bar. The item **ARTℇ** will appear and click on it you can select enable or disable.

### Run the project
In order to realize the system described above, you must have the following hardware components:

* Arduino Uno
* HC-SR04 (distance sensor)
* LM35 (temperature sensor)
* LED
* Electrical resistances
* Simple connectors for electronic devices.

In the **Circuit Diagram** folder there are two files that show how to connect the various components to the Arduino. The     file in .fzz format was created using the Fritzing software, the recommended tool for building electronic circuits. In         addition, there is an image that shows more intuitive how the various components must be connected.
