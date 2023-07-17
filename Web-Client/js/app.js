/* global API_KEY TOKEN SESSION_ID SAMPLE_SERVER_BASE_URL OT */
/* eslint-disable no-alert */


// handle error----------------------------------------------------------------------------------------
function handleError(error) {
  if (error) {
    console.error(error);
  }
}
// handle error----------------------------------------------------------------------------------------


// initialise session----------------------------------------------------------------------------------
let apiKey;
let session;
let sessionId;
let token;

function initializeSession() {
  const sessionOptions = {
    apiKey: apiKey,
    sessionId: sessionId,
    token: token,
    screenSharing: true // Enable screen sharing in the session options
  };
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
// initialise session----------------------------------------------------------------------------------


// Text chat-------------------------------------------------------------------------------------------
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
// Text chat-------------------------------------------------------------------------------------------


// move tool box---------------------------------------------------------------------------------------
const toolBox = $('#toolBox');
toolBox.draggable();
// move video window-----------------------------------------------------------------------------------


// able to toggle video window-------------------------------------------------------------------------
const toggleButton = document.querySelector('#toggleButton');

let isHidden = false;

toggleButton.addEventListener('click', () => {
  if (isHidden) {
    publisher.style.display = 'block';
    isHidden = false;
  } else {
    publisher.style.display = 'none';
    isHidden = true;
  }
});
// able to toggle video window-------------------------------------------------------------------------


// able to toggle video window-------------------------------------------------------------------------
const toggleChat = document.querySelector('#toggleChat');

let isChatHidden = false;

toggleChat.addEventListener('click', () => {
  if (isChatHidden) {
    textchat.style.display = 'block';
    isChatHidden = false;
  } else {
    textchat.style.display = 'none';
    isChatHidden = true;
  }
});
// able to toggle video window-------------------------------------------------------------------------


// able to toggle camera view--------------------------------------------------------------------------
const toggleCameraButton = document.querySelector('#toggleCamera');
let isSwapped = false;
const originalPublisherStyle = {
  position: 'absolute',
  width: '360px',
  height: '240px',
  bottom: '10px',
  left: '10px',
  marginLeft: '0px',
  zIndex: '100',
  border: '3px solid white',
  borderRadius: '3px'
};
const originalSubscriberStyle = {
  position: 'absolute',
  width: '100%',
  height: '100%',
  bottom: '0',
  left: '0',
  marginLeft: '50px',
  zIndex: '10',
  border: 'none',
  borderRadius: '0px'
};

toggleCameraButton.addEventListener('click', () => {
  // Get the publisher and subscriber elements
  const publisherElement = document.getElementById('publisher');
  const subscriberElement = document.getElementById('subscriber');

  // Swap the display formats of publisher and subscriber elements
  if (!isSwapped) {
    // Change the video format
    Object.assign(publisherElement.style, originalSubscriberStyle);
    Object.assign(subscriberElement.style, originalPublisherStyle);

    isSwapped = true;
  } else {
    // Change back the video format
    Object.assign(publisherElement.style, originalPublisherStyle);
    Object.assign(subscriberElement.style, originalSubscriberStyle);

    isSwapped = false;
  }
});
// able to toggle camera view--------------------------------------------------------------------------


// able to screenshot subscriber video-----------------------------------------------------------------
function takeScreenshot() {
  // Get the subscriber video element
  const subscriberVideo = document.querySelector('#subscriber video');

  // Create a canvas element with the same dimensions as the video
  const canvas = document.createElement('canvas');
  canvas.width = subscriberVideo.videoWidth;
  canvas.height = subscriberVideo.videoHeight;

  // Draw the video frame onto the canvas
  const context = canvas.getContext('2d');
  context.drawImage(subscriberVideo, 0, 0, canvas.width, canvas.height);

  // Convert the canvas image data to a data URL (PNG format)
  const dataURL = canvas.toDataURL('image/png');

  // Get the current date and time
  const now = new Date();
  const timestamp = now.toISOString().replace(/[-:]/g, '').replace(/\.\d{3}/, '');

  // Create a link element to download the image
  const link = document.createElement('a');
  link.href = dataURL;
  link.download = `subscriber_screenshot_${timestamp}.png`; // Set the filename with timestamp

  // Append the link to the document and click it to trigger the download
  document.body.appendChild(link);
  link.click();

  // Clean up by removing the link element
  document.body.removeChild(link);
}
// able to screenshot subscriber video-----------------------------------------------------------------


// start video share-----------------------------------------------------------------------------------
// Get the start screen share button element
const startScreenShareButton = document.getElementById('startScreenShare');

// Add click event listener to the screen sharing button
startScreenShareButton.addEventListener('click', () => {
  // Replace 'your-screen-share-stream-id' with a unique stream name for the screen sharing stream
  const screenShareStreamId = 'your-screen-share-stream-id';

  // Get the publisher element
  const publisherElement = document.getElementById('publisher');

  // Create a screen sharing publisher
  const screenSharingPublisher = OT.initPublisher(publisherElement, {
    insertMode: 'append',
    width: '100%',
    height: '100%',
    publishAudio: false, // Set to false if you don't want to publish audio along with screen share
    videoSource: 'screen' // Use 'screen' to enable screen sharing
  }, handleError);

  // Start publishing the screen sharing stream
  session.publish(screenSharingPublisher, { streamId: screenShareStreamId }, handleError);
});
// start video share-----------------------------------------------------------------------------------


// end video share-----------------------------------------------------------------------------------
// Get the stop screen share button element
const stopScreenShareButton = document.getElementById('stopScreenShare');

// Add click event listener to the stop screen share button
stopScreenShareButton.addEventListener('click', () => {
  // Get the screen sharing publisher
  const screenSharingPublisher = session.getPublisherForStream('your-screen-share-stream-id');

  // Stop publishing the screen sharing stream
  if (screenSharingPublisher) {
    session.unpublish(screenSharingPublisher);
  }
});
// end video share-----------------------------------------------------------------------------------


// able to make subcriber full screen------------------------------------------------------------------
const fullScreenButton = document.querySelector('#fullScreen');
let isFullScreen = false;
const newSubscriberStyle = {
  position: 'fixed',
  width: '100%',
  height: '100%',
  bottom: '0',
  left: '0',
  marginLeft: '0px',
  zIndex: '10',
  border: 'none',
  borderRadius: '0px'
};

fullScreenButton.addEventListener('click', () => {
  // Get the publisher and subscriber elements
  const publisherElement = document.getElementById('publisher');
  const subscriberElement = document.getElementById('subscriber');

  // Swap the display formats of publisher and subscriber elements
  if (!isFullScreen) {
    if(isSwapped){
      // make publisher fullscreen and subcriber disappear
      subscriber.style.display = 'none';
      Object.assign(publisherElement.style, newSubscriberStyle);
    } else{
      // make subcriber fullscreen and publisher disappear
      publisher.style.display = 'none';
      Object.assign(subscriberElement.style, newSubscriberStyle);
    }
    isFullScreen = true;
  } else {
    if(isSwapped){
      // Change back
      subscriber.style.display = 'block';
      Object.assign(publisherElement.style, originalSubscriberStyle);
    } else{
      // Change back
      publisher.style.display = 'block';
      Object.assign(subscriberElement.style, originalSubscriberStyle);
    }

    isFullScreen = false;
  }
});
// able to make subcriber full screen------------------------------------------------------------------


// See the config.js file------------------------------------------------------------------------------
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
// See the config.js file------------------------------------------------------------------------------