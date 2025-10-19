#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "PlayList.h"
#include "test/unit/UnitTest.h"


struct PlayList* PlayList_new() {
    struct PlayList* p = malloc(sizeof(struct PlayList));
    assert(p != NULL && "PlayList_new: ERROR");
    
    p->p_first = NULL;
    p->p_playing = NULL;

    return p;
}

// private function to allocate a new node on the heap
static struct Node* Node_new(const char* songName, struct Node* p_previous, struct Node* p_next) {
    struct Node* p_node = malloc(sizeof(struct Node));
    assert(p_node != NULL && "Node_new(): not enough memory");
    assert(strlen(songName) <= MAX_NAME_LENGTH && "Node_new(): songName too long");
    strcpy(p_node->songName, songName);
    p_node->p_next = p_next;
    p_node->p_previous = p_previous;
    return p_node;
}

void PlayList_insertAtFront(struct PlayList* p_playList, const char* songName) {
    assert(p_playList != NULL && "PlayList_insertAtFront(): p_playList is NULL");
    struct Node* p_new = Node_new(songName,NULL,p_playList->p_first);

    if(p_new->p_next != NULL){
        p_new->p_next->p_previous = p_new;
    }
    p_playList->p_first = p_new;

    if(p_playList->p_playing == NULL){
        p_playList->p_playing = p_new;
    }
}

void PlayList_insertInOrder(struct PlayList* p_playList, const char* songName) {
    assert(p_playList != NULL && "PlayList_insertInOrder(): p_playList is NULL");
    
    if (p_playList->p_first == NULL) {
        struct Node* p_new = Node_new(songName, NULL, NULL);
        p_playList->p_first = p_new;
        p_playList->p_playing = p_new;
        return;
    }

    if (strcmp(songName, p_playList->p_first->songName) < 0) {
        PlayList_insertAtFront(p_playList, songName);
        return;
    }

    struct Node* current = p_playList->p_first;
    while (current->p_next != NULL && strcmp(songName, current->p_next->songName) > 0) {
        current = current->p_next;
    }

    struct Node* p_new = Node_new(songName, current, current->p_next);
    if (current->p_next != NULL) {
        current->p_next->p_previous = p_new;
    }
    current->p_next = p_new;
}

void PlayList_insertAtEnd(struct PlayList* p_playList, const char* songName) {
    assert(p_playList != NULL && "PlayList_insertAtEnd(): p_playList is NULL");
    struct Node* p_new = Node_new(songName,NULL,NULL);

    if(p_playList->p_first == NULL){
        p_playList->p_first = p_new;
        p_playList->p_playing = p_new;
        return;
    }

    struct Node* current = p_playList->p_first;
    while(current->p_next != NULL){
        current = current->p_next;
    }
    current->p_next = p_new;
    p_new->p_previous = current;
}

// private function to find node with a gigen song name in the playList.
// returns pointer to node containing such song or NULL if song is not in the playList.
static struct Node *FindNode(struct Node* p_first, const char* songName) {
    struct Node* p_current = p_first;
    while ((p_current != NULL) && (strcmp(songName, p_current->songName) != 0)) {
        p_current = p_current->p_next;
    }
    return p_current;
}

bool PlayList_insertAfter(struct PlayList* p_playList, const char* targetSong, const char* newSong) {
    assert(p_playList != NULL && "PlayList_insertAfter(): p_playList is NULL");

    struct Node* target = FindNode(p_playList->p_first, targetSong);
    if (target == NULL) return false;

    struct Node* p_new = Node_new(newSong, target, target->p_next);
    if (target->p_next != NULL) {
        target->p_next->p_previous = p_new;
    }
    target->p_next = p_new;

    return true;
}

bool PlayList_insertBefore(struct PlayList* p_playList, const char* targetSong, const char* newSong) {
    assert(p_playList != NULL && "PlayList_insertBefore(): p_playList is NULL");

    struct Node* target = FindNode(p_playList->p_first, targetSong);
    if (target == NULL) return false;

    struct Node* p_new = Node_new(newSong, target->p_previous, target);
    if (target->p_previous != NULL) {
        target->p_previous->p_next = p_new;
    } else {
        p_playList->p_first = p_new;
    }
    target->p_previous = p_new;

    return true;
}

void PlayList_deleteFromFront(struct PlayList* p_playList) {
    assert(p_playList != NULL && "PlayList_deleteFromFront(): p_playList is NULL");
    assert(p_playList->p_first != NULL && "PlayList_deleteFromFront(): p_playList is empty");

    struct Node* to_delete = p_playList->p_first;
    p_playList->p_first = to_delete->p_next;
    if (p_playList->p_first != NULL) {
        p_playList->p_first->p_previous = NULL;
    }

    if (p_playList->p_playing == to_delete) {
        p_playList->p_playing = p_playList->p_first;
    }

    free(to_delete);
}

bool PlayList_deleteSong(struct PlayList* p_playList, const char* songName) {
    assert(p_playList != NULL && "PlayList_deleteSong(): p_playList is NULL");

    struct Node* target = FindNode(p_playList->p_first, songName);
    if (target == NULL) return false;

    if (target->p_previous != NULL)
        target->p_previous->p_next = target->p_next;
    else
        p_playList->p_first = target->p_next;

    if (target->p_next != NULL)
        target->p_next->p_previous = target->p_previous;

    if (p_playList->p_playing == target)
        p_playList->p_playing = p_playList->p_first;

    free(target);
    return true;
}

void PlayList_print(const struct PlayList* p_playList) {
    assert(p_playList != NULL && "PlayList_print(): p_playList is NULL");
    struct Node* p_current = p_playList->p_first;
    while (p_current != NULL) {
        printf("%s\n", p_current->songName);
        p_current = p_current->p_next;
    }
}

void PlayList_sort(struct PlayList* p_playList) {
    assert(p_playList != NULL && "PlayList_sort(): p_playList is NULL");

    if (p_playList->p_first == NULL || p_playList->p_first->p_next == NULL) return;

    bool swapped;
    do {
        swapped = false;
        struct Node* current = p_playList->p_first;

        while (current->p_next != NULL) {
            if (strcmp(current->songName, current->p_next->songName) > 0) {
                char temp[MAX_NAME_LENGTH + 1];
                strcpy(temp, current->songName);
                strcpy(current->songName, current->p_next->songName);
                strcpy(current->p_next->songName, temp);

                swapped = true;
            }
            current = current->p_next;
        }
    } while (swapped);

    p_playList->p_playing = p_playList->p_first;
}


void PlayList_deleteAll(struct PlayList* p_playList) {
    assert(p_playList != NULL && "PlayList_deleteAll(): p_playList is NULL");

    struct Node* current = p_playList->p_first;
    while (current != NULL) {
        struct Node* next = current->p_next;
        free(current);
        current = next;
    }
    p_playList->p_first = NULL;
    p_playList->p_playing = NULL;
}

char* PlayList_playingSong(const struct PlayList* p_playList) {
    assert(p_playList != NULL && "PlayList_playingSong(): p_playList is NULL");
    if (p_playList->p_playing == NULL) return NULL;

    size_t len = strlen(p_playList->p_playing->songName);
    char* name_copy = malloc(len + 1);
    assert(name_copy != NULL);
    strcpy(name_copy, p_playList->p_playing->songName);
    return name_copy;
}

void PlayList_playNext(struct PlayList* p_playList) {
    assert(p_playList != NULL && "PlayList_playNext(): p_playList is NULL");
    if (p_playList->p_playing != NULL && p_playList->p_playing->p_next != NULL)
        p_playList->p_playing = p_playList->p_playing->p_next;
}

void PlayList_playPrevious(struct PlayList* p_playList) {
    assert(p_playList != NULL && "PlayList_playPrevious(): p_playList is NULL");
    if (p_playList->p_playing != NULL && p_playList->p_playing->p_previous != NULL)
        p_playList->p_playing = p_playList->p_playing->p_previous;
}

void PlayList_free(struct PlayList** p_p_playList) {
    assert(p_p_playList != NULL && "PlayList_free(): p_p_playList is NULL");
    assert(*p_p_playList != NULL && "PlayList_free(): playlist is NULL");

    PlayList_deleteAll(*p_p_playList);
    free(*p_p_playList);
    *p_p_playList = NULL;
}

