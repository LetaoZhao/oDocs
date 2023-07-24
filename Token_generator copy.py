from flask import Flask
from opentok import Client, MediaModes, Roles
from flask_cors import CORS

app = Flask(__name__)
CORS(app)
token_request_count = 0
session_id = ''
token1 = ''
token2 = ''

@app.route('/')
def Token_generate():
    global token_request_count  # Use the global token_request_count variable
    global session_id
    global token1
    global token2
    if token_request_count == 0:
        opentok = Client('47679271', '70ff52066683b509b47555760b60f3d963752106')
        session = opentok.create_session(media_mode=MediaModes.routed)
        session_id = session.session_id
        token1 = session.generate_token(role=Roles.publisher, data=u'name=YJ', initial_layout_class_list=[u'focus'])
        token2 = session.generate_token(role=Roles.publisher, data=u'name=Toby', initial_layout_class_list=[u'focus'])

    token_request_count += 1

    if (token_request_count % 2) == 0:
        # second person, return [session_id, token1]
        token = token2
        return [session_id, token2]
    else:
        # first person, return [session_id, token2]
        token = token1
        return [session_id, token1]

if __name__ == '__main__':
    app.run(debug=True, port=5001)
