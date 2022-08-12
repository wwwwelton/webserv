const { request, fs, LoremIpsum, body_math } = require('./utils.js');

let server1 = "http://127.0.0.1:3490";
let server_root = "../../server_root/";

describe("DELETE", () => {
	beforeAll(async () => {
		if (!fs.existsSync("../www/delete")) {
			fs.mkdirSync("../www/delete");
		}
		const lorem = new LoremIpsum();
		fs.writeFileSync("../www/delete/file1.txt", lorem.generateWords(400));
		fs.writeFileSync("../www/delete/file2.txt", lorem.generateParagraphs(400));
		fs.writeFileSync("../www/delete/cannot_delete", lorem.generateParagraphs(400));
		fs.chmod("../www/delete/cannot_delete", 0o000, () => { });
	});

	afterAll(async () => {
		fs.unlink("../www/delete/file1.txt", () => { });
		fs.unlink("../www/delete/file2.txt", () => { });
		fs.chmod("../www/delete/cannot_delete", 0o777, () => { });
		fs.unlink("../www/delete/cannot_delete", () => { });
		// fs.rmdir("../www/delete", { recursive: true }, () => { });
	});

	beforeEach(async () => {
		await new Promise(r => setTimeout(() => r(), 10));
	});

	test(server1 + "/delete should return 204", async () => {
		const res = await request(server1)
			.delete("/delete")
			.set("X-Webserv-Test", "/delete should return 204");
		expect(res.status).toBe(204);
		expect(res.text).toBe("");
	});

	test(server1 + "/delete/ should return 204", async () => {
		const res = await request(server1)
			.delete("/delete/")
			.set("X-Webserv-Test", "/delete/ should return 204");
		expect(res.status).toBe(204);
		expect(res.text).toBe("");
	});

	test(server1 + "/delete/file1.txt should delete file1.txt", async () => {
		const res = await request(server1)
			.delete("/delete/file1.txt")
			.set("X-Webserv-Test", "/delete/file1.txt");
		expect(res.status).toBe(204);
		expect(fs.existsSync("../www/delete/file1.txt")).toBeFalsy();
		expect(res.text).toBe("");
	});

	test(server1 + "/delete/file2.txt should delete file2.txt", async () => {
		const res = await request(server1)
			.delete("/delete/file2.txt")
			.set("X-Webserv-Test", "/delete/file2.txt should delete file2.txt");
		expect(res.status).toBe(204);
		expect(fs.existsSync("../www/delete/file2.txt")).toBeFalsy();
		expect(res.text).toBe("");
	});

	test(server1 + "/delete/not_exists should return 404", async () => {
		const res = await request(server1)
			.delete("/delete/not_exists")
			.set("X-Webserv-Test", "/delete/not_exists");
		expect(res.status).toBe(404);
		expect(fs.existsSync("../www/delete/not_exists")).toBeFalsy();
		expect(body_math(res.text, server_root + "custom_404.html")).toBeTruthy();
	});

	test(server1 + "/delete/cannot_delete should return 401", async () => {
		const res = await request(server1)
			.delete("/delete/cannot_delete")
			.set("X-Webserv-Test", "/delete/cannot_delete should return 401");
		expect(res.status).toBe(401);
		expect(fs.existsSync("../www/delete/cannot_delete")).toBeTruthy();
		expect(res.text).toContain("401 Unauthorized");
	});

	test(server1 + "/ should return 405", async () => {
		const res = await request(server1)
			.delete("/")
			.set("X-Webserv-Test", "/ should return 405");
		expect(res.status).toBe(405);
		expect(body_math(res.text, server_root + "custom_405.html")).toBeTruthy();
	});

	test(server1 + "/post should return 405", async () => {
		const res = await request(server1)
			.delete("/post")
			.set("X-Webserv-Test", "/post should return 405");
		expect(res.status).toBe(405);
		expect(body_math(res.text, server_root + "custom_405.html")).toBeTruthy();
	});

	test(server1 + "/post/ should return 405", async () => {
		const res = await request(server1)
			.delete("/post/")
			.set("X-Webserv-Test", "/post/ should return 405");
		expect(res.status).toBe(405);
		expect(body_math(res.text, server_root + "custom_405.html")).toBeTruthy();
	});

	test(server1 + "/put should return 405", async () => {
		const res = await request(server1)
			.delete("/put")
			.set("X-Webserv-Test", "/put should return 405");
		expect(res.status).toBe(405);
		expect(body_math(res.text, server_root + "custom_405.html")).toBeTruthy();
	});

	test(server1 + "/put/ should return 405", async () => {
		const res = await request(server1)
			.delete("/put/")
			.set("X-Webserv-Test", "/put/ should return 405");
		expect(res.status).toBe(405);
		expect(body_math(res.text, server_root + "custom_405.html")).toBeTruthy();
	});
});
