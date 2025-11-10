#include <Arduino.h>
#include <SimpleModbusMaster.h>

/* 
   SimpleModbusMaster allows you to communicate
   to any slave using the Modbus RTU protocol.
  
   To communicate with a slave you need to create a packet that will contain 
   all the information required to communicate to the slave.
   Information counters are implemented for further diagnostic.
   These are variables already implemented in a packet. 
   You can set and clear these variables as needed.
   
   The following modbus information counters are implemented:    
   
   requests - contains the total requests to a slave
   successful_requests - contains the total successful requests
   failed_requests - general frame errors, checksum failures and buffer failures 
   retries - contains the number of retries
   exception_errors - contains the specific modbus exception response count
   These are normally illegal function, illegal address, illegal data value
   or a miscellaneous error response.
  
   And finally there is a variable called "connection" that 
   at any given moment contains the current connection 
   status of the packet. If true then the connection is 
   active. If false then communication will be stopped
   on this packet until the programmer sets the connection
   variable to true explicitly. The reason for this is 
   because of the time out involved in modbus communication.
   Each faulty slave that's not communicating will slow down
   communication on the line with the time out value. E.g.
   Using a time out of 1500ms, if you have 10 slaves and 9 of them
   stops communicating the latency burden placed on communication
   will be 1500ms * 9 = 13,5 seconds!  
   Communication will automatically be stopped after the retry count expires
   on each specific packet.
  
   All the error checking, updating and communication multitasking
   takes place in the background.
  
   In general to communicate with to a slave using modbus
   RTU you will request information using the specific
   slave id, the function request, the starting address
   and lastly the data to request.
   Function 1, 2, 3, 4, 15 & 16 are supported. In addition to
   this broadcasting (id = 0) is supported for function 15 & 16.
	 
   Constants are provided for:
   Function 1  - READ_COIL_STATUS
   Function 2  - READ_INPUT_STATUS
   Function 3  - READ_HOLDING_REGISTERS 
   Function 4  - READ_INPUT_REGISTERS
   Function 15 - FORCE_MULTIPLE_COILS
   Function 16 - PRESET_MULTIPLE_REGISTERS 
   
   Note:  
   The Arduino serial ring buffer is 64 bytes or 32 registers.
   Most of the time you will connect the Arduino using a MAX485 or similar.
 
   In a function 3 or 4 request the master will attempt to read from a
   slave and since 5 bytes is already used for ID, FUNCTION, NO OF BYTES
   and two BYTES CRC the master can only request 58 bytes or 29 registers.
 
   In a function 16 request the master will attempt to write to a 
   slave and since 9 bytes is already used for ID, FUNCTION, ADDRESS, 
   NO OF REGISTERS, NO OF BYTES and two BYTES CRC the master can only write
   54 bytes or 27 registers.
    
   Note:
   Using a USB to Serial converter the maximum bytes you can send is 
   limited to its internal buffer which differs between manufactures. 
 
   Since it is assumed that you will mostly use the Arduino to connect without
   using a USB to Serial converter the internal buffer is set the same as the
   Arduino Serial ring buffer which is 64 bytes.
   
   The example will use packet1 to read a register from address 0 (the adc ch0 value)
   from the arduino slave (id=1). It will then use this value to adjust the brightness
   of an led on pin 9 using PWM.
   It will then use packet2 to write a register (its own adc ch0 value) to address 1 
   on the arduino slave (id=1) adjusting the brightness of an led on pin 9 using PWM.
*/

//////////////////// Port information ///////////////////
#define baud 9600
#define timeout 1000
#define polling 200 // the scan rate

// If the packets internal retry register matches
// the set retry count then communication is stopped
// on that packet. To re-enable the packet you must
// set the "connection" variable to true.
#define retry_count 10

// used to toggle the receive/transmit pin on the driver
#define TxEnablePin 2 

#define LED 9

// This is the easiest way to create new packets
// Add as many as you want. TOTAL_NO_OF_PACKETS
// is automatically updated.
enum
{
  PACKET1,
  PACKET2,
  TOTAL_NO_OF_PACKETS // leave this last entry
};

// Create an array of Packets to be configured
Packet packets[TOTAL_NO_OF_PACKETS];

// Create a packetPointer to access each packet
// individually. This is not required you can access
// the array explicitly. E.g. packets[PACKET1].id = 2;
// This does become tedious though...
packetPointer packet1 = &packets[PACKET1];
packetPointer packet2 = &packets[PACKET2];

// Data read from the arduino slave will be stored in this array
// if the array is initialized to the packet.
unsigned int readRegs[1];

// Data to be written to the arduino slave
unsigned int writeRegs[1];

void setup()
{
  // The modbus packet constructor function will initialize
  // the individual packet with the assigned parameters. You can always do this
  // explicitly by using struct pointers. The first parameter is the address of the
  // packet in question. It is effectively the "this" parameter in Java that points to
  // the address of the passed object. It has the following form:
  // modbus_construct(packet, id, function, address, data, register array)
  
  // For functions 1 & 2 data is the number of points
  // For functions 3, 4 & 16 data is the number of registers
  // For function 15 data is the number of coils
  
  // read 1 register starting at address 0  
  modbus_construct(packet1, 1, READ_HOLDING_REGISTERS, 0, 1, readRegs);
  
  // write 1 register starting at address 1  
  modbus_construct(packet2, 1, PRESET_MULTIPLE_REGISTERS, 1, 1, writeRegs);
  
  // P.S. the register array entries above can be different arrays
  
  /* Initialize communication settings:
     parameters(HardwareSerial* SerialPort,
		long baud, 
		unsigned char byteFormat,
		unsigned int timeout, 
		unsigned int polling, 
		unsigned char retry_count, 
		unsigned char TxEnablePin,
		Packet* packets, 
		unsigned int total_no_of_packets);

     Valid modbus byte formats are:
     SERIAL_8N2: 1 start bit, 8 data bits, 2 stop bits
     SERIAL_8E1: 1 start bit, 8 data bits, 1 Even parity bit, 1 stop bit
     SERIAL_8O1: 1 start bit, 8 data bits, 1 Odd parity bit, 1 stop bit
     
     You can obviously use SERIAL_8N1 but this does not adhere to the
     Modbus specifications. That said, I have tested the SERIAL_8N1 option 
     on various commercial masters and slaves that were suppose to adhere
     to this specification and was always able to communicate... Go figure.
     
     These are already defined in the Arduino global name space. 
  */
  modbus_configure(&Serial, baud, SERIAL_8N2, timeout, polling, retry_count, TxEnablePin, packets, TOTAL_NO_OF_PACKETS);
  
  pinMode(LED, OUTPUT);
}

void loop()
{
  modbus_update();
  
  writeRegs[0] = analogRead(A0); // update data to be written to arduino slave
  
  analogWrite(LED, readRegs[0]>>2); // constrain adc value from the arduino slave to 255
  
  /* You can check or alter the internal counters of a specific packet like this:
     packet1->requests;
     packet1->successful_requests;
     packet1->failed_requests;
     packet1->exception_errors;
     packet2->requests;
     packet2->successful_requests;
     packet2->failed_requests;
     packet2->exception_errors;
  */
}