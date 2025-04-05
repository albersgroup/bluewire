![project bluewire](../img/bluewire_logo_alt2.jpg)

# SOFTWARE CONTROL

To execute the ruby scripts, you need a version of ruby installed.  On mac or linux, use a package manager to install ruby.  On windows, it's recommended to setup wsl and install ruby on a Ubuntu VM.

From the scripts in this [Software Control directory](/control_software/), edit and run the two pubsish scripts and then the monitor script.

Modify the [config file](/control_software/config.yml) with the name of your company and project.

Execute the scripts directly (after installing the 'mqtt' gem)
```console
$ gem install mqtt
$ ruby brokerPublishControl.rb
$ ruby brokerPublishValue.rb
$ ruby brokerMonitor.rb
```

## OPTIONAL: AWS IoT Core Setup

The test.mosquitto.org broker is great for initial testing - but it is *very* flaky.  Setting up a broker you can control and troubleshoot is highly recommended.

---
**NOTES:**

NOTE 1: ESP-32 connectivity to AWS IoT still in-work

NOTE 2: Retained messages require an additional policy enabled [link1](https://aws.amazon.com/about-aws/whats-new/2021/08/aws-iot-core-supports-mqtt-retained-messages/) [link2](https://docs.aws.amazon.com/iot/latest/developerguide/retained-message-policy-examples.html)

NOTE 3: your endpoint is not the same as your ARN.

For reference: https://docs.aws.amazon.com/iot/latest/developerguide/mqtt.html#mqtt-retain

---

### Instructions

1.1. Create a “Thing”
	•	Go to the AWS IoT Core Console
	•	Navigate to Manage → Things
	•	Click Create thing
	•	Choose “Create a single thing”
	•	Name it bluewire_kit or something similar

1.2. Generate Keys & Certificates
	•	On the “Create thing” page, choose:
	•	Create certificate
	•	Activate certificate
	•	Attach a policy (we’ll create one)

1.3. Create and Attach a Policy
	•	Go to Secure → Policies
	•	Create a policy like this:

```json
{
  "Version": "2012-10-17",
  "Statement": [
    {
      "Effect": "Allow",
      "Action": [
        "iot:Connect",
        "iot:Publish",
        "iot:Receive",
        "iot:Subscribe"
      ],
      "Resource": "*"
    }
  ]
}
```

For production: limit this policy to specific topic ARNs.

Attach this policy to your certificate.

1.4. Download the credentials

You’ll receive:
	•	private.pem.key
	•	certificate.pem.crt
	•	AmazonRootCA1.pem

Save them in the directory with your ruby scripts and make the configuration file updates (with appropriate filenames)

```yaml
mqtt:
  host: YOUR_ENDPOINT.iot.us-east-1.amazonaws.com
  port: 8883
  cert: certificate.pem.crt
  key: private.pem.key
  ca: AmazonRootCA1.pem
```

