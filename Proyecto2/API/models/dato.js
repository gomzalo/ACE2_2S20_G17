
const mongoose = require('mongoose');
const Schema = mongoose.Schema;

const datoSchema = new Schema({
    id : String,
    tiempo : Number,
    fecha : String
});

module.exports = mongoose.model('dato', datoSchema);