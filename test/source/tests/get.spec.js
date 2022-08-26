const { request, fs, LoremIpsum, body_math } = require('./utils.js');

let server1 = "http://127.0.0.1:3490";
let server_root = "../../server_root/";

describe("GET", () => {
	beforeEach(async () => {
		await new Promise(r => setTimeout(() => r(), 10));
	});

	test(server1 + " should return 200", async () => {
		const res = await request(server1)
			.get("")
			.set("X-Webserv-Test", " should return 200");
		expect(res.status).toBe(200);
		expect(res.headers["content-type"]).toContain("text/html");
		expect(res.headers["content-length"]).not.toBe("");
		expect(body_math(res.text, server_root + "index.html")).toBeTruthy();
	});

	test(server1 + "/ should return 200", async () => {
		const res = await request(server1)
			.get("/")
			.set("X-Webserv-Test", "/ should return 200");
		expect(res.status).toBe(200);
		expect(res.headers["content-type"]).toContain("text/html");
		expect(res.headers["content-length"]).not.toBe("");
		expect(body_math(res.text, server_root + "index.html")).toBeTruthy();
	});

	test(server1 + "/index.html should return 200", async () => {
		const res = await request(server1)
			.get("/index.html")
			.set("X-Webserv-Test", "/index.html should return 200");
		expect(res.status).toBe(200);
		expect(res.headers["content-type"]).toContain("text/html");
		expect(res.headers["content-length"]).not.toBe("");
		expect(body_math(res.text, server_root + "index.html")).toBeTruthy();
	});

	test(server1 + "/index.php should return 200", async () => {
		const res = await request(server1)
			.get("/index.php")
			.set("X-Webserv-Test", "/index.php should return 200");
		expect(res.status).toBe(200);
		expect(res.headers["content-type"]).toContain("text/html");
		expect(res.headers["content-length"]).not.toBe("");
		expect(res.text).toContain("Hello, World PHP!");
	});

	test(server1 + "/index.py should return 200", async () => {
		const res = await request(server1)
			.get("/index.py")
			.set("X-Webserv-Test", "/index.py should return 200");
		expect(res.status).toBe(200);
		expect(res.headers["content-type"]).toContain("text/html");
		expect(res.headers["content-length"]).not.toBe("");
		expect(res.text).toContain("Hello, World PYTHON!");
	});

	test(server1 + "/paulo.html should return 200", async () => {
		const res = await request(server1)
			.get("/paulo.html")
			.set("X-Webserv-Test", "/paulo.html should return 200");
		expect(res.status).toBe(200);
		expect(res.headers["content-type"]).toContain("text/html");
		expect(res.headers["content-length"]).not.toBe("");
		expect(body_math(res.text, server_root + "paulo.html")).toBeTruthy();
	});

	test(server1 + "/joao.html should return 200", async () => {
		const res = await request(server1)
			.get("/joao.html")
			.set("X-Webserv-Test", "/joao.html should return 200");
		expect(res.status).toBe(200);
		expect(res.headers["content-type"]).toContain("text/html");
		expect(res.headers["content-length"]).not.toBe("");
		expect(body_math(res.text, server_root + "joao.html")).toBeTruthy();
	});

	test(server1 + "/dinocat.jpg should return 200", async () => {
		const res = await request(server1)
			.get("/dinocat.jpg")
			.set("X-Webserv-Test", "/dinocat.jpg should return 200");
		expect(res.status).toBe(200);
		expect(res.headers["content-type"]).toContain("image/jpg");
		expect(res.headers["content-length"]).not.toBe("");
		expect(body_math(res.body, server_root + "dinocat.jpg")).toBeTruthy();
	});

	test(server1 + "/index.html/ should return 404", async () => {
		const res = await request(server1)
			.get("/index.html/")
			.set("X-Webserv-Test", "/index.html/ should return 404");
		expect(res.status).toBe(404);
		expect(res.headers["content-type"]).toContain("text/html");
		expect(res.headers["content-length"]).not.toBe("");
		expect(body_math(res.text, server_root + "custom_404.html")).toBeTruthy();
	});

	test(server1 + "/index.php/ should return 404", async () => {
		const res = await request(server1)
			.get("/index.php/")
			.set("X-Webserv-Test", "/index.php/ should return 404");
		expect(res.status).toBe(404);
		expect(res.headers["content-type"]).toContain("text/html");
		expect(res.headers["content-length"]).not.toBe("");
		expect(body_math(res.text, server_root + "custom_404.html")).toBeTruthy();
	});

	test(server1 + "/not_exists.html should return 404", async () => {
		const res = await request(server1)
			.get("/not_exists.html")
			.set("X-Webserv-Test", "/not_exists.html should return 404");
		expect(res.status).toBe(404);
		expect(res.headers["content-type"]).toContain("text/html");
		expect(res.headers["content-length"]).not.toBe("");
		expect(body_math(res.text, server_root + "custom_404.html")).toBeTruthy();
	});

	test(server1 + "/not_exists should return 404", async () => {
		const res = await request(server1)
			.get("/not_exists")
			.set("X-Webserv-Test", "/not_exists should return 404");
		expect(res.status).toBe(404);
		expect(res.headers["content-type"]).toContain("text/html");
		expect(res.headers["content-length"]).not.toBe("");
		expect(body_math(res.text, server_root + "custom_404.html")).toBeTruthy();
	});

	test(server1 + "/post should return 405", async () => {
		const res = await request(server1)
			.get("/post")
			.set("X-Webserv-Test", "/post should return 405");
		expect(res.status).toBe(405);
		expect(body_math(res.text, server_root + "custom_405.html")).toBeTruthy();
	});

	test(server1 + "/post/ should return 405", async () => {
		const res = await request(server1)
			.get("/post/")
			.set("X-Webserv-Test", "/post/ should return 405");
		expect(res.status).toBe(405);
		expect(body_math(res.text, server_root + "custom_405.html")).toBeTruthy();
	});

	test(server1 + "/put should return 405", async () => {
		const res = await request(server1)
			.get("/put")
			.set("X-Webserv-Test", "/put should return 405");
		expect(res.status).toBe(405);
		expect(body_math(res.text, server_root + "custom_405.html")).toBeTruthy();
	});

	test(server1 + "/put/ should return 405", async () => {
		const res = await request(server1)
			.get("/put/")
			.set("X-Webserv-Test", "/put/ should return 405");
		expect(res.status).toBe(405);
		expect(body_math(res.text, server_root + "custom_405.html")).toBeTruthy();
	});

	test(server1 + "/delete should return 405", async () => {
		const res = await request(server1)
			.get("/delete")
			.set("X-Webserv-Test", "/delete should return 405");
		expect(res.status).toBe(405);
		expect(body_math(res.text, server_root + "custom_405.html")).toBeTruthy();
	});

	test(server1 + "/delete/ should return 405", async () => {
		const res = await request(server1)
			.get("/delete/")
			.set("X-Webserv-Test", "/delete/ should return 405");
		expect(res.status).toBe(405);
		expect(body_math(res.text, server_root + "custom_405.html")).toBeTruthy();
	});

	test(server1 + "/query_string/index.php should return a valid query string", async () => {
		const res = await request(server1)
			.get("/query_string/index.php?value1=1&value2=2&value3=3&value4=4")
			.set("X-Webserv-Test", "/query_string/index.php should return valid query string");
		expect(res.status).toBe(200);
		expect(res.text).toBe("value1=1-value2=2-value3=3-value4=4-");
	});
});
