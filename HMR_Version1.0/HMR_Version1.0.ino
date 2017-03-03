
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
#include <dht11.h>
#include "DHT11_func.h"

dht11 DHT11;


void setup()
{
  Serial.begin(9600);
  Serial.println("DHT11 TEST PROGRAM ");
  Serial.print("LIBRARY VERSION: ");
  Serial.println(DHT11LIB_VERSION);
  Serial.println();
}

void loop()
{
  Serial.println("\n");

  int chk = DHT11.read(DHT11PIN);

  Serial.print("Read sensor: ");
  switch (chk)
  {
    case DHTLIB_OK: 
                Serial.println("OK"); 
                break;
    case DHTLIB_ERROR_CHECKSUM: 
                Serial.println("Checksum error"); 
                break;
    case DHTLIB_ERROR_TIMEOUT: 
                Serial.println("Time out error"); 
                break;
    default: 
                Serial.println("Unknown error"); 
                break;
  }

  Serial.print("Humidity (%): ");
  Serial.println((float)DHT11.humidity, 2);

  Serial.print("Temperature (oC): ");
  Serial.println((float)DHT11.temperature, 2);

  Serial.print("Temperature (oF): ");
  Serial.println(Fahrenheit(DHT11.temperature), 2);

  Serial.print("Temperature (K): ");
  Serial.println(Kelvin(DHT11.temperature), 2);

  Serial.print("Dew Point (oC): ");
  Serial.println(dewPoint(DHT11.temperature, DHT11.humidity));

  Serial.print("Dew PointFast (oC): ");
  Serial.println(dewPointFast(DHT11.temperature, DHT11.humidity));

  delay(2000);
}



//#include <Time.h>
//#include <TimeAlarms.h>
//
//#define TIME_MSG_LEN  11   /* 由一个首标+10位ASCII组成的时间同步信息*/
//#define TIME_HEADER   'T' /* 系列时间同步信息的首标  */
//#define TIME_GMT_8    28800   /* 东八区，快8小时=28,800秒 */
//
//void setup() {
//  // put your setup code here, to run once:
//  Serial.begin(9600);
//  Serial.println(" Waiting for time sync message" );
//} // end setup
//
//bool isSetTimerRepeatNow = false;
//bool isSetTimerRepeatOK = false;
//void RepeatTask(){
//  Serial.println( "3 minutes timer" );
//}
//
//void loop() {
//  // put your main code here, to run repeatedly:
//  if ( Serial.available() ){
//      processSyncMessage();
//      isSetTimerRepeatNow = true;
//  }
//  if( isSetTimerRepeatNow == true){
//      if(isSetTimerRepeatOK == false){
//          Alarm.timerRepeat(300, RepeatTask);     // 5min 的计时器
//          Serial.println( "Setted the Repeat Timer" );
//          isSetTimerRepeatOK = true;
//      }// end if
//      isSetTimerRepeatNow = false;
//  }
//  if ( timeStatus() != timeNotSet ){
//      digitalClockDisplay();
//      Serial.println();
//  }
//  Alarm.delay(1000);
//} // end loop
//
//void processSyncMessage(){
//    // 如果由串口提供时间同步，就更新时间和日期，并返回真值？
//    // 由一个首标和10位ASCII 组成的时间信息
//    if (Serial.available() == TIME_MSG_LEN ){
//        char c = Serial.read();
//        Serial.print( c );
//        if( c == TIME_HEADER ){
//            time_t pctime = 0;
//            for ( int i=0; i<TIME_MSG_LEN-1; i++){
//                c = Serial.read();
//                if( isDigit(c) ){
//                    pctime = (10*pctime) + (c-'0');         // 转化10位ASCII 到数字
//                } // end if
//            } // end for
//            pctime += TIME_GMT_8;
//            setTime( pctime );
//        } // end if
//    }else{
//      while( Serial.available()>0 ) Serial.read();
//      Serial.println( "Your enter is NOT right format, Please enter again! " );
//    } // end if...else... 
//    
//} // end func 
//
//
//void digitalClockDisplay() {
//  // 时间的数字时钟显示
//  Serial.print( hour() );
//  printDigits( minute() );
//  printDigits( second() );
//  Serial.print( " " );
//  Serial.print( day() );    Serial.print( " " );
//  Serial.print( month() );  Serial.print( " " );
//  Serial.print( year() );   Serial.print( " " );
//
//} // end func digitalClockDisplay
//
//void printDigits( int digits ) {
//  // 应用时间显示函数：打印前面的冒号和0
//  Serial.print(":");
//  if (digits < 10 )
//    Serial.print('0');
//  Serial.print(digits );
//} // end func printDigits
//
//



