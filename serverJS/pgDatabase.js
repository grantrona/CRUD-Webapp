const express = require('express');
const bodyParser = require('body-parser');
const app = express()
const port = 5000;

const database = require("./pgQuery");

app.use(bodyParser.json());
app.use(
    bodyParser.urlencoded({
        extended: true,
    })
);
app.use((req, res, next) => {
    res.setHeader("Access-Control-Allow-Origin", "*");
    res.setHeader("Access-Control-Allow-Methods", "GET, POST, DELETE");
    res.header(
        "Access-Control-Allow-Headers",
        "Origin, X-Requested-With, Content-Type, Accept"
    );
    next();
});

app.get('/items', database.getItems);

app.post('/items', database.postItems);

app.delete('/items', database.deleteItems);

app.listen(port, () => {
    console.log(`App listening on port ${port}`);
})

