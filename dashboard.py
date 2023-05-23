import serial
import requests
import time
import dateutil.parser
import binascii
import datetime
import msvcrt

MainPort = 'COM4'
TempFuelPort = 'COM5'
serialBaudrate = 115200

useWholeRPM = False
exit = False
pendulum = False
clock = False

largeGaugeRotation = 147
smallGaugeRotation = 70

telemetryServer = "http://127.0.0.1:25555/api/ets2/telemetry"

serMain = serial.Serial(port=MainPort, baudrate=serialBaudrate)
serTempFuel = serial.Serial(port=TempFuelPort, baudrate=serialBaudrate)

serMain.flushInput()
serTempFuel.flushInput()

cargoDamageTolerancy = 0.02
truckDamageTolerancy = 0.05

while (serMain.is_open and serTempFuel.is_open and not exit):
    telemetry = requests.get(telemetryServer).json()
    game = telemetry["game"]

    if (game["connected"] and not game["paused"]):
        truck = telemetry["truck"]
        cargo = telemetry["cargo"]

        packet = bytearray()

        RPM = int((truck["engineRpm"]))
        output = 0
        if (useWholeRPM and truck["engineRpmMax"] > 0):
            if (RPM > int(truck["engineRpmMax"])):
                RPM = int(truck["engineRpmMax"])
            output = int((RPM / truck["engineRpmMax"]) * largeGaugeRotation)
        else:
            if (RPM > 6000):
                RPM = 6000
            output = int((RPM / 6000) * largeGaugeRotation)

        packet += binascii.hexlify(output.to_bytes(1, 'little'))

        speed = abs(truck["speed"])
        if (speed > 240):
            speed = 240
        output = int((speed / 240) * largeGaugeRotation)
        packet += binascii.hexlify(output.to_bytes(1, 'little'))

        output = cargo["cargoLoaded"] and cargo["damage"] > cargoDamageTolerancy
        packet += binascii.hexlify(output.to_bytes(1, 'little'))

        packet += binascii.hexlify(truck["blinkerRightOn"].to_bytes(1, 'little'))
        packet += binascii.hexlify(truck["lightsBeaconOn"].to_bytes(1, 'little'))

        output = truck["lightsParkingOn"] or truck["lightsBeamLowOn"]
        packet += binascii.hexlify(output.to_bytes(1, 'little'))

        packet += binascii.hexlify(truck["lightsBeamHighOn"].to_bytes(1, 'little'))
        packet += binascii.hexlify(truck["blinkerLeftOn"].to_bytes(1, 'little'))

        truckDamaged = truck["wearEngine"] > truckDamageTolerancy
        packet += binascii.hexlify(truckDamaged.to_bytes(1, 'little'))

        packet += binascii.hexlify(truck["adblueWarningOn"].to_bytes(1, 'little'))
        packet += binascii.hexlify(truck["cruiseControlOn"].to_bytes(1, 'little'))

        packet += b'Z'

        binascii.hexlify(packet)
        # print(packet)
        serMain.write(packet)

        packet = bytearray();

        temp = int(truck["waterTemperature"])
        if (temp < 50 or not truck["engineOn"]):
            temp = 50
        if (temp > 130):
            temp = 130
        output = int(((temp - 50) / 80) * smallGaugeRotation)
        packet += binascii.hexlify(output.to_bytes(1, 'little'))

        fuel = 0
        if ((truck["fuelCapacity"] > 0) and truck["engineOn"]):
            fuel = truck["fuel"] / truck["fuelCapacity"]

        output = int(fuel * smallGaugeRotation)
        packet += binascii.hexlify(output.to_bytes(1, 'little'))

        packet += binascii.hexlify(truck["oilPressureWarningOn"].to_bytes(1, 'little'))
        packet += binascii.hexlify(truck["waterTemperatureWarningOn"].to_bytes(1, 'little'))

        output = truck["airPressureWarningOn"] or truck["airPressureEmergencyOn"]
        packet += binascii.hexlify(output.to_bytes(1, 'little'))

        output = truckDamaged and truck["wipersOn"]
        packet += binascii.hexlify(output.to_bytes(1, 'little'))

        packet += binascii.hexlify(truck["fuelWarningOn"].to_bytes(1, 'little'))
        packet += binascii.hexlify(truck["batteryVoltageWarningOn"].to_bytes(1, 'little'))
        packet += binascii.hexlify(truck["motorBrakeOn"].to_bytes(1, 'little'))
        packet += binascii.hexlify(truck["parkBrakeOn"].to_bytes(1, 'little'))

        packet += b'Z'

        binascii.hexlify(packet)
        # print(packet)
        serTempFuel.write(packet)


    elif clock:
        def int_to_bool_list(num):
            bin_string = format(num, '05b')
            return [x == '1' for x in bin_string[::1]]


        Time = datetime.datetime.now()

        Second = Time.second / 60
        Minute = (Time.minute + Second) / 60
        Hour = (Time.hour + Minute) / 24
        # print(str(Second)+" "+str(Minute)+" "+str(Hour))

        Day = int_to_bool_list(Time.day)
        Month = int_to_bool_list(Time.month)
        Weekday = int_to_bool_list(Time.weekday() + 1)
        # print(Day)
        # print(Month)
        # print(Weekday)

        Space = 0

        packet = bytearray()

        output = int(Minute * largeGaugeRotation)
        packet += binascii.hexlify(output.to_bytes(1, 'little'))

        output = int(Hour * largeGaugeRotation)
        packet += binascii.hexlify(output.to_bytes(1, 'little'))

        packet += binascii.hexlify(Space.to_bytes(1, 'little'))

        i = 0
        while (i < 5):
            packet += binascii.hexlify(Day[4 - i].to_bytes(1, 'little'))
            i += 1

        packet += binascii.hexlify(Space.to_bytes(1, 'little'))
        packet += binascii.hexlify(Space.to_bytes(1, 'little'))
        packet += binascii.hexlify(Space.to_bytes(1, 'little'))

        packet += b'Z'

        binascii.hexlify(packet)
        serMain.write(packet)

        packet = bytearray()
        if pendulum:
            if (Time.second % 2 == 0):
                output = int(0.25 * smallGaugeRotation)
            else:
                output = int(0.75 * smallGaugeRotation)
        else:
            output = int(0.5 * smallGaugeRotation)
        packet += binascii.hexlify(output.to_bytes(1, 'little'))

        output = int(Second * smallGaugeRotation)
        if (output < 0):
            output = 0
        elif (output > smallGaugeRotation):
            output = smallGaugeRotation
        packet += binascii.hexlify(output.to_bytes(1, 'little'))

        packet += binascii.hexlify(Space.to_bytes(1, 'little'))

        i = 0
        while (i < 4):
            packet += binascii.hexlify(Month[i + 1].to_bytes(1, 'little'))
            i += 1

        i = 0
        while (i < 3):
            packet += binascii.hexlify(Weekday[i + 2].to_bytes(1, 'little'))
            i += 1

        packet += b'Z'

        # print(packet)
        binascii.hexlify(packet)
        serTempFuel.write(packet)

    if msvcrt.kbhit():
        output = 0
        packet = bytearray()
        i = 0
        while i < 11:
            packet += binascii.hexlify(output.to_bytes(1, 'little'))
            i += 1
        packet += b'Z'
        binascii.hexlify(packet)
        serMain.write(packet)

        packet = bytearray()
        while i < 10:
            packet += binascii.hexlify(output.to_bytes(1, 'little'))
            i += 1
        packet += b'Z'
        binascii.hexlify(packet)
        serTempFuel.write(packet)

        exit = True
    time.sleep(0.01)

print("Serial connection closed")
