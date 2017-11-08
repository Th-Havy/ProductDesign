# Smart Trash Bin - Web App

This is a web app as part of the project for the product design course (Yves Bellouard at EPFL).
This web app shall receive data from "Smart" trash bins, process it and provide some visualisations and predictions, in order to help EPFL cleaners.
We use Django for the back-end (Server side of the app), which is a python framework for developping web apps, and Bootstrap for the front-end (client side), which is good for handling screens with different formats (responsive design).

## Getting Started

### Installing

Go to the url of the web app and enjoy.

## Deployment

When deploying the app into production:
* Don't forget to disable the DEBUG option in the Django settings
* Change to a more scallable database if necessary (for ex: PostgreSQL)
* Use an appropriate server (for ex: Apache)

## Built With

* [Django](https://www.djangoproject.com/) - Back-end web framework
* [Bootstrap](http://getbootstrap.com/) - front-end toolkit

## Contributing

Please read [CONTRIBUTING.md](https://gist.github.com/PurpleBooth/b24679402957c63ec426) for details on our code of conduct, how the app works and how to get started.

## Versioning

We use [Gitlab](https://gitlab.com/) for version control and code sharing.

## Authors

* **Thomas Havy**
* **Paul Donzier**

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Epic thanks to my man Marc Brossard for letting me know about the KNX bus and NIBT norms.
* Epic thanks to the coach Janina Loffler for her advices.
