/*
 * main.cpp
 *
 *  Created on: 31 janv. 2012
 *      Author: rox
 */

//#include "Noyau.h"
#include <QtCore>
#include <QApplication>
#include "IHM_Logger.h"
#include <iostream>

#include <QtCore/QDebug>
 
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <signal.h>
#include "Logger.h"

using namespace Logger_Logger;
using namespace std;

static IHM_Logger* interface_utilisateur = NULL;
static Logger* logger = NULL;

void exitAnyway( int s){
	logger->close();
	std::cout <<"Noyau SIGNAL : CTRL+C detected" << endl;
	exit(0);
}

int main(int argc,char**argv) {

	signal (SIGINT,exitAnyway);
	signal (SIGTERM,exitAnyway);

	string fileConf = "../ids.xml";
	string inputFile = "";
 switch(argc)
 {
  case 1:
	  // exec name
  break;
  case 2:
    fileConf = argv[1];
  break;
  case 3:
	  inputFile = argv[2];
  break;
  default:
          cerr << "Wrong number of arguments" << endl;
  break;
 }

	QApplication a(argc,argv);
	logger = new Logger(fileConf);
	interface_utilisateur = new IHM_Logger(logger,inputFile,fileConf, NULL);
	//interface_utilisateur->show();


//// Example use QSerialPortInfo
//    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
//        qDebug() << "Name        : " << info.portName();
//        qDebug() << "Description : " << info.description();
//        qDebug() << "Manufacturer: " << info.manufacturer();
//
//        // Example use QSerialPort
//        QSerialPort serial;
//        serial.setPort(info);
//        if (serial.open(QIODevice::ReadWrite))
//            serial.close();
//    }

	return a.exec();
}
