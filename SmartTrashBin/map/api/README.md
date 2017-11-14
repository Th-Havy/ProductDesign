# Smart Trash Bin api

We hereby implement a RESTful API for sending data from the smart trash bins to
the web server. A RESTful (REpresentational State Transfer) API is a way to
provide an interface between an application and a database using urls. Depending
on the type of HTTP requests that we make we can provide different behaviors.

In our case, we have several smart trash bins each connected to a Wi-Fi network,
that need to send their fullness level to a central web application.

## Description of the RESTful API

### Fetch the data of all trash bins:

```
GET website.com/api/trashbin/
```

### Fetch the data of a particular trashbin

```
GET website.com/api/trashbin/[id_of_the_trash_bin]
```

### Add a new trash bin
```
POST 'Content-Type: application/json' '{"latitude
":[value],"longitude":[value],"floor":[value],"building":"[value]"}' http://127.0.0.1:8000/api/trashbin/
(curl -i -X POST -H 'Content-Type: application/json' -d '{"latitude
":13.0,"longitude":54.0,"floor":5,"building":"BM"}' http://127.0.0.1:8000/api/trashbin/)
```

### Delete a particular trash bin
```
DELETE website.com/api/trashbin/[id_of_the_trash_bin]
```
