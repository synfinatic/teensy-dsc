
/* Matt Woodward, June 2013
 * Initialize WiFly for serial-over-wifi use for Astronomy DSC apps
 */

#include <Arduino.h>
#include <Streaming.h>
#include <SoftwareSerial.h>
#include "WiFlySerial.h"
WiFlySerial WiFly(8, 9);

int LED = 13;
int inByte;

#define IP_ADDRESS "10.0.0.1"
#define PORT "4030"
#define PAUSE "pause"
#define PAUSE_DURATION 100
#define LONG_PAUSE "longpause"
#define LONG_PAUSE_DURATION 500
#define SSID_KEY "SSID:"
#define SSID "telescope"
#define DONE "done"

void execute_wifi_commands(char ** commands);
void setup_wifi();

void setup() 
{ 
  setup_wifi();

  // setup for builtin LED
  pinMode (13, OUTPUT);
} 

void loop() { 
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  delay(1000);
}

void setup_wifi()
{
  char * commands[] = 
    {
      "factory RESET",
      LONG_PAUSE,
      "set uart mode 0x03",
      "set uart baudrate 115200",
      "set wlan join 7",
      SSID_KEY SSID,
      "set dhcp lease 2000",
      "set ip address " IP_ADDRESS,
      "set ip gateway " IP_ADDRESS,
      "set ip localport " PORT,
      "set dns address " IP_ADDRESS,
      "set ip protocol 2",
      "set ip netmask 255.255.255.0",
      "set ip dhcp 4",
      "set wlan tx 1",
      "set sys iofunc 0x7",
      "set comm remote 0",
      "set comm open 0",
      "save",
      PAUSE, 
      "reboot",
      LONG_PAUSE,
      DONE
    };
  
  WiFly.getDeviceStatus();
  execute_wifi_commands(commands);
  WiFly.closeConnection();

  WiFly.flush();
  while(WiFly.available())
    {
      WiFly.read();
    }
}

#define REQUEST_BUFFER_SIZE 120

void execute_wifi_commands(char * commands[])
{
  int cmd = 0;
  char * command = commands[cmd++];
  char bufRequest[REQUEST_BUFFER_SIZE];
  
  while(0 != strcmp(command, DONE))
    {
      if(0 == strncmp(command, SSID_KEY, strlen(SSID_KEY)))
      {
        WiFly.getMAC(bufRequest, REQUEST_BUFFER_SIZE);
        if(*bufRequest)
        {
          char ssid_command[100];
          sprintf(ssid_command, "set wlan ssid %s-%s", command + strlen(SSID_KEY), bufRequest + strlen(bufRequest)-2);
      	  WiFly.SendCommandSimple(ssid_command, "AOK");
        }
      }
      if(0 == strcmp(command, LONG_PAUSE))
	{
	  delay(LONG_PAUSE_DURATION);
	}
      else if(0 == strcmp(command, PAUSE))
	{
	  delay(PAUSE_DURATION);
	}
      else
	{
	  WiFly.SendCommandSimple(command, "AOK");
        }
      command = commands[cmd++];
    }
}

