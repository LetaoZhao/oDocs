from flask import Flask
from opentok import Client, MediaModes, Roles
from flask_cors import CORS

app = Flask(__name__)
CORS(app)
token_request_count = 0

@app.route('/')
def Token_generate():
    global token_request_count  # Use the global token_request_count variable
    token_request_count += 1

    opentok = Client('47679271', '70ff52066683b509b47555760b60f3d963752106')
    session = opentok.create_session(media_mode=MediaModes.routed)
    session_id = session.session_id
    token1 = session.generate_token(role=Roles.publisher, data=u'name=YJ', initial_layout_class_list=[u'focus'])
    token2 = session.generate_token(role=Roles.publisher, data=u'name=Toby', initial_layout_class_list=[u'focus'])

    if token_request_count == 1:
        # First person, return [session_id, token1]
        return [session_id, token1]
    elif token_request_count == 2:
        # Second person, return [session_id, token2]
        return [session_id, token2]
    else:
        # Third person or more, return an empty response
        return 'none'

if __name__ == '__main__':
    app.run(debug=True, port=5001) 