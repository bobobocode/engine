#!/usr/bin/env python3

#BoBoBo#

import yaml
import os
import json
import importlib
import requests
from pin.kit.common import get_conf


ss = None


def reset(servs_cfg_path):
    global ss
    try:
        with open(servs_cfg_path, mode='r') as ss:
            ss = yaml.load(ss, Loader=yaml.CLoader)
    except Exception as ex:
        print('Failed to reset servs cfg for: ' + str(ex))
        ss = None


cfg = get_conf('engine')
reset(cfg('pin', 'servs'))


def get_serv(path):
    server_cfg = None

    global ss
    if None is path or '' == path:
        return None
    else:
        elems = path.split('.')

        y = ss['servs']
        for e in elems:
            y = y[e]

        force_remote = y.get('remote', False)

        function_cfg = y.get('function', None)
        if not function_cfg:
            try:
                module_path = function_cfg[:function_cfg.rindex(".")]
                function_name = function_cfg[function_cfg.rindex("."):]
            except ValueError:
                force_remote = True

        server_cfg = y['server']
        timeout = y.get('timeout', 3)

    def _remote_serv(*args, **kw):
        nonlocal path
        nonlocal server_cfg

        if None is server_cfg:
            raise Exception('None server cfg.')

        url = 'http://' + server_cfg['host'] + \
            ':' + str(server_cfg['port']) + path
        resp = requests.post(url, json=kw, timeout=timeout)
        return resp.json()

        if force_remote:
            return _remote_serv
        else:
            try:
                module = importlib.import_module(module_path)
                fn = getattr(module, function_name)
                return fn
            except Exception as ex:
                print('Warning: Failed to import local servs moudle: ' +
                      module_path + ' for: ' + str(ex))
                print('Use remote servs to: ' + path)
                return _remote_serv
