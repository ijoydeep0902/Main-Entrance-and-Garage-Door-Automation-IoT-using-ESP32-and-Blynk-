# Main Entrance and Garage door Automation {IOT} (using ESP 32 and Blynk Mobile Control)

Implemented IoT automation with ESP32 and Blynk for Main Entrance and
Garage Door, enabling remote control via mobile devices. Proficient in IoT, microcontroller programming, and seamless mobile app integration for efficient
home automation.


# DEveloped By Joy
## important infos:
 
 (i)  adjusting the sensor distances----->find{ctrl+f} "adjust" it will lead you to the code line
 (ii) for testing the sensors S
      step-1> paste these line of codes in "loop" function 
          detectObstacle() ;                         
          Garage_sensorgatestate();
          Blynk.virtualWrite(V5,"\n\n"); 
      step-2>uncomment by finding "testing ultra sonic sensors." in detectobstracle() & Garage_sensorgatestate()
