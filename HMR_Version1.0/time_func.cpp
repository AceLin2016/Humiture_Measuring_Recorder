


#include "time_func.h"

void myTime_init(myTime_t *_compltTime){
    _compltTime->mYear = 1970;
    _compltTime->mMonth = 0;
    _compltTime->mDay = 0;
    _compltTime->mHour = 0;
    _compltTime->mMinute = 0;
    _compltTime->mSecond = 0;
} // end func init

myTime_t getDate(myTime_t me, int _year, int _month, int _day){
    myTime_t temp = me;
    temp.mYear = _year;
    temp.mMonth = _month;
    temp.mDay = _day;

    return temp;
} // end func 

myTime_t getTime(myTime_t me, int _hour, int _minute, int _second){
    myTime_t temp = me;
    temp.mHour = _hour;
    temp.mMinute = _minute;
    temp.mSecond = _second;
    return temp;
} // end func 
