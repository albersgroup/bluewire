![project bluewire](img/bluewire_logo.png)

# project bluewire

## OVERVIEW

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

## HARDWARE BOM
* [ESP-32 Microcontroller](https://www.amazon.com/gp/product/B0D8T53CQ5/ref=ppx_od_dt_b_asin_title_s02?ie=UTF8&psc=1)
* [Rotary Encoder Module](https://www.dfrobot.com/product-2575.html?srsltid=AfmBOooL40vc9hlK62E8fBJGLit5WPTY3ZMIfle7tOmoT5yqemdEWzI3)
* [Battery Shield](https://www.amazon.com/dp/B0CJR1Y967?ref_=ppx_hzsearch_conn_dt_b_fed_asin_title_5&th=1)
* [Battery](https://www.amazon.com/dp/B0CDRBR2M1?ref_=ppx_hzsearch_conn_dt_b_fed_asin_title_6)

TODO: add screw and hex nut sizes and link to purchase

Note: the hardware was connected solderlessly using 30 AWG wire ('blue wire') and a wire-wrapping tool.

## KIT PURCHASE

## ASSEMBLY
![project bluewire](img/bluewire_assembled.jpg)

* Reference STL files in [Case Design directory](/hardware_case/)
* Purchase parts referenced in HARDWARE BOM section above
* TODO assembly
* TODO wiring

TODO: Fritzing img

## SETUP

## TUTORIAL

## DEMO

## CONTACT