#!/usr/bin/env python3

#BoBoBo#

import requests


def test_get_engine_app():
    resp = requests.get('http://localhost:8080/app/anystr',
                        params={'p1': 'v1', 'p2': 'v2'})
    assert not resp.text is None
    print(resp.text)
    lines = resp.text.split('\n')
    assert 'Hello World!' == lines[0]
    assert 'QUERY_STRING:p1=v1&p2=v2' == lines[1]

def test_get_engine_func():
    resp = requests.get('http://localhost:8080/func/hello',
                        params={'p1': 'v1', 'p2': 'v2'})
    assert not resp.text is None
    print(resp.text)
    lines = resp.text.split('\n')
    assert 'Hello World!' == lines[0]
    assert 'QUERY_STRING:p1=v1&p2=v2' == lines[1]
