const { request, fs, LoremIpsum, body_math, exec } = require('./utils.js');

let server1 = "http://127.0.0.1:3490";
let server_root = "../../server_root/";

describe("POST", () => {
	beforeAll(async () => {
		if (!fs.existsSync("../www/files_to_upload")) {
			fs.mkdirSync("../www/files_to_upload");
		}
		fs.copyFile("../www/phpini", "../../php.ini", () => { });
		await exec("echo yes > ../www/files_to_upload/1W.txt", () => { });
		await exec("yes this is test file | head -c 1K > ../www/files_to_upload/1K.txt", () => { });
		await exec("yes this is test file | head -c 100K > ../www/files_to_upload/100K.txt", () => { });
		await exec("yes this is test file | head -c 1M > ../www/files_to_upload/1M.txt", () => { });
		await exec("yes this is test file | head -c 10M > ../www/files_to_upload/10M.txt", () => { });
		await exec("yes this is test file | head -c 20M > ../www/files_to_upload/20M.txt", () => { });
		await exec("yes this is test file | head -c 100M > ../www/files_to_upload/100M.txt", () => { });
		await exec("yes this is test file | head -c 101M > ../www/files_to_upload/101M.txt", () => { });
		await exec("yes this is test file | head -c 500M > ../www/files_to_upload/500M.txt", () => { });
		await exec("yes this is test file | head -c 600M > ../www/files_to_upload/600M.txt", () => { });
	});

	afterAll(async () => {
		fs.unlink("../www/files_to_upload/1W.txt", () => { });
		fs.unlink("../www/files_to_upload/1K.txt", () => { });
		fs.unlink("../www/files_to_upload/100K.txt", () => { });
		fs.unlink("../www/files_to_upload/1M.txt", () => { });
		fs.unlink("../www/files_to_upload/10M.txt", () => { });
		fs.unlink("../www/files_to_upload/20M.txt", () => { });
		fs.unlink("../www/files_to_upload/100M.txt", () => { });
		fs.unlink("../www/files_to_upload/101M.txt", () => { });
		fs.unlink("../www/files_to_upload/500M.txt", () => { });
		fs.unlink("../www/files_to_upload/600M.txt", () => { });

		fs.unlink("../www/post/uploads/1W.txt", () => { });
		fs.unlink("../www/post/uploads/1K.txt", () => { });
		fs.unlink("../www/post/uploads/100K.txt", () => { });
		fs.unlink("../www/post/uploads/1M.txt", () => { });
		fs.unlink("../www/post/uploads/10M.txt", () => { });
		fs.unlink("../www/post/uploads/20M.txt", () => { });
		fs.unlink("../www/post/uploads/100M.txt", () => { });
		fs.unlink("../www/post/uploads/101M.txt", () => { });
		fs.unlink("../www/post/uploads/500M.txt", () => { });
		fs.unlink("../www/post/uploads/600M.txt", () => { });

		fs.unlink("../../php.ini", () => { });
	});

	beforeEach(async () => {
		await new Promise(r => setTimeout(() => r(), 10));
	});

	jest.setTimeout(60000);

	test(server1 + "/post2/ should return 200 because upload.php exists", async () => {
		const res = await request(server1)
			.post("/post2/")
			.set("X-Webserv-Test", "/post2/ should return 200 because upload.php exists");
		expect(res.status).toBe(200);
		expect(res.headers["content-type"]).toContain("text/html");
		expect(res.headers["content-length"]).not.toBe("");
		expect(res.text).not.toBe("");
	});

	test(server1 + "/post should return 301 with Location: " + server1 + "/post/", async () => {
		const res = await request(server1)
			.post("/post")
			.set("X-Webserv-Test", "/post should return 301 with Location: " + server1 + "/post/");
		expect(res.status).toBe(301);
		expect(res.headers["content-type"]).toContain("text/html");
		expect(res.headers["content-length"]).not.toBe("");
		expect(res.headers["location"]).toBe(server1 + "/post/");
	});

	test(server1 + "/post2 should return 301 with Location: " + server1 + "/post2/", async () => {
		const res = await request(server1)
			.post("/post2")
			.set("X-Webserv-Test", "/post2 should return 301 with Location: " + server1 + "/post2/");
		expect(res.status).toBe(301);
		expect(res.headers["content-type"]).toContain("text/html");
		expect(res.headers["content-length"]).not.toBe("");
		expect(res.headers["location"]).toBe(server1 + "/post2/");
	});

	test(server1 + "/post/ should return 403 because index.php not exist", async () => {
		const res = await request(server1)
			.post("/post/")
			.set("X-Webserv-Test", "/post/ should return 403 because index.php not exist");
		expect(res.status).toBe(403);
		expect(res.headers["content-type"]).toContain("text/html");
		expect(res.headers["content-length"]).not.toBe("");
	});

	test(server1 + "/post/upload.php should receive a 1W file", async () => {
		const res = await request(server1)
			.post("/post/upload.php")
			.set("X-Webserv-Test", "/post/upload.php should receive a 1W file")
			.attach("fileToUpload", "../www/files_to_upload/1W.txt");
		expect(res.status).toBe(200);
		expect(res.headers["content-type"]).toContain("text/html");
		expect(res.headers["content-length"]).not.toBe("");
		expect(fs.existsSync("../www/post/uploads/1W.txt")).toBeTruthy();
		expect(res.text).toContain("1W.txt OK");
	});

	test(server1 + "/post/upload.php should receive a 1K file", async () => {
		const res = await request(server1)
			.post("/post/upload.php")
			.set("X-Webserv-Test", "/post/upload.php should receive a 1K file")
			.attach("fileToUpload", "../www/files_to_upload/1K.txt");
		expect(res.status).toBe(200);
		expect(res.headers["content-type"]).toContain("text/html");
		expect(res.headers["content-length"]).not.toBe("");
		expect(fs.existsSync("../www/post/uploads/1K.txt")).toBeTruthy();
		expect(res.text).toContain("1K.txt OK");
	});

	test(server1 + "/post/upload.php should receive a 100K file", async () => {
		const res = await request(server1)
			.post("/post/upload.php")
			.set("X-Webserv-Test", "/post/upload.php should receive a 100K file")
			.attach("fileToUpload", "../www/files_to_upload/100K.txt");
		expect(res.status).toBe(200);
		expect(res.headers["content-type"]).toContain("text/html");
		expect(res.headers["content-length"]).not.toBe("");
		expect(fs.existsSync("../www/post/uploads/100K.txt")).toBeTruthy();
		expect(res.text).toContain("100K.txt OK");
	});

	test(server1 + "/post/upload.php should receive a 1M file", async () => {
		const res = await request(server1)
			.post("/post/upload.php")
			.set("X-Webserv-Test", "/post/upload.php should receive a 1M file")
			.attach("fileToUpload", "../www/files_to_upload/1M.txt");
		expect(res.status).toBe(200);
		expect(res.headers["content-type"]).toContain("text/html");
		expect(res.headers["content-length"]).not.toBe("");
		expect(fs.existsSync("../www/post/uploads/1M.txt")).toBeTruthy();
		expect(res.text).toContain("1M.txt OK");
	});

	test(server1 + "/post/upload.php should receive a 10M file", async () => {
		const res = await request(server1)
			.post("/post/upload.php")
			.set("X-Webserv-Test", "/post/upload.php should receive a 10M file")
			.attach("fileToUpload", "../www/files_to_upload/10M.txt");
		expect(res.status).toBe(200);
		expect(res.headers["content-type"]).toContain("text/html");
		expect(res.headers["content-length"]).not.toBe("");
		expect(fs.existsSync("../www/post/uploads/10M.txt")).toBeTruthy();
		expect(res.text).toContain("10M.txt OK");
	});

	// test(server1 + "/post/upload.php should receive a 100M file", async () => {
	// 	const res = await request(server1)
	// 		.post("/post/upload.php")
	// 		.set("X-Webserv-Test", "/post/upload.php should receive a 100M file")
	// 		.attach("fileToUpload", "../www/files_to_upload/100M.txt");
	// 	expect(res.status).toBe(200);
	// 	expect(res.headers["content-type"]).toContain("text/html");
	// 	expect(res.headers["content-length"]).not.toBe("");
	// 	expect(fs.existsSync("../www/post/uploads/100M.txt")).toBeTruthy();
	// 	expect(res.text).toContain("100M.txt OK");
	// });

	// test(server1 + "/post/upload.php should receive a 101M file", async () => {
	// 	const res = await request(server1)
	// 		.post("/post/upload.php")
	// 		.set("X-Webserv-Test", "/post/upload.php should receive a 101M file")
	// 		.attach("fileToUpload", "../www/files_to_upload/101M.txt");
	// 	expect(res.status).toBe(200);
	// 	expect(res.headers["content-type"]).toContain("text/html");
	// 	expect(res.headers["content-length"]).not.toBe("");
	// 	expect(fs.existsSync("../www/post/uploads/101M.txt")).toBeTruthy();
	// 	expect(res.text).toContain("101M.txt OK");
	// });

	// test(server1 + "/post/upload.php should receive a 500M file", async () => {
	// 	const res = await request(server1)
	// 		.post("/post/upload.php")
	// 		.set("X-Webserv-Test", "/post/upload.php should receive a 500M file")
	// 		.attach("fileToUpload", "../www/files_to_upload/500M.txt");
	// 	expect(res.status).toBe(200);
	// 	expect(res.headers["content-type"]).toContain("text/html");
	// 	expect(res.headers["content-length"]).not.toBe("");
	// 	expect(fs.existsSync("../www/post/uploads/500M.txt")).toBeTruthy();
	// 	expect(res.text).toContain("500M.txt OK");
	// });

	// test(server1 + "/post/upload.php should not receive a 600M file", async () => {
	// 	const res = await request(server1)
	// 		.post("/post/upload.php")
	// 		.set("X-Webserv-Test", "/post/upload.php should not receive a 600M file")
	// 		.attach("fileToUpload", "../www/files_to_upload/600M.txt");
	// 	expect(res.status).toBe(200);
	// 	expect(res.headers["content-type"]).toContain("text/html");
	// 	expect(res.headers["content-length"]).not.toBe("");
	// 	expect(fs.existsSync("../www/post/uploads/600M.txt")).toBeFalsy();
	// 	expect(res.text).toContain("ERROR");
	// });

	test(server1 + "/post/upload.php should not receive a 20M file", async () => {
		const res = await request(server1)
			.post("/post/upload.php")
			.set("X-Webserv-Test", "/post/upload.php should not receive a 20M file")
			.attach("fileToUpload", "../www/files_to_upload/20M.txt");
		expect(res.status).toBe(200);
		expect(res.headers["content-type"]).toContain("text/html");
		expect(res.headers["content-length"]).not.toBe("");
		expect(fs.existsSync("../www/post/uploads/20M.txt")).toBeFalsy();
		expect(res.text).toContain("ERROR");
	});

	test(server1 + "/query_string/index.php should return a valid query string", async () => {
		const res = await request(server1)
			.post("/query_string/index.php?value1=1&value2=2&value3=3&value4=4")
			.set("X-Webserv-Test", "/query_string/index.php should return valid query string");
		expect(res.status).toBe(200);
		expect(res.text).toBe("value1=1-value2=2-value3=3-value4=4-");
	});
});
