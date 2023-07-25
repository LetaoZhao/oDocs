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
      insertMode: 'replace',
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
  session.on('signal:move_local', (event) => {
    // alert("local mode");
    const msg = document.createElement('p');
    msg.textContent = event.data;
    msg.className = event.from.connectionId === session.connection.connectionId ? 'mine' : 'theirs';
    msgHistory.appendChild(msg);
    msg.scrollIntoView();
  });
  session.on('signal:move_global', (event) => {
    // alert("global mode");
    const coord = document.createElement('p');
    coord.textContent = event.data;
    coord.className = event.from.connectionId === session.connection.connectionId ? 'mine' : 'theirs';
    msgHistory.appendChild(coord);
    coord.scrollIntoView();
  });
  // alert(token);
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
  if ((inputValues1 === '' || inputValues2 === '' || inputValues3 === '' || isNaN(inputValues1) || isNaN(inputValues2) || isNaN(inputValues3)) && (!isChatSwapped)) {
    alert('Please enter valid values for x, y or z.');
    return;
  } else if((inputValues1 === '' || inputValues2 === '' || inputValues3 === '' || isNaN(inputValues1) || isNaN(inputValues2) || isNaN(inputValues3)) && (isChatSwapped)) {
    alert('Please enter valid coords for x, y or z.');
    return;
  }

  // Parse the input values into numbers
  const x = parseFloat(inputValues1);
  const y = parseFloat(inputValues2);
  const z = parseFloat(inputValues3);

  // Combine the values into one message
  const newMessage = `${x},${y},${z}`;

  // Send a signal with the combined message
  if(!isChatSwapped){
    session.signal({
      type: 'move_local',
      data: newMessage
    }, (error) => {
      if (error && (!isChatSwapped)) {
        alert(error);
        handleError(error);
      } else {
        document.querySelector('#msgTxt1').value = '';
        document.querySelector('#msgTxt2').value = '';
        document.querySelector('#msgTxt3').value = '';
      }
    });
  } else {
    session.signal({
      type: 'move_global',
      data: newMessage
    }, (error) => {
      if (error && (isChatSwapped)) {
        alert(error);
        handleError(error);
      } else {
        document.querySelector('#msgTxt1').value = '';
        document.querySelector('#msgTxt2').value = '';
        document.querySelector('#msgTxt3').value = '';
      }
    });
  }
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

let isSharing = false;

// Add click event listener to the screen sharing button
startScreenShareButton.addEventListener('click', () => {
  
  if(!isSharing){
    // Get the publisher element
    const publisherElement = document.getElementById('publisher');

    // Create a screen sharing publisher
    const screenSharingPublisher = OT.initPublisher(publisherElement, {
      insertMode: 'replace',
      position: 'absolute',
      width: '360px',
      height: '240px',
      bottom: '10px',
      left: '373px',
      marginLeft: '0px',
      zIndex: '100',
      border: '3px solid white',
      borderRadius: '3px',
      publishAudio: true, // Set to false if you don't want to publish audio along with screen share
      videoSource: 'screen' // Use 'screen' to enable screen sharing
    }, handleError);

    // Start publishing the screen sharing stream
    session.publish(screenSharingPublisher, handleError);

    isSharing = true;
  }
});
// start video share-----------------------------------------------------------------------------------


// end video share-------------------------------------------------------------------------------------
// Get the stop screen share button element
const stopScreenShareButton = document.getElementById('stopScreenShare');

// Add click event listener to the stop screen share button
stopScreenShareButton.addEventListener('click', () => {

  // Stop publishing the screen sharing stream
  if (isSharing) {
    // Get the publisher element
    const publisherElement = document.getElementById('publisher');

    // // Create a screen sharing publisher
    const webCamPublisher = OT.initPublisher(publisherElement, {
      insertMode: 'replace',
      position: 'absolute',
      width: '360px',
      height: '240px',
      bottom: '10px',
      left: '10px',
      marginLeft: '0px',
      zIndex: '100',
      border: '3px solid white',
      borderRadius: '3px',
      publishAudio: true, // Set to false if you don't want to publish audio along with screen share
    }, handleError);

    session.publish(webCamPublisher, handleError);

    isSharing = false;
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


// able to swtich coord mode---------------------------------------------------------------------------
const txt1Element = document.getElementById('msgTxt1');
const txt2Element = document.getElementById('msgTxt2');
const txt3Element = document.getElementById('msgTxt3');

const switchCoord = document.querySelector('#switchCoord');
const oldTxtStyle = {
  backgroundColor: '#f0f0f0',
};
const newTxtStyle = {
  backgroundColor: '#ccffcc',
};

let isChatSwapped = false;

switchCoord.addEventListener('click', () => {
  if (isChatSwapped) {
    alert("local mode");
    Object.assign(txt1Element.style, oldTxtStyle);
    Object.assign(txt2Element.style, oldTxtStyle);
    Object.assign(txt3Element.style, oldTxtStyle);
    txt1Element.placeholder = "Enter x value";
    txt2Element.placeholder = "Enter y value";
    txt3Element.placeholder = "Enter z value";
    isChatSwapped = false;
    session.on('signal:move_local');
    session.off('signal:move_global');
  } else {
    alert("global mode");
    Object.assign(txt1Element.style, newTxtStyle);
    Object.assign(txt2Element.style, newTxtStyle);
    Object.assign(txt3Element.style, newTxtStyle);
    txt1Element.placeholder = "Enter x coord";
    txt2Element.placeholder = "Enter y coord";
    txt3Element.placeholder = "Enter z coord";
    isChatSwapped = true;
    session.on('signal:move_local');
    session.off('signal:move_global');
  }
});
// able to swtich coord mode---------------------------------------------------------------------------

// move tool box---------------------------------------------------------------------------------------
const controlPanel = $('#controlPanel');
controlPanel.draggable();
// move video window-----------------------------------------------------------------------------------


// See the config.js file------------------------------------------------------------------------------
// Fetch session data from Flask server and initialize the session
function fetchSessionData() {
  fetch(SAMPLE_SERVER_BASE_URL) // Replace 'http://localhost:5001/' with the appropriate URL of your Flask server
    .then((response) => response.json())
    .then((data) => {
      apiKey = API_KEY;
      sessionId = data[0];
      token = data[1];

      // Initialize the session using the updated SESSION_ID and TOKEN
      initializeSession();
    })
    .catch((error) => {
      console.error('Failed to fetch session info:', error);
      alert('Failed to get OpenTok sessionId and token from the server. Make sure your Flask server is running and returning the correct data.');
    });
}

// See the config.js file
if (API_KEY && TOKEN && SESSION_ID) {
  apiKey = API_KEY;
  sessionId = SESSION_ID;
  token = TOKEN;
  initializeSession();
} else if (SAMPLE_SERVER_BASE_URL) {
  // Fetch session data from the Flask server
  fetchSessionData();
} else {
  alert('You need to set either API_KEY, TOKEN, and SESSION_ID in config.js or SAMPLE_SERVER_BASE_URL to fetch session data from the Flask server.');
}

// See the config.js file------------------------------------------------------------------------------