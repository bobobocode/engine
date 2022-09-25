#!/usr/bin/env python3

# BoBoBo

from boon.protocol.context import _build_context


def build_context(config_file):
    return _build_context(config_file, 'test-conf')
