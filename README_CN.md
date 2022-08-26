# Engine

Engine是一个将Python嵌入到Nginx中的Nginx模块项目.
其目标是支持快速开发高性能的Web App.

Engine基于Nginx的高性能处理能力, 致力于低成本高性能(包括开发维护成本和部署运行成本)地支撑中小规模的Web应用程序.

使用Engine可以

* 将Python函数快速部署成Web服务
* 将WSGI应用程序部署在Nginx内部
* 下一步还将与[Driven](https://github.com/bobobocode/driven.git)项目形成配合, 使Nginx可以调用Driven异步执行的WSGI App

以上三种方式可以支持Web服务项目从零开始逐步发展成长成复杂工程的过程.

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

** 部署Python函数 **

1. 要部署的函数需要与其所在的python脚本文件同名

```
def helloworld(contenxt, p1, p2):
    return f'Hello World!\nQUERY_STRING:p1={p1}&p2={p2}'
```

该函数至少有一个context参数, 用于获取Web应用程序执行的上下文. 该上下文环境是Engine初始化并持有的, 在驱动的多个python函数之间共享

2. 编写context脚本文件, 定义**build_context**函数, 实现数据库连接初始化等处理

```
def build_context(conf_file):
    db = 'This is DB pool'
    return {'config': conf_file, 'db': db}
```

*build_context*的返回值将被用作每个部署函数的第一个参数调用

3. Web应用的配置文件

*build_context*函数的参数为项目的配置文件路径
该路径值由Nginx配置文件内定义, 见4

4. 配置nginx如下:

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

** 部署wsgi应用程序 **

1. helloworld.py

```

#任意wsgi应用
def wsgi_app(environ, start_response):
    params = environ['QUERY_STRING']
    status = '200 OK'
    response_headers = [('Content-type', 'text/plain')]
    start_response(status, response_headers)
    return ['Hello World!\n'.encode('utf-8'), ('QUERY_STRING:' + params).encode('utf-8')]

#将wsgi app转化为engine_app, 以供Engine驱动
from engine.py.app import engine_app
app = engine_app(wsgi_app)
```

2. 配置Nginx如下:

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

当前还只支持URL加参数的GET请求

```sh
$ curl -G --data "p1=v1&p2=v2" http://localhost:8080/hello
Hello World!
QUERY_STRING:p1=v1&p2=v2
```

## 使用Docker容器

docker目录下提供了docker编译调试的实验环境

examples目录下提供了docker一键启动的示例

tests目录下提供了单元测试用docker环境

## Author

[BoBoBo](https://github.com/bobobocode)
