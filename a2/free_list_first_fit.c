#include <string.h>
#include <stdlib.h>

#include "free_list.h"

/* Implement the first fit algorithm to find free space for the
   simulated file data.
 */

int get_free_block(FS *fs, int size) {
    Freeblock *temp = fs->freelist;
    int result =  -1;
    if (temp != NULL) {
        if (temp->next == NULL) {
            result = temp->offset;
            temp->length -= size;
            temp->offset += size;
            return result;
        }
        else {
            while (temp->next != NULL && temp->length < size) {
                temp = temp->next;
            }
            if (temp->length > size) {
                result = temp->offset;
                temp->length -= size;
                temp->offset += size;
                return result;
            }
            if (temp->length == size) {
                Freeblock *new_temp = fs->freelist;
                if (temp == new_temp) {
                    result = temp->offset;
                    fs->freelist = fs->freelist->next;
                    free(temp);
                    return result;
                }
                while (new_temp->next != temp) {
                    new_temp = new_temp->next;
                }
                result = temp->offset;
                new_temp->next = temp->next;
                free(temp);
                return result;
            }
        }
    }
    return -1;
}

