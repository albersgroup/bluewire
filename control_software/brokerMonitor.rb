require "mqtt"
require "yaml"

# Load config from file
CONFIG_PATH = File.join(__dir__, "config.yml")
unless File.exist?(CONFIG_PATH)
  abort "Missing config.yml. Please create one with your MQTT settings."
end

config = YAML.load_file(CONFIG_PATH)

mqtt_host = config.dig("mqtt", "host") || "localhost"
mqtt_port = config.dig("mqtt", "port") || 1883
company    = config.dig("topic", "company") || "company"
project    = config.dig("topic", "project") || "project"

value_topic = "#{company}/#{project}/hil/value"
control_topic = "#{company}/#{project}/hil/control"

puts "[bluewire MQTT monitor]"
puts "  MQTT Server: #{mqtt_host}:#{mqtt_port}"
puts "  Topics:"
puts "    - #{value_topic}"
puts "    - #{control_topic}"
puts "  Edit 'config.yml' in this directory to change these values.\n\n"

client = MQTT::Client.connect(mqtt_host, mqtt_port)

client.subscribe(value_topic, control_topic)

client.get do |topic, message|
  puts "[#{topic}] #{message}"
end