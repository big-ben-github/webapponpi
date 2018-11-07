# Raspberry-Pi webapp project

This is a webapp running on Raspberry-Pi, you can access the web page from Pi and control the LED on the Pi to turn it on or off. This is a 'helloworld' project for now.

### How to run

The detailed process are as follows:

1. clone the repository and create the virtual environment with venv or virtualenv, then activate it:
'''
   python -m venv venv/ && . venv/bin/activate
'''
2. install flask package:
'''
   pip install flask
'''
3. run the app in different ways:
   * flask way
'''
   export FLASK_APP=webapp
'''
   * with manage.py
'''
   python manage.py
'''
   * with VSCode, open it and run

### package the source code

package the project into module with python so it will be to deploy the package:
'''
python3 -m pip install --user --upgrade setuptools wheel
python3 setup.py sdist bdist_wheel
'''
then you get the xxx.whl file within dist/ and you can install it everywhere within virtual environment:
'''
pip install xxx.whl
'''

### run on the Raspberry-Pi

