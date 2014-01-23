arduLogger
==========

A simple arduino logger, for text, analog values to curve, error levels...

![main screenshot](http://lauters.fr/blog/wp-content/uploads/2014/01/sinLogging.png "main screenshot")

Most important features are

- On the target (arduino compatible)
    -   only required logs are embeded in the final binary, selected by log level
    -  can write log to any Stream (Serial, SoftareSerial...)
    -  Several log levels: 
		- Error
		- Warning
		- Info
		- Debug
		- Verbose
    -   Log level can be set at run time (Attention, only embebded log levels will be sent)
    -   Text logging
    -   Analog value logging
    -   Low memory consumtion
	- Exemple
- On the Host
    -   Qt based client
    -	multiplatforme ( QSerialPort )
    -	QRegexp log filtering
    -   Curve drawing for analog values
    -   Id of signals and modules translated to a human readable information thanks to an XML configuration file
    -	Compilation CMake based
	
	
Compilation
- Host
	- mkdir /path/to/build
	- cd /path/to/build
	- cmake /path/to/src/host
	- make
	- ./arduLogger [/path/to/xml/file] [/path/to/serial/log/for/replay]
- Target
	- cp -R /path/to/src/target /usr/share/arduino/libraries
	- start arduino
	- examples => ArduLogger => ArduLogger
	- compile & upload
	
More information available at http://lauters.fr/blog/complete-arduino-logger/
