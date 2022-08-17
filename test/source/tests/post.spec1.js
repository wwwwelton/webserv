const { request, fs, LoremIpsum, body_math } = require('./utils.js');

let server1 = "http://127.0.0.1:3490";
let server_root = "../../server_root/";

describe("POST", () => {
	beforeEach(async () => {
		await new Promise(r => setTimeout(() => r(), 10));
	});

	test(server1 + "/post should return 200", async () => {
		const res = await request(server1)
			.post("/post")
			.set("X-Webserv-Test", "/post should return 200");
		expect(res.status).toBe(200);
		// expect(res.headers["content-type"]).toContain("text/html");
		// expect(res.headers["content-length"]).not.toBe("");
		// expect(body_math(res.text, server_root + "index.html")).toBeTruthy();
	});

	test(server1 + "/post/ should return 200", async () => {
		const res = await request(server1)
			.post("/post/")
			.set("X-Webserv-Test", "/post/ should return 200");
		expect(res.status).toBe(200);
		// expect(res.headers["content-type"]).toContain("text/html");
		// expect(res.headers["content-length"]).not.toBe("");
		// expect(body_math(res.text, server_root + "index.html")).toBeTruthy();
	});

	test(server1 + "/sito should return 200", async () => {
		const res = await request(server1)
			.post("/sito")
			.set("X-Webserv-Test", "/sito should return 200");
		expect(res.status).toBe(200);
		// expect(res.headers["content-type"]).toContain("text/html");
		// expect(res.headers["content-length"]).not.toBe("");
		// expect(body_math(res.text, server_root + "index.html")).toBeTruthy();
	});

	test(server1 + "/sito/ should return 200", async () => {
		const res = await request(server1)
			.post("/sito/")
			.set("X-Webserv-Test", "/sito/ should return 200");
		expect(res.status).toBe(200);
		// expect(res.headers["content-type"]).toContain("text/html");
		// expect(res.headers["content-length"]).not.toBe("");
		// expect(body_math(res.text, server_root + "index.html")).toBeTruthy();
	});

	test(server1 + "/post/upload.php should receive a 1K file", async () => {
		const res = await request(server1)
			.post("/post/upload.php")
			.set("X-Webserv-Test", "/post/upload.php should receive a 1K file")
			.attach("fileToUpload", "../www/files_to_upload/1K.txt");
		expect(res.status).toBe(200);
		// expect(res.headers["content-type"]).toContain("text/html");
		// expect(res.headers["content-length"]).not.toBe("");
		// expect(body_math(res.text, server_root + "index.html")).toBeTruthy();
	});

	test(server1 + "/post/upload.php should receive a 1M file", async () => {
		const res = await request(server1)
			.post("/post/upload.php")
			.set("X-Webserv-Test", "/post/upload.php should receive a 1M file")
			.attach("fileToUpload", "../www/files_to_upload/1M.txt");
		expect(res.status).toBe(200);
		// expect(res.headers["content-type"]).toContain("text/html");
		// expect(res.headers["content-length"]).not.toBe("");
		// expect(body_math(res.text, server_root + "index.html")).toBeTruthy();
	});

	// test(server1 + "/post/upload.php should receive a 10M file", async () => {
	// 	const res = await request(server1)
	// 		.post("/post/upload.php")
	// 		.attach("fileToUpload", "../www/files_to_upload/10M.txt");
	// 	expect(res.status).toBe(200);
	// 	// expect(res.headers["content-type"]).toContain("text/html");
	// 	// expect(res.headers["content-length"]).not.toBe("");
	// 	// expect(body_math(res.text, server_root + "index.html")).toBeTruthy();
	// });
});
