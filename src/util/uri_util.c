#include "uri_util.h"

#include <stdlib.h>

int parse_uri(uri_t *dst, char *str) {
    UriParserStateA state;
    state.uri = dst;

    return uriParseUriA(&state, str);
}

char *textrange2str(UriTextRangeA *range) {
    return strndup(
        range->first,
        range->afterLast - range->first
    );
}

char *uri_scheme(uri_t *uri) {
    return textrange2str(&(uri->scheme));
}

char *uri_userinfo(uri_t *uri) {
    return textrange2str(&(uri->userInfo));
}

char *uri_host(uri_t *uri) {
    return textrange2str(&(uri->hostText));
}

int uri_port(uri_t *uri) {
    UriTextRangeA range = uri->portText;
    int port = 0;

    for (const char *p = range.first; p < range.afterLast; p++)
        port = (port * 10) + (*p - '0');

    return port;
}

int uri_query(uri_query_list_t **dst, uri_t *uri, int *items) {
    return uriDissectQueryMallocA(
        dst, items,
        uri->query.first, uri->query.afterLast
    );
}

uri_query_list_t *uri_query_list_append(char *key, char *value,
    uri_query_list_t *dst, uri_query_list_t *prev) {

    dst->key   = key;
    dst->value = value;
    dst->next  = NULL;

    if (prev != NULL) {
        prev->next = dst;
    }

    return dst;
}

int uri_to_string(uri_t *uri, char **dst) {
    int len;
    if (uriToStringCharsRequiredA(uri, &len) != URI_SUCCESS)
        return -1;

    len++; // trailing '\0'

    *dst = malloc(len);
    if (uriToStringA(*dst, uri, len, NULL) != URI_SUCCESS)
        return -1;

    return len - 1;
}
