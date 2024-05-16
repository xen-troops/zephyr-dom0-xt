# Copyright (c) 2023 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: Apache-2.0

import logging
import re

from twister_harness import Shell

logger = logging.getLogger(__name__)


def test_shell_print_help(shell: Shell):
    logger.info('send "help" command')
    lines = shell.exec_command('help')
    assert 'Available commands:' in lines, 'expected response not found'
    logger.info('response is valid')


def test_shell_print_version(shell: Shell):
    logger.info('send "kernel version" command')
    lines = shell.exec_command('kernel version')
    assert any(['Zephyr version' in line for line in lines]), 'expected response not found'
    logger.info('response is valid')

def test_dom_create(shell: Shell):
    logger.info('send "xu config_list" command')
    lines = shell.exec_command('xu config_list')
    assert any(['rpi_5_domu' in line for line in lines]), 'expected response "rpi_5_domu" not found'
    logger.info('response is valid')

    logger.info('send "xu create" command')
    lines = shell.exec_command('xu create rpi_5_domu -p')
    r = re.compile(".*domain:.*created")
    newlist = list(filter(r.match, lines))
    assert newlist != [], 'expected response "created" not found'
    ss = newlist[0].split("domain:")[1]
    ss = ss.split()[0]
    domid = int(ss)
    logger.info('response is valid domid:' + str(domid))

    logger.info('send "xu unpause" command')
    lines = shell.exec_command('xu unpause '+ str(domid))
    assert any(['unpaused' in line for line in lines]), 'expected response "unpaused" not found'
    logger.info('response is valid')

    logger.info('send "xu pause" command')
    lines = shell.exec_command('xu pause '+ str(domid))
    assert any(['paused' in line for line in lines]), 'expected response "paused" not found'
    logger.info('response is valid')

    logger.info('send "xu destroy" command')
    lines = shell.exec_command('xu destroy '+ str(domid))
    assert any(['destroy' in line for line in lines]), 'expected response "destroy" not found'
    logger.info('response is valid')