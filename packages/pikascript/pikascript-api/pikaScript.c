/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#include "PikaMain.h"
#include <stdio.h>
#include <stdlib.h>

PikaObj * pikaScriptInit(){
    PikaObj * pikaMain = newRootObj("pikaMain", New_PikaMain);
    obj_run(pikaMain, "screen.init()");
    obj_run(pikaMain, "screen.background.setColor('white')");
    obj_run(pikaMain, "print('hello world')");
    return pikaMain;
}

