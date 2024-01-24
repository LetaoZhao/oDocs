# Python-Client
## Dependencies
 - configparser
 - opentok
 - opencv2
 - pyserial
While no requirements.txt file is provided here, the required packages can be installed via the following command:
``` BASH
pip install configparser opentok opencv-python pyserial
```
**Note** you may need to use `pip3` and `python3`

## Scripts
 - **g_code_sender.py** uses pyserial to send G-Code to the printer.  Just used for initial testing, but has been provided anyway.
 - **opencv_test.py** is a simple opencv script for testing video devices.  Was used to validate the web-cam and the phone camera.
 - **Token_generator.py** is a python script that generates tokens and session-id's.  Just run `python ./Token_generator.py` and it will print valid tokens and session ID's for testing.  Also saves these to a token_storage.ini file, however this is currently unused.