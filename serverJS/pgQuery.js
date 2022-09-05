require("dotenv").config();
const {Pool} = require("pg");

const connectionString = process.env.DATABASE_URL;
process.env["NODE_TLS_REJECT_UNAUTHORIZED"] = 0;

const pool = new Pool ({
    connectionString:connectionString,
    ssl:true,
});

const getItems = (req, resp) => {
    pool.query('SELECT * FROM items ORDER BY id ASC', (err, results) => {
        if (err) {
            console.log(err.stack);
        }
        if (results.rows.length === 0) {
            resp.status(204).send("No Content to return");
        }
        else {
            resp.status(200).json(results.rows);
        }
    });
}

const postItems = (req, resp) => {
    let items = req.body;
    items.forEach((item) => {
        const { id, name, price, desc, color, url } = item;
        pool.query('INSERT INTO items (id, name, price, description, color, url) VALUES ($1, $2, $3, $4, $5, $6) RETURNING *'
            , [id, name, price, desc, color, url], (error, results) => {
                if (error) {
                    console.log(error.stack);
                }
            })
    })
    resp.status(201).send(`Item added to DB`)
}

const deleteItems = (req, resp) => {
    pool.query('DELETE FROM items', (err, results) => {
        if (err) {
            console.log(err.stack);
        }
        resp.status(200).send("Table deleted successfully");
    });
}

module.exports = {
    getItems,
    postItems,
    deleteItems,
}