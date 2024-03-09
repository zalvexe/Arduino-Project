# Blue Object Following Robot

## Description : 
Simple object recognition with Arduino Uno and OpenCV in C++. Designed for my robotic final project, This robot will follow a blue object that appears in front of the camera and will rotate according to object's position.  
It uses 2 Arduino unos for wireless connection. For webcam, i use my phone connected with the OpenCV through Iriun virtual webcam.  
Honestly it was pretty complicated project to made cause we need to use c++ instead of python.. ;_;

## Components :
| Name | Quantity |
| ------------- | ------------- |
| Arduino Uno  | 2  |
| NRF24-Module  | 2  |
| Motor Wheel  | 4  |
| Motor Driver LN298  | 1  |  

## Serial Communication
To make the serial communication between the Transmitter Arduino and OpenCV, i use the ```Windows.h``` library and this code  
```c++
HANDLE hSerial = CreateFile(L"COM9", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0); // change the com for the receiver

	if (hSerial != INVALID_HANDLE_VALUE) {
		printf("Port opened! \n");

		DCB dcbSerialParams;
		GetCommState(hSerial, &dcbSerialParams);

		dcbSerialParams.BaudRate = CBR_9600;
		dcbSerialParams.ByteSize = 8;
		dcbSerialParams.Parity = NOPARITY;
		dcbSerialParams.StopBits = ONESTOPBIT;

		SetCommState(hSerial, &dcbSerialParams);
	}
	else {
		if (GetLastError() == ERROR_FILE_NOT_FOUND) {
			printf("Serial port doesn't exist! \n");
		}

		printf("Error while setting up serial port! \n");
	}

	char outputChars[] = "c";
	DWORD btsIO;
```
change the COM and BaudRate based on your Arduino.  

## Diagram?

<img src="https://github.com/zalvexe/Projects/assets/92864261/7d64f57f-6b00-45d1-8570-ec3877f04044" width="700" height="250"/>  

-- grazie :) --
