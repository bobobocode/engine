# Engine

Engine is a Nginx module project to embed Python in Nginx,
which goal is to support rapid development of high-performance Web App.

Engine is based on high-performance capability of Nginx, and provide 
a lowcost and stand-alone way to develop and maintain middle and small-sized Web App.

With Engine: 

* Deploy Python functions to be web services easily
* Deploy WSGI App inside Nginx
* In future, you can use Engine with [Driven](https://github.com/bobobocode/driven.git)

The three ways above to use Engine could create Web App from zero to a big project.

## Requirement

- Python 3.6+
- nginx-1.19.3+ 

## Installation

```sh
$ git clone https://github.com/bobobocode/engine.git

$ wget 'http://nginx.org/download/nginx-1.19.3.tar.gz'
$ tar -zxvf nginx-1.19.3.tar.gz
$ cd nginx-1.19.3

$ ./configure --user=www --group=www \
      --prefix=/path/to/nginx \
      --add-module=/path/to/engine/engine \
      --with-ld-opt="`python3.6-config --ldflags`"

$ make
$ make install
```

## Usage

** Deploy Python functions **

1. The name of function should have same name with its python script

```
def helloworld(contenxt, p1, p2):
    return f'Hello World!\nQUERY_STRING:p1={p1}&p2={p2}'
```

This function should put  context as its first parameter. 
This parameter will be put the context environment data by Engine,
which is created by *with_context* command in nginx.conf.

2. Program python script to tell Engine how to create context environment data

The script must have an function named *build_context* which take
configure file as its parameter

You can make dict with data as database connnection pool and return
it in the function. Engine will call it and hold the data returned
which will be set as parameter when call functions with command *engine_func*

```
def build_context(conf_file):
    db = 'This is DB pool'
    return {'config': conf_file, 'db': db}
```

3. Web App Configuration File

The configuration file is for *build_context* build context data
Its path is defined in nginx.conf by command *with_context*, see 4

4. Config Nginx as below:

```

http {
    server {
        listen       8080;
        server_name  localhost;
        with_context /path/to/context-script context-module /path/to/config-file

        location /simple {
            engine_func  /path/to/function;
        }
    }
}

```

** Deploy WSGI App **

1. helloworld.py

```

#Any WSGI App
def wsgi_app(environ, start_response):
    params = environ['QUERY_STRING']
    status = '200 OK'
    response_headers = [('Content-type', 'text/plain')]
    start_response(status, response_headers)
    return ['Hello World!\n'.encode('utf-8'), ('QUERY_STRING:' + params).encode('utf-8')]

#Convert the WSGI App to engine_app
from engine.py.app import engine_app
app = engine_app(wsgi_app)
```

2. Config Nginx as below:

```

http {
    server {
        listen       8080;
        server_name  localhost;

        location /hello {
            engine_app /path/to/app helloworld app;
        }
    }
}

```

## Test

```sh
$ curl -G --data "p1=v1&p2=v2" http://localhost:8080/hello
Hello World!
QUERY_STRING:p1=v1&p2=v2
```

## 使用Docker容器

In dir docker, there`s docker compose environment for compiler and
debug Engine

In dir examples, there`s docker compose environment with which you can 
start the examples very quickly

In dir tests, you can run unit test in docker very conveniently

## Author

[BoBoBo](https://github.com/bobobocode)
