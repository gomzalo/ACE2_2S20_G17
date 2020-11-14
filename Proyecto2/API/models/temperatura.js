
const mongoose = require('mongoose');
const Schema = mongoose.Schema;

const temperaturaSchema = new Schema({
    temperatura : Number,
});

module.exports = mongoose.model('temperatura', temperaturaSchema);