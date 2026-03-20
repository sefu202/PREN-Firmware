#!/usr/bin/env python3
"""
Filename: ControlUnit.py
Author: Aschwanden Josef (josef.aschwanden@stud.hslu.ch)
Date: 2026-03-12
Version: 1.0
Description: Interfacing with ControlUnit of PREN Object Placement machine
"""
import socket
import threading
import time
import copy

HOST = "10.0.1.1"   # Server-Adresse
PORT = 36769        # Server-Port
POLLING_INTERVAL_MS = 50
PEACE_WAR_SWITCH = "PEACE" # Set to WAR to enable confetti cannon

class ProcessImage:
    def __init__(self):
        self.btnEStop = True
        self.btnStart = False
        self.limitSwitches = [0,0,0,0,0,0]
        self.vacuumOn = False
        self.xPositionSteps = 0
        self.yPositionSteps = 0
        self.zPositionSteps = 0
        self.rotationSteps = 0
        self.isInitialized = False



class ControlUnit:
    def __init__(self):
        """
        Constructor
        """
        self._stopCommunication = False
        self._processImage = ProcessImage
        self._mutex = threading.Lock()  # only one 


    def getProcessImage(self):
        """
        Returns a copy of the most recent process image

        Returns:
            ProcessImage: most recent process image
        """
        return copy.deepcopy(self._processImage)


    def startCommunication(self):
        """
        Starts the communication thread        
        """
        self._stopCommunication = False
        self._thread = threading.Thread(target=ControlUnit.threadFunc, args=(self,))
        self._thread.start()


    def stopCommunication(self):
        """
        sends a signal to stop the communication thread
        """
        self._stopCommunication = True


    def echo(self, msg : str):
        """
        Sends a Echo request to the Control Unit

        Args:
            msg (str): string to send

        Returns:
            (str): echo reply string
        """
        responseMsg = ""
        with self._mutex:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                s.connect((HOST, PORT))
                message = createProtocol(0, bytes(msg, 'utf-8'))
                s.sendall(message)
                response = s.recv(1024)
                responseMsg = parseEchoReply(parseResponse(response))

        return responseMsg


    def setXTarget(self, targetSteps):
        """
        Sets X Target coords in steps

        Args:
            targetSteps (int): target coords in steps (must be positive)
        """
        if targetSteps < 0:
            raise Exception("Invalid x Target, steps must be positive")
        
        with self._mutex:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                s.connect((HOST, PORT))
                message = createProtocol(2, targetSteps.to_bytes(4, byteorder="big", signed=False))
                s.sendall(message)
                response = s.recv(1024)
                checkOtherReply(parseResponse(response))


    def setYTarget(self, targetSteps):
        """
        Sets Y Target coords in steps

        Args:
            targetSteps (int): target coords in steps (must be positive)
        """
        if targetSteps < 0:
            raise Exception("Invalid y Target, steps must be positive")
        
        with self._mutex:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                s.connect((HOST, PORT))
                message = createProtocol(3, targetSteps.to_bytes(4, byteorder="big", signed=False))
                s.sendall(message)
                response = s.recv(1024)
                checkOtherReply(parseResponse(response))


    def setZTarget(self, targetSteps):
        """
        Sets Z Target coords in steps

        Args:
            targetSteps (int): target coords in steps (must be positive)
        """
        if targetSteps < 0:
            raise Exception("Invalid z Target, steps must be positive")
        
        with self._mutex:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                s.connect((HOST, PORT))
                message = createProtocol(4, targetSteps.to_bytes(4, byteorder="big", signed=False))
                s.sendall(message)
                response = s.recv(1024)
                checkOtherReply(parseResponse(response))


    def setEnableVacuum(self, enableVacuum : bool):
        """
        Enables or disables vacuum 

        Args:
            enableVacuum (bool): true = enable vacuum, false = disable vacuum
        """
        if not isinstance(enableVacuum, bool):
            raise Exception("Invalid parameter enableVacuum must be bool")
        
        with self._mutex:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                s.connect((HOST, PORT))
                message = createProtocol(5, bytes([enableVacuum]))
                s.sendall(message)
                response = s.recv(1024)
                checkOtherReply(parseResponse(response))


    def setRot(self, rotationSteps):
        """
        Sets rotation target coords in steps

        Args:
            rotationSteps (int): Rotation target coords in steps (positive or negative)
        """ 
        with self._mutex:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                s.connect((HOST, PORT))
                message = createProtocol(6, rotationSteps.to_bytes(4, byteorder="big", signed=True))
                s.sendall(message)
                response = s.recv(1024)
                checkOtherReply(parseResponse(response))


    def shootConfetti(self):
        """
        Shoots Confetti cannon

        Note: PEACE_WAR_SWITCH must be set to "WAR" for Requests to be sent on the interface, otherwise this returns with no effect
        """
        if PEACE_WAR_SWITCH != "WAR":
            return
        
        with self._mutex:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                s.connect((HOST, PORT))
                message = createProtocol(7)
                s.sendall(message)
                response = s.recv(1024)
                checkOtherReply(parseResponse(response))


    def setLed(self, red, green, blue):
        """
        Sets led color and brightness

        Args:
            red (int): red brightness
            green (int): green brightness
            blue (int): blue brightness
        """
        if red > 255 or green > 255 or blue > 255 or red < 0 or green < 0 or blue < 0:
            raise Exception("Invalid color: 0 <= value <= 255")

        with self._mutex:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                s.connect((HOST, PORT))
                message = createProtocol(8, bytes([red, green, blue]))
                s.sendall(message)
                response = s.recv(1024)
                checkOtherReply(parseResponse(response))


    def threadFunc(self):
        """
        Thread function, runs polling of the control unit
        """
        next_time = time.time()
        while not self._stopCommunication:
            with self._mutex:
                with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                    s.connect((HOST, PORT))

                    message = createGetStatusRequest()

                    s.sendall(message)
                    response = s.recv(1024)
                    self._processImage = parseStateReply(parseResponse(response))

            next_time += POLLING_INTERVAL_MS/1000
            sleep_time = next_time - time.time()
            if sleep_time > 0:
                time.sleep(sleep_time)


    def join(self):
        """
        Joins communication thread
        """
        self._thread.join()


def createGetStatusRequest():
    return createProtocol(1)

def createProtocol(opcode, data : bytes = bytes([])):
    length = 2 + len(data)
    return bytes([opcode]) + bytes([length]) + data

def parseResponse(protocol):
    response= {}
    response["opcode"] = int(protocol[0])
    response["length"] = int(protocol[1])
    response["data"] = protocol[2:]
    return response

def parseStateReply(response):
    if response == None or response["opcode"] == None or response["opcode"] != 1 or response["length"] != 23 or response["data"] == None or len(response["data"]) != 21:
        raise Exception("Invalid StateReply")
    
    pi = ProcessImage()
    data = response["data"]
    pi.btnEStop = bool(data[0])
    pi.btnStart = bool(data[1])
    pi.limitSwitches = [(data[2] >> i) & 1 for i in range(6)]
    pi.vacuumOn = bool(data[3])
    pi.xPositionSteps = (data[ 4] << 24) | (data[ 5] << 16) | (data[ 6] << 8) | data[ 7]
    pi.yPositionSteps = (data[ 8] << 24) | (data[ 9] << 16) | (data[10] << 8) | data[11]
    pi.zPositionSteps = (data[12] << 24) | (data[13] << 16) | (data[14] << 8) | data[15]
    pi.rotationSteps  = (data[16] << 24) | (data[17] << 16) | (data[18] << 8) | data[19]
    pi.isInitialized = bool(data[20])
    return pi

def parseEchoReply(response):
    if response == None or response["opcode"] == None or response["opcode"] != 0 or response["data"] == None:
        raise Exception("Invalid EchoReply")
    
    return response["data"].decode('utf-8')

def checkOtherReply(response):
    if response == None or response["opcode"] == None or response["opcode"] != 255 or response["length"] != 3 or response["data"] == None or len(response["data"]) != 1:
        raise Exception("Invalid OtherReply")
    
    if int.from_bytes(response["data"], byteorder="big") == 1:
        raise Exception("Reply from control unit: Invalid value sent to control unit")
    
    if int.from_bytes(response["data"], byteorder="big") == 2:
        raise Exception("Reply from control unit: Invalid opcode sent to control unit")
    
            

if __name__ == "__main__":

    cu = ControlUnit()
    cu.startCommunication()
    cu.setEnableVacuum(True)

    cu.stopCommunication()

    cu.join()

