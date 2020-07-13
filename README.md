# Electrical Water Heater
this project is implementaion of the Swift Act requirment of Electrical Water Heater [link](Kit_info\Electric Water Heater.pdf).
to view full documentation as pdf  [full_doc](https://github.com/Abdelbary/ElectricWaterHeater/blob/master/documentation/doc.pdf).
## Demo
### click image to check demo video 
<div style="text-align:center">[![Demo](picture\demo.png)](https://www.youtube.com/watch?v=F_yb4pN2a-Q)</div>
## Dependencies:
1. PICSimlab simulation program [link](https://github.com/lcgamboa/picsimlab).
2. project developed using MPLAB X IDE from microship.
3. xc8 compiler from microchip.

##### project documented useing doxygen documentaion in docmentation/html/index.html 
## Main Components:
### Hardware:
The project emulate Electrical Water Heater on PICSimlab simulation program using PicGenios kit with PIC16F877A.
<div style="text-align:center">![project schmatic](picture\ProjectSchematic.png)</div>

# Software Components:
### Project Static Architecutre:
_Note_: Application calls MCAL directly to inialize the state of LED, Heater and Cooler to off state as there is no 
HAL driver for thoes peripheral_i saw that those peripheral logic is too simple to make a sperated driver for them_.
<div style="text-align:center">![Static Architecture](picture\projectStaticArchitecture.png)![init fun](F:\Carier\Embedded\PIC\ElectricalWaterHeater\picture\initFun.png)</div>

### Project Detailed Design:
check detailed Design in [pdf](Detailed Design.pdf)
<div style="text-align:center">![DetailedDesign](picture\DetailedDesign.png)</div>

### Task-TimeLine:
* Project uses non-premative operating system with 50ms tick time.
* task peroidic time specified in the below graph.
* project has five diffferent tasks {tempTask,ButtonTask,tempControlTask,sevenSegTask} thier pirority as thier listing 
  order with highest pirority task [tempTask] and lowest proirty is [sevenSegTask].
* the system repeat it's operation every 100ms _Major Cycle_.
* os prehook runs tempTask once to be prepare current temp for seven segment display befor it's calling.
* operation time of each task is neglicatable _no blocking for considerable amount of time_.

<div style="text-align:center">![timeLine.png](picture\timeLine.png)</div>

### Operating System:
This project uses a non premative OS with a periodic task, the os prority feature is turned off as a result task take it's
prority from the order of it's creation realative to other tasks.
The project has five diffferent tasks {tempTask,ButtonTask,tempControlTask,sevenSegTask} thier pirority as thier listing 
order with highest pirority task [tempTask] and lowest proirty is [sevenSegTask].

### System Tasks:

#### tempTask:
* check if user setted a tempreature
* if a temp is setted then
	1. get average temp of the last ten readings.
	2. take an action based on the state of the heater and cooler state and the setted temp value.
* if the cooler element is on turn on the led, if the heater element is on blink led every 1 sec.


#### ButtonTask:
* call button manager.
* update the state of each button{up-down-on/off}.
* if the on off button is pressed switch the system {on/off} respectivly and exit task.
* if in normal mode and up or down button pressed get the last setted value from EXT_EEPROM and change mode to TEMP_SET_MOD.
* if in temp set mode and up or down button pressed increase or decrease the temp to set by 5 depending on which button pressed respectivily.
* if neither the up or down button pressed save the setted temp to EXT_EEPROM and change mode to normal mode.

#### tempControlTask:
* get the latest adc converted value form the ADC chanal connected to temp sensor.
* calculate the current temp in clsius.
* save current temp in _last 10 temp reading_ array.

#### sevenSegTask:
* if in normal mode display the current temp value.
* if in temp  setted mode flash led every 1sec and display last setted temp
  * interact with user if up or down button pressed change the temp to set by 5 degrees above or blow the current temp based on the button pressed.
  * Max temp 75 and Min temp to set is 35.
