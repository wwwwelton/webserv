const request = require('supertest');

let server;
let route;

describe("GET", () => {
	server = "http://127.0.0.1:3490";
	route = "/";
	test(server + route + " should return 200", async () => {
		const response = await request(server)
			.get(route);
		expect(response.status).toBe(200);
		//expect(response.headers["content-type"]).toContain("text/html");
	});

	server = "http://127.0.0.1:3490";
	route = "";
	test(server + route + " should return 200", async () => {
		const response = await request(server)
			.get(route)
		expect(response.status).toBe(200);
		//expect(response.headers["content-type"]).toContain("text/html");
		// console.log(response.text);
	});

	server = "http://127.0.0.1:3490";
	route = "/";
	test(server + route + " should return 200", async () => {
		const response = await request(server)
			.head(route)
		expect(response.status).toBe(405);
	});
});
