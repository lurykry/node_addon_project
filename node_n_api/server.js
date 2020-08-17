'use strict';
const addon = require('bindings')('addon');
const express = require("express");
const bodyParser = require("body-parser");
const urlencodedParser = bodyParser.urlencoded({ extended: false });
const app = express();

app.set("view engine", "hbs");
app.set('views', __dirname + '/views');
app.use(express.static(__dirname + '/static'));

app.get('/', urlencodedParser, (req, res) => {
    res.sendFile(__dirname + "/views/index.html");
});


app.post("/", urlencodedParser,(req, res) => {

    if (!req.body) return res.sendStatus(400);

    let userName = req.body.userName.trim();
    let isAccountName = addon.IsRegisteredAsUser(userName);

    if (isAccountName) {
        res.render('success.hbs', {
            userName: userName
        });
    } else {
        res.render('failure.hbs', {
            userName: userName
        });
    }
});

var port = process.env.port || 1337;
app.listen(port);
