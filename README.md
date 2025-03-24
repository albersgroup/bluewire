![project bluewire](img/bluewire_logo.png)

# project bluewire

## PROJECT OVERVIEW

This is the project repository for a simplified demonstration of:
* Message broker
* Hardware control
* Design control
* Software control

The goal of project bluewire is to use the minimal amount of hardware, software, and modeling complexity to create a hardware-connected demonstration that a user can hold in their hands and control - watching the response from the design perspective.

## DETAILS

This repository contains a proof-of-concept, building off of what Saulius Pavalkis provided at a symposium in 2024.  Reference this [LinkedIn Post by Saulius](https://www.linkedin.com/posts/sauliuspavalkis_iot-and-hil-with-catia-magic-and-sysml-simulation-activity-7203442605337026560-9NrX/).

This implementation uses MQTT for the message broker, COTS hardware with an ESP-32 microcontroller for hardware, Cameo Systems Modeler 2024x for design software, and simple executable ruby files for software control.

The MQTT message broker has 2 active topics - one for showing whether CAMEO or REMOTE has control of the broker (the control topic) and the other for storing the value reading.

The MQTT publishers set the 'retain' flag on the messages to ensure that when clients connect, they receive the latest message published to the topic.  The ruby scripts are utilized to initialize the values on the topics.

## INTEGRATION ENGINEERING

This project started with a simple definition so we could jump right in and start building and failing.  With each failure, we learn and move forward in a better way.  Learning on projects like this is what keeps us sharp and ready to solve more complex problems.  The pace of 'ah-ha' moments when we are able to make progress and not stay stuck for long is what keeps us motivated.  Sometimes complex problems are grueling - that's where we maintain side-projects like this to break up the grind.

Consider this repository an invitation to jump in and particpate with us.  We'd like to know what you're working on and how we can adapt this solution (or come up with a new one) to apply to new problems.

Use your team - multiple disciplines of engineering particpated in this project.  Electrical, Software, Mechanical, plus a touch of all-around Linux/dev-ops are all needed at times.  If you find a single engineer that can take this repository and stand up the 4 aspects listed in the project overview, you've got some of the most unique 5% of talent in the top 20% of engineers out there.  What's even better is when a person like that can work with a team to accelerate and develop others.  We hope this project helps activate that kind of teamwork and sharpens all engineers involved.

## EXTENDING TO A SOLUTION

This is a proof of concept.  It should spark ideas.  What if you could have a wireless control panel that interacted with your test domain with design, software, and hardware all connected?  What would you want?  Three knobs, five buttons, and a display?  With basic MBSE toolset familiarity, you are a few hours of work away from having that in a design model representing that using what's provided here, connected to any number of topics and messages for monitoring and control.  Integration with software and hardware can move in parallel with design.

## HARDWARE BOM
* [ESP-32 Microcontroller](https://www.amazon.com/gp/product/B0D8T53CQ5/ref=ppx_od_dt_b_asin_title_s02?ie=UTF8&psc=1)
* [Rotary Encoder Module](https://www.dfrobot.com/product-2575.html?srsltid=AfmBOooL40vc9hlK62E8fBJGLit5WPTY3ZMIfle7tOmoT5yqemdEWzI3)
* [Battery Shield](https://www.amazon.com/dp/B0CJR1Y967?ref_=ppx_hzsearch_conn_dt_b_fed_asin_title_5&th=1)
* [Battery](https://www.amazon.com/dp/B0CDRBR2M1?ref_=ppx_hzsearch_conn_dt_b_fed_asin_title_6)

---
**NOTE:**

1) The electronic hardware was connected solderlessly using 30 AWG wire ('blue wire') and a wire-wrapping tool.

2) M3x16mm screws [from a kit similar to this one](https://www.amazon.com/mxuteuk-Metric-Screws-Suitable-Printer/dp/B0C7ZPZ214/ref=sr_1_4?crid=2SHDEFC0N69ZV&dib=eyJ2IjoiMSJ9.rZUOhTY8cDBy-wbn_BU8KBTcfiNrMw6xyA0A3JQUoiRfNJQSJAh1eZ8m5vh65m5J.h0pbxsaEiqoLijBjxzKqgwTJ42O1dUlcqAEtnxugZRA&dib_tag=se&keywords=mxuteuk%2B620PCS&qid=1742610488&s=industrial&sprefix=mxuteuk%2B620pcs%2Cindustrial%2C147&sr=1-4&th=1) were used for assembly.

3) Heat inset threaded inserts of the size M3x4mmx4.2mm were used on the case using a kit [such as this one](https://www.amazon.com/dp/B0CNWJR455?ref_=ppx_hzsearch_conn_dt_b_fed_asin_title_1&th=1).

---

## KIT PURCHASE

3 kit options are available for purchase if you reach out to us using the contact information at the bottom of this page.  Kit purchase not required - all information for purchasing the components in the kits is available in this repository.  If you prefer the convinience of it all in a box for you, or pre-assembled, please reach out and we'll get you on the list.

* Option 1: unassembled kit only
* Option 2: pre-assembled kit
* Option 3: unassembled + pre-assembled kit (2 kits total) + 2 engineering consulting sessions with our team

The unassembled kit included in options 1 and 3 include a wire wrap tool along with enough 30 AWG wire to assemble the kit.  Threaded inserts inset in the case material for kits for ease of assembly.  The kits come with the necessary connecting hardware.

## ASSEMBLY
![project bluewire](img/bluewire_assembled.jpg)
*rev1 of the project bluewire case*

* Reference STL files in [Case Design directory](/hardware_case/)
* Purchase parts referenced in HARDWARE BOM section above
* TODO assembly
* TODO wiring

TODO: Fritzing img

## TUTORIAL

The guides in the sections linked below will walk through options and steps for setting up a minimal example completing the objective laid out in the project overview.

### Message Broker

For initial testing, everything is configured to use the MQTT test broker, as described in the first section.  For more serious applications past testing, it is recommended to stand up a production MQTT broker.  One quick option is described in the guide below.

#### MQTT Test Broker

By default, the software/firmware code and the Cameo model are configured to use the test.mosquitto.org server.  Be aware, it is flaky and not for use in production environments.

Please see: https://test.mosquitto.org/ for more information.

#### AWS MQTT Broker

Configure and deploy an MQTT broker using a service such as AWS ECS or similar.  An example tutorial is linked below:

https://www.atom8.ai/blog/how-to-deploy-mqtt-broker-using-eclipse-mosquitto-on-amazon-ecs

### Software Control

Please view the README in the [Software Control directory](/control_software/).

### Design Control

Please view the README in the [Design Control directory](/control_design/).

### Hardware Control

Please view the readme in the [Hardware Control directory](/control_hardware/).

## DEMO

If you would like a demonstration of the interaction between design, hardware, and software, see the contact section below and we'll find a time that to connect.

## CONTACT

Please reach out if we can be of assistance in helping you accelerate digital engineering adoption.

defense [at] albers.aero