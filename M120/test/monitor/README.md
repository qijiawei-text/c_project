# Monitor Template
This directory showcases how the user can write a simple python script to automate monitoring devices' behavior, as well as how the user can containerize the application with docker. 

## Requirement
`ncclient`

## Description
### `ncConn.py`
This file contains a super class that connects to the device and create subscription. It filters the subscription to only receive alarm. 

### `monitor.py`
This file contains the main script that runs the automation. It queries power info every 5 seconds to see whether the target gain is aligned with the user-configured target output. If the difference is greater than a specified threshold, it will send a syslog to the syslog server of the device. 

### `settings.py`
This file contains the info which the user would want to change per device.

### `xmlUtil.py`
This file contains the XML filter and xpath, utilized by the main script.

## Docker usage
Please refer to `DockerGuide` for details on Docker usage.
