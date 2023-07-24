/* eslint-disable no-unused-vars */

// Make a copy of this file and save it as config.js (in the js directory).

// Set this to the base URL of your sample server, such as 'https://your-app-name.herokuapp.com'.
// Do not include the trailing slash. See the README for more information:

const SAMPLE_SERVER_BASE_URL = 'http://127.0.0.1:5001';

// OR, if you have not set up a web server that runs the learning-opentok-php code,
// set these values to OpenTok API key, a valid session ID, and a token for the session.
// For test purposes, you can obtain these from https://tokbox.com/account.

// config.js
const API_KEY = '47679271';
let SESSION_ID= '';
let TOKEN = '';

// // // Fetch session info from your Flask server
// fetch(SAMPLE_SERVER_BASE_URL) // Replace with the appropriate URL of your Flask server
//   .then((response) => response.json())
//   .then((json) => {
//     // Update SESSION_ID and TOKEN with the fetched values
//     SESSION_ID = json[0];
//     TOKEN = json[1];

//     // Initialize the session using the updated SESSION_ID and TOKEN
//     // initializeSession();
//   })
//   .catch((error) => {
//     console.error('Failed to fetch session info:', error);
//     alert('Failed to get opentok sessionId and token. Make sure you have updated the config.js file.');
//   });








