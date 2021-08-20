/* canvas.h */

#ifndef CANVAS_H   /* include guards */
    #define CANVAS_H

    #include <Arduino.h>

    // tabs and offsets
    const int htab0 = 10;                                                                                     // Temp.: Humidity: VPD:
    const int htab1 = 95 + 27;                                                                                     // current values
    const int htab2 = 170;                                                                                    // c %RH kPa
    const int htab3 = 230;                                                                                    // Max: Avg: Min:
    const int htab4 = 275;                                                                                    // statistics values
    const int htab5 = 95;                  // Temperature, Humidity and VPD labels

    const int vtab0 = 45;                                                                                     // Temp.:
    const int vtab1 = 124;                                                                                    // Humidity:
    const int vtab2 = 203;                                                                                    // VPD:

    const int maxOffset = -25;                                                                                // vertical offsets from main categories
    const int avgOffset = 0;
    const int minOffset = 25;

    // canvas
    const int canvas1Width = 70;
    const int canvas1Height = 32;                               // was 32   this adjusts height of canvas up (how far down it goes)
    GFXcanvas1 canvas1(canvas1Width, canvas1Height);            // canvas for the FreeSans18pt7b font used for temperature, humidity & VPD values
    const int canvas1FontOffsetY = -19;                         // was -26  moves the canvas up (-ve UP)
    const int canvas1CursorY = 27;                              // was 30   this moves the text within the cursor (-ve UP)

    const int canvas2Width = 35;
    const int canvas2Height = 20;
    GFXcanvas1 canvas2(canvas2Width, canvas2Height);            // canvas for the FreeSans9pt7b font used for max, avg & min values
    const int canvas2FontOffsetY = -15;
    const int canvas2CursorY = 15;

    const int canvas3Width = 70;
    const int canvas3Height = 20;
    GFXcanvas1 canvas3(canvas3Width, canvas3Height);            // canvas for the FreeSans9pt7b font used for statistics timer display
    const int canvas3FontOffsetY = -15;
    const int canvas3CursorY = 15;

    const int canvas4Width = 40;
    const int canvas4Height = 20;
    GFXcanvas1 canvas4(canvas4Width, canvas4Height);            // canvas for the FreeSans9pt7b font used for voltage
    const int canvas4FontOffsetY = -15;
    const int canvas4CursorY = 15;

    const int canvas5Width = 33;
    const int canvas5Height = 20;
    GFXcanvas1 canvas5(canvas4Width, canvas5Height);            // canvas for the FreeSans9pt7b font used for rssi
    const int canvas5FontOffsetY = -15;
    const int canvas5CursorY = 15;

#endif // CANVAS_H