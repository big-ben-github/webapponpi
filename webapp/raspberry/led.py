from ctypes import CDLL


def toggle(status):
    led = CDLL('./raspberrypiled.so')

    ledstatus = {'OFF': 0, 'ON': 1}

    res = led.toggleled(ledstatus[status])

    print('the led status now is %s' % ('ON' if res == 1 else 'OFF'))

    return res
