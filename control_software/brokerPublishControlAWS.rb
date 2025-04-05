require "mqtt"
require "yaml"

CONFIG_PATH = File.join(__dir__, "config_aws.yml")
abort("Missing config.yml.") unless File.exist?(CONFIG_PATH)
config = YAML.load_file(CONFIG_PATH)

endpoint = config.dig("mqtt", "host")          # e.g. a1b2c3d4e5f6-ats.iot.us-east-1.amazonaws.com
port     = config.dig("mqtt", "port") || 8883
cert     = config.dig("mqtt", "cert")          # certificate.pem.crt
key      = config.dig("mqtt", "key")           # private.pem.key
ca       = config.dig("mqtt", "ca")            # AmazonRootCA1.pem
topic    = "albers/bluewire/hil/control"
message  = "REMOTE"

puts "[bluewire MQTT publish to AWS IoT]"
puts "  Endpoint: #{endpoint}:#{port}"
puts "  Topic: #{topic}"
puts "  Certs: #{cert}, #{key}, #{ca}"

MQTT::Client.connect(
  host: endpoint,
  port: port,
  ssl: true,
  cert_file: cert,
  key_file: key,
  ca_file: ca
) do |c|
  retain = true
  qos = 0
  c.publish(topic, message, retain, qos)
  puts "Message published to #{topic}: #{message}"
  c.disconnect
  puts "Disconnected from MQTT broker."
end