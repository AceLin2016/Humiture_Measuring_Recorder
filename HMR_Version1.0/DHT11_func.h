

#ifndef DHT11_FUNC_H
#define DHT11_FUNC_H

#include <Arduino.h>
#include <dht11.h>          /* DHT11 库 */

#define DHT11PIN    2   /* DHT11 DATA脚连接Arduino 的引脚 */


double Fahrenheit(double celsius);
double Kelvin(double celsius);

double dewPoint(double celsius, double humidity);
double dewPointFast(double celsius, double humidity);


#endif /* DHT11_FUNC_H */
