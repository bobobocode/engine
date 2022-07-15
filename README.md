# Engine

[engine](https://github.com/bobobocode/engine.git)
- Embedded Python in Nginx 

* Request python functions with context object
* Support WSGI

## Todo

* Filter

## Requirement

- Python 3.6+
- nginx-1.6.3+ 

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

1.Program python wsgi app like this, or you can use web framework driven to develop wsgi app.

```
#File: hello.py

from engine.py.app import engine_app


def wsgi_app(environ, start_response):
    params = environ['QUERY_STRING']
    status = '200 OK'
    response_headers = [('Content-type', 'text/plain')]
    start_response(status, response_headers)
    return ['Hello World!\n'.encode('utf-8'), ('QUERY_STRING:' + params).encode('utf-8')]


app = engine_app(wsgi_app)
```

2.Configure nginx like the example below and start it:

```
events {}

http {
    server {
        error_log /usr/local/nginx/logs/debug.log debug;
        listen       8080;
        server_name  localhost;

        location /simple {
            engine_func  /functions-path;
            with_context /functions-path/context.py
        }

        location /hello {
            engine_app /path/to/app hello app;
        }
    }
}
```

3.Request and get the response like this:

```sh
$ curl -G --data "p1=v1&p2=v2" http://localhost:8080/hello
Hello World!
QUERY_STRING:p1=v1&p2=v2
```

docker(just an enviroment to run engine from code now):

```
cd /path/to/engine/docker
docker-compose up -d engine_compiler
```

## Test

You can run auto test shell scripts in directory tests.
These scripts run on docker. You just need docker engine.

## Contributors

- BoBoBo
