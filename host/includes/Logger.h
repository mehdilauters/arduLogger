/*
 * Noyau.h
 *
 *  Created on: 31 janv. 2012
 *      Author: rox
 */

#ifndef LOGGER_H_
#define LOGGER_H_
#include <string>
#include <vector>
#include <QThread>
#include <fstream>
#include "pugixml.hpp"
#define LOG_LEVEL_NOOUTPUT 0
#define LOG_LEVEL_ERROR 1
#define LOG_LEVEL_WARNING 2
#define LOG_LEVEL_INFO 3
#define LOG_LEVEL_DEBUG 4
#define LOG_LEVEL_VERBOSE 5

#define LOGGER_ANALOG_REGEXP "^\\{(\\d+),\\s?(\\d+),\\s?(\\d+),\\s?(-?\\d+\\.?\\d{0,})\\}$"

#define LOGGER_MESSAGE_REGEXP "^\\{(\\d+),\\s?(\\d+),\\s?(.*)\\}$"

#include <QtSerialPort/QSerialPort>

#include <map>

namespace Logger_Logger {
class IHM_Logger;

	class Logger : public QThread
	{
 
        Q_OBJECT
 
		private:
			//libResources::ResourcesManager * m_resourcesManager;
        std::ofstream m_logFile;
		std::map<QString,QString> m_symbolsNames;
		std::ifstream m_inputFile;
		 QSerialPort *m_serial;
		 Logger_Logger::IHM_Logger *m_hmi;
		 pugi::xml_document m_xmlDoc;
		 static QStringList m_logLevelText;
		public:
		    struct Settings {
		        QString name;
		        qint32 baudRate;
		        QString stringBaudRate;
		        QSerialPort::DataBits dataBits;
		        QString stringDataBits;
		        QSerialPort::Parity parity;
		        QString stringParity;
		        QSerialPort::StopBits stopBits;
		        QString stringStopBits;
		        QSerialPort::FlowControl flowControl;
		        QString stringFlowControl;
		        bool localEchoEnabled;
		    };
			/**
			* Constructor
			* @param p_fileConf : configuration file
			* @return
			*/
			Logger(std::string p_fileConf);
			void openSerialPort(Settings _p);
			void closeSerialPort();
			void openInputFile(std::string _file);
			char read();
			int dataAvailable();
			void run();
			void setHmi(Logger_Logger::IHM_Logger *_hmi);
			void send(QString _command);
			void log_internal(QString _message, int _moduleId, int _logLevel = LOG_LEVEL_INFO);
			void log_internal(float _value, int _moduleId = 0, int _signalId = 0, int _logLevel = LOG_LEVEL_INFO);
			void close();
      QString getSymbolName(int _moduleId, int _signalId = -1);
			/**
			* Destructor
			* @return
			*/
			virtual ~Logger();

			 signals:
			 	 void log(QString _message, int _moduleId, int _logLevel = LOG_LEVEL_INFO);
			     void log(float _value, int _moduleId = 0, int _signalId = 0, int _logLevel = LOG_LEVEL_INFO);

	};
}

#endif /* NOYAU_H_ */
