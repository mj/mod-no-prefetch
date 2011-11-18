/* Copyright 2005 Martin Jansen <martin@divbyzero.net>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* 
 * Simple module to block "prefetch" requests from Mozilla or Google Web Accelerator
 */

#include "httpd.h"
#include "http_config.h"
#include "http_log.h"

module MODULE_VAR_EXPORT no_prefetch_module;

/* Header parser
 *
 * This function will search for the existance of the X-moz or
 * HTTP_X_MOZ header.  If one of those headers exists and has the
 * value "prefetch", HTTP_FORBIDDEN is returned.  Otherwise
 * DECLINED is returned.
 */
static int no_prefetch_header_parser(request_rec *r)
{
    const char *prefetch;

    /* First look for a simple Mozilla prefetch request */
    prefetch = ap_table_get(r->headers_in, "X-moz");

    if (!prefetch) {
        /* Then look for a Google Web Accelerator request */
        prefetch = ap_table_get(r->headers_in, "HTTP_X_MOZ");
    }

    if (prefetch) {
        if (strcasecmp(prefetch, "prefetch") == 0) {
            ap_log_error(NULL, 0, APLOG_ERR | APLOG_NOERRNO, r->server, "Blocking prefetch request from %s", r->connection->remote_ip);
            return HTTP_FORBIDDEN;
        }
    }

    return DECLINED;
}

/* 
 * Module definition for configuration.
 */
module MODULE_VAR_EXPORT no_prefetch_module =
{
    STANDARD_MODULE_STUFF,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    no_prefetch_header_parser, /* header parser */
    NULL,
    NULL,
    NULL
};
