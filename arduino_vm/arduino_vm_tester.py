# python serial hex test
# winxos 2016-09-23
import time
import serial
import serial.tools.list_ports as slp
from threading import Thread

lps = slp.comports()
ser = ""
if len(lps) > 1:
    ser = lps[1].device  # com name
elif len(lps) == 1:
    ser = lps[0].device  # com name


class getcmd(Thread):
    def __init__(self, s: serial):
        Thread.__init__(self)
        self.ser = s

    def run(self):
        while True:
            s = self.ser.read(1)
            print(s.decode('utf-8'), end="")


def pack_msg(n):
    cmd_out = [0xE5, 0x00]
    for i in n:
        cmd_out.append(ord(i))
    cmd_out[1] = (len(n))
    cmd_out.append(0)  # check_sum
    return cmd_out


if ser != None:
    t = serial.Serial(ser, 250000)
    getcmd(t).start()
    time.sleep(0.2)
    while True:
        cmd = str(input("\ninput>")).strip()
        t.write(pack_msg(cmd))
        time.sleep(0.1)
    t.close()
