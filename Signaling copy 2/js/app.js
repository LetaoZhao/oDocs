/* global API_KEY TOKEN SESSION_ID SAMPLE_SERVER_BASE_URL OT */
/* eslint-disable no-alert */

let apiKey;
let session;
let sessionId;
let token;

function handleError(error) {
  if (error) {
    console.error(error);
  }
}

function initializeSession() {
  session = OT.initSession(apiKey, sessionId);

  // Subscribe to a newly created stream
  session.on('streamCreated', (event) => {
    const subscriberOptions = {
      insertMode: 'append',
      width: '100%',
      height: '100%'
    };
    session.subscribe(event.stream, 'subscriber', subscriberOptions, handleError);
  });

  session.on('sessionDisconnected', (event) => {
    console.error('You were disconnected from the session.', event.reason);
  });

  // Initialize the publisher
  const publisherOptions = {
    insertMode: 'append',
    width: '100%',
    height: '100%'
  };
  const publisher = OT.initPublisher('publisher', publisherOptions, handleError);

  // Connect to the session
  session.connect(token, (error) => {
    if (error) {
      handleError(error);
    } else {
      // If the connection is successful, publish the publisher to the session
      session.publish(publisher, handleError);
    }
  });

  // Receive a message and append it to the history
  const msgHistory = document.querySelector('#history');
  session.on('signal:msg', (event) => {
    const msg = document.createElement('p');
    msg.textContent = event.data;
    msg.className = event.from.connectionId === session.connection.connectionId ? 'mine' : 'theirs';
    msgHistory.appendChild(msg);
    msg.scrollIntoView();
  });
}

// Text chat
const form = document.querySelector('form');
const msgTxt1 = document.querySelector('#msgTxt1');
const msgTxt2 = document.querySelector('#msgTxt2');
const msgTxt3 = document.querySelector('#msgTxt3');


form.addEventListener('submit', (event) => {
  event.preventDefault();

  // Get the input values
  const inputValues1 = msgTxt1.value.trim();
  const inputValues2 = msgTxt2.value.trim();
  const inputValues3 = msgTxt3.value.trim();

  // Validate the input
  if (inputValues1 === '' || inputValues2 === '' || inputValues3 === '' || isNaN(inputValues1) || isNaN(inputValues2) || isNaN(inputValues3)) {
    alert('Please enter valid values for x, y or z.');
    return;
  }

  // Parse the input values into numbers
  const x = parseFloat(inputValues1);
  const y = parseFloat(inputValues2);
  const z = parseFloat(inputValues3);

  // Combine the values into one message
  const newMessage = `${x},${y},${z}`;

  // Send a signal with the combined message
  session.signal({
    type: 'msg',
    data: newMessage
  }, (error) => {
    if (error) {
      alert(error);
      handleError(error);
    } else {
      document.querySelector('#msgTxt1').value = '';
      document.querySelector('#msgTxt2').value = '';
      document.querySelector('#msgTxt3').value = '';
    }
  });
});


// See the config.js file.
if (API_KEY && TOKEN && SESSION_ID) {
  apiKey = API_KEY;
  sessionId = SESSION_ID;
  token = TOKEN;
  initializeSession();
} else if (SAMPLE_SERVER_BASE_URL) {
  // Make a GET request to get the OpenTok API key, session ID, and token from the server
  fetch(SAMPLE_SERVER_BASE_URL + '/session')
  .then((response) => response.json())
  .then((json) => {
    apiKey = json.apiKey;
    sessionId = json.sessionId;
    token = json.token;
    // Initialize an OpenTok Session object
    initializeSession();
  }).catch((error) => {
    handleError(error);
    alert('Failed to get opentok sessionId and token. Make sure you have updated the config.js file.');
  });
}