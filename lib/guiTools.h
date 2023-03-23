#ifndef GUITOOLS_H_INCLUDED
#define GUITOOLS_H_INCLUDED

    #include <stdbool.h>
    #include "conio2.h"

    void showConsoleCursor(bool show);
    void getConsoleSize(int *width, int *height);
    void initConsoleWindowSize(short width, short height);
    void initConsoleBufferSize(short width, short height);
    void bResizableWindow(bool resizable);
    void deleteConsoleLines(int number_of_lines_to_delete);
    void drawRect(int xPos, int yPos, int width, int height, COLORS color);

    int menu(char *choices[], int numberOfChoices, bool centeredChoices, int defaultSelected, int xPos, int yPos, int spacing, COLORS textColor, COLORS backgroundColor, COLORS selectedTextColor, COLORS selectedBackgroundColor, bool cercular, bool showCursor);

#endif // GUITOOLS_H_INCLUDED
