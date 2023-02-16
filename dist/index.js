import path from 'path';
import WebSocket from 'ws';
import nedb from 'nedb';
import IOTServer, { Connection, Client, Device, //Class
 } from "websocketiot";
const __dirname = process.cwd();
/*
TODO: error handler on:
*database


*/
// Utils
// console.log();
const sleep = (milliseconds) => {
    return new Promise(resolve => setTimeout(resolve, milliseconds));
};
// Databases 
class Databases {
    database;
    static databaseFolder = './src/db/';
    count = async (query) => new Promise((resolve, reject) => { this.database.count(query, (err, n) => { if (err)
        throw err; resolve(n); }); });
    remove = async (query, option = {}) => new Promise((resolve, reject) => { this.database.remove(query, option, (err, numberRemoved) => { if (err)
        throw err; resolve(numberRemoved); }); });
    insert = async (data) => new Promise((resolve, reject) => { this.database.insert(data, (err, doc) => { if (err)
        throw err; resolve(doc); }); });
    update = async (query, update, option = {}) => new Promise((resolve, reject) => { this.database.update(query, update, option, (err, numberUpdated) => { if (err)
        throw err; resolve(numberUpdated); }); });
    find = async (query, projection) => new Promise((resolve, reject) => {
        const handleDoc = (err, doc) => { if (err)
            throw err; resolve(doc); };
        if (projection)
            this.database.find(query, projection, handleDoc);
        else
            this.database.find(query, handleDoc);
    });
    // dbObjects
    dbObjects = {};
    addObjects = (key, value) => this.dbObjects[key] = new dbObject(key, value, this);
    constructor(databaseName) {
        this.database = new nedb({ filename: Databases.databaseFolder + databaseName + '.db', autoload: true });
        this.database.persistence.setAutocompactionInterval(5000);
        this.database.loadDatabase();
    }
}
class dbObject {
    static objects = {};
    database;
    key;
    _value;
    set value(val) { this.database.update({ type: 'dbObject', key: this.key }, { $set: { value: val } }, { upsert: true }); this._value = val; }
    ;
    get value() { return this._value; }
    ;
    loadValue = async () => { let load = (await this.database.find({ type: 'dbObject', key: this.key }))[0].value; return load; };
    constructor(key, value, database = db.Objects) {
        this.database = database;
        this.key = key;
        this._value = value;
        this.loadValue().then((load) => { if (load)
            this._value = load; });
        dbObject.objects[key] = this;
    }
}
const db = {
    Users: new Databases('Users'),
    Sesions: new Databases('Sesions'),
    Objects: new Databases('Objects'),
};
// * "an compiler code, because typescript lacking in built in function .-_-."
// client
const blankTime = new Date();
const extend = (obj1, obj2) => Object.assign({}, obj1, obj2);
;
;
;
;
const userLiteral = { cardId: "69420", name: "Zib Zibowsky", classData: { class: "9.9", absen: 1 }, data: { bornDate: "*insert date here*", gender: "male" }, history: [] };
const literals = {
    accesCommands: { message: '', type: 'new' },
    client: {
        user: {
            user: userLiteral,
            newUserCommand: { message: '', type: 'new', user: userLiteral },
            deleteUserCommand: { message: '', type: 'new', cardId: '' },
        },
        sesion: {
            sesion: { attendence: [], sesionTime: { startTime: blankTime } },
            newSesionCommand: { message: '', type: 'new' },
        }
    },
    benyamin: {
        absenRequest: { message: "absenRequest", cardId: "69420" }
    }
};
// Local Circular Connection //basicly decoy connection just to use the client handler and stuff
const lcc = new Connection(new WebSocket(null), () => { }, true);
const currentSesion = new dbObject("currentSesion", "none");
await currentSesion.loadValue();
const getCurrentSesion = async () => (await db.Sesions.find({ _id: currentSesion.value }))[0];
// handler utils
class Handler {
    connection;
    error = (errorMsg, lccVal, ...log) => { this.connection.sendError(errorMsg); log ? this.connection.log(...log) : undefined; return lccVal; };
    noCommandFound = () => { this.connection.sendError("no command found"); return "no command found"; };
    ok = () => { this.connection.send({ message: '200 ok' }); return '200 ok'; };
    constructor(connection) {
        this.connection = connection;
    }
}
Client.clientHandler = async (req, connection = lcc) => {
    try {
        if (connection.isDecoy)
            connection.log('using lcc (Local Circular Connection):');
        const clientHandler = new Handler(connection);
        const { error, noCommandFound, ok } = clientHandler;
        switch (req.message) {
            case "user":
                switch (req.type) {
                    case "new":
                        // *Create new User
                        if (!connection.checkFormat(req, literals.client.user.newUserCommand))
                            return "wrong format";
                        const { user } = req;
                        let registerd = await db.Users.find({ cardId: user.cardId });
                        if (registerd.length !== 0)
                            return error(`the id: ${user.cardId} is already registered`, "already done", `reattemp to registering the id ${user.cardId}, its already taken by: `, registerd);
                        db.Users.insert(user);
                        connection.log('sucsessfully register the id ', user.cardId, ' as: ', user);
                        return ok();
                    case "delete":
                        // *Delete User
                        if (!connection.checkFormat(req, literals.client.user.deleteUserCommand))
                            return "wrong format";
                        if (await db.Users.remove({ cardId: req.cardId }) == 0)
                            return error(`cannot find the id: ${req.cardId}`, "not exist", `attemp to delete nonexisting id: `, req.cardId);
                        connection.log(`successfully delete the id: `, req.cardId);
                        return ok();
                    default:
                        return noCommandFound();
                }
                ;
            // Accessing the *current* sesion
            case "sesion":
                if (!connection.checkFormat(req, literals.client.sesion.newSesionCommand))
                    return "wrong format";
                switch (req.type) {
                    case "new":
                        // if the "end time" is defined, then compile it into "Date" object
                        if (req.endTime)
                            req.endTime = new Date(req.endTime);
                        // check if theris already sesion running
                        // if there is, then error
                        if (currentSesion.value !== "none")
                            return error(`A sesion is already running`, "already done", `attemp on override a sesion`);
                        // if there is not create one, then set the current sesion to this one
                        // create new sesion
                        const newSesion = {
                            attendence: [],
                            sesionTime: {
                                startTime: new Date(),
                                endTime: req.endTime,
                            }
                        };
                        // save it to the database, and set it as current sesion
                        const sesion = (await db.Sesions.insert(newSesion));
                        currentSesion.value = sesion._id;
                        connection.log(`new sesion created: `, sesion);
                        return ok();
                    case "delete":
                        // if there is no sesion running
                        if (currentSesion.value === "none")
                            return error(`no sesion is currently running`, "not exist", 'attemp to stop an non existing sesion');
                        stopSesion();
                        connection.log(`sesion stopped as requested`);
                        return ok();
                    default:
                        return noCommandFound();
                }
            default:
                return noCommandFound();
        }
    }
    catch (e) {
        throw e;
    }
};
class Benyamin extends Device {
    deviceKind = 'Benyamin';
    absenBuffer;
    constructor(ws, id) {
        super(ws, async (req) => {
            const { connection } = this;
            const robotHandler = new Handler(connection);
            const { error, noCommandFound, ok } = robotHandler;
            switch (req.message) {
                case "ping":
                    // TODO: connection checker?
                    connection.send({ message: "pong" });
                    return "pong";
                case "absenRequest":
                    // absen request
                    if (!connection.checkFormat(req, literals.benyamin.absenRequest))
                        return "wrong format";
                    // get the user
                    let user_ = await db.Users.find({ cardId: req.cardId });
                    // if the user is not found
                    if (user_.length === 0)
                        return error("card id not found", "not exist", "attemp to absen on non existing user");
                    const user = user_[0];
                    // check if there is a sesion running
                    // If not then error
                    if (currentSesion.value == "none")
                        return error("no sesion is currently running", "not exist", "absen attemp on non existing sesion");
                    // get the sesion, then check have they absen already
                    const sesion = await getCurrentSesion();
                    if (sesion === undefined)
                        throw "(main code error by me)Unexpected Internal Error, null pointer to sesion";
                    // If yes, then error
                    if (sesion.attendence.filter((val) => val.cardId == user.cardId).length > 0)
                        return error("this card is already present", "already done", "reabsen attemb on card ", user.cardId);
                    // send them the user name
                    connection.send({ message: "absenResponse", name: user.name });
                    // save the data to the buffer while waiting for the temperatur report
                    this.absenBuffer = { user: user, sesionId: sesion._id };
                    return "200 ok";
                case "tempReport":
                    if (!this.absenBuffer) {
                        return error("internal server error", "internalError", "buffer is empty!!!");
                    }
                    // add the user to the attendence list
                    db.Sesions.update({ _id: this.absenBuffer.sesionId }, { $push: { attendence: { cardId: this.absenBuffer.user.cardId, time: new Date(), temp: req.temp } } });
                    db.Users.update({ _id: this.absenBuffer.user._id }, { $push: { history: this.absenBuffer.sesionId } });
                    // add sesion to the user history
                    return ok();
                case "cancel":
                    this.absenBuffer = undefined;
                    return ok();
                default:
                    return noCommandFound();
            }
        }, id);
    }
}
Device.addDeviceKinds(Benyamin);
// ! TODO:
// Just to check wather it is expired or not
function updateSesion() {
}
function stopSesion() {
    console.log("!stopping sesion!");
    currentSesion.value = "none";
}
//! Main
const Server = new IOTServer({ port: 8000, usePublic: true, publicPath: path.join(__dirname, 'src', 'public'), initOnStart: false, useRouter: false, routersPath: path.join(__dirname, 'dist', 'router'), });
const { app } = Server;
app.set('views', path.join(__dirname, 'src', 'views'));
app.set('view engine', 'ejs');
Server.init();
const render = (res, tab, obj = {}) => {
    res.render(tab, { selected: tab, ...obj });
};
//* get
app.get('/home', (req, res) => render(res, "home"));
app.get('/', (req, res) => render(res, "home"));
app.get('/user', async (req, res) => {
    render(res, "users", { selected: "user", users: await db.Users.find({}) });
});
app.get('/user/:id', async (req, res) => {
    const { id } = req.params;
    var User = (await db.Users.find({ cardId: id }))[0];
    render(res, "user", { user: User });
});
//*post
app.post('/sesion/new', async (req, res) => {
    console.log(await Client.clientHandler({ message: "sesion", type: "new" }));
});
app.get('/sesion/current', async (req, res) => {
    const { value: sesionId } = currentSesion;
    if (sesionId === "none") {
        render(res, "noSesion", { selected: "currentSesion" });
        return;
    }
    const sesion = await getCurrentSesion();
    var report = [];
    // var
    sesion.attendence.forEach((absen) => {
        const { cardId, temp } = absen;
        db.Users.find({ cardId: cardId });
        // report.push({ name: user.name, temp: temp });
    });
    console.log(report);
    render(res, "sesion", { selected: "currentSesion", });
});
app.get('/sesion', async (req, res) => {
    render(res, "history", { selected: "sesion", history: await db.Sesions.find({}) });
});
app.get('/sesion/:id', async (req, res) => {
    const { id } = req.params;
    render(res, "sesion", { sesion: await db.Sesions.find({ _id: id }) });
});
/*
!low priority todo:
TODO: User.edit
*/ 
//# sourceMappingURL=index.js.map