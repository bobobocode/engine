/*BoBoBo*/

#ifndef _NGX_HTTP_ENGINE_MODULE_H_INCLUDED_
#define _NGX_HTTP_ENGINE_MODULE_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include <nginx.h>
#include "engine_pin.h"


typedef struct {
    ngx_str_t engine_app;
} ngx_http_engine_conf_t;

typedef struct {
    ngx_str_t engine_func;
    ngx_str_t engine_func_ctx;
} ngx_http_engine_func_conf_t;


static char * ngx_http_set_engine_app(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static char * ngx_http_set_engine_func(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static char * ngx_http_set_engine_func_ctx(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);


static ngx_int_t ngx_http_engine_handler(ngx_http_request_t *r);
static ngx_int_t ngx_http_engine_func_handler(ngx_http_request_t *r);


ngx_int_t ngx_response_pin(ngx_http_request_t *r, ngx_str_t response);

#endif /* _NGX_HTTP_ENGINE_MODULE_H_INCLUDED_ */
