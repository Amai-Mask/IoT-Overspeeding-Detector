# IoT Overspeeding Detector 🚗💨

An IoT-based overspeeding detector uses an ultrasonic sensor to monitor vehicle speed within a certain range. If overspeeding is detected, a notification is sent to a Telegram bot, and data can also be monitored via ThingSpeak.

## Features
- **Real-time Speed Detection:** Detects overspeeding using an ultrasonic sensor.
- **Alerts and Notifications:** Sends instant alerts to a Telegram bot when overspeeding occurs.
- **Data Monitoring:** Logs speed data on ThingSpeak for analysis.
- **Visual Indicators:** Uses LEDs and a buzzer for local alerts.

## Components
- **NodeMCU (ESP8266):** Main microcontroller for IoT connectivity.
- **Ultrasonic Sensors:** Measure speed and distance.
- **16x2 LCD Display:** Displays real-time speed and status.
- **100Ω Resistors:** Current limiting for LEDs.
- **Red & Green LEDs:** Indicate normal and overspeed conditions.
- **Buzzer:** Audible alert for overspeeding.
- **Jumper Wires:** Connections between components.

## Circuit Diagram
<div align="center">
<img src="Images/Hardware.jpg" alt="Circuit Diagram" width="700"/>
</div>

## How to Use
1. Assemble the circuit as shown in the diagram.
2. Upload the code to the NodeMCU using the Arduino IDE.
3. Configure the ThingSpeak and Telegram bot credentials in the code.
4. Power the circuit and observe the speed readings on the LCD.
5. Notifications for overspeeding will be sent to the Telegram bot and logged on ThingSpeak.

## Tools and Libraries
- Arduino IDE
- ThingSpeak API
- Telegram Bot API
- LiquidCrystal library for LCD

## Demonstration
<div align="center">
  <img src="Images/Device_1.jpg" alt="Picture 1" width="330" height ="300"/>
  <img src="Images/Device_3.jpg" alt="Picture 2" width="330"height ="300"/>
  <img src="Images/Device_2.jpg" alt="Picture 3" width="330"height ="300"/>
</div>

## Output In Console
<div align="center">
<img src="Images/Running_Output.png" alt="Console Output" width="450" height ="450"/>
</div>

## ThingSpeak Cloud Output and Graph
<div align="center">
<img src="Images/ThingSpeak_Output.png" alt="Cloud Output" width="900"/>
</div>

## Telegram Bot
<div align="center">
<img src="Images/Telegram_Bot.jpg" alt="Tbot" width="350" height = "350"/>
</div>
<div align="center">
  <img src="Images/Telegram _Bot_Messages.jpg" alt="T1" width="400" height ="700"/>
  <img src="Images/Notification.png" alt="T2" width="400"height ="700"/>
</div>

## Summary
This IoT Overspeeding Detector combines real-time speed monitoring with efficient notification and data logging mechanisms. By leveraging ultrasonic sensors, LEDs, and IoT platforms like Telegram and ThingSpeak, it provides a practical solution for enhancing road safety and monitoring vehicle behavior effectively.


