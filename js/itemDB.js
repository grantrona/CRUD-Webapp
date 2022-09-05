const {Client} = require('pg');

const client = new Client({
    host: "ec2-3-224-8-189.compute-1.amazonaws.com",
    user: "qfszfxgiijpwtf",
    port: "5432",
    password: "ef957c66f851132c19567be8fb532131809b8cd265d0e3ceb99ec481a2097904",
    database: "d4vi9mo6kqq81k",
});

export function save(items) {
    client.connect()
    client.end();
}

export function load() {
    client.connect();
    //Client promise TODO
    client
        .query('SELECT * from ITEMS')
        .then(res => console.log("FETCHED!\n" + res))
        .catch(e => console.error(e.stack));

    client.end();
}