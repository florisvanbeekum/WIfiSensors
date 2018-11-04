#------------------------------------------
#--- Author: Pradeep Singh
#--- Date: 20th January 2017
#--- Version: 1.0
#--- Python Ver: 2.7
#--- Details At: https://iotbytes.wordpress.com/store-mqtt-data-from-sensors-into-sql-database/
#------------------------------------------

import paho.mqtt.client as mqtt
import json
import sqlite3

# MQTT Settings 
MQTT_Broker = "192.168.2.6"
MQTT_Port = 1883
Keep_Alive_Interval = 45
MQTT_Topic = "Home/Temperature/#"

#===============================================================
# Database Manager Class

class DatabaseManager():
        def __init__(self):
                self.conn = sqlite3.connect(DB_Name)
                self.conn.execute('pragma foreign_keys = on')
                self.conn.commit()
                self.cur = self.conn.cursor()

        def add_del_update_db_record(self, sql_query, args=()):
                self.cur.execute(sql_query, args)
                self.conn.commit()
                return

        def __del__(self):
                self.cur.close()
                self.conn.close()

#===============================================================

#Subscribe to all Sensors at Base Topic
def on_connect(mosq, obj, flags, rc):
	mqttc.subscribe(MQTT_Topic, 0)

#Save Data into DB Table
def on_message(mosq, obj, msg):
	print "MQTT Data Received..."
	print "MQTT Topic: " + msg.topic  
	print "Data: " + msg.payload
        
        json_Dict = json.loads(msg.payload)
        SensorID = json_Dict['Sensor_ID']
        Temperature = json_Dict['Temperature']
        
        print "SensorID: " + SensorID
        #print "Data_and_Time: " + Data_and_Time 
        print "Temperature: " + Temperature
         
 #       dbObj = DatabaseManager()
 #       dbObj.add_del_update_db_record("insert into Temperature_Data (SensorID, Date_n_Time, Temperature) values (?,?,?)",[SensorID, Data_and_Time, Temperature])
        del dbObj
 #       print "Inserted Temperature Data into Database."
        print ""
 
def on_subscribe(mosq, obj, mid, granted_qos):
    pass

mqttc = mqtt.Client()

# Assign event callbacks
mqttc.on_message = on_message
mqttc.on_connect = on_connect
mqttc.on_subscribe = on_subscribe

# Connect
mqttc.connect(MQTT_Broker, int(MQTT_Port), int(Keep_Alive_Interval))

# Continue the network loop
mqttc.loop_forever()
