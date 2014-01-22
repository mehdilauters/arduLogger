#include <QtGui>
#include <QtCore>
#include <QTextEdit>
#include <QLineEdit>

#include <qwt_plot_curve.h>
#include <qwt_plot.h>
#include <qwt_plot_magnifier.h>

#include <iostream>
#include "ui_IHM_Logger.h"
#include "IHM_Logger.h"
#include "Logger.h"
#include "settingsdialog.h"

using namespace std;
using namespace pugi;

using namespace Logger_Logger;


QStringList IHM_Logger::m_logLevelText = QStringList() << "No Output" << "Error" << "Warning" << "Info" << "Debug" << "Verbose";

#define NB_COLS 3

IHM_Logger::IHM_Logger(Logger *_logger, string _inputFile, string configuration_file, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::IHM_Logger)
{
	this->m_logger = _logger;
 this->m_configFile = configuration_file;
 this->m_inputFile = _inputFile;

 if(!this->m_xmlDoc.load_file(this->m_configFile.c_str()))
 {
  qDebug()<< "Could not load xmlfile ";
 }
 
 
 
    ui->setupUi(this);
    

    
    ui->tableLog->setColumnCount(NB_COLS);
    ui->tableLog->verticalHeader()->setVisible(false);
    ui->tableLog->horizontalHeader()->setVisible(false);
    ui->tableLog->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableLog->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableLog->setSelectionMode(QAbstractItemView::SingleSelection);
    
    
    ui->tableHeader->setColumnCount(NB_COLS);
    ui->tableHeader->verticalHeader()->setVisible(false);
    ui->tableHeader->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableHeader->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableHeader->setSelectionMode(QAbstractItemView::SingleSelection);

    m_tableLogHeader << "Level" << "Module" << "Log";
    ui->tableHeader->setHorizontalHeaderLabels(m_tableLogHeader);    
    
    //ui->tableLog->setShowGrid(false);
    ui->tableLog->setStyleSheet("QTableView {selection-background-color: red;} *[logLevel=\"error\"] { background-color: red } *[logLevel=\"warning\"] { background-color: yellow } *[logLevel=\"info\"] { background-color: orange } *[logLevel=\"debug\"] { background-color: blue } *[logLevel=\"verbose\"] { background-color: green }");

    
    int rowId = ui->tableHeader->rowCount();
     ui->tableHeader->setRowCount(rowId+1);
    for(int  i=0; i <NB_COLUMN_TABLE; i++)
    {
      this->m_filters[i] = new QLineEdit(this);
      ui->tableHeader->setCellWidget ( rowId,i, this->m_filters[i] );
      connect( this->m_filters[i], SIGNAL( returnPressed () ),
             this, SLOT( filter() ) );
    }
     
    this->m_graphLayout = new QVBoxLayout;

    this->ui->chartContainer->setLayout(m_graphLayout);
//    this->ui->scrollArea->setWidgetResizable ( true );
//    QScrollArea *scroll=new QScrollArea();
//    scroll->setGeometry(ui->tabWidget->geometry());
//    scroll->setWidget(ui->tabWidget->widget(1));
//    scroll->show();


    
//    int nbItems = 100;
//    //ui->tableLog->setRowCount(nbItems);
//    for(int i =0; i< nbItems;i++)
//    {
//     this->log("Hello", i, i%LOG_LEVEL_VERBOSE);
//    }
    
    // refresh style
    ui->tableLog->update();
    ui->tableLog->style()->unpolish(ui->tableLog);
    ui->tableLog->style()->polish(ui->tableLog);
    ui->tableLog->update();
    /* ui->tableLog->setItem(0,0,new QTableWidgetItem("ITEM 1_1"));
     ui->tableLog->setItem(0,1,new QTableWidgetItem("ITEM 1_1"));
     ui->tableLog->setItem(1,1,new QTableWidgetItem("ITEM 1_1"));
     ui->tableLog->setItem(1,0,new QTableWidgetItem("ITEM 1_1"));*/


     
     
     connect( this->m_logger, SIGNAL( log(float , int, int, int) ),
             this, SLOT( log(float , int, int, int) ) );
             
     connect( this->m_logger, SIGNAL( log(QString, int, int) ),
             this, SLOT( log(QString, int, int) ) );
             
     connect( this->ui->tableHeader->horizontalHeader(), SIGNAL( sectionResized ( int , int , int  ) ),
             this, SLOT( tableResized ( int , int , int  ) ));
     
     connect( this->ui->serialSendButton, SIGNAL( clicked() ),
                  this, SLOT( serialSend () ));

     /*connect( ui->tableLog, SIGNAL( cellDoubleClicked (int, int) ),
             this, SLOT( cellSelected( int, int ) ) );*/


     if(this->m_inputFile == "")
     {
 		 this->settings = new SettingsDialog(this);
 		 this->settings->show();
     }
     else
     {
    	 this->applySettings();
     }
}


void IHM_Logger::serialSend()
{
	QString command = this->ui->serialEdit->text();
	this->m_logger->send( command );
	this->ui->serialEdit->setText("");
	// TODO add historic
}

void IHM_Logger::tableResized(int _logicalIndex, int _oldSize, int _newSize)
{
     this->ui->tableLog->setColumnWidth(_logicalIndex, _newSize);
}


void IHM_Logger::applySettings()
{
	if(this->m_inputFile == "")
	{
		this->m_logger->openSerialPort(settings->settings());
	}
	else
	{
		this->m_logger->openInputFile(this->m_inputFile);
	}
 this->show();
 this->m_logger->start();
 this->m_logger->setPriority ( QThread::TimeCriticalPriority );


// for(int i=0; i< 100; i++)
// {
//	 log("Loop", 90, LOG_LEVEL_WARNING);
//	 log(sin(i));
//	 log(cos(10*i) + 10,2);
//	 log(cos(i/2) + 10,3);
//	 log(sin(i/2) + 10,4);
//	 log(tan(i/2) + 10,5);
//	 log(tan(i/2) + 10,6);
//	 log((i/2) + 10,7);
//
// }

}


void IHM_Logger::filterRow(int _rowId)
{
     QRegExp *reg[NB_COLUMN_TABLE];
     for(int  i=0; i <NB_COLUMN_TABLE; i++)
     {
      reg[i] = new QRegExp(this->m_filters[i]->text());
  
       }

     bool display = true;
      for(int  j=0; j <NB_COLUMN_TABLE; j++)
      {
//      qDebug() << ui->tableLog->item(i,j)->text() <<  reg[j]->exactMatch(ui->tableLog->item(i,j)->text())  << "   " << ui->tableLog->item(i,j)->text().contains(*reg[j]) <<endl;
       if(!ui->tableLog->item(_rowId,j)->text().contains(*reg[j]))
      {
       //qDebug() << ui->tableLog->item(_rowId,j)->text() <<endl;
        display = false;
       }
       
      }
       //qDebug() << "    " << display <<endl;
       ui->tableLog->setRowHidden(_rowId,!display);
}

void IHM_Logger::filter()
{

 
   int rowNb = ui->tableLog->rowCount();
    for(int  i=0; i <rowNb; i++)
    {
             this->filterRow(i);
      
      
    }
 
 
}


void IHM_Logger::log(QString _message, int _moduleId, int _logLevel)
{
	_logLevel = constrain(_logLevel,LOG_LEVEL_NOOUTPUT,LOG_LEVEL_VERBOSE);

     int rowId = ui->tableLog->rowCount();
     ui->tableLog->setRowCount(rowId+1);
     

     QTableWidgetItem *messageItem = new QTableWidgetItem(IHM_Logger::m_logLevelText[_logLevel]);
     ui->tableLog->setItem(rowId,COLUMN_LEVEL,messageItem);
     
     messageItem = new QTableWidgetItem(_message);
     ui->tableLog->setItem(rowId,COLUMN_MESSAGE,messageItem);
     
     messageItem = new QTableWidgetItem(this->m_logger->getSymbolName(_moduleId));
     ui->tableLog->setItem(rowId,COLUMN_MODULE,messageItem);
 
     this->filterRow(rowId);    
}


void IHM_Logger::closeSerialPort()
{
    this->m_logger->closeSerialPort();

//    ui->actionConnect->setEnabled(true);
//    ui->actionDisconnect->setEnabled(false);
//    ui->actionConfigure->setEnabled(true);
//    ui->statusBar->showMessage(tr("Disconnected"));
}

void IHM_Logger::log(float _value, int _moduleId, int _signalId , int _logLevel)
{
	QString v;
	 v.setNum (_value, 'f',2);

QString serieName = this->m_logger->getSymbolName(_moduleId, _signalId);


  this->log(
		  serieName + " => " + v,
		  _moduleId, _logLevel
  );


  

  if(this->m_series[serieName].size() == 0)
  {
	  this->m_seriesTime[serieName].push_back( 0 );
	  this->m_seriesPlot[serieName] =  new QwtPlotCurve(serieName);
	  this->m_seriesPlotWidget[serieName] = new QwtPlot(this);
	  this->m_graphLayout->addWidget(this->m_seriesPlotWidget[serieName]);
//	  QwtPlotMagnifier* magnifier = new QwtPlotMagnifier(this->m_seriesPlotWidget[serieName]->canvas());
//	  magnifier->setMouseButton(Qt::RightButton);

	  this->m_seriesPlotWidget[serieName]->setAxisTitle(QwtPlot::xBottom, serieName);
  }

  this->m_seriesTime[serieName].push_back(this->m_seriesTime[serieName][this->m_seriesTime[serieName].size()-1] + 1);
  this->m_series[serieName].push_back(_value);
  this->m_seriesPlot[serieName]->setRawSamples(
		  &(this->m_seriesTime[serieName])[0],
		  &(this->m_series[serieName])[0],
		  (int)this->m_series[serieName].size());
  this->m_seriesPlot[serieName]->attach(this->m_seriesPlotWidget[serieName]);
  this->m_seriesPlotWidget[serieName]->replot();
  //this->ui->mainPlot->setCurveData( this->m_seriesPlot[serieName], x, sn, &(this->m_series[serieName])[0] );

}

IHM_Logger::~IHM_Logger()
{
    
}

