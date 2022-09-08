const request = require('supertest');
const { v4: uuidv4 } = require('uuid');
const fs = require('fs')
const LoremIpsum = require("lorem-ipsum").LoremIpsum;
const { exec } = require("child_process");

function file_toString(path) {
	var file = fs.readFileSync(path)
		.toString('utf-8');
	return file;
}

function create_file(content) {
	var file = uuidv4();
	fs.writeFileSync("/tmp/" + file, content);
	return "/tmp/" + file;
}

function body_math(res_body, file) {

	var body = create_file(res_body);
	var ret = fs.readFileSync(body).equals(fs.readFileSync(file));
	fs.unlinkSync(body);
	return ret;
}

module.exports = {
	request, uuidv4, fs, LoremIpsum, file_toString, create_file, body_math, exec
};
