/*  -*-  mode: c; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4; coding: utf-8  -*-  */
/************************************************************************************
 **                                                                                 **
 **                               mcHF QRP Transceiver                              **
 **                             K Atanassov - M0NKA 2014                            **
 **                                                                                 **
 **---------------------------------------------------------------------------------**
 **                                                                                 **
 **  File name:      ui_gauge.c                                                     **
 **  Description:    Analogue gauge for Signals                                     **
 **  Last Modified:                                                                 **
 **  Licence:        GNU GPLv3                                                      **
 ************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "uhsdr_board.h"
#include "ui_lcd_hy28.h"

int old_analog =  -999; // Value last displayed
float ltx = 0;    // Saved x coord of bottom of needle
uint16_t osx = 120, osy = 120; // Saved x & y coords
uint32_t updateTime = 0;       // time for next update

// #########################################################################
// Update needle position
// This function is blocking while needle moves, time depends on ms_delay
// 10ms minimises needle flicker if text is drawn within needle sweep area
// Smaller values OK if text not in sweep area, zero for instant movement but
// does not look realistic... (note: 100 increments for full scale deflection)
// #########################################################################
void plotNeedle(int value, byte ms_delay)
{
  char buf[8]; dtostrf(value, 4, 0, buf);
  // tft.drawRightString(buf, 40, 119 - 20, 2);

  if (value < -10) value = -10; // Limit value to emulate needle end stops
  if (value > 110) value = 110;

  // Move the needle util new value reached
  while (!(value == old_analog)) {
    if (old_analog < value) old_analog++;
    else old_analog--;
    
    if (ms_delay == 0) old_analog = value; // Update immediately id delay is 0
    
    float sdeg = map(old_analog, -10, 110, -150, -30); // Map value to angle 
    // Calcualte tip of needle coords
    float sx = cos(sdeg * 0.0174532925);
    float sy = sin(sdeg * 0.0174532925);

    // Calculate x delta of needle start (does not start at pivot point)
    float tx = tan((sdeg+90) * 0.0174532925);
    
    // Erase old needle image
    // tft.drawLine(120 + 20 * ltx - 1, 140 - 20, osx - 1, osy, ILI9341_WHITE);
    // tft.drawLine(120 + 20 * ltx, 140 - 20, osx, osy, ILI9341_WHITE);
    // tft.drawLine(120 + 20 * ltx + 1, 140 - 20, osx + 1, osy, ILI9341_WHITE);
    UiLcdHy28_DrawLine(120 + 20 * ltx - 1, 140 - 20, osx - 1, osy, Black);
    UiLcdHy28_DrawLine(120 + 20 * ltx, 140 - 20, osx, osy, Black);
    UiLcdHy28_DrawLine(120 + 20 * ltx + 1, 140 - 20, osx + 1, osy, Black);
    
    // Re-plot text under needle
    // tft.setTextColor(ILI9341_BLACK);
    // tft.drawCentreString("%RH", 120, 70, 4); // // Comment out to avoid font 4
    
    // Store new needle end coords for next erase
    ltx = tx;
    osx = sx * 98 + 120;
    osy = sy * 98 + 140;
    
    // Draw the needle in the new postion, magenta makes needle a bit bolder
    // draws 3 lines to thicken needle
    // tft.drawLine(120 + 20 * ltx - 1, 140 - 20, osx - 1, osy, ILI9341_RED);
    // tft.drawLine(120 + 20 * ltx, 140 - 20, osx, osy, ILI9341_MAGENTA);
    // tft.drawLine(120 + 20 * ltx + 1, 140 - 20, osx + 1, osy, ILI9341_RED);
    UiLcdHy28_DrawLine(120 + 20 * ltx - 1, 140 - 20, osx - 1, osy, Red);
    UiLcdHy28_DrawLine(120 + 20 * ltx, 140 - 20, osx, osy, Magenta);
    UiLcdHy28_DrawLine(120 + 20 * ltx + 1, 140 - 20, osx + 1, osy, Red);
    
    // Slow needle down slightly as it approaches new postion
    if (abs(old_analog - value) < 10) ms_delay += ms_delay / 5;
    
    // Wait before next update
    delay(ms_delay);
  }
}

// #########################################################################
//  Draw the analogue meter on the screen
// #########################################################################
void CreateAnalogMeter()
{
  // Meter outline
    uint16_t textColor = White;

    // .SG_IND = { .x = 113, .y = 0, .h = 52, .w = 203 },

  	UiLcdHy28_DrawFullRect(ts.Layout->SG_IND.x,ts.Layout->SG_IND.y,ts.Layout->SG_IND.h,ts.Layout->SG_IND.w, Black);
  //	UiLcdHy28_DrawFullRect(ts.Layout->SG_IND.x + 5,ts.Layout->SG_IND.y + 3,ts.Layout->SG_IND.h - 9,ts.Layout->SG_IND.w - 7, Black);
    // tft.fillRect(0, 0, 239, 126, ILI9341_GREY);
    // tft.fillRect(5, 3, 230, 119, ILI9341_WHITE);
    
//   tft.setTextColor(ILI9341_BLACK);  // Text colour
  
  // Draw ticks every 5 degrees from -50 to +50 degrees (100 deg. FSD swing)
  for (int i = -50; i < 51; i += 5) {
    // Long scale tick length
    int tl = 4; //15
    
    uint8_t factor = 57;
    uint8_t yfactor = 70; // 140
    uint8_t xfactor =175; // 120
    float k = 0.0174532925;
//    float k = 0.02;

    // Coodinates of tick to draw
    float sx = cos((i - 90) * k);
    float sy = sin((i - 90) * k);
    uint16_t x0 = sx * (factor + tl) + xfactor;
    uint16_t y0 = sy * (factor + tl) + yfactor;
    uint16_t x1 = sx * factor + xfactor;
    uint16_t y1 = sy * factor + yfactor;
    
    // Coordinates of next tick for zone fill
    float sx2 = cos((i + 5 - 90) * k);
    float sy2 = sin((i + 5 - 90) * k);
    int x2 = sx2 * (factor + tl) + xfactor;
    int y2 = sy2 * (factor + tl) + yfactor;
    int x3 = sx2 * factor + xfactor;
    int y3 = sy2 * factor + yfactor;
    
    // Yellow zone limits
    //if (i >= -50 && i < 0) {
    //  tft.fillTriangle(x0, y0, x1, y1, x2, y2, ILI9341_YELLOW);
    //  tft.fillTriangle(x1, y1, x2, y2, x3, y3, ILI9341_YELLOW);
    //}
    
    // Green zone limits
    if (i >= -50 && i < 0) {
      UiLcdHy28_FillTriangle(x0, y0, x1, y1, x2, y2, Green);
      UiLcdHy28_FillTriangle(x1, y1, x2, y2, x3, y3, Green);
    }

    // Orange zone limits
    if (i >= 0 && i < 25) {
      UiLcdHy28_FillTriangle(x0, y0, x1, y1, x2, y2, Orange);
      UiLcdHy28_FillTriangle(x1, y1, x2, y2, x3, y3, Orange);
    }
    // Red zone limits
    if (i >= 25 && i < 50) {
      UiLcdHy28_FillTriangle(x0, y0, x1, y1, x2, y2, Red);
      UiLcdHy28_FillTriangle(x1, y1, x2, y2, x3, y3, Red);
    }
    
    // Short scale tick length
    if (i % 25 != 0) tl = 2;
    
    // Recalculate coords incase tick lenght changed
    x0 = sx * (factor + tl) + xfactor;
    y0 = sy * (factor + tl) + yfactor;
    x1 = sx * factor + xfactor;
    y1 = sy * factor + yfactor;
    
    // Draw tick
    UiLcdHy28_DrawLine(x0, y0, x1, y1, Black);
    
    // Check if labels should be drawn, with position tweaks
    if (i % 25 == 0) {
      // Calculate label positions
      x0 = sx * (factor + tl + 10) + xfactor;
      y0 = sy * (factor + tl + 10) + yfactor;
      switch (i / 25) {
        case -2: 
            UiLcdHy28_PrintTextCentered(x0, y0 - 12, 2, "0", textColor, Black, ts.Layout->TextMsg_font);
            // tft.drawCentreString("0", x0, y0 - 12, 2); 
            break;
        case -1: 
            UiLcdHy28_PrintTextCentered(x0, y0 - 9, 2, "25", textColor, Black, ts.Layout->TextMsg_font);
            // tft.drawCentreString("25", x0, y0 - 9, 2); 
            break;
        case 0: 
            UiLcdHy28_PrintTextCentered(x0, y0 - 6, 2, "50", textColor, Black, ts.Layout->TextMsg_font);
            // tft.drawCentreString("50", x0, y0 - 6, 2); 
            break;
        case 1: 
            UiLcdHy28_PrintTextCentered(x0, y0 - 9, 2, "75", textColor, Black, ts.Layout->TextMsg_font);
            // tft.drawCentreString("75", x0, y0 - 9, 2); 
            break;
        case 2: 
            UiLcdHy28_PrintTextCentered(x0, y0 - 12, 2, "100", textColor, Black, ts.Layout->TextMsg_font);
            // tft.drawCentreString("100", x0, y0 - 12, 2); 
            break;
      }
    }
    
    // Now draw the arc of the scale
    sx = cos((i + 5 - 90) * k);
    sy = sin((i + 5 - 90) * k);
    x0 = sx * factor + xfactor;
    y0 = sy * factor + yfactor;
    // Draw scale arc, don't draw the last part
    if (i < 50) UiLcdHy28_DrawLine(x0, y0, x1, y1, Black);
        //tft.drawLine(x0, y0, x1, y1, ILI9341_BLACK);
  }
  
//   tft.drawString("%RH", 5 + 230 - 40, 119 - 20, 2); // Units at bottom right

    //   tft.drawCentreString("%RH", 120, 70, 4); // Comment out to avoid font 4

    UiLcdHy28_PrintTextCentered( 120, 70, 4, "S", textColor, Black, ts.Layout->TextMsg_font);
    
    UiLcdHy28_DrawEmptyRect(ts.Layout->SG_IND.x + 5,ts.Layout->SG_IND.y + 3,ts.Layout->SG_IND.h - 9,ts.Layout->SG_IND.w - 7, Black);
//   tft.drawRect(5, 3, 230, 119, ILI9341_BLACK); // Draw bezel line
  
   plotNeedle(0,0); // Put meter needle at 0
}
