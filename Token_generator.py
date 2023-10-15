# Import necessary modules
from flask import Flask  # Import Flask for creating the web application
from opentok import Client, MediaModes, Roles  # Import Opentok classes for session and token management
from flask_cors import CORS  # Import CORS to handle Cross-Origin Resource Sharing

# Initialize Flask app
app = Flask(__name__)

# Enable CORS (Cross-Origin Resource Sharing)
CORS(app)

# Initialize variables to store token request count, session ID, and tokens
token_request_count = 0
session_id = ''
token1 = ''
token2 = ''

# Define route to generate tokens
@app.route('/')
def Token_generate():
    global token_request_count  # Use the global token_request_count variable
    global session_id
    global token1
    global token2

    # Check if this is the first token request
    if token_request_count == 0:
        # Create an Opentok client and session for the first request
        opentok = Client('47679271', '70ff52066683b509b47555760b60f3d963752106')
        session = opentok.create_session(media_mode=MediaModes.routed)
        session_id = session.session_id
        # Generate tokens with specific roles and data
        token1 = session.generate_token(role=Roles.publisher, data=u'name=YJ', initial_layout_class_list=[u'focus'])
        token2 = session.generate_token(role=Roles.publisher, data=u'name=Toby', initial_layout_class_list=[u'focus'])

    token_request_count += 1

    if (token_request_count % 2) == 0:
        # For the second person, return [session_id, token2]
        token = token2
        return [session_id, token2]
    else:
        # For the first person, return [session_id, token1]
        token = token1
        return [session_id, token1]

# Run the app if this is the main module
if __name__ == '__main__':
    app.run(debug=True, port=5001)  # Run the Flask app in debug mode on port 5001

