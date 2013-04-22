/*
 * Copyright (c) 2008-2012 Red Hat, Inc. http://www.redhat.com
 * This file is part of glfs-ndmp.
 * This file is licensed to you under your choice of the GNU Lesser
 * General Public License, version 3 or any later version (LGPLv3 or
 * later), or the GNU General Public License, version 2 (GPLv2), in all
 * cases as published by the Free Software Foundation.
 */

/*
 * This file and its components written by Shrinidhi
 * unless otherwise specified.
 */

#include <queue.h>
#include <locks.h>
#include <stdlib.h>

void enter_critical_section(struct lock* q_lock);
void exit_critical_section(struct lock *q_lock);

struct queue_hdr* init_queue()
{
        struct queue_hdr* retval = (struct queue_hdr *) calloc(1,
                        sizeof(struct queue_hdr));
        retval->lock = get_lock();

        return retval;
}

void enqueue(struct queue_hdr* hdr, void *elem)
{
        struct queue_node *node = (struct queue_node *) malloc(
                        sizeof(struct queue_node));
        node->elem = elem;
        node->next = NULL;

        enter_critical_section(hdr->lock);
        /*
         * Queue can either be empty or non-empty
         */

        if (hdr->first == NULL ) {  // empty queue
                hdr->first = node;
                hdr->last = node;
        } else {                    // non-empty queue
                hdr->last->next = node;
                hdr->last = node;
        }
        hdr->num_elems++;
        exit_critical_section(hdr->lock);
}

void *dequeue(struct queue_hdr *hdr)
{
        void* retval;
        struct queue_node *node;

        enter_critical_section(hdr->lock);
        if (hdr->first == NULL ) {
                exit_critical_section(hdr->lock);
                return NULL ;
        }
        node = hdr->first;
        hdr->first = node->next; // dequeue elem

        retval = node->elem;
        free(node); // free what we created in enqueue
        hdr->num_elems--;
        exit_critical_section(hdr->lock);

        return retval;
}

void* get_elem(struct queue_hdr *hdr, void* target, comparator cmp)
{
        int result;
        struct queue_node *node;
        enter_critical_section(hdr->lock);
        node = hdr->first;
        while (node != NULL ) {
                result = cmp(target, node->elem);
                if (result) {
                        exit_critical_section(hdr->lock);
                        return node->elem;
                } else
                        node = node->next;
        }
        exit_critical_section(hdr->lock);
        return NULL ;
}

void remove_elem(struct queue_hdr *hdr, void* target, comparator cmp)
{
        struct queue_node *node, *nodeprev;

        enter_critical_section(hdr->lock);
        nodeprev = hdr->first;
        node = nodeprev;

        while (node != NULL ) {
                if (cmp(target, node->elem)) {
                        /* Is target the first element? */
                        if (node == hdr->first) {
                                hdr->first = node->next;
                        }
                        /* Is target the last element? */
                        else if (node == hdr->last) {
                                hdr->last = nodeprev;
                                nodeprev->next = NULL;
                        }
                        /* Else it is in the middle of the list */
                        else {
                                nodeprev->next = node->next;
                        }
                        hdr->num_elems--;
                        free(node);
                        break;
                }
                nodeprev = node;
                node = node->next;
        }
        exit_critical_section(hdr->lock);
}

void** get_all_elems(struct queue_hdr *hdr)
{
        int n = num_elems(hdr);
        int i = 0;
        struct queue_node *node = hdr->first;
        void** retval = calloc(n, sizeof(void *));
        enter_critical_section(hdr->lock);
        while (node != NULL ) {
                retval[i++] = node->elem;
                node = node->next;
        }
        exit_critical_section(hdr->lock);
        return retval;
}

int num_elems(struct queue_hdr *hdr)
{
        int retval = 0;

        enter_critical_section(hdr->lock);
        retval = hdr->num_elems;
        exit_critical_section(hdr->lock);

        return retval;
}
