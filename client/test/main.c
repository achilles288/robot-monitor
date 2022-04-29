#include <robotmonitor.h>

#include "virtual_connection.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


rmString text1;
float speed;
int32_t a;
uint16_t b;
uint8_t c;
int16_t banana;
int16_t orange;
float voltage = 4.96f;
float cog = 71.043f;
uint32_t time;
int16_t apple = 46;
int16_t pineapple = 88;


void text1OnChange() {
    rmEcho("text1: %s %d", text1.data, text1.size);
}


void speedOnChange() {
    rmEcho("speed: %f", speed);
}


void bananaOnChange() {
    rmEcho("banana: %d", banana);
}


void showAttributes(int argc, char *argv[]) {
    rmSyncUpdate(0);
}


void showRandom(int argc, char *argv[]) {
    int r = (rand() % 1999999) - 999999;
    printf("generated: %d\n", r);
    rmEcho("random: %d %f", r, r/1000.0);;
}


int main() {
    // Client init
    text1 = rmCreateString("Hello", 12);
    rmCreateInputAttribute("speed", &speed, RM_ATTRIBUTE_FLOAT);
    rmCreateInputAttribute("attrA", &a, RM_ATTRIBUTE_FLOAT);
    rmCreateInputAttribute("attrB", &b, RM_ATTRIBUTE_INT32);
    rmCreateInputAttribute("attrC", &c, RM_ATTRIBUTE_INT8);
    rmCreateInputAttribute("banana", &banana, RM_ATTRIBUTE_INT16);
    rmCreateInputAttribute("orange", &orange, RM_ATTRIBUTE_INT16);
    rmCreateInputAttribute("text1", &text1, RM_ATTRIBUTE_STRING);
    rmInputAttributeSetBoundaries("speed", 0, 100);
    rmInputAttributeSetBoundaries("banana", -32, 64);
    rmInputAttributeSetOnChange("text1", text1OnChange);
    rmInputAttributeSetOnChange("speed", speedOnChange);
    rmInputAttributeSetOnChange("banana", bananaOnChange);
    rmCreateSync();
    rmCreateOutputAttribute("voltage", &voltage, RM_ATTRIBUTE_FLOAT, 0);
    rmCreateOutputAttribute("COG", &cog, RM_ATTRIBUTE_FLOAT, 0);
    rmCreateOutputAttribute("time", &time, RM_ATTRIBUTE_UINT32, 0);
    rmCreateOutputAttribute("apple", &apple, RM_ATTRIBUTE_UINT16, 0);
    rmCreateOutputAttribute("pineapple", &pineapple, RM_ATTRIBUTE_UINT16, 0);
    rmCreateCall("show", showAttributes);
    rmCreateCall("rand", showRandom);
    rmConnectVirtual();
    
    char str1[128];
    do {
        // Station
        char* str2 = rmVirtualStationRead();
        if(str2 != NULL) {
            printf("%s", str2);
        }
        fgets(str1, sizeof(str1), stdin);
        rmVirtualStationSendMessage(str1);
        
        // Client
        time++;
        voltage -= 0.01f;
        cog += 0.4f;
        rmProcessMessage();
    } while(strcmp(str1, "q\n") != 0);
}
