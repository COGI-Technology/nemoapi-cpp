#ifndef NEMOAPI_H
#define NEMOAPI_H

#include "nemoapi/dsa.h"
#include "nemoapi/utils.h"
#include "nemoapi/api/authentication.h"
#include "nemoapi/api/client.h"
#include <iostream>
#include <rapidjson/document.h>
#include <ctime>
#include <chrono>



inline void print_headers(struct curl_slist* headers) {
    struct curl_slist* node = headers;
    printf("Start\n");
    while (node != NULL) {
        printf("%s\n", node->data);
        node = node->next;
    }
    printf("End\n");
}

#endif