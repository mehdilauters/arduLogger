#include "Logger.h"

#define MODULE_ID 11

Logger *Logger::m_instance = NULL;

	Logger::Logger(Stream *_loggingStream, int _logLevel, bool _serial)
	{
		Logger::m_instance = this;
		this->m_loggingStream = _loggingStream;
		this->m_logLevel = constrain(_logLevel,LOG_LEVEL_NOOUTPUT,LOG_LEVEL_VERBOSE);
		this->m_serial = _serial;
	}

	Logger *Logger::getInstance()
	{
		return Logger::m_instance;
	}

	Stream *Logger::getStream()
	{
		return this->m_loggingStream;
	}

	void Logger::error(String _message, int _moduleId)
	{
		this->log(_message, _moduleId, LOG_LEVEL_ERROR);
	}

	void Logger::warning(String _message, int _moduleId)
	{
		this->log(_message, _moduleId, LOG_LEVEL_WARNING);
	}

	void Logger::info(String _message, int _moduleId)
	{
		this->log(_message, _moduleId, LOG_LEVEL_INFO);
	}

	void Logger::debug(String _message, int _moduleId)
	{
		this->log(_message, _moduleId, LOG_LEVEL_DEBUG);
	}

	void Logger::verbose(String _message, int _moduleId)
	{
		this->log(_message, _moduleId, LOG_LEVEL_VERBOSE);
	}


	void Logger::log(String _message, int moduleId, int _logLevel)
	{
		if( _logLevel > this->m_logLevel )
		{
			return;
		}

		String message;
		message = String("{") + String(_logLevel) + String(", ") + String(moduleId) + String(", ") + _message + String("}");

		if(this->m_serial)
		{
			this->m_loggingStream->println(message);
		}

	}


	void Logger::log(float _value, int _moduleId , int _signalId, int _logLevel)
	{
		if( _logLevel > this->m_logLevel )
		{
			return;
		}

		String message;
		if(this->m_serial)
		{
			this->m_loggingStream->print(String("{") + String(_logLevel) + String(", ") + String(_moduleId) + String(", ")+ String(_signalId)  + String(", ") );
			this->m_loggingStream->print(_value);
			this->m_loggingStream->println("}");
		}
	}

	bool Logger::ready()
	{
		#ifdef DEBUG_WAIT_FOR_LOGGER
//		int startingTime = millis();
		if(this->m_loggingStream->available())
		{
			char c = this->m_loggingStream->read();
			if(c == 0x55)
			{
				INFO("ready");
				return true;
			}
		}
		return false;
		#else
				INFO("ready");
				return true;
		#endif
	}
/*
	void Logger::log(int _value, int _moduleId , int _signalId, int _logLevel, int _displayedChannel )
	{
		if( _logLevel > this->m_logLevel )
		{
			return;
		}

		String message;
		if(this->m_serial)
		{
			message = String("{") + String(_logLevel) + String(", ") + String(_moduleId) + String(", ")+ String(_signalId)  + String(", ") + String(_value) + String("}");
			Serial.println(message);
		}


		if(this->m_screen != NULL && _displayedChannel != -1 )
		{
			this->m_screen->log(_value, _displayedChannel);
		}
	}
*/
