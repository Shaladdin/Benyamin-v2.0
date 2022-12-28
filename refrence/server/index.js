"use strict";

const WebSocket = require('ws');
const http = require('http');
const express = require('express');
const nedb = require('nedb');

require('dotenv').config({ path: __dirname + '\\.env' });

const port = 8080;
const app = express();
const server = http.createServer(app);
const wss = new WebSocket.Server({ server: server });
const db = {
    sensor: new nedb({ filename: `${__dirname}/db/sensors.db`, autoload: true })
}


for (const [key, value] of Object.entries(db)) {
    value.loadDatabase();
    value.persistence.setAutocompactionInterval(60);
}


const { stringify, parse: jsonify } = JSON;
const deserializeError = stringify({
    msg: "error",
    err: "input is not valid json, json only"
})

// common function
// make id for websocket
function getUniqueID() {
    function s4() {
        return Math.floor((1 + Math.random()) * 0x10000).toString(16).substring(1);
    }
    return s4() + s4() + '-' + s4();
};
// check if device already connected
function alreadyConnected(ws) {
    if (ws.device != 'unknown') {
        ws.send(stringify({
            msg: "error",
            err: `acces denied, this device already connected as ${ws.device}`
        }));
        ws.log(`relogin attemp, denied`);
        return true;
    }
    return false;
}
// turn string into json SAFELY 
function objify(str) {
    try {
        return jsonify(String(str));
    } catch (e) {
        return false;
    }
}

const deviceModule = {
    devices: {
        mainReader: {

        }
    },
    update: () => {
        const { devices } = deviceModule;
        for (const [key, device] of Object.entries(devices)) {
            if (device.ws === undefined) return;
            const { ws, id } = device;
            if (ws === undefined && (ws.readyState != WebSocket.OPEN && ws.readyState != WebSocket.CONNECTING)) {
                delete device.ws;
                device.ws = undefined;
            }
        }
    },
};

function updateWs() {
    deviceModule.update();
}


const usersModule = {
    user: class {
        constructor(ws) {
            // const ws = WebSocket;
            this.ws = ws;
            this.id = ws.device;

            ws.on('message', (msg) => {
                let res = objify(msg);
                ws.log('incoming: ' + msg);

            })
        }
    },
    users: {},
    update: () => {
        for (const [key, value] of Object.entries(usersModule.users))
            if (value.ws.readyState != WebSocket.OPEN && value.ws.readyState != WebSocket.CONNECTING)
                delete usersModule.users[key];
    },
    forEach: (f) => {
        for (const [key, value] of Object.entries(usersModule))
            f(value, key);
    },
    scream: (msg) => {
        usersModule.forEach((user) => {
            user.ws.send(msg);
        })
    }
}

// Ws main
wss.on('connection', (ws) => {
    updateWs();
    console.log(`connection attemp been made`);
    ws.send(stringify({ msg: "identity requested" }));

    // ws variables
    // identification needed before progressing
    ws.allowed = false;
    ws.allowStream = false;
    ws.device = "unknown";
    // send log
    ws.log = (logout) => { console.log(ws.device, logout) };
    const { log } = ws;

    // code for starting connection and handle web client
    ws.on('message', (msg) => {
        updateWs();
        // turn it into json
        let res = objify(msg);

        // client connect request
        if (res.msg === "connectClient") {
            if (alreadyConnected(ws)) return;
            ws.device = `client:${getUniqueID()}`
            usersModule.users[ws.device] = new usersModule.user(ws);
            ws.allowed = true;

            ws.send(stringify({
                msg: "connected",
                details: `connected sucsesfully as ${ws.device}`
            }))
            log('connected sucsesfully');
            return;
        }

        // identify request
        if (res.msg === 'identity') {
            // if already connected
            if (alreadyConnected(ws)) return;
            // check if device type legit
            if (!Object.keys(device).includes(res.device)) {
                ws.send(stringify({
                    msg: "error",
                    err: "access denied, device type not found"
                }));
                log('denied, device not found');
                return;
            }
            // check if other device already connected as ws.device
            if (device[res.device].ws !== undefined && res.device != 'user') {
                const { ws } = device[res.device];
                ws.send(stringify({
                    msg: "error",
                    err: `${res.device} connected as other device`
                }));
                ws.terminate();
            }
            // allow and connected
            ws.device = res.device;
            device[ws.device].ws = ws;
            ws.allowed = true;
            ws.send(stringify({
                msg: "connected",
                details: `connected sucsesfully as ${ws.device}`
            }))
            log('device connected');
            // run local script
            device[ws.device].run();
            return;
        }
    })
})


// routers
app.use('/api', require('./Router/api'));
// init
app.use(express.static(`${__dirname}/public`));
server.listen(port, () => { console.log(`server is up on port ${port}!`); })