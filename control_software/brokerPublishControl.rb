require "mqtt"

c = MQTT::Client.connect('test.mosquitto.org', 1883)

c.publish('company/name/hil/control', 'REMOTE', retain=true)

c.disconnect()