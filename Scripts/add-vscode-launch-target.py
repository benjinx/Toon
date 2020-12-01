#!/usr/bin/env python3

import os
import sys
import json

filename = sys.argv[1] # Path to launch.json
name = sys.argv[2] # Name of debug target
program = sys.argv[3] # Path to executable
cwd = sys.argv[4] # Working directory

externalConsole = False
debugType = 'cppdbg'

if os.name == 'nt':
    externalConsole = True
    debugType = 'cppvsdbg'

# Pairs of KEY=VALUE environment variables
environment = []
for i in range(5, len(sys.argv)):
    parts = sys.argv[i].split('=')
    environment.append({
        "name": parts[0],
        "value": parts[1]
    })


data = {}
try:
    file = open(filename, 'r')
    data = json.load(file)
except:
    pass

if 'version' not in data:
    data['version'] = '0.2.0'

if 'configurations' not in data:
    data['configurations'] = []

config = {
    "name": name,
    "type": debugType,
    "request": "launch",
    "program": program,
    "cwd": cwd,
    "environment": environment,
    "externalConsole": externalConsole
}

print('Generating launch rule for %s' % name)

found = False
for i in range(0, len(data['configurations'])):
    if data['configurations'][i]['name'] == name:
        data['configurations'][i] = config

        found = True
        break

if not found:
    data['configurations'].append(config)

file = open(filename, 'w')
json.dump(data, file, indent=2)