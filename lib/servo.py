import subprocess

class servo:

    def __init__(self, min, max, trans, pololu_number):
        self.min = min
        self.max = max
        self.trans = trans
        self.pololu = pololu_number
        self.command = "./UscCmd --servo "

    def run(self, degre):
        command_to_exe = self.command + self.pololu + "," + str(self.degre*self.trans)
        process = subprocess.Popen(command_to_exe.split(), stdout=subprocess.PIPE, cwd='/homer/pi/Documents/pololu-usb-sdk/Maestro/UscCmd')
        output, error = process.communicate()
