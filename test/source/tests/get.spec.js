const request = require('supertest');

let server1 = "http://127.0.0.1:3490";

describe("GET", () => {
	// beforeEach(async () => {
	// 	await new Promise(r => setTimeout(() => r(), 1000));
	// });

	test(server1 + " should return 200", async () => {
		const response = await request(server1)
			.get("");
		expect(response.status).toBe(200);
	});

	test(server1 + "/ should return 200", async () => {
		const response = await request(server1)
			.get("/");
		expect(response.status).toBe(200);
	});

	test(server1 + "/index.html should return 200", async () => {
		const response = await request(server1)
			.get("/index.html");
		expect(response.status).toBe(200);
	});

	test(server1 + "/paulo.html should return 200", async () => {
		const response = await request(server1)
			.get("/paulo.html")
		expect(response.status).toBe(200);
	});

	test(server1 + "/joao.html should return 200", async () => {
		const response = await request(server1)
			.get("/joao.html")
		expect(response.status).toBe(200);
	});

	test(server1 + "/dinocat.jpg should return 200", async () => {
		const response = await request(server1)
			.get("/dinocat.jpg")
		expect(response.status).toBe(200);
	});

	test(server1 + "/index.html/ should return 404", async () => {
		const response = await request(server1)
			.get("/index.html/");
		expect(response.status).toBe(404);
	});

	test(server1 + "/not_exists.html should return 404", async () => {
		const response = await request(server1)
			.get("/not_exists.html");
		expect(response.status).toBe(404);
	});

	test(server1 + "/not_exists should return 404", async () => {
		const response = await request(server1)
			.get("/not_exists");
		expect(response.status).toBe(404);
	});
});
