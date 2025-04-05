require "mqtt"
require "yaml"

# Load config from file
CONFIG_PATH = File.join(__dir__, "config_aws.yml")
unless File.exist?(CONFIG_PATH)
  abort "Missing config.yml. Please create one with your MQTT settings."
end

config = YAML.load_file(CONFIG_PATH)

# MQTT config
mqtt_host = config.dig("mqtt", "host")
mqtt_port = config.dig("mqtt", "port") || 8883
cert_file = config.dig("mqtt", "cert")
key_file  = config.dig("mqtt", "key")
ca_file   = config.dig("mqtt", "ca")

# Topic config
company  = config.dig("topic", "company") || "company"
project  = config.dig("topic", "project") || "project"
value_topic   = "#{company}/#{project}/hil/value"
control_topic = "#{company}/#{project}/hil/control"

puts "[bluewire MQTT monitor - AWS IoT Core]"
puts "  Endpoint : #{mqtt_host}:#{mqtt_port}"
puts "  Cert     : #{cert_file}"
puts "  Key      : #{key_file}"
puts "  CA       : #{ca_file}"
puts "  Topics:"
puts "    - #{value_topic}"
puts "    - #{control_topic}"
puts "  Edit 'config.yml' in this directory to change these values.\n\n"

client = MQTT::Client.connect(
  host: mqtt_host,
  port: mqtt_port,
  ssl: true,
  cert_file: cert_file,
  key_file: key_file,
  ca_file: ca_file
)

client.subscribe(value_topic, control_topic)

client.get do |topic, message|
  puts "[#{topic}] #{message}"
end