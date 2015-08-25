# mslogger-lcd-gps-sd-mega

An Arduino ATmega2560 microcontroller application that logs Megasquirt (via Serial) and uses a GPS+SD shield to log data together. Also uses LCD shield to display information including a RPM limiter and menu to add/remove displayed items, start/stop logging etc.

Current project is based on Platformio's code builder - see http://platformio.org/

After pulling down the project's repo, run the following platformio command
`platformio init --board megaatmega2560 --ide eclipse`

To build the project (and upload) you run `platformio run -t upload`

The Arduino board actually used is the Seeeduino Mega (same size as an Uno) - we've then got the following additions...

1) Adafruit's ILI9328 TFT LCD Display and Touch screen shield - uses digital pins D5-D13 and analog A0-A3. 
2) Adafruit's Ultimate GPS Logger shield - using pins D18(TX1) and D19(RX1) by running wires from GPS.
3) DB9 Serial TTL board wired to digital pins D0-D1 for Hardware Serial for Megasquirt logging.
3) DB9 Serial TTL board wired to digital pins D2-D3 Software Serial for debuging.

.. more details to follow soon ..