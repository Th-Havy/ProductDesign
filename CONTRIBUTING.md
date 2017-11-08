# Contributing to the project

## Coding conventions

class: ClassName
method/function/variables: methodOrVariableName

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
source bin/activate
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
