# Electrical Water Heater
The project consist of five tasks using Small Operating system that operates on a 50ms tick timer
## Main Components:
### Operating System:
This project uses a non premative OS with a periodic task, the os prority feature is turned off as a result task take it's
prority from the order of it's creation realative to other tasks.
The project has five diffferent tasks {tempTask,ButtonTask,tempControlTask,sevenSegTask} thier pirority as thier listing 
order with highest pirority task [tempTask] and lowest proirty is [sevenSegTask].


### tempTask:
* check if user setted a tempreature
* if a temp is setted then-->
..1. get average temp of the last ten readings.
..2. take an action based on the state of the heater and cooler state and the setted temp value.
* if the cooler element is on turn on the led, if the heater element is on blink led every 1 sec.


### ButtonTask:
* call button manager.
* update the state of each button{up-down-on/off}.
* if the on off button is pressed switch the system {on/off} respectivly and exit task.
* if in normal mode and up or down button pressed get the last setted value from EXT_EEPROM and change mode to TEMP_SET_MOD.
* if in temp set mode and up or down button pressed increase or decrease the temp to set by 5 depending on which button pressed respectivily.
* if neither the up or down button pressed save the setted temp to EXT_EEPROM and change mode to normal mode.

### tempControlTask:
* get the latest adc converted value form the ADC chanal connected to temp sensor.
* calculate the current temp in clsius.
* save current temp in _last 10 temp reading_ array.

### sevenSegTask:
* if in normal mode display the current temp value.
* if in temp  setted mode flash led every 1sec and display last setted temp
...interact with user if up or down button pressed change the temp to set by 5 degrees above or blow the current temp based on the button pressed.
...Max temp 75 and Min temp to set is 35.

##Task-TimeLine:
* Project uses non-premative operating system with 50ms.
* task peroidic time specified in the below graph.
* project has five diffferent tasks {tempTask,ButtonTask,tempControlTask,sevenSegTask} thier pirority as thier listing 
  order with highest pirority task [tempTask] and lowest proirty is [sevenSegTask].
* the system repeat it's operation every 100ms _Major Cycle_.
* os prehook runs tempTask once to be prepare current temp for seven segment display befor it's calling.
* operation time of each task is neglicatable _no blocking for considerable amount of time_.
![timeLine](picture/timeLine.png)

## Project Static Architecutre:
_Note_: Application calls MCAL directly to inialize the state of LED, Heater and Cooler to off state as there is no 
HAL driver for thoes peripheral_i saw that those peripheral logic is too simple to make a sperated driver for them_.
![timeLine](picture/projectStaticArchitecture.png)