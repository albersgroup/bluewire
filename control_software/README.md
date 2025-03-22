![project bluewire](../img/bluewire_logo_alt2.jpg)

# SOFTWARE CONTROL

To execute the ruby scripts, you need a version of ruby installed.  On mac or linux, use a package manager to install ruby.  On windows, it's recommended to setup wsl and install ruby on a Ubuntu VM.

From the scripts in this [Software Control directory](/control_software/), edit and run the two pubsish scripts and then the monitor script.

Replace 'company/name' in the scripts with the name of your company and project (e.g., 'acme/abc' for project abc at company acme).

Execute the scripts directly (after installing the 'mqtt' gem)
```console
$ gem install mqtt
$ ruby brokerPublishControl.rb
$ ruby brokerPublishValue.rb
$ ruby brokerMonitor.rb
```