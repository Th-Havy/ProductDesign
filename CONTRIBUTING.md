# Contributing to the project

Django is controlled mostly through command line, so we recommand using an appropriate environment (i.e. Linux or Mac with bash script).

We use GitLab for version control and code sharing. The whole point of GitLab is to have all the code in one place, on an online server.
This way, all contributors make a local copy of the project, modify their copy, and once they are fine with their modifications they apply it to the shared copy on the server.

## Start working on the app

Open a terminal

Copy the gitlab repository (install git if necessary), you should have a GitLab account with right to access this project.
```
git clone https://gitlab.com/Th-Havy/ProductDesign.git
# Enter your username and password as prompted
```

## Work on the app

Move to the app directory.

Make sure you have the latest version of the code:
```
git pull
# Enter your username and password as prompted
```

Start the virtual environment:
```
source virt_env/bin/activate
```

In case you want to deactivate it, simply type:
```
deactivate
```

Now, make some modifications to the code: add files, modify files, remove files, ...

Whenever you add new files:
```
git add file
```

After a few modifications:
```
git commit -a -m "Small description of the modifications"
```

Finally to apply your modifications to the server copy:
```
git push -u origin master
```

## Coding conventions

Try to follow the [PEP8](https://www.python.org/dev/peps/pep-0008/#maximum-line-length) and PEP257(https://www.python.org/dev/peps/pep-0257/) recommandations.

* **class**: ClassName
* **method/function/variables**: methodOrVariableName
* **constant**: CONSTANT

## How to initially create the project

Create a project folder:
```
mkdir ProductDesign
```

Setup a virtual environment to avoid version conflict and increase sustainability:
```
# create a folder for the virtual environment:
mkdir virt_env
# create virtual environment with python 3:
virtualenv virt_env -p /usr/bin/python3
# start the virtual environment:
source virt_env/bin/activate
# install django:
pip install django
```

Create a django project
```
# create a Django project:
django-admin startproject SmartTrashBin
# move to project directory:
cd SmartTrashBin
# create a Django app:
python manage.py startapp map
```

Setup a git repository for version control and  code sharing:
```
git init
git remote add origin https://gitlab.com/Th-Havy/ProductDesign.git
git add .
git commit -m "Initial commit"
git push -u origin master
```
