/*
  Author: lyon
  Tencent QQ: 645275593
*/

#ifndef _arg__H
#define _arg__H
#include "dataMemory.h"

typedef uint8_t Arg;

uint16_t content_typeOffset(uint8_t* content);
uint16_t content_contentOffset(uint8_t* content);
uint16_t content_sizeOffset(uint8_t* self);
uint16_t content_contentOffset(uint8_t* self);
uint16_t content_nextOffset(uint8_t* self);
uint8_t content_nameOffset(uint8_t* self);

char* content_getName(uint8_t* content);
char* content_getType(uint8_t* content);
uint8_t* content_getNext(uint8_t* self);
uint16_t content_getSize(uint8_t* self);
uint8_t* content_getContent(uint8_t* content);

uint16_t content_totleSize(uint8_t* self);

uint8_t* content_init(char* name,
                      char* type,
                      uint8_t* content,
                      uint16_t size,
                      uint8_t* next);
uint8_t* content_deinit(uint8_t* self);
uint8_t* content_setName(uint8_t* self, char* name);
uint8_t* content_setType(uint8_t* self, char* type);
uint8_t* content_setContent(uint8_t* self, uint8_t* content, uint16_t size);
void content_setNext(uint8_t* self, uint8_t* next);

uint16_t arg_getTotleSize(Arg* self);
void arg_freeContent(Arg* self);

Arg* arg_setName(Arg* self, char* name);
Arg* arg_setContent(Arg* self, uint8_t* content, uint32_t size);
Arg* arg_newContent(Arg* self, uint32_t size);
Arg* arg_setType(Arg* self, char* type);
char* arg_getName(Arg* self);
char* arg_getType(Arg* self);
uint8_t* arg_getContent(Arg* self);
uint16_t arg_getContentSize(Arg* self);

Arg* arg_setInt(Arg* self, char* name, int64_t val);
Arg* arg_setFloat(Arg* self, char* name, float val);
Arg* arg_setPtr(Arg* self, char* name, char* type, void* pointer);
Arg* arg_setStr(Arg* self, char* name, char* string);

int64_t arg_getInt(Arg* self);
float arg_getFloat(Arg* self);
void* arg_getPtr(Arg* self);
char* arg_getStr(Arg* self);
Arg* arg_copy(Arg* argToBeCopy);

Arg* arg_init(Arg* self, void* voidPointer);
void arg_deinit(Arg* self);

Arg* New_arg(void* voidPointer);
#endif
