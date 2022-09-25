/*BoBoBo*/

#include "engine_pin.h"


PyObject *pyo_engine_app_py;
PyObject *pyo_engine_app;

PyObject *pyo_engine_func_py;
PyObject *pyo_engine_func;


ngx_str_t
pin(ngx_http_request_t *r)
{
    PyObject *pin_request = assemble_dict(r);
    PyObject *pin_response = engine_app_ex(pin_request);
    return assemble_response(r , pin_response);
}


ngx_str_t
pin_func(ngx_http_request_t *r)
{
    PyObject *pin_request = assemble_dict(r);
    PyObject *pin_response = engine_func_ex(pin_request);
    return assemble_response(r , pin_response);
}


PyObject *
assemble_dict(ngx_http_request_t *r)
{
    PyObject *pArgsT = PyTuple_New(1);
    PyObject *pArgsD = PyDict_New();

    ngx_str_t uri = r->uri;
    char *uri_dst = (char *)malloc(sizeof(char) * (uri.len + 1));
    memcpy(uri_dst, uri.data, uri.len);
    uri_dst[uri.len] = '\0';

    ngx_str_t args = r->args;
    char *args_dst = (char *)malloc(sizeof(char) * (args.len + 1));
    memcpy(args_dst, args.data, args.len);
    args_dst[args.len] = '\0';

    PyDict_SetItemString(pArgsD, "QUERY_STRING", Py_BuildValue("s", args_dst));
    free(args_dst);
    PyDict_SetItemString(pArgsD, "uri", Py_BuildValue("s", uri_dst));
    free(uri_dst);

    PyTuple_SetItem(pArgsT, 0, pArgsD);
    return pArgsT;
}


ngx_str_t
assemble_response(ngx_http_request_t *r, PyObject *pin_response)
{
    //TODO: make response from dict
    r->headers_out.status = NGX_HTTP_OK;
    r->headers_out.content_type.len = sizeof("text/plain") - 1;
    r->headers_out.content_type.data = (u_char*)"text/plain";

    ngx_str_t ns = ngx_string("NULL");
    if(NULL != pin_response){
        PyObject *pContent = PyDict_GetItemString(pin_response, "content");
        char *content;
        PyArg_Parse(pContent, "s", &content);
        ns.len = strlen(content);
        ns.data = (u_char *)content;
    }
    return ns;
}


int
engine_app_initialize(char *py_path, char *py_file, char *py_func, ngx_conf_t *cf)
{
    if (!Py_IsInitialized()) {
        Py_Initialize();
        if (!Py_IsInitialized()) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                    "Failed to init for command engine_app: %s",
                    "failed to init python vm");
            return -1;
        }
    }

    PyRun_SimpleString("import sys");

    char *syspath_stmt = NULL;
    strcatn(&syspath_stmt, "sys.path.append('", py_path ,"')", NULL);
    PyRun_SimpleString(syspath_stmt);
    ngx_conf_log_error(NGX_LOG_NOTICE, cf, 0,
            "Add python module path for command engine_app: %s", py_path);

    PyObject *pName, *pModule;
    pName = PyUnicode_FromString(py_file);
    pModule = PyImport_Import(pName);
    if (!pModule) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                "Failed to init for command engine_app: %s",
                "failed to import python app module");
        return -1;
    }
    pyo_engine_app_py = PyModule_GetDict(pModule);
    if (!pyo_engine_app_py) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                "Failed to init for command engine_app: %s",
                "failed to get python app module dict");
        return -1;
    }

    pyo_engine_app = PyDict_GetItemString(pyo_engine_app_py, py_func);
    if (!pyo_engine_app || !PyCallable_Check(pyo_engine_app)) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                "Failed to init for command engine_app: %s",
                "failed to get python app function");
        return -1;

    }
    return 0;
}


int
engine_func_ctx_initialize(char *py_ctx_path, char *py_ctx_module,
        char *py_conf_path, ngx_conf_t *cf)
{
    if (!Py_IsInitialized()) {
        Py_Initialize();
        if (!Py_IsInitialized()) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                    "Failed to init for command with_context: %s",
                    "failed to init python vm");
            return -1;
        }
    }

    PyRun_SimpleString("import sys");

    char *syspath_stmt = NULL;
    strcatn(&syspath_stmt, "sys.path.append('", py_ctx_path ,"')", NULL);
    PyRun_SimpleString(syspath_stmt);
    ngx_conf_log_error(NGX_LOG_NOTICE, cf, 0,
            "Add python module path for command with_context: %s", py_ctx_path);

    PyObject *pName, *pModule;
    pName = PyUnicode_FromString(py_ctx_module);
    pModule = PyImport_Import(pName);
    if (!pModule) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                "Failed to init for command with_context: %s",
                "failed to import func context module");
        return -1;
    }

    pName = PyUnicode_FromString("engine_py.func");
    pModule = PyImport_Import(pName);
    if (!pModule) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                "Failed to init for command with_context: %s",
                "failed to import engine_py.func");
        return -1;
    }
    PyObject *pyo_py = PyModule_GetDict(pModule);
    if (!pyo_py) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                "Failed to init for command with_context: %s",
                "failed to get engine_py.func dict");
        return -1;
    }

    PyObject *pyo_func = PyDict_GetItemString(pyo_py, "build_context");
    if (!pyo_func || !PyCallable_Check(pyo_func)) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                "Failed to init for command with_context: %s",
                "failed to get engine_py.func.build_context");
        return -1;
    }

    PyObject *pArgsT = PyTuple_New(1);
    PyObject *pArgsD = PyDict_New();
    PyDict_SetItemString(pArgsD, "context_module_name",
            Py_BuildValue("s", py_ctx_module));
    PyDict_SetItemString(pArgsD, "context_conf_file",
            Py_BuildValue("s", py_conf_path));
    PyTuple_SetItem(pArgsT, 0, pArgsD);
    PyEval_CallObject(pyo_func, pArgsT);

    return 0;
}


int
engine_func_initialize(char *py_func_path, ngx_conf_t *cf)
{
    if (!Py_IsInitialized()) {
        Py_Initialize();
        if (!Py_IsInitialized()) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                    "Failed to init for command engine_func: %s",
                    "failed to init python vm");
            return -1;
        }
    }

    PyRun_SimpleString("import sys");

    char *syspath_stmt = NULL;
    strcatn(&syspath_stmt, "sys.path.append('", py_func_path ,"')", NULL);
    PyRun_SimpleString(syspath_stmt);
    ngx_conf_log_error(NGX_LOG_NOTICE, cf, 0,
            "Add python module path for command engine_func: %s", py_func_path);

    PyObject *pName, *pModule;
    pName = PyUnicode_FromString("engine_py.func");
    pModule = PyImport_Import(pName);
    if (!pModule) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                "Failed to init for command engine_func: %s",
                "failed to import engine.py.func");
        return -1;
    }
    pyo_engine_func_py = PyModule_GetDict(pModule);
    if (!pyo_engine_func_py) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                "Failed to init for command engine_func: %s",
                "failed to get engine.py.func dict");
        return -1;
    }

    pyo_engine_func = PyDict_GetItemString(pyo_engine_func_py, "route");
    if (!pyo_engine_func || !PyCallable_Check(pyo_engine_func)) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                "Failed to init for command engine_func: %s",
                "failed to get engine.py.func.route");
        return -1;
    }

    return 0;
}


void
engine_app_finalize()
{
    Py_Finalize();
}


PyObject *
engine_app_ex(PyObject *pArgsT)
{
    return engine_ex(pyo_engine_app, pArgsT);
}


PyObject *
engine_func_ex(PyObject *pArgsT)
{
    return engine_ex(pyo_engine_func, pArgsT);
}


PyObject *
engine_ex(PyObject *pyo, PyObject *pArgsT)
{
    if (!pyo || !PyCallable_Check(pyo)) {
        PyObject *pDict = PyDict_New();
        PyDict_SetItemString(pDict, "content", Py_BuildValue("s", "ERROR"));
        return pDict;
    }
    return PyEval_CallObject(pyo, pArgsT);
}
