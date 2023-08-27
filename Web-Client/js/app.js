
// handle error
// initialise session
// Text chat
// enable move tool box
// able to toggle video window-
// change between text mode and jogging mode
// able to toggle camera view
// able to screenshot subscriber video
// start video share
// end video share
// able to make subcriber full screen
// able to swtich coord mode
// able update step size and feed rate
// send message to move in X Y Z direction depening on button and step size
// smaller
// larger
// left eye home
// zero eye home
// Fetch session data from Flask server and initialize the session


// handle error----------------------------------------------------------------------------------------
function handleError(error) {
  // Check if an error exists
  if (error) {
    // Log the error message to the console with console.error()
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
  // Configure session options with apiKey, sessionId, and token
  const sessionOptions = {
    apiKey: apiKey,
    sessionId: sessionId,
    token: token,
    screenSharing: true // Enable screen sharing in the session options
  };

  // Initialize an OpenTok session using the provided apiKey and sessionId
  session = OT.initSession(apiKey, sessionId);

  // Subscribe to a newly created stream to display video from other participants
  session.on('streamCreated', (event) => {
    // Configure subscriber options for stream display
    const subscriberOptions = {
      insertMode: 'replace',
      width: '100%',
      height: '100%'
    };

    // Subscribe to the stream and display it in the 'subscriber' element
    session.subscribe(event.stream, 'subscriber', subscriberOptions, handleError);
  });

  // Handle disconnection from the session
  session.on('sessionDisconnected', (event) => {
    console.error('You were disconnected from the session.', event.reason);
  });

  // Initialize the local publisher for the user's own video
  const publisherOptions = {
    insertMode: 'append',
    width: '100%',
    height: '100%'
  };
  const publisher = OT.initPublisher('publisher', publisherOptions, handleError);

  // Connect to the session using the provided token
  session.connect(token, (error) => {
    if (error) {
      handleError(error);
    } else {
      // If the connection is successful, publish the user's video stream to the session
      session.publish(publisher, handleError);
    }
  });

  // Handle incoming chat messages and append them to the chat history
  const msgHistory = document.querySelector('#history');
  // For message type = msg (movement message send through text chat under local mode) (only for manual control)
  session.on('signal:move_local', (event) => {
    const msg = document.createElement('p');
    msg.textContent = event.data;
    // Determine whether the message is sent by the user or others and assign appropriate styling
    msg.className = event.from.connectionId === session.connection.connectionId ? 'mine' : 'theirs';
    msgHistory.appendChild(msg);
    msg.scrollIntoView();
  });
  // For message type = coord (movement message send through text chat under global mode) (for both manual and jogging control)
  session.on('signal:move_global', (event) => {
    const coord = document.createElement('p');
    coord.textContent = event.data;
    coord.className = event.from.connectionId === session.connection.connectionId ? 'mine' : 'theirs';
    msgHistory.appendChild(coord);
    coord.scrollIntoView();
  });
  // For message type = config_step (movement message send through text chat under jogging mode used to update
  // step size and feed rate)
  session.on('signal:config_step', (event) => {
    const config_step = document.createElement('p');
    config_step.textContent = event.data;
    config_step.className = event.from.connectionId === session.connection.connectionId ? 'mine' : 'theirs';
    msgHistory.appendChild(config_step);
    config_step.scrollIntoView();
  });
  // For message type = home (movement message send through by clicking on one of the homing buttons)
  session.on('signal:home', (event) => {
    const home = document.createElement('p');
    home.textContent = event.data;
    home.className = event.from.connectionId === session.connection.connectionId ? 'mine' : 'theirs';
    msgHistory.appendChild(home);
    home.scrollIntoView();
  });
  // alert(token);
}
// initialise session----------------------------------------------------------------------------------



// Text chat-------------------------------------------------------------------------------------------
const form = document.querySelector('form');
const msgTxt1 = document.querySelector('#msgTxt1');
const msgTxt2 = document.querySelector('#msgTxt2');
const msgTxt3 = document.querySelector('#msgTxt3');

// Add a click event listener to the any submit event inside the text chat
form.addEventListener('submit', (event) => {
  event.preventDefault();

  // Get the input values for x, y, and z
  const inputValues1 = msgTxt1.value.trim();
  const inputValues2 = msgTxt2.value.trim();
  const inputValues3 = msgTxt3.value.trim();

  // when the inputs are not numbers and if any of the boxes are empty alert based on the mode (local/global)
  if ((inputValues1 === '' || inputValues2 === '' || inputValues3 === '' || isNaN(inputValues1) || isNaN(inputValues2) || isNaN(inputValues3)) && (!isChatSwapped)) {
    alert('Please enter valid values for x, y, or z.');
    return;
  } else if ((inputValues1 === '' || inputValues2 === '' || inputValues3 === '' || isNaN(inputValues1) || isNaN(inputValues2) || isNaN(inputValues3)) && (isChatSwapped)) {
    alert('Please enter valid coords for x, y, or z.');
    return;
  }

  // Parse the input values into numeric variables
  const x = parseFloat(inputValues1);
  const y = parseFloat(inputValues2);
  const z = parseFloat(inputValues3);

  // Combine the values into a single message
  const newMessage = `${x},${y},${z}`;

  // Send a signal with the combined message, distinguishing between local and global modes isChatSwapped is explained
  // in detail in swtich coord mode
  if (!isChatSwapped) {
    session.signal({
      // message type move_local is for movement messages thats in local sub-mode
      type: 'move_local',
      data: newMessage
    }, (error) => {
      if (error && (!isChatSwapped)) {
        alert(error);
        handleError(error);
      } else {
        // Clear the input fields after successful signal
        document.querySelector('#msgTxt1').value = '';
        document.querySelector('#msgTxt2').value = '';
        document.querySelector('#msgTxt3').value = '';
      }
    });
  } else {
    session.signal({
      // message type move_global is for movement messages thats in global sub-mode
      type: 'move_global',
      data: newMessage
    }, (error) => {
      if (error && (isChatSwapped)) {
        alert(error);
        handleError(error);
      } else {
        // Clear the input fields after successful signal
        document.querySelector('#msgTxt1').value = '';
        document.querySelector('#msgTxt2').value = '';
        document.querySelector('#msgTxt3').value = '';
      }
    });
  }
});
// Text chat-------------------------------------------------------------------------------------------



// enable move tool box---------------------------------------------------------------------------------------
// Select the 'toolBox' element using jQuery and assign it to the 'toolBox' variable
const toolBox = $('#toolBox');
// Make the selected 'toolBox' element draggable using jQuery UI's 'draggable()' method
toolBox.draggable();
// move video window-----------------------------------------------------------------------------------



// able to toggle video window-------------------------------------------------------------------------
// Select the element with the ID 'toggleButton' from the DOM and store it in 'toggleButton'
const toggleButton = document.querySelector('#toggleButton');

// Declare a variable 'isHidden' when it is false the publish window is not hidden, when it is true
// the public window is hidden
let isHidden = false;

// Add a click event listener to the 'toggleButton'
toggleButton.addEventListener('click', () => {
  if (isHidden) {
    // If 'isHidden' is true, set the display style of 'publisher' to 'block'
    publisher.style.display = 'block';
    // Set 'isHidden' to false, indicating the publisher is now visible
    isHidden = false;
  } else {
    // If 'isHidden' is false, set the display style of 'publisher' to 'none'
    publisher.style.display = 'none';
    // Set 'isHidden' to true, indicating the publisher is now hidden
    isHidden = true;
  }
});
// able to toggle video window-------------------------------------------------------------------------



// change between text mode and jogging mode------------------------------------------------------------
// Select the element with the ID 'toggleChat' from the DOM and store it in the variable 'toggleChat'
const toggleChat = document.querySelector('#toggleChat');

// Declare a variable 'jogging_mode', when it is false jogging mode is not on, when it is true jogging
// mode is on
let jogging_mode = false;

// Add a click event listener to the 'toggleChat' element
toggleChat.addEventListener('click', () => {
  // Check if 'jogging_mode' is true
  if (jogging_mode) {
    // If 'jogging_mode' is true, set the display style of 'textchat' to 'block'
    textchat.style.display = 'block';
    // Set 'jogging_mode' to false, indicating the text chat is now visible
    jogging_mode = false;
  } else {
    // If 'jogging_mode' is false, set the display style of 'textchat' to 'none'
    textchat.style.display = 'none';
    // Set 'jogging_mode' to true, indicating the text chat is now hidden
    jogging_mode = true;
  }
});
// change between text mode and jogging mode------------------------------------------------------------



// able to toggle camera view--------------------------------------------------------------------------
// Select the element with the ID 'toggleCamera' from the DOM and store it in the variable 'toggleCameraButton'
const toggleCameraButton = document.querySelector('#toggleCamera');

// Declare a variable 'isSwapped', when it is false the camera window between publisher and subscriber has not 
// been swapped, when it is true they have been swapped
let isSwapped = false;

// Define 'originalPublisherStyle' with the initial CSS styles for the publisher video element
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

// Define 'originalSubscriberStyle' with the initial CSS styles for the subscriber video element
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

// Add a click event listener to the 'toggleCameraButton' element
toggleCameraButton.addEventListener('click', () => {
  // Get the publisher and subscriber elements by their respective IDs
  const publisherElement = document.getElementById('publisher');
  const subscriberElement = document.getElementById('subscriber');

  // Swap the display formats of publisher and subscriber elements
  if (!isSwapped) {
    // Change the video format to subscriber format
    Object.assign(publisherElement.style, originalSubscriberStyle);
    Object.assign(subscriberElement.style, originalPublisherStyle);

    // Update the 'isSwapped' flag to indicate the swap
    isSwapped = true;
  } else {
    // Change back the video format to the original publisher format
    Object.assign(publisherElement.style, originalPublisherStyle);
    Object.assign(subscriberElement.style, originalSubscriberStyle);

    // Update the 'isSwapped' flag to indicate the revert
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

// Declare a variable 'isSharing', when it is false screen sharing has not started,  when it is true 
// screen sharing has started
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

    // // Create a publisher element same as before
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

    // publish the new publisher
    session.publish(webCamPublisher, handleError);

    // Update the 'isSharing' flag to indicate the screen sharing has ended
    isSharing = false;
  }
});
// end video share-----------------------------------------------------------------------------------


// able to make subcriber full screen------------------------------------------------------------------
const fullScreenButton = document.querySelector('#fullScreen');

// Declare a variable 'isFullScreen', when it is false the screen has not in full screen mode,  when 
// it is true full screen mode has started
let isFullScreen = false;

// create a new subscriber window element that takes up the whole screen
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
  if(jogging_mode){
    alert('control panel only allows local mode');
    return
  }
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


// able update step size and feed rate-----------------------------------------------------------------
const update = document.querySelector('#update');
const stepSize_XYZ = document.querySelector('#stepSize_XYZ');
const feedRate = document.querySelector('#feedRate');
let updated = false;
let stepSize_Value = '';

update.addEventListener('click', () => {
  // Get the input values
  const input1 = stepSize_XYZ.value.trim();
  const input2 = feedRate.value.trim();

  // Validate the input
  if (input1 === '' || input2 === '' || isNaN(input1) || isNaN(input2)) {
    alert('Please enter valid feed rate or step size.');
    return;
  } else{
    updated = true;
  }

  // Parse the input values into numbers
  const step_size = parseFloat(input1);
  stepSize_Value = step_size;
  const feed_rate = parseFloat(input2);

  // Combine the values into one message
  const newMessage = `${feed_rate}`;

  // Send a signal with the combined message
  if(jogging_mode){
    session.signal({
      type: 'config_step',
      data: newMessage
    }, (error) => {
      if (error && (!jogging_mode)) {
        alert(error);
        handleError(error);
      }
    });
  } else {
    alert('Please turn on jogging mode.');
  }
});
// able update step size and feed rate-----------------------------------------------------------------


// send message to move in X Y Z direction depening on button and step size----------------------------
const move_X_Neg_Z_Pos = document.querySelector('#move_X_Neg_Z_Pos'); // 1
const move_Z_Pos = document.querySelector('#move_Z_Pos'); // 2
const move_X_Pos_Z_Pos = document.querySelector('#move_X_Pos_Z_Pos'); // 3
const move_X_Neg = document.querySelector('#move_X_Neg'); // 4
const move_X_Pos = document.querySelector('#move_X_Pos'); // 5
const move_X_Neg_Z_Neg = document.querySelector('#move_X_Neg_Z_Neg'); // 6
const move_Z_Neg = document.querySelector('#move_Z_Neg'); // 7
const move_X_Pos_Z_Neg = document.querySelector('#move_X_Pos_Z_Neg'); //8
const move_Y_Pos = document.querySelector('#move_Y_Pos'); //9
const move_Y_Neg = document.querySelector('#move_Y_Neg'); //10

move_X_Neg_Z_Pos.addEventListener('click', () => {
  // Validate the input
  if (!updated) {
    alert('Please set feed rate or step size first.');
    return;
  }
  // Parse the input values into numbers
  const x = '-' + stepSize_Value;
  const y = '0';
  const z = stepSize_Value;
  // Combine the values into one message
  const newMessage = `${x},${y},${z}`;

  session.signal({
    type: 'move_local',
    data: newMessage
  }, (error) => {
    if (error) {
      alert(error);
      handleError(error);
    }
  });
});

move_Z_Pos.addEventListener('click', () => {
  // Validate the input
  if (!updated) {
    alert('Please set feed rate or step size first.');
    return;
  }
  // Parse the input values into numbers
  const x = '0';
  const y = '0';
  const z = stepSize_Value;
  // Combine the values into one message
  const newMessage = `${x},${y},${z}`;

  session.signal({
    type: 'move_local',
    data: newMessage
  }, (error) => {
    if (error) {
      alert(error);
      handleError(error);
    }
  });
});

move_X_Pos_Z_Pos.addEventListener('click', () => {
  // Validate the input
  if (!updated) {
    alert('Please set feed rate or step size first.');
    return;
  }
  // Parse the input values into numbers
  const x = stepSize_Value;
  const y = '0';
  const z = stepSize_Value;
  // Combine the values into one message
  const newMessage = `${x},${y},${z}`;

  session.signal({
    type: 'move_local',
    data: newMessage
  }, (error) => {
    if (error) {
      alert(error);
      handleError(error);
    }
  });
});

move_X_Neg.addEventListener('click', () => {
  // Validate the input
  if (!updated) {
    alert('Please set feed rate or step size first.');
    return;
  }
  // Parse the input values into numbers
  const x = '-' + stepSize_Value;
  const y = '0';
  const z = '0';
  // Combine the values into one message
  const newMessage = `${x},${y},${z}`;

  session.signal({
    type: 'move_local',
    data: newMessage
  }, (error) => {
    if (error) {
      alert(error);
      handleError(error);
    }
  });
});

move_X_Pos.addEventListener('click', () => {
  // Validate the input
  if (!updated) {
    alert('Please set feed rate or step size first.');
    return;
  }
  // Parse the input values into numbers
  const x = stepSize_Value;
  const y = '0';
  const z = '0';
  // Combine the values into one message
  const newMessage = `${x},${y},${z}`;

  session.signal({
    type: 'move_local',
    data: newMessage
  }, (error) => {
    if (error) {
      alert(error);
      handleError(error);
    }
  });
});

move_X_Neg_Z_Neg.addEventListener('click', () => {
  // Validate the input
  if (!updated) {
    alert('Please set feed rate or step size first.');
    return;
  }
  // Parse the input values into numbers
  const x = '-' + stepSize_Value;
  const y = '0';
  const z = '-' + stepSize_Value;
  // Combine the values into one message
  const newMessage = `${x},${y},${z}`;

  session.signal({
    type: 'move_local',
    data: newMessage
  }, (error) => {
    if (error) {
      alert(error);
      handleError(error);
    }
  });
});

move_Z_Neg.addEventListener('click', () => {
  // Validate the input
  if (!updated) {
    alert('Please set feed rate or step size first.');
    return;
  }
  // Parse the input values into numbers
  const x = '0';
  const y = '0';
  const z = '-' + stepSize_Value;
  // Combine the values into one message
  const newMessage = `${x},${y},${z}`;

  session.signal({
    type: 'move_local',
    data: newMessage
  }, (error) => {
    if (error) {
      alert(error);
      handleError(error);
    }
  });
});

move_X_Pos_Z_Neg.addEventListener('click', () => {
  // Validate the input
  if (!updated) {
    alert('Please set feed rate or step size first.');
    return;
  }
  // Parse the input values into numbers
  const x = stepSize_Value;
  const y = '0';
  const z = '-' + stepSize_Value;
  // Combine the values into one message
  const newMessage = `${x},${y},${z}`;

  session.signal({
    type: 'move_local',
    data: newMessage
  }, (error) => {
    if (error) {
      alert(error);
      handleError(error);
    }
  });
});

move_Y_Pos.addEventListener('click', () => {
  // Validate the input
  if (!updated) {
    alert('Please set feed rate or step size first.');
    return;
  }
  // Parse the input values into numbers
  const x = '0';
  const y = stepSize_Value;
  const z = '0';
  // Combine the values into one message
  const newMessage = `${x},${y},${z}`;

  session.signal({
    type: 'move_local',
    data: newMessage
  }, (error) => {
    if (error) {
      alert(error);
      handleError(error);
    }
  });
});

move_Y_Neg.addEventListener('click', () => {
  // Validate the input
  if (!updated) {
    alert('Please set feed rate or step size first.');
    return;
  }
  // Parse the input values into numbers
  const x = '0';
  const y = '-' + stepSize_Value;
  const z = '0';
  // Combine the values into one message
  const newMessage = `${x},${y},${z}`;

  session.signal({
    type: 'move_local',
    data: newMessage
  }, (error) => {
    if (error) {
      alert(error);
      handleError(error);
    }
  });
});
// send message to move in X Y Z direction depening on button and step size----------------------------


// smaller---------------------------------------------------------------------------------------------
const smaller = document.querySelector('#smaller');

smaller.addEventListener('click', () => {
  stepSize_XYZ.value = stepSize_XYZ.value / 10;
});
// smaller---------------------------------------------------------------------------------------------


// larger---------------------------------------------------------------------------------------------
const larger = document.querySelector('#larger');

larger.addEventListener('click', () => {
  stepSize_XYZ.value = stepSize_XYZ.value * 10;
});
// smaller---------------------------------------------------------------------------------------------


// left eye home---------------------------------------------------------------------------------------
const left_eye_home = document.querySelector('#left_eye_home');

left_eye_home.addEventListener('click', () => {
  // Combine the values into one message
  const newMessage = 'left';

  // Send a signal with the combined message
  session.signal({
    type: 'home',
    data: newMessage
  }, (error) => {
    if (error) {
      alert(error);
      handleError(error);
    }
  });
});
// left eye home---------------------------------------------------------------------------------------

// zero eye home---------------------------------------------------------------------------------------
const zero_eye_home = document.querySelector('#zero_eye_home');

zero_eye_home.addEventListener('click', () => {
  // Combine the values into one message
  const newMessage = 'zero';

  // Send a signal with the combined message
  session.signal({
    type: 'home',
    data: newMessage
  }, (error) => {
    if (error) {
      alert(error);
      handleError(error);
    }
  });
});
// zero eye home---------------------------------------------------------------------------------------


// right eye home---------------------------------------------------------------------------------------
const right_eye_home = document.querySelector('#right_eye_home');

right_eye_home.addEventListener('click', () => {
  // Combine the values into one message
  const newMessage = 'right';

  // Send a signal with the combined message
  session.signal({
    type: 'home',
    data: newMessage
  }, (error) => {
    if (error) {
      alert(error);
      handleError(error);
    }
  });
});
// zero eye home---------------------------------------------------------------------------------------


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

