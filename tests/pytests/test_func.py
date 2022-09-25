#!/usr/bin/env python3

# BoBoBo

import os
import engine_py.func as func


def test_build_context():
    context_material = {}
    context_material['context_module_name'] = 'pytests.context'
    context_material['context_conf_file'] = os.path.dirname(__file__) + \
        '/conf.yaml'

    ctx = func.build_context(context_material)
    assert 'conf' in ctx


def test_route():
    r = {'uri': '/pytests/hello', 'QUERY_STRING': 'p1=v1&p2=v2'}
    response = func.route(r)
    assert 'v1' in response['content']
