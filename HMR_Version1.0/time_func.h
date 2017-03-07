


#ifndef TIME_FUNC_H
#define TIME_FUNC_H


typedef struct myTime myTime_t;
struct myTime{
    int mYear,
        mMonth,
        mDay;
    int mHour,
        mMinute,
        mSecond;
};

void myTime_init(myTime_t *_compltTime);

myTime_t getDate(myTime_t me, int _year, int _month, int _day);
myTime_t getTime(myTime_t me, int _minute, int _second);



#endif /* TIME_FUNC_H */

