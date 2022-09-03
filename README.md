# Webserv
This project is about writing a [HTTP server](https://en.wikipedia.org/wiki/Web_server), your own version of [Nginx](https://www.nginx.com/), [Apache](https://apache.org/) or [Lighttpd](https://www.lighttpd.net/).

### What is Webserv?
Webserv is a group project at [42](42sp.org.br) which requires us to build a [HTTP server](https://en.wikipedia.org/wiki/Web_server) using [I/O Multiplexing](https://notes.shichao.io/unp/ch6/) built on top of a [event loop](https://en.wikipedia.org/wiki/Event_loop#:~:text=In%20computer%20science%2C%20the%20event,or%20messages%20in%20a%20program.).

In the mandatory part, we start the server by reading a [configuration file](https://docs.nginx.com/nginx/admin-guide/basic-functionality/managing-configuration-files/) that can be based on [Nginx](https://www.nginx.com/), containing information like hostname(ip/port), servername, index file, directory listing, URL redirect, location(vhost path) and some other settings. It is forbidden to use [fork](https://man7.org/linux/man-pages/man2/fork.2.html) to create new [non-CGI](https://en.wikipedia.org/wiki/Common_Gateway_Interface) processes or spawn [threads](https://en.wikipedia.org/wiki/Thread_(computing)) (threads are not available in [C++98](https://cplusplus.com/reference/)).

For the [I/O Multiplexing](https://notes.shichao.io/unp/ch6/), ie informing the [Kernel](https://en.wikipedia.org/wiki/Kernel_(operating_system)) that we want to be notified if there is a change in a previously specified list of [file descriptors](https://en.wikipedia.org/wiki/File_descriptor); we can use [select](https://man7.org/linux/man-pages/man2/select.2.html), [poll](https://man7.org/linux/man-pages/man2/poll.2.html), [epoll](https://man7.org/linux/man-pages/man7/epoll.7.html) or [kqueue](https://www.freebsd.org/cgi/man.cgi?query=kqueue&sektion=2). All socket reads or writes must pass through [poll()](https://man7.org/linux/man-pages/man2/poll.2.html) or equivalent, ensuring high server availability, respecting the [event loop](https://en.wikipedia.org/wiki/Event_loop#:~:text=In%20computer%20science%2C%20the%20event,or%20messages%20in%20a%20program.).

[HTTP requests](https://en.wikipedia.org/wiki/Hypertext_Transfer_Protocol) are read, parsed, processed and sent to a [CGI](https://en.wikipedia.org/wiki/Common_Gateway_Interface) if configured in the [configuration file](https://docs.nginx.com/nginx/admin-guide/basic-functionality/managing-configuration-files/) and finally a [HTTP response](https://en.wikipedia.org/wiki/Hypertext_Transfer_Protocol) is returned to the client with [headers](https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers), [body](https://en.wikipedia.org/wiki/HTTP_message_body) and correct [HTTP status code](https://en.wikipedia.org/wiki/List_of_HTTP_status_codes).

For the bonus we have to support [cookies](https://en.wikipedia.org/wiki/HTTP_cookie), [session](https://en.wikipedia.org/wiki/Hypertext_Transfer_Protocol#HTTP_session) management and handle multiple [CGI](https://en.wikipedia.org/wiki/Common_Gateway_Interface).

### Availability
![alt text](./images/preview_siege_1.gif)

### Badge
<img src="./images/webservm.png" width="150" height="150"/>

#### Skills
- Network & system administration
- Unix
- Rigor
- Object-oriented programming

#### My grade
<img src="./images/score2.png" width="150" height="150"/>


## Getting started
**Follow the steps below**
```bash
# Clone the project and access the folder
git clone https://github.com/wwwwelton/webserv && cd webserv/

# Run make so you can build the program and install dependencies
make && make dep

# Run the application with a config file
./webserv default.conf

# Access the default website using the URL below
http://localhost:3490

# Clean output objects with
make fclean

# Well done!
```

## Updating

The project is regularly updated with bug fixes and code optimization.

## 📝 License

This project is licensed under the GNU General Public License v3.0 - see the [LICENSE](LICENSE) file for details.

---

Made by:
João Rodriguez 👋 [See my github](https://github.com/VLN37)<br/>
Paulo Santana 👋 [See my linkedin](https://www.linkedin.com/in/paulostn/)<br/>
Welton Leite 👋 [See my linkedin](https://www.linkedin.com/in/welton-leite-b3492985/)<br/>
