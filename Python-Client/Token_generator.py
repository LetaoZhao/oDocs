from opentok import Client, MediaModes

# Set your OpenTok API credentials
api_key = '47679271'
api_secret = '70ff52066683b509b47555760b60f3d963752106'

# Initialize the OpenTok client
opentok = Client(api_key, api_secret)

# Create a session
session = opentok.create_session(media_mode=MediaModes.routed)
session_id = session.session_id

# Generate tokens for participants
token1 = opentok.generate_token(session_id)
token2 = opentok.generate_token(session_id)

# Use session ID and tokens in your application
print('Session ID:', session_id)
print('Token 1:', token1)
print('Token 2:', token2)