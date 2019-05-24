/* The functions operate on a linked list of free blocks.  Each node of the
 * list contains the starting location and the length of a free block.
 * 
 */

#include <string.h>
#include <stdlib.h>

#include "free_list.h"

/* Give free space back to the free list.  Since the list is ordered by 
 * location, this function is the same for both algorithms.
 */
void add_free_block(FS *fs, int location, int size) {
    if (fs->freelist == NULL) {
        Freeblock *new_block = malloc(sizeof(Freeblock));
        new_block->length = size;
        new_block->offset = location;
        new_block->next = NULL;
        fs->freelist = new_block;
    }else {
        Freeblock *temp = fs->freelist;
        // freelist only have one freeblock
        if (temp->next == NULL) {
            // delete that only freeblock
            if (location + size == temp->offset) {
                temp->offset -= size;
                temp->length += size;
                return;
            }
            // delete somethinig before that freeblock
            if (location + size < temp->offset) {
                Freeblock *new_block = malloc(sizeof(Freeblock));
                new_block->length = size;
                new_block->offset = location;
                new_block->next = temp;
                fs->freelist = new_block;
                return;
            }
        }else {// Have more than one freeblock
            if (location + size == temp->offset) { // delete the first freeblock
                temp->offset = location;
                temp->length += size;
            }else { // delete rest freeblock
                while (temp->next != NULL && temp->next->offset <= location + size) {
                    temp = temp->next;
                } // free the middle freeblock
                if (temp->next != NULL) {
                    if(temp->offset + temp->length == location + size) { // check combine
                        temp->offset = location;
                        temp->length += size;
                    } else { // can't combine and insert new block
                        Freeblock *new_block = malloc(sizeof(Freeblock));
                        new_block->length = size;
                        new_block->offset = location;
                        new_block->next = temp->next;
                        temp->next = new_block;
                    }
                } else { // free the last block
                    temp->offset = location;
                    temp->length += size;
                    Freeblock *check = fs->freelist; // check combine
                    while(check->next->next != NULL) {
                        check = check->next;
                    }
                    if (temp->offset == check->offset + check->length && check->next == temp) {
                        check->length += temp->length;
                        check->next = NULL;
                    }
                }
            }
        }
    }
    return;
}

/* Print the contents of the free list */

void show_freelist(FS *fs) {
    Freeblock *temp = fs->freelist;
    printf("Free List\n");
    while (temp != NULL) {
        printf("(offset: %d, length: %d)\n", temp->offset, temp->length);
        temp = temp->next;
    }
    return;
}


/* To be used after the metadata has been read from a file, to rebuild the
 * free list.
 */
void rebuild_freelist(FS *fs) {
        int i;
        Freeblock *new_block = malloc(sizeof(Freeblock));
        new_block->length = MAX_FS_SIZE;
        new_block->offset = METADATA_ENDS;
        new_block->next = NULL;
        fs->freelist = new_block;
        int flag = 0;
        int record;
        for (i = 0; i < MAXFILES; i++){
            if (fs->metadata[i].offset != -1) {
                if (flag == 0) { // initialize the first block
                    Freeblock *temp = fs->freelist;
                    temp->length -= fs->metadata[i].length;
                    temp->offset += fs->metadata[i].length;
                    record = fs->metadata[i].length + fs->metadata[i].offset;
                    flag = 1;
                } else {
                    if (record < fs->metadata[i].offset) { // find there is a gap
                        Freeblock *temp = fs->freelist;
                        if (temp->next == NULL) { // only have one block
                            Freeblock *new_block = malloc(sizeof(Freeblock));
                            new_block->length = fs->metadata[i].offset - record;
                            new_block->offset = record;
                            new_block->next = temp;
                            fs->freelist = new_block;
                            temp->offset = fs->metadata[i].offset + fs->metadata[i].length;
                            temp->length = temp->length - fs->metadata[i].length - new_block->length;
                            record = fs->metadata[i].length + fs->metadata[i].offset;
                        } else { // have more than one freeblock
                            while (temp->next != NULL && temp->next->offset < record) {
                                temp = temp->next;
                            }
                            Freeblock *new_block = malloc(sizeof(Freeblock));
                            new_block->length = fs->metadata[i].offset - record;
                            new_block->offset = record;
                            new_block->next = temp->next;
                            temp->next = new_block;
                            new_block->next->offset = fs->metadata[i].offset + fs->metadata[i].length;
                            new_block->next->length -= (fs->metadata[i].length + new_block->length);
                            record = fs->metadata[i].length + fs->metadata[i].offset;
                        }
                    } else { // misordered node
                        Freeblock *temp = fs->freelist;
                        while (temp->next != NULL && temp->next->offset < fs->metadata[i].offset) {
                            temp = temp->next;
                        }
                        Freeblock *new_block = malloc(sizeof(Freeblock));
                        int pre_temp_length = temp->length;
                        temp->length = fs->metadata[i].offset - temp->offset;
                        new_block->length = pre_temp_length - fs->metadata[i].length - temp->length;
                        new_block->offset = fs->metadata[i].length + fs->metadata[i].offset;
                        new_block->next = temp->next;
                        temp->next = new_block;
                    }
                }
            }
        }
    return;
}
