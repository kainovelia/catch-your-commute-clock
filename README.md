# Catch-Your-Commute-Clock
A ESP32 powered alarm clock that will help you catch your commute in real-time. Based on NYC MTA public transit info.
For more details about my creative process, visit [my notion page](https://wool-dungeon-a3b.notion.site/Catch-Your-Commute-Alarm-Clock-15bb81cc7c2780868961fb1c537bb210)


https://github.com/user-attachments/assets/113cec41-e696-4e0c-a4b0-b98049d00a72


### Hardware:
- [LilyGo T-Display board](https://www.lilygo.cc/products/lilygo%C2%AE-ttgo-t-display-1-14-inch-lcd-esp32-control-board?srsltid=AfmBOor7mrm_vmfWB6wu8yITau-LUsnGRL8LQiT9-7cyLsRfz-yet-uV) (with headers)
- USB-C cable
- A breadboard
- 2 Pushbuttons
- 1 [KY-023 Dual Axis Joystick Module](https://arduinomodules.info/ky-023-joystick-dual-axis-module/)

### Software:
- [Arduino IDE](https://www.arduino.cc/en/software)
- Protocol Buffer Compiler for Python
- [LilyGo T-Display USB Driver](https://github.com/Xinyuan-LilyGO/TTGO-T-Display)
 
## Setup
- Download the correct driver for the LilyGo T-Display
- Ensure USB-C cable can connect the board and be recognized by Arduino IDE
- Solder the headers into both sides of the board (from beneath, short side up)
- Fit the board headers into the breadboard
- Wire the components as shown in the diagram below

![Circuit Diagram3_bb](https://github.com/user-attachments/assets/c935cde3-3ccf-45eb-91db-c632e82af710)

## Arduino Program
- Open the file 'commute_clock.ino'
- Select port connected to wired board
- Change the ssid and password to connect to your local WiFi network
- Change other variables, such as MTA route url and station ID
- Click 'Upload'
- Check Serial output in 'Serial Monitor' (most top-right icon)
- Debug wire connections, as needed

## Protocol Buffers for GTFS File, Compiled For Python
- Install Python
- Install PySerialTransfer
  ```
  pip install pyserial
  ```
- Install requests
  ```
  pip install requests
  ```
- Install latest release of protobuf following the instructions from [https://grpc.io/docs/protoc-installation/]
  (Installing with package manager may not give the most up to date release -- you need protoc v3.19 or higher)
- In the same folder as the python app, download the base gtfs-rt.proto file[https://github.com/google/transit/blob/master/gtfs-realtime/proto/gtfs-realtime.proto] and the MTA's version of gtfs-rt.proto[https://raw.githubusercontent.com/OneBusAway/onebusaway-gtfs-realtime-api/master/src/main/proto/com/google/transit/realtime/gtfs-realtime-NYCT.proto]
- Compile both files with protoc
  ```
  protoc --python_out=. your_proto_file.proto
  ```
  'python_out=.' points to your current directory, so be sure to run it in there

## Enclosure Design
- Open the provided STL files in Ultimaker Cura
- Adjust the print settings and send the files over to a compatible 3D printer
- Remove the 3D print and clean up the edges
- Secure the wire connections between the hardware components (by soldering or heatshrink wrapping)
- Add heatsets to the provided holes in each corner of the main enclosure and the lid.
- Attach the hardware components to the outer shell of the main enclosure. Depending on the type of component, you can glue or securely screw mount them into place.
