#include <string.h>
#include <stdlib.h>

#include "free_list.h"

/* Implement the best fit algorithm to find free space for the
   simulated file data.
 */

int get_free_block(FS *fs, int size) {
    Freeblock *temp = fs->freelist;
    int result = -1;
    if (temp->next == NULL) { // only one free block
        result = temp->offset;
        temp->length -= size;
        temp->offset += size;
        return result;
    } else { // more than one free block
        int min = MAX_FS_SIZE;
        while (temp->next != NULL) {
            if (temp->length == size) { // find perfectly fit block
                Freeblock *new_temp = fs->freelist;
                int offset = temp->offset;
                if (new_temp->offset == offset) { // check whether it is the first block fit
                    fs->freelist = fs->freelist->next;
                    free(temp);
                    return offset;
                } else {
                    while (new_temp->next->offset != offset) {
                        new_temp = new_temp->next;
                    }
                    new_temp->next = temp->next;
                    free(temp);
                    return offset;
                }
            }
            else if (temp->length > size) { // find acceptable block
                if (temp->length - size < min) {
                    min = temp->length - size;
                }
                temp = temp->next;
            }else {
                temp = temp->next;
            }
        }
        if (min > 0) { // has at least one fit block
            Freeblock *temp = fs->freelist;
            while (temp->length - size != min && temp->next != NULL) {
                temp = temp->next;
            }
            result = temp->offset;
            temp->length -= size;
            temp->offset += size;
            return result;
        }
    }
    return -1;
}

