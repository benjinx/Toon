#!/usr/bin/env python3

import os
import sys
import json
import copy

isWindows = (os.name == 'nt')

toonproj = sys.argv[1]      # Path to .toonproj
binaryDir = sys.argv[2]     # CMAKE_BINARY_DIR
executable = sys.argv[3]    # Path to executable
assetPath = sys.argv[4]     # Asset search path, separated by ';'
modulePath = sys.argv[5]    # Module search path, separated by ';'

assetPath = assetPath.replace(';', os.pathsep)
modulePath = modulePath.replace(';', os.pathsep)

projectDirectory = os.path.dirname(toonproj)

project = {}
try:
    file = open(toonproj, 'r')
    project = json.load(file)
except:
    pass

def add_or_update_config(data, configurations):
    found = False
    for i in range(0, len(configurations)):
        if configurations[i]['name'] == data['name']:
            configurations[i] = data
            found = True
            break
    
    if not found:
        configurations.append(data)

if os.path.isdir('.vscode'):
    filename = '.vscode/launch.json'

    launch = {}
    try:
        file = open(filename, 'r')
        launch = json.load(file)
    except:
        pass

    if 'version' not in launch:
        launch['version'] = '0.2.0'

    if 'configurations' not in launch:
        launch['configurations'] = []

    default = {
        'name': '',
        'type': 'cppdbg',
        'request': 'launch',
        'program': os.path.join(binaryDir, executable),
        'cwd': projectDirectory,
        'environment': [
            {
                'name': 'TOON_ASSET_PATH',
                'value': assetPath
            }
        ],
        'externalConsole': False
    }
    
    if isWindows:
        default['type'] = 'cppvsdbg'
        default['externalConsole'] = True
        default['environment'].append({
            'name': 'PATH',
            'value': '${env:PATH};' + modulePath
        })
    else:
        default['environment'].append({
            'name': 'LD_LIBRARY_PATH',
            'value': modulePath
        })

    if 'configurations' in project:
        for name,config in project['configurations'].items():
            data = copy.deepcopy(default)
            data['name'] = "{} ({})".format(project['name'], name)
            data['args'] = [ toonproj, '-c', name ]

            add_or_update_config(data, launch['configurations'])
    else:
        data = copy.copy(default)
        data['name'] = project['name']
        data['args'] = [ toonproj ]

        add_or_update_config(data, launch['configurations'])

    file = open(filename, 'w')
    json.dump(launch, file, indent=4)

if isWindows and os.path.isdir('.vs'):
    filename = '.vs/launch.vs.json'

    launch = {}
    try:
        file = open(filename, 'r')
        launch = json.load(file)
    except:
        pass

    if 'version' not in launch:
        launch['version'] = '0.2.1'

    if 'configurations' not in launch:
        launch['configurations'] = []

    default = {
        'name': '',
        'type': 'default', # dll
        # 'exe': executable,
        'project': 'CMakeLists.txt',
        'projectTarget': '{} ({})'.format(os.path.basename(executable), executable),
        'cwd': projectDirectory,
        'env': {
            'PATH': '${env.PATH};' + modulePath,
            'TOON_ASSET_PATH': assetPath,
        },
    }

    if 'configurations' in project:
        for name,config in project['configurations'].items():
            data = copy.deepcopy(default)
            data['name'] = "%s (%s)" % (project['name'], name)
            data['args'] = [ toonproj, '-c', name ]

            add_or_update_config(data, launch['configurations'])
    else:
        data = copy.copy(default)
        data['name'] = project['name']
        data['args'] = [ toonproj ]

        add_or_update_config(data, launch['configurations'])

    file = open(filename, 'w')
    json.dump(launch, file, indent=4)
