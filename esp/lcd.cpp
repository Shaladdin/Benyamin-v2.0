#include "header.h"

LiquidCrystal_I2C lcd_(0x27, LCD_COLMN, LCD_ROW);

LCD::LCD()
{
    lcd_.begin(LCD_COLMN, LCD_ROW);
    lcd_.backlight();
}
// lcd function
void LCD ::print(String top, String bottom = "", bool force = true)
{
    if (top == currentText[0] && bottom == currentText[1] && !force)
        return;

    String text[2] = {top, bottom};
    if (force)
        lcd_.clear();
    // update the text
    for (int y = 0; y < LCD_ROW; y++)
    {
        lcd_.setCursor(0, y);
        if (force)
            lcd_.print(text[y]);
        else
            for (int x = 0; x < LCD_COLMN + 1; x++)
            {
                lcd_.setCursor(x, y);
                if (!force && currentText[y][x] != text[y][x])
                    if (text[y].length() > x)
                        lcd_.print(String(text[y][x]));
                    else
                        lcd_.print(F(" "));
            }
    }
    currentText[0] = top;
    currentText[1] = bottom;
}

LCD lcd;