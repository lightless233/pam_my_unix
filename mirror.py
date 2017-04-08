#!/usr/bin/env python3
# coding: utf-8

import sys
import socket
import datetime

import paramiko


if len(sys.argv) == 1:
    today = datetime.date.today()
    filename = "/home/password.txt.{0}".format(today.strftime("%Y%m%d"))
else:
    filename = sys.argv[1]

with open(filename, "r") as ff:
    for line in ff:
        line = line.strip()
        ip, username, password = line.split("|<*>|")
        ssh = paramiko.SSHClient()
        ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
        try:
            ssh.connect(ip, 22, username, password, timeout=30)
        except (paramiko.SSHException, socket.timeout):
            continue

