const request = require('supertest');
const { v4: uuidv4 } = require('uuid');
const fs = require('fs')

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

let server1 = "http://127.0.0.1:3490";
let server_root = "../../server_root/";

describe("POST", () => {
	beforeEach(async () => {
		await new Promise(r => setTimeout(() => r(), 10));
	});

	test(server1 + "/post should return 200", async () => {
		const res = await request(server1)
			.post("/post")
		expect(res.status).toBe(200);
		// expect(res.headers["content-type"]).toContain("text/html");
		// expect(res.headers["content-length"]).not.toBe("");
		// expect(body_math(res.text, server_root + "index.html")).toBeTruthy();
	});

	test(server1 + "/post/ should return 200", async () => {
		const res = await request(server1)
			.post("/post/")
		expect(res.status).toBe(200);
		// expect(res.headers["content-type"]).toContain("text/html");
		// expect(res.headers["content-length"]).not.toBe("");
		// expect(body_math(res.text, server_root + "index.html")).toBeTruthy();
	});

	test(server1 + "/sito should return 200", async () => {
		const res = await request(server1)
			.post("/sito")
		expect(res.status).toBe(200);
		// expect(res.headers["content-type"]).toContain("text/html");
		// expect(res.headers["content-length"]).not.toBe("");
		// expect(body_math(res.text, server_root + "index.html")).toBeTruthy();
	});

	test(server1 + "/sito/ should return 200", async () => {
		const res = await request(server1)
			.post("/sito/")
		expect(res.status).toBe(200);
		// expect(res.headers["content-type"]).toContain("text/html");
		// expect(res.headers["content-length"]).not.toBe("");
		// expect(body_math(res.text, server_root + "index.html")).toBeTruthy();
	});

	test(server1 + "/sito/upload.php should receive a 1K file", async () => {
		const res = await request(server1)
			.post("/sito/upload.php")
			.attach("fileToUpload", "../www/file_to_upload/1K.txt");
		expect(res.status).toBe(200);
		// expect(res.headers["content-type"]).toContain("text/html");
		// expect(res.headers["content-length"]).not.toBe("");
		// expect(body_math(res.text, server_root + "index.html")).toBeTruthy();
	});
});
