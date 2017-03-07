
/*
 * 使用时间库来显示一天内的时间。
 * 时间库可以从这里下载：
 *  http://www.arduino.cc/playground/Code/Time
*/

/*
 * Time 程序
 * Project -> Include Library -> Manage Library -> Time -> Install
*/

/* 
 * #更新# 通过串口输入Unix时间来同步当前时间。
 * #2017/3/2 16”57 更新#  加入北京时间 UTC/GMT+08:00 
 * #2017/3/2 23:08 更新#    周期性执行特定操作，而非使用delay的方式
*/
/* 
 *  #2017/3/3 16：36 整合功能# 1、设定时间 2、读取数据 3、写入文件。
 * 
*/


#include <Time.h>
#include <TimeAlarms.h>
#include "time_func.h"

#include <dht11.h>
#include "DHT11_func.h"

#include <SPI.h>
#include <SD.h>

/**  Arduino 中使用String 类 将 string 继承 */
// #include <string>               /* String 类 */
// #include <sstream>              /* 代码中int 转 String 需要用到 */

/*** 时钟部分 ***/
#define TIME_MSG_LEN  11        /* 由一个首标+10位ASCII组成的时间同步信息*/
#define TIME_HEADER   'T'       /* 系列时间同步信息的首标  */
#define TIME_GMT_8    28800     /* 东八区，快8小时=28,800秒 */
myTime_t timeNow;
myTime_t timePre;

/*** DHT11 部分 ***/
dht11 DHT11;                /* 创建DHT11 数据类的实例对象 */
int DHT_chk = 0;            /* 创建监测与DHT11通信成功与否的对象 */

/*** SD卡 部分 ***/
const int chipSelect = 4;  //设定CS接口
bool isNewFile = false;     /*  */

/*** 循环的任务 ***/
bool isProcData = false;
void RepeatTask(){
  Serial.println( "3 minutes timer" );
  isProcData = true;
}

/** 格式： area,date,time,tempreture,humity */
void RecordTime( File _TargetFile, myTime_t _time ){
    String str;
    str = getTargetFileName( _time );                   // 20170307
    str += ",";                                         // 20170307,
    _TargetFile.print(str);

    str = getStringTime( _time );                       // 15:30:11
    str += ",";                                         // 15:30:11,
    _TargetFile.print(str);
    //  *wuxi,*  20170307,15:30:11,                     
}
void RecordData( File _TargetFile, dht11 _data ){
    String str;
    float f_data = (float)_data.temperature;
    str = String( f_data, 2 );                  // 20.00
    str += ",";                                         // 20.00,
    _TargetFile.print(str);

    f_data = (float)_data.humidity;
    str = String( f_data, 2 );                     // 31.00
    _TargetFile.println(str);                           // + Enter
    //  *wuxi,*  20170307,15:30:11,20.00,31.00
}

void setup() {
  // put your setup code here, to run once:
  myTime_init( &timeNow );                       // 初始化用于标记当前时间对象
  myTime_init( &timePre );                       // 初始化用于标记上一时间对象

  Serial.begin(9600);
  Serial.print("Initializing SD card...");
  //测试CS口是否与SD卡通信成功
  if (!SD.begin(chipSelect)){
    Serial.println("Card failed, or not present");
    return;
  } else{   Serial.println("card initialized.");   }
  
  Serial.println(" Waiting for time sync message" );
} // end setup

bool isSetTimerRepeatNow = false;
bool isSetTimerRepeatOK = false;



void loop() {
  // put your main code here, to run repeatedly:
  /* 设定时间？ */
  if ( Serial.available() ){
      processSyncMessage();
      isSetTimerRepeatNow = true;
      isSetTimerRepeatOK = false;
  }
  /* 创建循环任务？ */
  if( isSetTimerRepeatNow == true){
      if(isSetTimerRepeatOK == false){
          Alarm.timerRepeat(300, RepeatTask);     // 5min 的计时器
          Serial.println();
          Serial.println( "Setted the Repeat Timer completed" );
          isSetTimerRepeatOK = true;
      }// end if
      isSetTimerRepeatNow = false;
  }
  /*老版原子测试部分代码*/
  if ( timeStatus() != timeNotSet ){
      digitalClockDisplay();
      Serial.println();
  }
  /* 判断是否做读取数据并处理的工作 */
  if( isProcData == true ){
      // 放置你的处理代码在这里...
      /* 读取当前时间值 */
       timeNow = getTimeNow();
      /* 读取DHT11的数据 */
       DHT_chk = DHT11.read(DHT11PIN);              // 如果成功读取到数据的话，数据已经保存在类的实例对象DHT11 中了
       // Result_ReadDataMesg_Proc();                  // ？
      /* 打开哪个文件用于写入数据？*/
       String fileName = getTargetFileName(timeNow);       // 格式：20170304.csv Done
       fileName += ".csv";                                  /* + 文件后缀 */
       isNewFile = isOpenFileNew(timeNow, timePre);            // done!
       if( SD.exists( fileName ) ){  isNewFile = false;    }
      /* 打开文件 */
       File TargetFile = SD.open(fileName, FILE_WRITE);
      /* 写入数据 */
       if( TargetFile ){ // 数据流非NULL-> 打开文件成功
            /* 判断是否是新文件 */
            //timePre = getPreRecordTime();
            if( isNewFile == true ){ // 是
                Serial.println("New day, New File");
                /* 写入键值 */
                TargetFile.println("area,date,time,tempreture,humity");
                isNewFile = false;          // 处理完标志位归位
            } // 否 do nothing
            // 写入数据的代码...
            TargetFile.print("wuxi,");                  // wuxi,
            RecordTime( TargetFile, timeNow );          // wuxi,20170307,15:30:11,
            RecordData( TargetFile, DHT11 );            // wuxi,20170307,15:30:11,20.00,31.00
            /* 关闭文件 */
            TargetFile.close();
            timePre = timeNow;              // setPreRecordTime( &timePre, timeNow );
            Serial.println("Writed over!");
       }else{
           Alarm.delay(0);      // do noting now
       }      

      // 处理完毕，标志位复位！
      isProcData = false;
  } // end if is Proc Data == true
  /* 调用Alarm.delay() 来保证Alarm类中创建的任务被按时执行 */
  Alarm.delay(2000);
} // end loop




void processSyncMessage(){
    // 如果由串口提供时间同步，就更新时间和日期，并返回真值？
    // 由一个首标和10位ASCII 组成的时间信息
    if (Serial.available() == TIME_MSG_LEN ){
        char c = Serial.read();
        Serial.print( c );
        if( c == TIME_HEADER ){
            time_t pctime = 0;
            for ( int i=0; i<TIME_MSG_LEN-1; i++){
                c = Serial.read();
                if( isDigit(c) ){
                    pctime = (10*pctime) + (c-'0');         // 转化10位ASCII 到数字
                } // end if
            } // end for
            pctime += TIME_GMT_8;
            setTime( pctime );
        } // end if
    }else{
      while( Serial.available()>0 ) Serial.read();
      Serial.println( "Your enter is NOT right format, Please enter again! " );
    } // end if...else... 
    
} // end func 


void digitalClockDisplay() {
  // 时间的数字时钟显示
  Serial.print( hour() );
  printDigits( minute() );
  printDigits( second() );  Serial.print( " " );
  Serial.print( day() );    Serial.print( " " );
  Serial.print( month() );  Serial.print( " " );
  Serial.print( year() );   Serial.print( " " );

} // end func digitalClockDisplay

void printDigits( int digits ) {
  // 应用时间显示函数：打印前面的冒号和0
  Serial.print(":");
  if (digits < 10 )
    Serial.print('0');
  Serial.print(digits );
} // end func printDigits



myTime_t getTimeNow(){
    myTime_t _temp;    myTime_init( &_temp );
    //_temp = getDate( _temp, year(), month(), day() );
    _temp.mYear=year(); _temp.mMonth=month(); _temp.mDay=day();
    _temp.mHour=hour(); _temp.mMinute=minute(); _temp.mSecond=second();
    return _temp;
}

bool setPreRecordTime(myTime_t *PreT,myTime_t _myTime){
    *PreT = _myTime;
    return true;
}
myTime_t getPreRecordTime(myTime_t *PreT){
    return *PreT;
}



bool isOpenFileNew(myTime_t _now, myTime_t _pre){
    if( _now.mYear>_pre.mYear ){
        return true;
    }else if(_now.mMonth>_pre.mMonth){
        return true;
    }else if(_now.mDay>_pre.mDay){
        return true;
    }else{ // 这是同一天的记录
        return false;
    }
}


/** 格式：20170304 */ /**  .csv   */
String getTargetFileName( myTime_t _timeNow ){
    String FileName, Stemp;

    FileName = String( _timeNow.mYear );

    if( _timeNow.mMonth < 10 ){
        Stemp = String( 0 );
        Stemp += String( _timeNow.mMonth );
    }
    FileName += Stemp;

    if( _timeNow.mDay < 10 ){
        Stemp = String( 0 );
        Stemp += String( _timeNow.mDay );
    }
    FileName += Stemp;
    return FileName;
} // end func

String getStringTime( myTime_t _timeNow ){
    String StrTime, Stemp;

    if( _timeNow.mHour < 10 ){
        StrTime = String( 0 );
        StrTime += String( _timeNow.mHour );
    }else{ StrTime = String(_timeNow.mHour); }
    StrTime += ":";

    if( _timeNow.mMinute < 10 ){
        StrTime += String( 0 );                   // Stemp = String( 0 );
    }
    StrTime += String( _timeNow.mMinute );    // Stemp += String( _timeNow.mMinute );
    // StrTime += Stemp;
    StrTime += ":";

    if( _timeNow.mSecond < 10 ){
        StrTime += String( 0 );                   // Stemp = String( 0 );
    }
    StrTime += String( _timeNow.mSecond );    // Stemp += String( _timeNow.mSecond );
    // StrTime += Stemp;
    return StrTime;
}




