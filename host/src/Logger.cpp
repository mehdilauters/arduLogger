/*
 * Noyau.cpp
 *
 *  Created on: 31 janv. 2012
 *      Author: rox
 */


#include "Logger.h"
#include <QtSerialPort/QSerialPort>
#include <QDebug>
#include <IHM_Logger.h>

#include "ansiColors.h"

#include <iostream>
#include <math.h>

using namespace std;
using namespace pugi;
//using namespace Logger_Logger;
namespace Logger_Logger {

QStringList Logger::m_logLevelText = QStringList() << "No Output" << "Error" << "Warning" << "Info" << "Debug" << "Verbose";

Logger::Logger(string _fileConf){
	// TODO Auto-generated constructor stub
	//this->m_resourcesManager = new ResourcesManager(tNoyau,p_fileConf);

	 if(!this->m_xmlDoc.load_file(_fileConf.c_str()))
	  {
	   qDebug()<< "Could not load xmlfile ";
	  }
	 this->m_logFile.open("serialIn.log", ios::out | ios::binary | ios::trunc);
	 this->m_serial = NULL;

}

void Logger::close()
{
	if( this->m_serial != NULL)
	{
		qDebug() << "closing Serial Port";
		this->m_serial->close();
	}
	if(this->m_inputFile.is_open())
	{
		qDebug() << "closing inputFile";
		this->m_inputFile.close();
	}

	if(this->m_logFile.is_open())
	{
		qDebug() << "closing log file";
		this->m_logFile.close();
	}
}

int Logger::dataAvailable()
{
	if(this->m_inputFile.is_open())
	{
		if(!this->m_inputFile.eof())
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return this->m_serial->bytesAvailable();
	}
}


void Logger::send(QString _command)
{
	if(this->m_inputFile.is_open())
	{
		qDebug() << "serial not initialized";
	}
	else
	{
		QByteArray data;
		data.append(_command);
		this->m_serial->write(data);
	}
}

char Logger::read()
{
	char c;
	if(this->m_inputFile.is_open())
	{
		this->m_inputFile >> c;
//		this->msleep(1);
	}
	else
	{
		this->m_serial->read(&c, 1);
		this->m_logFile << c;
	}

	return c;
}

QString Logger::getSymbolName(int _moduleId, int _signalId)
{
     QString name;
     QString moduleName = QString::number(_moduleId);
     QString signalName = QString::number(_signalId);

     QString tmpName = m_symbolsNames[QString::number(_moduleId)];
     if(tmpName == QString())
     {
      QString query = QString("/ids/module[@id='")+moduleName+"']";
      
      xpath_node moduleNode = this->m_xmlDoc.select_single_node(query.toStdString().c_str());
      if(moduleNode != false)
      {
         moduleName = moduleNode.node().attribute("name").value();
         m_symbolsNames[QString::number(_moduleId)] = moduleName;
          
      }
      
     }
     else
     {
      moduleName = tmpName;
     }
     name = moduleName;
     if(_signalId != -1)
     {
        QString signalId = QString::number(_moduleId) + "_" + QString::number(_signalId);
           QString tmpName = m_symbolsNames[signalId];
           if(tmpName == QString())
                {
                      QString query = QString("/ids/module[@id='")+QString::number(_moduleId)+"']/signal[@id='"+QString::number(_signalId)+"']";
                      xpath_node moduleNode = this->m_xmlDoc.select_single_node(query.toStdString().c_str());  
                           if(moduleNode != false)
                           {
                            signalName = moduleNode.node().attribute("name").value(); 
                            m_symbolsNames[signalId] = signalName;
                           }
                }
           else
           {
            signalName = tmpName;
           }
     
        name = name + "_" + signalName;
     }

     
     
     return name;
}


void Logger::log_internal(QString _message, int _moduleId, int _logLevel)
{
     ansiColors colors;

	_logLevel = constrain(_logLevel,LOG_LEVEL_NOOUTPUT,LOG_LEVEL_VERBOSE);
	QString moduleName = QString(colors.colorStart("yellow").c_str()) + this->getSymbolName(_moduleId) + QString(colors.colorStop().c_str());
     
 
 
	     qDebug() << Logger::m_logLevelText[_logLevel] << "		" << moduleName << "		" << _message;
           emit(log(
			    		  	  	  _message, // value
								 _moduleId,	//moduleID
								_logLevel    // logLevel
											 	 ));
}

void Logger::log_internal(float _value, int _moduleId, int _signalId , int _logLevel)
{
ansiColors colors;
	_logLevel = constrain(_logLevel,LOG_LEVEL_NOOUTPUT,LOG_LEVEL_VERBOSE);
 
 QString moduleName = QString(colors.colorStart("yellow").c_str()) + this->getSymbolName(_moduleId) + QString(colors.colorStop().c_str());
 QString signalName = QString(colors.colorStart("blue").c_str()) + this->getSymbolName(_moduleId, _signalId) + QString(colors.colorStop().c_str());
 
	  qDebug() << Logger::m_logLevelText[_logLevel] << "		" << moduleName << "		" << signalName << "	" <<_value;
     			      emit(log(
    		  	  	  _value, // value
								 _moduleId,	//moduleID
								_signalId,    //signalID
								_logLevel    // logLevel
											 	 ));
     			      
}


void Logger::run()
{
	QString buffer;
	bool record = false;
	QRegExp messageRegexp(LOGGER_MESSAGE_REGEXP);
	QRegExp analogRegexp(LOGGER_ANALOG_REGEXP);
	messageRegexp.setMinimal(true);
	analogRegexp.setMinimal(true);

//	 for(float i=0; i< 100; i+=0.1)
//	 {
//             log_internal(
//							 	 sin(i),
//							 	88,
//							 	0
//							 	 );
//               log_internal(
//							 	 cos(i),
//							 	88,
//							 	1
//							 	 );
//               log_internal(
//							 	 tan(i),
//							 	0,
//							 	2
//							 	 );
//          this->msleep(10);
//
//	 }
//	return;
	while(true)
	{

//		while(this->m_serial->bytesAvailable() != 0 )
//		{
//			QByteArray data = this->m_serial->readAll();
//			qDebug() << data;
//		}


//		qint64 readData(char *data, qint64 maxSize) Q_DECL_OVERRIDE;
		while(this->dataAvailable() != 0 )
		{
			char c;
//			cout << c;
			c = this->read();
			if(c == '{')
			{
				buffer = QString();
				record = true;
			}
			if(record)
			{
				buffer += c;
//				qDebug() << c;
			}
			if(c == '}')
			{
//				qDebug() << buffer;
				record = false;
				 if (analogRegexp.indexIn(buffer) != -1) {
					 //LOG_LEVEL MODULE_ID SIGNAL_ID, VALUE
					 //{1, 3, 1, 12.27}
					 QStringList list = analogRegexp.capturedTexts();
//					 qDebug() << list;
					 float value = analogRegexp.cap(4).toFloat();
					 int module = analogRegexp.cap(2).toInt();
					 int signal = analogRegexp.cap(3).toInt();
					 int logLevel = analogRegexp.cap(1).toInt();

					 this->log_internal(
		    		  	  	  value, // value
							 module,	//moduleID
							signal,    //signalID
							logLevel    // logLevel
							);
//					 qDebug() << "value " << value;
//					 qDebug() << "module " << module;
//					 qDebug() << "signal " << signal;
//					 qDebug() << "logLevel " << logLevel;

					 /*this->m_hmi->log(
								 analogRegexp.cap(1).toInt(),
								analogRegexp.cap(2).toInt(),
								analogRegexp.cap(3).toInt(),
								analogRegexp.cap(4).toFloat()
											 	 );*/
				 }
				else
				{
					 if (messageRegexp.indexIn(buffer) != -1) {
//						 qDebug() << buffer;
//						 qDebug() << messageRegexp.pattern();
						 QStringList list = messageRegexp.capturedTexts();
//						 qDebug() << list;
						 this->log_internal(
								 list[3],
								 list[2].toInt(),
								 list[1].toInt()
							 );
						 /*this->m_hmi->log(
								 	 analogRegexp.cap(3),
								 	analogRegexp.cap(0).toInt(),
								 	analogRegexp.cap(1).toInt()
								 	 );*/
					 }
					else
					{

//						qDebug() << "protocol error";
						qDebug() << analogRegexp.pattern();
						qDebug() << analogRegexp.errorString();
						qDebug() << buffer;
					}
				}
				buffer = QString();
			}



		}
	/*	while(this->m_serial->canReadLine())
		{
		 QByteArray data = this->m_serial->readLine();
//		 int j;
//		 while ((j = data.indexOf("{", j)) != -1) {
//		     qDebug() << "Found { tag at index position " << j << endl;
//		     ++j;
//		 }
		 qDebug() << data;
		}*/
		this->msleep(10);
	}
}

void Logger::openInputFile(string _file)
{
	this->m_inputFile.open(_file.c_str(), ios::in | ios::binary);
}

void Logger::openSerialPort(Settings _p)
{
	this->m_serial = new QSerialPort();
	this->m_serial->setPortName(_p.name);
    if (this->m_serial->open(QIODevice::ReadWrite)) {
        if (this->m_serial->setBaudRate(_p.baudRate)
                && this->m_serial->setDataBits(_p.dataBits)
                && this->m_serial->setParity(_p.parity)
                && this->m_serial->setStopBits(_p.stopBits)
                && this->m_serial->setFlowControl(_p.flowControl)) {

//            console->setLocalEchoEnabled(p.localEchoEnabled);
  /*
  ui->actionConnect->setEnabled(false);
            ui->actionDisconnect->setEnabled(true);
            ui->actionConfigure->setEnabled(false);
            ui->statusBar->showMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                                       .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                                       .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
*/
        } else {
        	this->m_serial->close();
            qDebug() << this->m_serial->errorString();
        }
    } else {
    	qDebug() << "config Error";
    }
}
//! [4]

//! [5]
void Logger::closeSerialPort()
{
    this->m_serial->close();
}


Logger::~Logger() {
	this->m_logFile.close();
	//delete(m_resourcesManager);
}

}
