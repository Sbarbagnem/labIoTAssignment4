var express = require('express');
var path = require('path');
var cookieParser = require('cookie-parser');
var logger = require('morgan');
var http = require('http');
const bodyParser = require('body-parser');
const fs = require('fs');
const axios = require('axios');

var app = express();
const server = http.createServer(app);
server.listen(8080);

const io = require('socket.io')(server);

app.use(logger('dev'));
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));
app.use(bodyParser.urlencoded({ extended: true }));
app.set('view engine', 'ejs');

axios.defaults.port = 80;
 
app.get('/', function(req, res, next) {

    fs.readFile( __dirname + "/model/" + "network.json", 'utf8', function (err, data) {
        network = JSON.parse(data);
        res.render('index.ejs', {network: network});
    });
});
  
app.get('/:system', (req,res)=>{

    fs.readFile( __dirname + "/model/" + "network.json", 'utf8', function (err, data) {
        
        network = JSON.parse(data);

        console.log(req.params.system);
        
        var node = network.find(function(element) { 
            //if(element.link == req.params.system){
            if(element.id == req.params.system){
                return element;
            } 
        });   

        res.render('node.ejs', {node: node, id: node.id, net: network});
    });

});

app.post('/updateNetwork/createNode', function (req, res) {

    console.log("ho ricevuto dati da arduino");
    console.log(req.body);

    var id = req.body.id;
    var inNet = false;
    var body = req.body;
	
    fs.readFile( __dirname + "/model/" + "network.json", 'utf8', function (err, data) {
        data = JSON.parse(data);
        data.forEach(element => {
            if(element.id == id){
                inNet = true;
            }
        });
        if(!inNet){
            body.ping = [true,true,true];
            data.push(body);
            fs.writeFile(__dirname + "/model/" + "network.json", JSON.stringify(data, null, 2), 'utf8', function (err) { 
                if (err) {	
                    console.log("An error occured while writing JSON Object to File."); 
                    inNet = false;
                    return console.log(err); 
                } 	
                console.log("JSON file has been saved.");	
                inNet = false;
                console.log("nodo inserito con successo");

                res.end("nodo inserito con successo");
                io.emit('add', req.body.name);
            });
        } else {
            console.log("modello gia' presente")
            res.end("modello gia' presente");
        } 
    });
});

app.post('/sensor/value', (req, res) =>{
    console.log(req.body);

    io.emit(req.body.id, req.body);

    res.end();
});

app.post('/node/check', (req,res) => {

    var id = req.body.id;

    fs.readFile( __dirname + "/model/" + "network.json", 'utf8', function (err, data) {

        data = JSON.parse(data);

        for(var i=1 ; i<data.length; i++)
        {
            if(data[i].id == id){
                console.log("sono dentro");
                data[i].ping.fill(true);
                console.log(data[i].ping);
            }
        }

        fs.writeFile(__dirname + "/model/" + "network.json", JSON.stringify(data, null, 2), 'utf8', function (err) { 
            if (err) {	
                console.log("An error occured while writing JSON Object to File."); 
                remove = false;
                return console.log(err); 
            } 		
        }); 
    }); 
    
    res.end("check effettuato");
});

app.use(function(req, res, next){
    res.setHeader('Content-Type', 'text/plain');
    res.status(404).send('La pagina non esiste amico!');
});

setInterval(function(){
    check_function()}, 40000)

function check_function(){

    fs.readFile( __dirname + "/model/" + "network.json", 'utf8', function (err, data) {

        var data = JSON.parse(data);

        var remove = false;

        //var remove_id = []; vecchio

        var remove_elements = []; // nuovo

        var element = {}; // nuovo

        for(var i=1 ; i<data.length; i++)
        {
            //var index = data[i].ping.indexOf(true); 
            index = data[i].ping.indexOf(true);
            data[i].ping[index] = false;
            console.log(index);
            console.log(data[i].ping);

            // vecchio
            /*if(data[i].ping.every(allFalse)){
                console.log("nodo eliminato");
                remove_id.push(data[i].id);
                data.splice(i);
                remove = true;
            }*/

            // nuovo
            if (data[i].ping.every(allFalse)) {
                console.log("nodo eliminato");
                element.id = data[i].id;
                element.name = data[i].name;
                remove_elements.push(element);
                //remove_id.push(data[i].id);
                data.splice(i,1);
                remove = true;
            }           
        }

        fs.writeFile(__dirname + "/model/" + "network.json", JSON.stringify(data, null, 2), 'utf8', function (err) { 
            if (err) {	
                console.log("An error occured while writing JSON Object to File."); 
                remove = false;
                return console.log(err); 
            } 		
            remove = false;
            console.log("ping modificati");
        });

        if(remove){
            // io.emit('remove', remove_id); vecchio
            io.emit('remove', remove_elements);
        }
    });
}

function allFalse(currentValue) {
    return currentValue == false;
}
    
module.exports = app;
