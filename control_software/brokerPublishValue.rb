require "mqtt"

c = MQTT::Client.new
c.host='test.mosquitto.org'
c.port=1883
#c.client_id='testClient123'

c.connect(MQTT::Client.generate_client_id('hil-publish-test-company-name'))

c.publish('company/name/hil/value', '23', retain=true)

c.disconnect()