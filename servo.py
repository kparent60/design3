import subprocess

class servo:

    def __init__(self, min, max, trans, pololu_number):
        self.min = min
        self.max = max
        self.trans = trans
        self.pololu = pololu_number
        self.command = "./UscCmd --servo "

    def run(self, degre):
        command_to_exe = self.command + str(self.pololu) + "," + str(degre)
        print(command_to_exe)
        process = subprocess.Popen(command_to_exe.split(), stdout=subprocess.PIPE, cwd='/home/pi/Documents/pololu-usb-sdk/Maestro/UscCmd')
        output, error = process.communicate()
