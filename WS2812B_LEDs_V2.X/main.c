/*
 * File:   main.c
 * Author: mvanwijk
 *
 * Created on 11 November 2021, 10:32 PM
 */

#define FOSC (16000000ULL) // Set oscillation freq
#define FCY (FOSC/2) // Set cycle freq
#define NUM_LEDS 60

#include "mcc_generated_files/system.h"
#include "mcc_generated_files/tmr1.h"
#include "mcc_generated_files/tmr2.h"
#include "mcc_generated_files/ext_int.h"
#include "libpic30.h"
#include <stdlib.h>

// Global variables
unsigned char Pixel[NUM_LEDS][3];
unsigned char* PixelPtr = (unsigned char*) Pixel;
int ColourIndex1 = 0;
int ColourIndex2 = 1;

struct RGBColour_t
{    
    // RGB values
    unsigned char Red;
    unsigned char Green; 
    unsigned char Blue;
    
    // Number of bits needed for each RGB colour
    // Used for changing brightness
    uint8_t nRed;
    uint8_t nGreen;
    uint8_t nBlue;
};
typedef struct RGBColour_t RGBColour;

// Defining RGB colours
RGBColour WHITE = {.Red = 255, .Green = 255, .Blue = 255,
                 .nRed = 8, .nGreen = 8, .nBlue = 8};

RGBColour RED = {.Red = 255, .Green = 0, .Blue = 0,
                 .nRed = 8, .nGreen = 0, .nBlue = 0};

RGBColour ORANGE = {.Red = 255, .Green = 127, .Blue = 0,
                    .nRed = 8, .nGreen = 7, .nBlue = 0};

RGBColour YELLOW = {.Red = 255, .Green = 255, .Blue = 0, 
                    .nRed = 8, .nGreen = 8, .nBlue = 0};

RGBColour GREEN = {.Red = 0, .Green = 255, .Blue = 0,
                   .nRed = 0, .nGreen = 8, .nBlue = 0};

RGBColour LIGHT_BLUE = {.Red = 0, .Green = 255, .Blue = 255,
                        .nRed = 0, .nGreen = 8, .nBlue = 8};

RGBColour BLUE = {.Red = 0, .Green = 0, .Blue = 255,
                  .nRed = 0, .nGreen = 0, .nBlue = 8};

RGBColour PURPLE = {.Red = 128, .Green = 0, .Blue = 255,
                    .nRed = 7, .nGreen = 0, .nBlue = 8};

RGBColour PINK = {.Red = 255, .Green = 0, .Blue = 255,
                  .nRed = 8, .nGreen = 0, .nBlue = 8};

RGBColour BLACK = {.Red = 0, .Green = 0, .Blue = 0,
                 .nRed = 0, .nGreen = 0, .nBlue = 0};

// Array of pointers to RGBColour structs
RGBColour* COLOURS[10] = {&WHITE, &RED, &ORANGE, &YELLOW, &GREEN, &LIGHT_BLUE, &BLUE, &PURPLE, &PINK, &BLACK};

void changeBrightness(RGBColour* RGB)
{
    // Check if right shifted 6 times
    if ((RGB->Red == 0b00000011 && RGB->nRed == 8) || (RGB->Red == 0b00000001 && RGB->nRed == 7) ||
       (RGB->Green == 0b00000011 && RGB->nGreen == 8) || (RGB->Green == 0b00000001 && RGB->nGreen == 7) ||
       (RGB->Blue == 0b00000011 && RGB->nBlue == 8) || (RGB->Blue == 0b00000001 && RGB->nBlue == 7))
    {
        // Reset red colour
        if (RGB->nRed == 8) // If RGB->Red is an 8 bit number
        {
            RGB->Red = 0b11111111;
        }
        else if (RGB->nRed == 7) // If RGB->Red is an 7 bit number
        {
            RGB->Red = 0b01111111;
        }
        
        
        // Reset green colour
        if (RGB->nGreen == 8) // If RGB->Green is an 8 bit number
        {
            RGB->Green = 0b11111111;
        }
        else if (RGB->nGreen == 7) // If RGB->Green is an 7 bit number
        {
            RGB->Green = 0b01111111;
        }
        
        
        // Reset blue colour
        if (RGB->nBlue == 8) // If RGB->Blue is an 8 bit number
        {
            RGB->Blue = 0b11111111;
        }
        else if (RGB->nBlue == 7) // If RGB->Blue is an 7 bit number
        {
            RGB->Blue = 0b01111111;
        }
    }
    else
    {
        // Change colour by bitwise right shift
        RGB->Red = RGB->Red >> 1;
        RGB->Green = RGB->Green >> 1;
        RGB->Blue = RGB->Blue >> 1;
    }
}

int button1_pressed(void)
{
    if (INT0Flag && TMR1Flag)
    {
        TMR1_Counter16BitSet(0); // Reset the debounce timer1
        
        LATBbits.LATB6 = ~LATBbits.LATB6; // Toggle LED
        
        // Reset the flags
        INT0Flag = 0; 
        TMR1Flag = 0;
        return 1;
    }
    else 
    {
        INT0Flag = 0;
        return 0;
    }
}

int button2_pressed(void)
{
    if (INT1Flag && TMR1Flag)
    {
        TMR1_Counter16BitSet(0); // Reset the debounce timer1
        
        LATBbits.LATB6 = ~LATBbits.LATB6; // Toggle LED
        
        // Reset the flags
        INT1Flag = 0; 
        TMR1Flag = 0;
        return 1;
    }
    else 
    {
        INT1Flag = 0;
        return 0;
    }
}

int button3_pressed(void)
{
    if (INT2Flag && TMR1Flag)
    {
        TMR1_Counter16BitSet(0); // Reset the debounce timer1
        
        LATBbits.LATB6 = ~LATBbits.LATB6; // Toggle LED
        
        // Reset the flags
        INT2Flag = 0; 
        TMR1Flag = 0;
        return 1;
    }
    else 
    {
        INT2Flag = 0;
        return 0;
    }
}

void _onePulse(void)
{
    LATBbits.LATB8 = 1;
    LATBbits.LATB8 = 1;
    LATBbits.LATB8 = 1;
    LATBbits.LATB8 = 0;
}

void _zeroPulse(void)
{
    LATBbits.LATB8 = 1;
    LATBbits.LATB8 = 0;
}

void _ws_send_byte(unsigned char K)
{
    if (K & 0b10000000) {_onePulse();} else {_zeroPulse();}
    if (K & 0b01000000) {_onePulse();} else {_zeroPulse();}
    if (K & 0b00100000) {_onePulse();} else {_zeroPulse();}
    if (K & 0b00010000) {_onePulse();} else {_zeroPulse();}
    if (K & 0b00001000) {_onePulse();} else {_zeroPulse();}
    if (K & 0b00000100) {_onePulse();} else {_zeroPulse();}
    if (K & 0b00000010) {_onePulse();} else {_zeroPulse();}
    if (K & 0b00000001) {_onePulse();} else {_zeroPulse();}
}

void _ws_set_color(unsigned char Red, unsigned char Green, unsigned char Blue)
{
    _ws_send_byte(Green);
    _ws_send_byte(Red);
    _ws_send_byte(Blue);
}

void _ws_show(void)
{
    INTCON2bits.GIE = 0; // Global interrupt disable
    
    int i;
    for (i = 0; i < NUM_LEDS; i++)
    {
        _ws_set_color(*(PixelPtr + i*3), *(PixelPtr + i*3 + 1), *(PixelPtr + i*3 + 2));
    }
    
    INTCON2bits.GIE = 1; // Global interrupt enable
}

void _ws_clear(void)
{
    int j;
    for (j = 0; j < NUM_LEDS; j++)
    {
        *(PixelPtr + j*3) = 0;
        *(PixelPtr + j*3 + 1) = 0;
        *(PixelPtr + j*3 + 2) = 0;
    }
    _ws_show();
}

void _ws_fill(unsigned char Red, unsigned char Green, unsigned char Blue)
{
    int i = 0;
    for (i = 0; i < NUM_LEDS; i++)
    {
        *(PixelPtr + i*3) = Red;
        *(PixelPtr + i*3 + 1) = Green;
        *(PixelPtr + i*3 + 2) = Blue;
    }
    _ws_show();
}

void ws_chaser_animation(RGBColour* Colours[], uint8_t length)
{
    // Function parameters
    const int CHASER_WIDTH = 8;
    const int CHASER_DELAY = 7;
    
    // Define variables
    RGBColour* RGB = Colours[ColourIndex1];
    
    int start_index = 0;
    int end_index = CHASER_WIDTH;
    int index;
    
    // Perform initial clear 
    _ws_clear();
    while (!button1_pressed())
    {  
        // Determine previous index
        // Decrement start_index with wrap around
        int prevIndex = ((start_index - 1) % NUM_LEDS) >= 0 ?  (start_index - 1) % NUM_LEDS : (start_index - 1) % NUM_LEDS + NUM_LEDS;
        
        // Turn previous pixel off 
        *(PixelPtr + prevIndex*3) = 0;
        *(PixelPtr + prevIndex*3 + 1) = 0;
        *(PixelPtr + prevIndex*3 + 2) = 0;
        
        // Turn on chaser pixels
        index = start_index;
        while (index != end_index)
        {
            *(PixelPtr + index*3) = RGB->Red;
            *(PixelPtr + index*3 + 1) = RGB->Green;
            *(PixelPtr + index*3 + 2) = RGB->Blue;
            
            // Increment index with wrap around
            index = ((index + 1)% NUM_LEDS) >= 0 ? (index + 1)% NUM_LEDS : (index + 1)% NUM_LEDS + NUM_LEDS; 
        }
        
        // Update start and end for next iteration
        // Increment with wrap around
        start_index = ((start_index + 1)% NUM_LEDS) >= 0 ? (start_index + 1)% NUM_LEDS : (start_index + 1)% NUM_LEDS + NUM_LEDS;
        end_index = ((end_index + 1)% NUM_LEDS) >= 0 ? (end_index + 1)% NUM_LEDS : (end_index + 1)% NUM_LEDS + NUM_LEDS;
        
        // Display LEDs
        _ws_show();
        
        // Change brightness
        if (button2_pressed()) {changeBrightness(RGB);}
        
        // Change RGB colour
        if (button3_pressed())
        {
            ColourIndex1++;
            if (ColourIndex1 == length - 1) {ColourIndex1 = 0;} // exclude black colour
            RGB = Colours[ColourIndex1];
        }
        
        __delay_ms(CHASER_DELAY);
    }
}

void ws_double_chaser_animation(RGBColour* Colours[], uint8_t length)
{
    // Function parameters
    const int CHASER_WIDTH1 = 12;
    const int CHASER_WIDTH2 = 10;
    const int CHASER_DELAY = 2;
    
    // Define variables
    RGBColour* RGB1 = Colours[ColourIndex1];
    RGBColour* RGB2 = Colours[ColourIndex2];
    
    int start_index1 = 0;
    int end_index1 = CHASER_WIDTH1;
    int index1;
    
    int start_index2 = 0; //NUM_LEDS - 1;
    int end_index2 = CHASER_WIDTH2; // NUM_LEDS - 1 - CHASER_WIDTH2;
    int index2;
    
    uint8_t iterationNum = 0;
    
    // Perform initial clear 
    _ws_clear();
    
    while (!button1_pressed())
    {  
        // Determine previous index
        // Decrement start_index with wrap around
        int prevIndex1 = ((start_index1 - 1) % NUM_LEDS) >= 0 ?  (start_index1 - 1) % NUM_LEDS : (start_index1 - 1) % NUM_LEDS + NUM_LEDS;
        int prevIndex2 = ((start_index2 - 1) % NUM_LEDS) >= 0 ?  (start_index2 - 1) % NUM_LEDS : (start_index2 - 1) % NUM_LEDS + NUM_LEDS;
        
        // Turn previous pixel off 
        *(PixelPtr + prevIndex1*3) = 0;
        *(PixelPtr + prevIndex1*3 + 1) = 0;
        *(PixelPtr + prevIndex1*3 + 2) = 0;
        
        *(PixelPtr + prevIndex2*3) = 0;
        *(PixelPtr + prevIndex2*3 + 1) = 0;
        *(PixelPtr + prevIndex2*3 + 2) = 0;
        
        // Turn on chaser pixels
        index1 = start_index1;
        index2 = start_index2;
        
        while (index1 != end_index1)
        {
            *(PixelPtr + index1*3) = RGB1->Red;
            *(PixelPtr + index1*3 + 1) = RGB1->Green;
            *(PixelPtr + index1*3 + 2) = RGB1->Blue;
            
            // Increment index with wrap around
            index1 = ((index1 + 1)% NUM_LEDS) >= 0 ? (index1 + 1)% NUM_LEDS : (index1 + 1)% NUM_LEDS + NUM_LEDS; 
        }
        
        while (index2 != end_index2)
        {
            *(PixelPtr + index2*3) = RGB2->Red;
            *(PixelPtr + index2*3 + 1) = RGB2->Green;
            *(PixelPtr + index2*3 + 2) = RGB2->Blue;
            
            // Increment index with wrap around
            index2 = ((index2 + 1)% NUM_LEDS) >= 0 ? (index2 + 1)% NUM_LEDS : (index2 + 1)% NUM_LEDS + NUM_LEDS; 
        }
        
        // Update start and end indices for next iteration
        // Increment with wrap around
        if ((iterationNum % 5 == 0) || (iterationNum % 5 == 1))
        {
            start_index1 = ((start_index1 + 1)% NUM_LEDS) >= 0 ? (start_index1 + 1)% NUM_LEDS : (start_index1 + 1)% NUM_LEDS + NUM_LEDS;
            end_index1 = ((end_index1 + 1)% NUM_LEDS) >= 0 ? (end_index1 + 1)% NUM_LEDS : (end_index1 + 1)% NUM_LEDS + NUM_LEDS;
        }
        else
        {
            start_index2 = ((start_index2 + 1)% NUM_LEDS) >= 0 ? (start_index2 + 1)% NUM_LEDS : (start_index2 + 1)% NUM_LEDS + NUM_LEDS;
            end_index2 = ((end_index2 + 1)% NUM_LEDS) >= 0 ? (end_index2 + 1)% NUM_LEDS : (end_index2 + 1)% NUM_LEDS + NUM_LEDS;
        }
        
        // Update iterationNum
        iterationNum = (iterationNum + 1) % 5;
        
        // Display LEDs
        _ws_show();
        
        // Change brightness
        if (button2_pressed()) 
        {
            changeBrightness(RGB1);
            changeBrightness(RGB2);
        }
        
        // Change RGB colour
        if (button3_pressed())
        {
            if (PORTBbits.RB12) // If ColourSelect pin high
            {
                // Change RGB1 colour
                ColourIndex1++;
                if (ColourIndex1 == length - 1) {ColourIndex1 = 0;} // exclude black colour
                RGB1 = Colours[ColourIndex1];
            }
            else
            {
                // Change RGB2 colour
                ColourIndex2++;
                if (ColourIndex2 == length - 1) {ColourIndex2 = 0;} // exclude black colour
                RGB2 = Colours[ColourIndex2];
            }
        }
        
        __delay_ms(CHASER_DELAY);
    }
}

void ws_creeping_animation(RGBColour* Colours[], uint8_t length)
{
    // Defining variables
    RGBColour* RGB = Colours[ColourIndex1];
    
    // Initial clear
    _ws_clear();
    
    int j = 0;
    int forwards = 1;
    int rainbow = 0;
    int RainbowIndex;
    int nIteration = 0;
    
    while (!button1_pressed())
    {
        // Set pixel colours
        if (forwards)
        {
            if (rainbow)
            {
                // Set pixel colour
                *(PixelPtr + j*3) = Colours[RainbowIndex]->Red;
                *(PixelPtr + j*3 + 1) = Colours[RainbowIndex]->Green;
                *(PixelPtr + j*3 + 2) = Colours[RainbowIndex]->Blue;
                
                // Increment RainbowIndex for next iteration
                nIteration = (nIteration + 1) % 5; // 5 pixel colour segments
                if (!nIteration) {RainbowIndex++;}
                if (RainbowIndex == length - 1) {RainbowIndex = 1;}
            }
            else
            {
                // Set pixel colour
                *(PixelPtr + j*3) = RGB->Red;
                *(PixelPtr + j*3 + 1) = RGB->Green;
                *(PixelPtr + j*3 + 2) = RGB->Blue;
            }
        }
        else
        {
            *(PixelPtr + j*3) = 0;
            *(PixelPtr + j*3 + 1) = 0;
            *(PixelPtr + j*3 + 2) = 0;
        }
        
        // Reset pixels at end
        if (j == NUM_LEDS - 1)
        {
            forwards = !forwards;
            j = -1; // Start at -1 since incrementing j at end
        }
        
        
        // Display LEDs
        _ws_show();
        
        // Change brightness 
        if ((button2_pressed()) && (!rainbow))
        {
            changeBrightness(RGB);
            
            // Adjust Pixel array to new RGB brightness
            int i;
            for (i = 0; i < NUM_LEDS; i++)
            {
                // If pixel is not black (i.e. off)
                // if red != 0 || green != 0 || blue != 0
                if ((*(PixelPtr + i*3) != 0) || (*(PixelPtr + i*3 + 1) != 0) || (*(PixelPtr + i*3 + 2) != 0)) 
                {
                    *(PixelPtr + i*3) = RGB->Red;
                    *(PixelPtr + i*3 + 1) = RGB->Green;
                    *(PixelPtr + i*3 + 2) = RGB->Blue;
                }
            }
        }
        
        // Change RGB colour
        if (button3_pressed())
        {
            ColourIndex1++;
            
            if (ColourIndex1 == length - 1) // exclude black colour (rainbow mode)
            {   
                // Set ColourIndex1 for next iteration
                ColourIndex1 = -1;
                
                // Enable rainbow mode
                RainbowIndex = 1;
                rainbow = 1;
                
                // Adjust Pixel array to new RGB colour
                int i;
                for (i = 0; i < NUM_LEDS; i++)
                {
                    // If pixel is not black (i.e. off)
                    // if red != 0 || green != 0 || blue != 0
                    if ((*(PixelPtr + i*3) != 0) || (*(PixelPtr + i*3 + 1) != 0) || (*(PixelPtr + i*3 + 2) != 0)) 
                    {
                        // Set pixel colour
                        *(PixelPtr + i*3) = Colours[RainbowIndex]->Red;
                        *(PixelPtr + i*3 + 1) = Colours[RainbowIndex]->Green;
                        *(PixelPtr + i*3 + 2) = Colours[RainbowIndex]->Blue;

                        // Increment RainbowIndex for next iteration
                        nIteration = (nIteration + 1) % 5; // 5 pixel colour segments
                        if (!nIteration) {RainbowIndex++;}
                        if (RainbowIndex == length - 1) {RainbowIndex = 1;}
                    }
                }
            } 
            else // static colour mode
            {
                // If leaving rainbow mode
                if (rainbow) {rainbow = 0;}
                
                RGB = Colours[ColourIndex1];
                // Adjust Pixel array to new RGB colour
                int i;
                for (i = 0; i < NUM_LEDS; i++)
                {
                    // If pixel is not black (i.e. off)
                    // if red != 0 || green != 0 || blue != 0
                    if ((*(PixelPtr + i*3) != 0) || (*(PixelPtr + i*3 + 1) != 0) || (*(PixelPtr + i*3 + 2) != 0)) 
                    {
                        *(PixelPtr + i*3) = RGB->Red;
                        *(PixelPtr + i*3 + 1) = RGB->Green;
                        *(PixelPtr + i*3 + 2) = RGB->Blue;
                    }
                }
            }
            
            
        }
        
        // Increment counter
        j++;
        
        __delay_ms(50);
    }
    
    // Set ColourIndex1 from -1 to 0
    if (rainbow) {ColourIndex1 = 0;}
}

void ws_flashing_animation(RGBColour* Colours[],  uint8_t length)
{
    // Perform initial clear 
    _ws_clear();
    
    // Define variables
    int colour1 = 0;
    RGBColour* RGB1 = Colours[ColourIndex1];
    RGBColour* RGB2 = Colours[ColourIndex2];
    
    while (!button1_pressed())
    {
        // Change colour when timer2 flag raised
        if (TMR2Flag)
        {
            if (colour1)
            {
                _ws_fill(RGB1->Red, RGB1->Green, RGB1->Blue);
                colour1 = !colour1;
            }
            else 
            {
                _ws_fill(RGB2->Red, RGB2->Green, RGB2->Blue);
                colour1 = !colour1;
            }
             
            // Reset flag
            TMR2Flag = 0;
        }
        
        // Change brightness
        if (button2_pressed())
        {
            changeBrightness(RGB1);
            changeBrightness(RGB2);
        }
        
        // Change RGB colour
        if (button3_pressed())
        {
            if (PORTBbits.RB12) // If ColourSelect pin high
            {
                // Change RGB1 colour
                ColourIndex1++;
                if (ColourIndex1 == length) {ColourIndex1 = 0;} // include black colour 
                if ((ColourIndex1 == length - 1) && (ColourIndex2 == length - 1)) {ColourIndex1 = 0;} // both colours can't be black
                RGB1 = Colours[ColourIndex1];
            }
            else
            {
                // Change RGB2 colour
                ColourIndex2++;
                if (ColourIndex2 == length) {ColourIndex2 = 0;}
                if ((ColourIndex1 == length - 1) && (ColourIndex2 == length - 1)) {ColourIndex2 = 0;} // both colours can't be black
                RGB2 = Colours[ColourIndex2];
            }
        }
    }
}

void ws_static_colour(RGBColour* Colours[], uint8_t length)
{
    RGBColour* RGB = Colours[ColourIndex1];
    
    while (!button1_pressed())
    {
        _ws_fill(RGB->Red, RGB->Green, RGB->Blue);
    
        // Change brightness
        if (button2_pressed()) {changeBrightness(RGB);}
        
        // Change RGB colour
        if (button3_pressed())
        {
            ColourIndex1++;
            if (ColourIndex1 == length - 1) {ColourIndex1 = 0;} // exclude black colour
            RGB = Colours[ColourIndex1];
        }
        __delay_ms(50);
    }
}

int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    
    while (1)
    {
        // Add your application code
        ws_chaser_animation(COLOURS, 10);
        ws_double_chaser_animation(COLOURS, 10);
        ws_flashing_animation(COLOURS, 10);
        ws_creeping_animation(COLOURS, 10);
        ws_static_colour(COLOURS, 10);
    }
    return 1; 
}

