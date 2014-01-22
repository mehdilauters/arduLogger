#ifndef IHM_Logger_H
#define IHM_Logger_H
#include <QtGui/QMainWindow>
#include <string>
#include <map>

#include "Logger.h"


#define COLUMN_MESSAGE 2
#define COLUMN_MODULE 1
#define COLUMN_LEVEL 0
#define NB_COLUMN_TABLE 3

#include "pugixml.hpp"

namespace Ui {
    class IHM_Logger;
    class Logger;
}
class QVBoxLayout;
class QwtPlotCurve;
class SettingsDialog;
class QwtPlot;
class QLineEdit;

#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

namespace Logger_Logger {

class Logger;


class IHM_Logger : public QMainWindow
{
    Q_OBJECT

public:
    /**
      * Constructor of The kernel's IHM
    */
    explicit IHM_Logger(Logger *_logger, std::string _device, std::string configuration_file="JiminyConfig.xml", QWidget *parent = 0);
    void applySettings();
    void closeSerialPort();
    void filterRow(int _rowId);
    virtual ~IHM_Logger();
    
public slots:
    void filter();
    void serialSend();
    void log(QString _message, int _moduleId, int _logLevel = LOG_LEVEL_INFO);
    void log(float _value, int _moduleId = 0, int _signalId = 0, int _logLevel = LOG_LEVEL_INFO);
    void tableResized(int _logicalIndex, int _oldSize, int _newSize);
private slots:
   

private:
	Logger *m_logger;
	QVBoxLayout *m_graphLayout;
	std::map <QString,std::vector<double> > m_series;
	std::map <QString,std::vector<double> > m_seriesTime;
	std::map <QString,QwtPlotCurve *> m_seriesPlot;
	std::map <QString,QwtPlot *> m_seriesPlotWidget;
	std::string m_configFile;
  	std::string m_inputFile;
  pugi::xml_document m_xmlDoc;
  
    QLineEdit *m_filters[NB_COLUMN_TABLE];
    Ui::IHM_Logger *ui;
    SettingsDialog *settings;
    QStringList m_tableLogHeader;
    static QStringList m_logLevelText;
};

}

#endif // IHM_Logger_H
