from opentok import Client, MediaModes, Roles
from flask import Flask, jsonify
import configparser

# Set your OpenTok API credentials
api_key = '47679271'
api_secret = '70ff52066683b509b47555760b60f3d963752106'

# Initialize the OpenTok client
opentok = Client(api_key, api_secret)

# Create a session
session = opentok.create_session(media_mode=MediaModes.routed)
session_id = session.session_id

# Generate tokens for participants
token1 = session.generate_token(role=Roles.publisher, data=u'name=YJ', initial_layout_class_list=[u'focus'])
token2 = session.generate_token(role=Roles.publisher, data=u'name=Toby', initial_layout_class_list=[u'focus'])

# Use session ID and tokens in your application
print('Session ID:', session_id)
print('Token 1:', token1)
print('Token 2:', token2)

# creating object of configparser
config = configparser.ConfigParser()

# creating a section
config.add_section("OpenTok Token")

# adding key-value pairs
config.set("OpenTok Token", "Token1", token1)
config.set("OpenTok Token", "Token2", token2)

#Write the above sections to config.ini file
with open('token_storage.ini', 'w') as conf:
    config.write(conf)