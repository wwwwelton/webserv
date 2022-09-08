const { request, fs, LoremIpsum, body_math, file_toString, create_file } = require('./utils.js');

let server1 = "http://127.0.0.1:3490";
let server_root = "../../server_root/";

describe("CGI", () => {
	describe("GET", () => {
		beforeEach(async () => {
			await new Promise(r => setTimeout(() => r(), 10));
		});

		test(server1 + "/cgi/hello.php should return Hello, World!", async () => {
			const res = await request(server1)
				.get("/cgi/hello.php")
				.set("X-Webserv-Test", "/cgi/hello.php should return Hello, World!");
			expect(res.status).toBe(200);
			expect(res.headers["content-type"]).toContain("text/html");
			expect(res.headers["content-length"]).not.toBe("");
			expect(res.text).toContain("Hello, World!");
		});

		test(server1 + "/cgi/hello.py should return Hello, World!", async () => {
			const res = await request(server1)
				.get("/cgi/hello.py")
				.set("X-Webserv-Test", "/cgi/hello.py should return Hello, World!");
			expect(res.status).toBe(200);
			expect(res.headers["content-type"]).toContain("text/html");
			expect(res.headers["content-length"]).not.toBe("");
			expect(res.text).toContain("Hello, World!");
		});
	});
});
