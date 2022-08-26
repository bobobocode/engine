/*BoBoBo*/

#ifndef _ENGINE_PIN_H_INCLUDED_
#define _ENGINE_PIN_H_INCLUDED_

#include <Python.h>
#include <ngx_http.h>
#include <ngx_http_request.h>
#include "utils.h"


ngx_str_t pin(ngx_http_request_t *r);
ngx_str_t pin_func(ngx_http_request_t *r);


int engine_app_initialize(char *py_path, char *py_file, char *py_func, ngx_conf_t *cf);
int engine_func_initialize(char *py_func_path, ngx_conf_t *cf);
int engine_func_ctx_initialize(char *py_ctx_path, char *py_ctx_file, char *py_conf_path, ngx_conf_t *cf);

void engine_app_finalize();


PyObject *engine_app_ex(PyObject *pArgsT);
PyObject *engine_func_ex(PyObject *pArgsT);
PyObject * engine_ex(PyObject *pyo, PyObject *pArgsT);


PyObject *assemble_dict(ngx_http_request_t *r);
ngx_str_t assemble_response(ngx_http_request_t *r, PyObject *pin_response);


#endif /* _ENGINE_PIN_H_INCLUDED_ */
