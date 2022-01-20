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

// #########################################################################
//  Draw the analogue meter on the screen
// #########################################################################
void CreateAnalogMeter()
{
  // Meter outline
    uint16_t textColor = White;
    uint16_t backgroundColor = Black;
  	UiLcdHy28_DrawFullRect(ts.Layout->SG_IND.x,ts.Layout->SG_IND.y,ts.Layout->SG_IND.h,ts.Layout->SG_IND.w,Grey);
  	UiLcdHy28_DrawFullRect(ts.Layout->SG_IND.x + 5,ts.Layout->SG_IND.y + 3,ts.Layout->SG_IND.h - 9,ts.Layout->SG_IND.w - 7,backgroundColor);
    // tft.fillRect(0, 0, 239, 126, ILI9341_GREY);
    // tft.fillRect(5, 3, 230, 119, ILI9341_WHITE);
    
//   tft.setTextColor(ILI9341_BLACK);  // Text colour

  uint16_t width = ts.Layout->SG_IND.w - 7;
  uint16_t height = ts.Layout->SG_IND.h - 9;
  
  // Draw ticks every 5 degrees from -50 to +50 degrees (100 deg. FSD swing)
  for (int i = -50; i < 51; i += 5) {
    // Long scale tick length
    int tl = 15;
    
    // Coodinates of tick to draw
    float sx = cos((i - 90) * 0.0174532925);
    float sy = sin((i - 90) * 0.0174532925);
    uint16_t x0 = sx * (100 + tl) + width;
    uint16_t y0 = sy * (100 + tl) + height;
    uint16_t x1 = sx * 100 + width;
    uint16_t y1 = sy * 100 + height;
    
    // Coordinates of next tick for zone fill
    float sx2 = cos((i + 5 - 90) * 0.0174532925);
    float sy2 = sin((i + 5 - 90) * 0.0174532925);
    int x2 = sx2 * (100 + tl) + width;
    int y2 = sy2 * (100 + tl) + height;
    int x3 = sx2 * 100 + width;
    int y3 = sy2 * 100 + height;
    
    // Yellow zone limits
    //if (i >= -50 && i < 0) {
    //  tft.fillTriangle(x0, y0, x1, y1, x2, y2, ILI9341_YELLOW);
    //  tft.fillTriangle(x1, y1, x2, y2, x3, y3, ILI9341_YELLOW);
    //}
    
    // Green zone limits
    if (i >= 0 && i < 25) {
      UiLcdHy28_FillTriangle_ILI(x0, y0, x1, y1, x2, y2, Green);
      UiLcdHy28_FillTriangle_ILI(x1, y1, x2, y2, x3, y3, Green);
    }

    // Orange zone limits
    if (i >= 25 && i < 50) {
      UiLcdHy28_FillTriangle_ILI(x0, y0, x1, y1, x2, y2, Orange);
      UiLcdHy28_FillTriangle_ILI(x1, y1, x2, y2, x3, y3, Orange);
    }
    
    // Short scale tick length
    if (i % 25 != 0) tl = 8;
    
    // Recalculate coords incase tick lenght changed
    x0 = sx * (100 + tl) + width;
    y0 = sy * (100 + tl) + height;
    x1 = sx * 100 + width;
    y1 = sy * 100 + height;
    
    // Draw tick
    UiLcdHy28_DrawLine_ILI(x0, y0, x1, y1, Black);
    
    // Check if labels should be drawn, with position tweaks
    if (i % 25 == 0) {
      // Calculate label positions
      x0 = sx * (100 + tl + 10) + width;
      y0 = sy * (100 + tl + 10) + height;
      switch (i / 25) {
        case -2: 
            UiLcdHy28_PrintTextCentered(x0, y0 - 12, 2, "0", textColor, backgroundColor, ts.Layout->TextMsg_font);
            // tft.drawCentreString("0", x0, y0 - 12, 2); 
            break;
        case -1: 
            UiLcdHy28_PrintTextCentered(x0, y0 - 9, 2, "25", textColor, backgroundColor, ts.Layout->TextMsg_font);
            // tft.drawCentreString("25", x0, y0 - 9, 2); 
            break;
        case 0: 
            UiLcdHy28_PrintTextCentered(x0, y0 - 6, 2, "50", textColor, backgroundColor, ts.Layout->TextMsg_font);
            // tft.drawCentreString("50", x0, y0 - 6, 2); 
            break;
        case 1: 
            UiLcdHy28_PrintTextCentered(x0, y0 - 9, 2, "75", textColor, backgroundColor, ts.Layout->TextMsg_font);
            // tft.drawCentreString("75", x0, y0 - 9, 2); 
            break;
        case 2: 
            UiLcdHy28_PrintTextCentered(x0, y0 - 12, 2, "100", textColor, backgroundColor, ts.Layout->TextMsg_font);
            // tft.drawCentreString("100", x0, y0 - 12, 2); 
            break;
      }
    }
    
    // Now draw the arc of the scale
    sx = cos((i + 5 - 90) * 0.0174532925);
    sy = sin((i + 5 - 90) * 0.0174532925);
    x0 = sx * 100 + width;
    y0 = sy * 100 + height;
    // Draw scale arc, don't draw the last part
    if (i < 50) UiLcdHy28_DrawLine_ILI(x0, y0, x1, y1, Black);
        //tft.drawLine(x0, y0, x1, y1, ILI9341_BLACK);
  }
  
//   tft.drawString("%RH", 5 + 230 - 40, 119 - 20, 2); // Units at bottom right

    //   tft.drawCentreString("%RH", 120, 70, 4); // Comment out to avoid font 4
  UiLcdHy28_PrintTextCentered( ts.Layout->SG_IND.x-(ts.Layout->SG_IND.w/2), y0 - 12, 4, "S", textColor, backgroundColor, ts.Layout->TextMsg_font);
    
    UiLcdHy28_DrawEmptyRect(ts.Layout->SG_IND.x + 5,ts.Layout->SG_IND.y + 3,ts.Layout->SG_IND.h - 9,ts.Layout->SG_IND.w - 7, Black);
//   tft.drawRect(5, 3, 230, 119, ILI9341_BLACK); // Draw bezel line
  
//   plotNeedle(0,0); // Put meter needle at 0
}
