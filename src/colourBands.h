/* colourBands.h */

#ifndef COLOUR_BANDS_H   /* include guards */
    #define COLOUR_BANDS_H

    #include <Arduino.h>
    
    bool temperatureDispalayState = true;

    uint16_t temperatureColourBand(float value)
        {
        uint16_t textColour;
        //temperatureDispalayOn = false;
        if (value <= 16) 
            {
            textColour = ILI9341_BLUE;
            //temperatureDispalayOn = true;
            }
        else if (value <= 18) textColour = ILI9341_CYAN;
        else if (value <= 22) textColour = ILI9341_GREEN;
        else if (value <= 26) textColour = ILI9341_GREEN;
        else if (value <= 28) textColour = 0xFD83; // Orange
        else textColour = ILI9341_RED;
        /*
            {
            textColour = ILI9341_RED;
            if (temperatureDispalayState == true)
                {
                textColour = ILI9341_RED;
                temperatureDispalayState = false;
                }
            else 
                {
                textColour = 0x0000;
                temperatureDispalayState = true;
                }
            }
        */
        return textColour;
        }

    uint16_t humidityColourBand(float value)
        {
        uint16_t textColour;
        if (value <= 40) textColour = ILI9341_RED;
        else if (value <= 50) textColour = 0xFD83; // Orange
        else if (value <= 70) textColour = ILI9341_GREEN;
        else if (value <= 75) textColour = ILI9341_CYAN;
        else textColour = ILI9341_BLUE;
        return textColour;
        }

    uint16_t vpdColourBand(float value)
        {
        uint16_t textColour;
        if (value <= 0.4) textColour = ILI9341_BLUE;
        else if (value <= 0.6) textColour = ILI9341_CYAN;
        else if (value <= 1.25) textColour = ILI9341_GREEN;
        else if (value <= 1.5) textColour = 0xFD83; // Orange
        else textColour = ILI9341_RED;
        return textColour;
        }

    uint16_t voltageColourBand(float value)
        {
        uint16_t textColour;
        if (value <= 3.47) textColour = ILI9341_RED;
        else if (value <= 3.53) textColour = 0xFD83; // Orange
        else if (value <= 3.68) textColour = ILI9341_DARKGREEN;
        else textColour = ILI9341_GREEN;
        return textColour;
        }

        uint16_t rssiColourBand(float value)
        {
        uint16_t textColour;
        if (value <= -80) textColour = ILI9341_RED;
        else if (value <= -70) textColour = 0xFD83; // Orange
        else if (value <= -65) textColour = ILI9341_GREEN;
        else textColour = ILI9341_GREEN;
        return textColour;
        }


// ------------------------------------------------------------------------------------------------

#endif // COLOUR_BANDS_H