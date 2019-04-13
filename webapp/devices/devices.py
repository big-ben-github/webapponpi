import json
from . import unixsocket

def lampcontrol(operation):
    message = {'device': 'lamp',
               'operation': operation}
    print('******sending message with unix socket******')
    echo = unixsocket.clientsocket(json.dumps(message))
    print('******received {} with unix socket******', echo)
    if json.loads(echo)['res'] == 'ok':
        return True
    else:
        return False


'''
from ctypes import CDLL

def toggle(status):
    led = CDLL('./raspberrypiled.so') # may use absolute path when on raspberry-pi
    ledstatus = {'OFF': 0, 'ON': 1}
    res = led.toggleled(ledstatus[status])
    print('the led status now is %s' % ('ON' if res == 1 else 'OFF'))
    return res
'''
