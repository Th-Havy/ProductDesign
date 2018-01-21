# C code for the ESP32 microcontroller embedded in the trash bin

The ESP32 represents the "brain" of our smart trash bin. This microcontroller has built-in Wi-Fi capabilities allowing it to communicated with our web app via a Wi-Fi network. This chip has to manage several peripherals:

* Ultrasound sensors for level detection (there are 4 of them).
* Speakers to play sound whenever the sensors detects that an object is thrown in the trash bin.
* A LED bar that displays the current fullness of the trash bin.
* A SD-card used to store sounds, configurations (Wi-Fi credentials) and record fullness state when the server cannot be reached.

The communication with the server is made via a REST API described in 

## License

Like the code of the web app, this one is licensed under the MIT License. Please note that the libraries included may have been modified for performance reasons, or to reduce the complexity of the code. These modification are in adequation with the respective licenses of said libraries.
