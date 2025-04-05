require "mqtt"
require "yaml"

# Load config from config.yml
CONFIG_PATH = File.join(__dir__, "config.yml")

unless File.exist?(CONFIG_PATH)
  abort "Missing config.yml. Please create one in this directory with your MQTT settings."
end

config = YAML.load_file(CONFIG_PATH)

mqtt_host = config.dig("mqtt", "host") || "localhost"
mqtt_port = config.dig("mqtt", "port") || 1883
company   = config.dig("topic", "company") || "company"
project   = config.dig("topic", "project") || "project"

control_topic = "#{company}/#{project}/hil/value"
message = "23"

puts "[bluewire MQTT publish]"
puts "  MQTT Server: #{mqtt_host}:#{mqtt_port}"
puts "  Publishing to topic: #{control_topic}"
puts "  Message: #{message}"
puts "  Edit 'config.yml' in this directory to change company/project/host.\n\n"

MQTT::Client.connect(host: mqtt_host, port: mqtt_port) do |c|
  c.publish(control_topic, message, retain = true)
end