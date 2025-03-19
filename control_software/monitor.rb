require "mqtt"

client = MQTT::Client.connect('test.mosquitto.org', 1883)

client.subscribe( 'company/name/hil/value' )
client.subscribe( 'company/name/hil/control' )

client.get do |topic,message|
  puts message
end