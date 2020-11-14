const express = require('express');
const bodyParser = require('body-parser');
const mongoose = require('mongoose');
const Temperatura = require('./models/temperatura');
const Dato = require('./models/dato');
const uri = "mongodb+srv://Josue:admin@cluster0.j60fu.mongodb.net/proydos?retryWrites=true&w=majority";

const app = express();

mongoose.connect(uri)
  .then(db => console.log('BD Conectada'))
  .catch(err => console.log(err));


app.use(
    bodyParser.urlencoded({
        extended: true
    })
);

app.use(bodyParser.json());

const port = process.env.PORT || 8080;

app.post('/temperatura', postTemperatura);
app.get('/temperatura', getTemperatura);
app.post('/dato', postDato);
app.get('/dato', getDatos);


const server = app.listen(process.env.PORT || 5000);


async function postTemperatura(req, res) {
    const temperatura = new Temperatura(req.body);
  temperatura.save();
  res.send('Se guardaron los datos');
}


async function getTemperatura(req, res) {
    const temperaturas = await Temperatura.find().sort({$natural:-1}).limit(1);;
    res.send(temperaturas);

}
async function postDato(req, res) {
    const hoy= new Date;
    var options = { day: 'numeric', month: 'numeric', year: 'numeric' };
    var datojson={
        id: req.body.id,
        tiempo:req.body.tiempo,
        fecha:hoy.toLocaleDateString("es-GT", options)
    }
    const dato = new Dato(datojson);
    dato.save();
    res.send('Se guardaron los datos');
   
}
async function getDatos(req, res) {
    const datos = await Dato.find(({fecha: { $regex: '.*' + req.query.fecha + '.*' } }));
    res.send(datos);

}


