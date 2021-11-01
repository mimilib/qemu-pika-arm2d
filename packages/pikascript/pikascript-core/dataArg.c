/*
  Author: lyon
  Tencent QQ: 645275593
*/

#include "dataArg.h"

#include <stdint.h>

#include "dataArgs.h"
#include "dataMemory.h"
#include "dataString.h"
#include "stdlib.h"

void arg_deinit(Arg* self) {
    arg_freeContent(self);
}

uint16_t arg_getTotleSize(Arg* self) {
    return content_totleSize(self);
}

uint16_t content_sizeOffset(uint8_t* self) {
    const uint8_t nextLength = sizeof(uint8_t*);
    return nextLength;
}

uint16_t content_getSize(uint8_t* self) {
    uint16_t size = 0;
    // add 0x30 to avoid 0
    size += self[content_sizeOffset(self) + 1];
    size = (size << 8);
    size += self[content_sizeOffset(self)];
    return size;
}

void content_setNext(uint8_t* self, uint8_t* next) {
    uint8_t* nextDir = self + content_nextOffset(self);
    uint64_t pointerTemp = (uint64_t)next;
    for (uint32_t i = 0; i < sizeof(uint8_t*); i++) {
        // aboid \0
        nextDir[i] = pointerTemp;
        pointerTemp = pointerTemp >> 8;
    }
}

uint8_t* content_init(char* name,
                      char* type,
                      uint8_t* content,
                      uint16_t size,
                      uint8_t* next) {
    const uint8_t nextLength = sizeof(uint8_t*);
    const uint8_t sizeLength = 2;
    uint16_t nameSize = strGetSize(name);
    uint16_t typeSize = strGetSize(type);
    uint8_t* self = (uint8_t*)pikaMalloc(nextLength + sizeLength + nameSize +
                                         1 + size + typeSize + 1);

    uint8_t* nextDir = self;
    uint8_t* sizeDir = nextDir + nextLength;
    uint8_t* nameDir = sizeDir + sizeLength;
    uint8_t* contentDir = nameDir + nameSize + 1;
    uint8_t* typeDir = contentDir + size;

    memcpy(nameDir, name, nameSize + 1);
    memcpy(typeDir, type, typeSize + 1);
    sizeDir[0] = size;
    sizeDir[1] = size >> 8;
    if (NULL != content) {
        memcpy(contentDir, content, size);
    } else {
        memset(contentDir, 0, size);
    }

    uint64_t pointerTemp = (uint64_t)next;
    for (uint32_t i = 0; i < sizeof(uint8_t*); i++) {
        // aboid \0
        nextDir[i] = pointerTemp;
        pointerTemp = pointerTemp >> 8;
    }

    return self;
}

uint16_t content_totleSize(uint8_t* self) {
    char* name = content_getName(self);
    char* type = content_getType(self);
    const uint8_t sizeLenth = 2;
    const uint8_t nextLength = sizeof(uint8_t*);
    uint16_t size = content_getSize(self);
    return size + strGetSize(name) + 1 + strGetSize(type) + 1 + sizeLenth +
           nextLength;
}

void arg_freeContent(Arg* self) {
    if (NULL != self) {
        content_deinit(self);
    }
}

uint8_t content_nameOffset(uint8_t* self) {
    const uint8_t nextLength = sizeof(uint8_t*);
    const uint8_t sizeLength = 2;
    return nextLength + sizeLength;
}

char* content_getName(uint8_t* self) {
    return (char*)self + content_nameOffset(self);
}

uint8_t* content_deinit(uint8_t* self) {
    uint16_t totleSize = content_totleSize(self);
    pikaFree(self, totleSize);
    return 0;
}

uint8_t* content_setContent(uint8_t* self, uint8_t* content, uint16_t size) {
    if (NULL == self) {
        return content_init("", "", content, size, NULL);
    }
    char* name = content_getName(self);
    char* type = content_getType(self);
    uint8_t* next = content_getNext(self);
    uint8_t* newContent = content_init(name, type, content, size, next);
    content_deinit(self);
    return newContent;
}

uint8_t* content_setName(uint8_t* self, char* name) {
    if (NULL == self) {
        return content_init(name, "", NULL, 0, NULL);
    }
    char* type = content_getType(self);
    uint8_t* content = content_getContent(self);
    uint16_t size = content_getSize(self);
    uint8_t* next = content_getNext(self);
    uint8_t* newContent = content_init(name, type, content, size, next);
    content_deinit(self);
    return newContent;
}

uint8_t* content_setType(uint8_t* self, char* type) {
    if (NULL == self) {
        return content_init("", type, NULL, 0, NULL);
    }
    char* name = content_getName(self);
    uint8_t* content = content_getContent(self);
    uint16_t size = content_getSize(self);
    uint8_t* next = content_getNext(self);
    uint8_t* newContent = content_init(name, type, content, size, next);
    content_deinit(self);
    return newContent;
}

Arg* arg_newContent(Arg* self, uint32_t size) {
    uint8_t* newContent = content_init("", "", NULL, size, NULL);
    arg_freeContent(self);
    return newContent;
}

Arg* arg_setContent(Arg* self, uint8_t* content, uint32_t size) {
    return content_setContent(self, content, size);
}

Arg* arg_setName(Arg* self, char* name) {
    return content_setName(self, name);
}

Arg* arg_setType(Arg* self, char* type) {
    return content_setType(self, type);
}

char* content_getType(uint8_t* self) {
    return (char*)self + content_typeOffset(self);
}

uint16_t content_contentOffset(uint8_t* self) {
    const uint8_t nextLength = sizeof(uint8_t*);
    const uint8_t sizeLength = 2;
    char* name = content_getName(self);
    return nextLength + sizeLength + strGetSize(name) + 1;
}

uint16_t content_nextOffset(uint8_t* self) {
    return 0;
}

uint8_t* content_getNext(uint8_t* self) {
    uint8_t* nextDir = self + content_nextOffset(self);
    uint8_t* next = NULL;
    uint64_t pointerTemp = 0;

    for (int32_t i = sizeof(uint8_t*); i > -1; i--) {
        // avoid \0
        uint8_t val = nextDir[i];
        pointerTemp = (pointerTemp << 8);
        pointerTemp += val;
    }
    next = (uint8_t*)pointerTemp;
    return next;
}

uint8_t* content_getContent(uint8_t* self) {
    return self + content_contentOffset(self);
}

uint8_t* arg_getContent(Arg* self) {
    return content_getContent(self);
}

Arg* arg_setInt(Arg* self, char* name, int64_t val) {
    int64_t int64Temp = val;
    uint8_t contentBuff[8];
    for (uint32_t i = 0; i < 4; i++) {
        // add 0x30 to void \0
        contentBuff[i] = int64Temp;
        int64Temp = int64Temp >> 8;
    }
    return content_init(name, "int", contentBuff, 4, NULL);
}

Arg* arg_setFloat(Arg* self, char* name, float val) {
    uint8_t contentBuff[4];
    uint8_t* valPtr = (uint8_t*)&val;
    for (uint32_t i = 0; i < 4; i++) {
        // add 0x30 to void \0
        contentBuff[i] = valPtr[i];
    }
    return content_init(name, "float", contentBuff, 4, NULL);
}

float arg_getFloat(Arg* self) {
    if (NULL == arg_getContent(self)) {
        return -999.999;
    }

    float valOut = 0;
    uint8_t* valOutPtr = (uint8_t*)(&valOut);
    uint8_t* valPtr = arg_getContent(self);
    for (uint32_t i = 0; i < 4; i++) {
        valOutPtr[i] = valPtr[i];
    }
    return valOut;
}

Arg* arg_setPtr(Arg* self, char* name, char* type, void* pointer) {
    uint64_t pointerTemp = (uint64_t)pointer;
    uint8_t contentBuff[8];
    for (uint32_t i = 0; i < sizeof(uint8_t*); i++) {
        // aboid \0
        contentBuff[i] = pointerTemp;
        pointerTemp = pointerTemp >> 8;
    }
    return content_init(name, type, contentBuff, sizeof(uint8_t*), NULL);
}

Arg* arg_setStr(Arg* self, char* name, char* string) {
    return content_init(name, "str", (uint8_t*)string, strGetSize(string) + 1,
                        NULL);
}

int64_t arg_getInt(Arg* self) {
    if (NULL == arg_getContent(self)) {
        return -999999;
    }
    int64_t int64Temp = 0;
    for (int32_t i = 3; i > -1; i--) {
        // add 0x30 to avoid 0
        int64Temp = (int64Temp << 8);
        int64Temp += arg_getContent(self)[i];
    }
    return int64Temp;
}

void* arg_getPtr(Arg* self) {
    void* pointer = NULL;
    uint64_t pointerTemp = 0;
    if (NULL == arg_getContent(self)) {
        return NULL;
    }
    uint8_t* content = arg_getContent(self);
    for (int32_t i = sizeof(uint8_t*) - 1; i > -1; i--) {
        // avoid \0
        uint8_t val = content[i];
        pointerTemp = (pointerTemp << 8);
        pointerTemp += val;
    }
    pointer = (void*)pointerTemp;
    return pointer;
}
char* arg_getStr(Arg* self) {
    return (char*)arg_getContent(self);
}

uint16_t content_typeOffset(uint8_t* self) {
    const uint8_t nextLength = sizeof(uint8_t*);
    const uint8_t sizeLength = 2;
    uint16_t size = content_getSize(self);
    char* name = content_getName(self);
    uint16_t nameSize = strGetSize(name);
    return nextLength + sizeLength + nameSize + 1 + size;
}

char* arg_getName(Arg* self) {
    if (NULL == self) {
        return NULL;
    }
    return content_getName(self);
}

char* arg_getType(Arg* self) {
    if (NULL == self) {
        return NULL;
    }
    return content_getType(self);
}

uint16_t arg_getContentSize(Arg* self) {
    return content_getSize(self);
}

Arg* New_arg(void* voidPointer) {
    return NULL;
}

Arg* arg_copy(Arg* argToBeCopy) {
    if (NULL == argToBeCopy) {
        return NULL;
    }
    Arg* argCopied = New_arg(NULL);
    argCopied = arg_setContent(argCopied, arg_getContent(argToBeCopy),
                               arg_getContentSize(argToBeCopy));
    argCopied = arg_setName(argCopied, arg_getName(argToBeCopy));
    argCopied = arg_setType(argCopied, arg_getType(argToBeCopy));
    return argCopied;
}
