#ifndef _linkNode__H
#define _linkNode__H
#include "dataMemory.h"
typedef struct Class_linkNode LinkNode;
struct Class_linkNode
{
    /* attribute */
    LinkNode *priorNode;
    LinkNode *nextNode;
    int64_t id;
    uint16_t memSize;

    /* virtual operation */
    void (*_contentDinit)(void *content);
    /* object */
    void *content;
};

void linkNode_deinit(LinkNode *self);
void linkNode_init(LinkNode *self, void *args);
int64_t linkNode_getId(LinkNode *self);
int32_t linkNode_isId(LinkNode *self, int64_t id);

LinkNode *New_linkNode(void *args);
#endif
