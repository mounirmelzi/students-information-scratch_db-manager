#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include "conio2.h"
#include "guiTools.h"


void showConsoleCursor(bool show)
{
    const HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci;
    GetConsoleCursorInfo(handle, &cci);
    cci.bVisible = show;
    SetConsoleCursorInfo(handle, &cci);
}

void getConsoleSize(int *width, int *height)
{
    CONSOLE_SCREEN_BUFFER_INFO win_info;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &win_info);
    *height = win_info.srWindow.Bottom - win_info.srWindow.Top + 1;
    *width = win_info.srWindow.Right - win_info.srWindow.Left + 1;
}

void initConsoleWindowSize(short width, short height)
{
    SMALL_RECT tempRect = {0, 0, width-1, height-1};
    SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), (WINBOOL)true, &tempRect);
}

void initConsoleBufferSize(short width, short height)
{
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), (COORD){width, height});
}

void bResizableWindow(bool resizable)
{
    HWND consoleWindow = GetConsoleWindow();
    if(resizable) SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) | WS_MAXIMIZEBOX | WS_SIZEBOX);
    else SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
}

/**
 * @brief Supprimer des lignes affichees dans la console
 *
 * @param number_of_lines_to_delete le nombre de lignes a effacer (peut etre negative: la suppression se fait dans le sens inverse)
 */
void deleteConsoleLines(int number_of_lines_to_delete)
{
    const int n = abs(number_of_lines_to_delete);
    int y = wherey(), cpt = 0;

    for(int i = 0; i < n; i++)
    {
        delline(); // supprimer la ligne courante de la console
        insline(); // inserer une line vide a sa place

        // bouger le curseur vers la ligne suivante
        (number_of_lines_to_delete > 0) ? gotoxy(wherex(), wherey()+1) : gotoxy(wherex(), wherey()-1);
        if(wherey() == 1) cpt++;
    }

    // mise à jour la position de curseur
    if(cpt > 1) gotoxy(1, wherey());
    else (y <= wherey()) ? gotoxy(1, y) : gotoxy(1, wherey()+1);
}

void drawRect(int xPos, int yPos, int width, int height, COLORS color)
{
    textbackground(color);

    gotoxy(xPos, yPos);
    for(int i = 0; i < width; i++) printf(" ");

    gotoxy(xPos, yPos + height - 1);
    for(int i = 0; i < width; i++) printf(" ");

    for(int i = 0; i < height; i++)
    {
        gotoxy(xPos, yPos + i);
        printf(" ");
    }

    for(int i = 0; i < height; i++)
    {
        gotoxy(xPos + width - 1, yPos + i);
        printf(" ");
    }

    gotoxy(1, yPos + height);
}


int menu(char *choices[], int numberOfChoices, bool centeredChoices, int defaultSelected, int xPos, int yPos, int spacing, COLORS textColor, COLORS backgroundColor, COLORS selectedTextColor, COLORS selectedBackgroundColor, bool cercular, bool showCursor)
{
    // some error checks ...
    if(spacing < 0) spacing = 0;
    if(xPos < 1) xPos = 1;
    if(yPos < 1) yPos = 1;
    if((defaultSelected < 0) || (defaultSelected > numberOfChoices - 1)) defaultSelected = -1;

    showConsoleCursor(showCursor); // to hide cursor
    int choice = defaultSelected; // initialize choice to 0 : from 0 [or -1 in some cases] to numberOfChoices - 1

    // calculate the length of the largest choice in choices if they are centered
    int maxChoiceLength = -1;
    if(centeredChoices)
    {
        int _temp;
        for(int i = 0; i < numberOfChoices; i++)
        {
            _temp = strlen(choices[i]);
            if(_temp > maxChoiceLength) maxChoiceLength = _temp;
        }
    }

    // print all choices
    for(int i = 0; i < numberOfChoices; i++)
    {
        if(i == choice) // check if it's selected
        {
            textbackground(selectedBackgroundColor);
            textcolor(selectedTextColor);
        }
        else
        {
            textbackground(backgroundColor);
            textcolor(textColor);
        }
        gotoxy(xPos + (centeredChoices ? (maxChoiceLength - strlen(choices[i])) / 2 : 0), yPos + i * (spacing + 1));
        printf("%s", choices[i]);
    }

    // main loop
    clearkeybuf();
    int pressedKey;
    while(true)
    {
        // check for pressed key
        pressedKey = getch();

        if(pressedKey == 13 && choice >= 0 && choice < numberOfChoices) // press ENTER + choice validation !!
        {
            textcolor(LIGHTGRAY);
            textbackground(BLACK);
            clrscr();

            showConsoleCursor(true); // show cursor
            return choice;
        }

        if(pressedKey == 0 || pressedKey == 224) // Kind of escape char or something like that ... !!
        {
            pressedKey = getch();
            if(pressedKey == 72) // UP arrow
            {
                if(choice >= 0 && choice < numberOfChoices)
                {
                    textcolor(textColor);
                    textbackground(backgroundColor);
                    gotoxy(xPos + (centeredChoices ? (maxChoiceLength - strlen(choices[choice])) / 2 : 0), yPos + choice * (spacing + 1));
                    printf("%s", choices[choice]);
                }

                --choice;
                if(choice < 0) choice = (cercular) ? numberOfChoices - 1 : 0;

                textcolor(selectedTextColor);
                textbackground(selectedBackgroundColor);
                gotoxy(xPos + (centeredChoices ? (maxChoiceLength - strlen(choices[choice])) / 2 : 0), yPos + choice * (spacing + 1));
                printf("%s", choices[choice]);
            }
            else if(pressedKey == 80) // DOWN arrow
            {
                if(choice >= 0 && choice < numberOfChoices)
                {
                    textcolor(textColor);
                    textbackground(backgroundColor);
                    gotoxy(xPos + (centeredChoices ? (maxChoiceLength - strlen(choices[choice])) / 2 : 0), yPos + choice * (spacing + 1));
                    printf("%s", choices[choice]);
                }

                ++choice;
                if(choice > numberOfChoices - 1) choice = (cercular) ? 0 : numberOfChoices - 1;

                textcolor(selectedTextColor);
                textbackground(selectedBackgroundColor);
                gotoxy(xPos + (centeredChoices ? (maxChoiceLength - strlen(choices[choice])) / 2 : 0), yPos + choice * (spacing + 1));
                printf("%s", choices[choice]);
            }
        }
    }
    clearkeybuf();
}
