import { PolymerElement, html } from '@polymer/polymer/polymer-element.js';

import '@polymer/iron-icons/iron-icons.js';
import '@polymer/iron-input/iron-input.js';

import '@polymer/paper-styles/paper-styles.js';
import '@polymer/paper-toast/paper-toast.js';

import '@hunsalz/web-socket/web-socket.js';

class TrackedRacerApp extends PolymerElement {
  static get template() {
    return html`
      <style>
        :host {
          display: block;
          @apply --paper-font-common-base;
        }

        .container {
          position: absolute;
          top: 0;
          left: 0;
          height: 100%;
          width: 100%;
          overflow: hidden;
          background-color: #455a64;
        }

        .control {
          display: inline-flex;
          flex-flow: column nowrap;
          justify-content: center;
          align-content: center;
          align-items: center;
          position: absolute;
          top: 50%;
          left: 50%;
          width: 100vh;
          height: 100vw;
          transform: translate(-50%, -50%) rotate(-90deg);
        }

        .value {
          flex: 0 1 auto;
          margin-top: 15vw;
          margin-bottom: 15vw;
          color: white;
          font-size: 2em;
          transform: rotate(90deg);
        }

        .middle {
          flex: 1 1 auto;
          display: inline-flex;
          flex-flow: column nowrap;
          justify-content: center;
          align-content: center;
          align-items: center;
          min-height: 32vw;
        }

        iron-icon {
          color: var(--state-color, var(--paper-orange-700));
          width: 8vw;
          height: 8vw;
          transform: rotate(90deg);
        }

        input[type=range] {
          -webkit-appearance: none;
          flex: 0 1 auto;
          width: 100vh;
        }

        input[type=range]::-webkit-slider-runnable-track {
          -webkit-appearance: none;
          width: 100%;
          height: 2vw;
          border: none;
          background: white;
        }

        input[type=range]::-moz-range-track {
          width: 100%;
          height: 2vw;
          border: none;
          background: white;
        }

        input[type=range]::-webkit-slider-thumb {
          -webkit-appearance: none;
          height: 12vw;
          width: 12vw;
          margin-top: -5vw;
          border-radius: 50%;
          border: 2vw solid white;
          box-shadow: 0vh 0vh 1vh rgba(0, 0, 0, .8);
          background: var(--paper-orange-700);
        }

        input[type=range]::-moz-range-thumb {
          height: 9vw;
          width: 9vw;
          margin-top: -5vw;
          border-radius: 50%;
          border: 2vw solid white;
          box-shadow: 0vh 0vh 1vh rgba(0, 0, 0, .8);
          background: var(--paper-orange-700);
        }

        input[type=range]:focus {
          outline: none;
        }

        input[type=range]:-moz-focusring {
          outline: none;
        }

        input[type=range]:focus::-webkit-slider-runnable-track {
          background: #ccc;
        }

        input[type=range]:focus::-moz-range-track {
          background: #ccc;
        }

        paper-toast {
          width: 100%;
          text-align: center;
        }
      </style>
      
      <!-- service components -->

      <web-socket id="ws" url="[[url]]" state="{{state}}"></web-socket>

      <!-- UI components -->
      
      <div class="container">
        <div class="control">
          <div class="value">[[__getPercent(motorA)]]%</div>
          <input type="range" value="{{motorA::input}}" min="-[[range]]" max="[[range]]" step="[[step]]"></input>
          <div class="middle">
            <iron-icon icon="icons:settings-remote"></iron-icon>
          </div>
          <input type="range" value="{{motorB::input}}" min="-[[range]]" max="[[range]]" step="[[step]]"></input>
          <div class="value">[[__getPercent(motorB)]]%</div>
        </div>
      </div>

      <paper-toast id="toast" text="[[__showWSMessage(state)]]" duration="6000"></paper-toast>
    `;
  }
  static get properties() {
    return {
      url: {
        type: String,
        notify: true,
        computed: '__computeWSUrl()'
      },
      state: {
        type: Number,
        observer: '__handleWSStateChanges',
        notify: true
      },
      range: {
        type: Number,
        value: 25
      },
      step: {
        type: Number,
        value: 5
      },
      motorA: {
        type: Number,
        value: 0,
        notify: true,
        observer: '__changed'
      },
      motorB: {
        type: Number,
        value: 0,
        notify: true,
        observer: '__changed'
      }
    };
  }

  __changed() {
    this.$.ws.send({ motorA: this.motorA, motorB: this.motorB, mode: 'absolute' }, true);
  }

  __handleWSStateChanges() {

    var _color = 'var(--paper-orange-700)';
    switch (this.state) {
      case 1:
        _color = 'var(--paper-green-700)';
        break;
      case 3:
        _color = 'var(--paper-red-700)';
        break;
      default:
        _color = 'var(--paper-orange-700)';
    }
    this.updateStyles({
      '--state-color': _color
    });
  }

  __getPercent(value) {
    return 100 / this.range * value;
  }

  /**
   * Show a human readable WS state message
   * @param {*} state 
   */
  __showWSMessage(state) {

    this.$.toast.open();
    switch (this.state) {
      case -1:
        return "No response from " + this.url;
      case 0:
        return "Connecting with " + this.url;
      case 1:
        return "Connected with " + this.url;
      case 2:
        return "Closing connection with " + this.url;
      case 3:
        return "Connection with " + this.url + " closed";
      default:
        // shouldn't be called ever
        this.$.toast.close();
        return "";
    }
  }

  __computeWSUrl() {

    console.log(window.location.hostname);
    console.log("hostname.endsWith(github.io)", window.location.hostname.endsWith("github.io"));

    let hostname = window.location.hostname;
    if (hostname === "127.0.0.1" || hostname.endsWith("github.io")) {
      let wsUrlDev = "wss://echo.websocket.org/";
      console.warn(hostname + " is defined as dev environment. Use " + wsUrlDev + " as mock service.");
      return wsUrlDev;
    } else {
      return "wss://echo.websocket.org/"; // URL must match with device preferences
      //return "ws://" + window.location.hostname + ":8000/racer"; // URL must match with device preferences
    }
  }
}

window.customElements.define('tracked-racer-app', TrackedRacerApp);