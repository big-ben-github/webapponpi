# Raspberry-Pi webapp project

This is a webapp running on Raspberry-Pi, you can access the web page from Pi and control the LED on the Pi to turn it on or off. This is a 'helloworld' project for now.

### How to run

The detailed process are as follows:

1. clone the repository and create the virtual environment with venv or virtualenv, then activate it:
```
python -m venv venv/ && . venv/bin/activate
```
2. install flask package:
```
pip install flask
```
3. run the app in different ways:
   * flask way
   ```
   export FLASK_APP=webapp
   ```
   * with manage.py
   ```
   python manage.py
   ```
   * with VSCode, open it and run

### package the source code

package the project into module with python so it will be to deploy the package:
```
python3 -m pip install --user --upgrade setuptools wheel
python3 setup.py sdist bdist_wheel
```
then you get the xxx.whl file within dist/ and you can install it everywhere within virtual environment:
```
pip install xxx.whl
```

### run on the Raspberry-Pi

1. create the virtual environment and activate it, put the source code or install the .whl file on raspberry-pi;

2. install the uWSGI as web server within virtual environment and configure it;
```
[uwsgi]
wsgi-file = /path/to/yourapplication
virtualenv = venv/
module = webapp
master=true
socket = socket/uwsgi.sock
chmod-socket = 666
callable = app
processes = 4
threads = 2
```

3. install the Nginx as Reverse proxy server and configure it;
```
server {
    listen 8080;
    server_name localhost;
    charset utf-8;
    client_max_body_size 75M;
    location / {
        include uwsgi_params;
        uwsgi_pass unix:/path/to/uwsgi.sock;
    }
}
```


4. Connect raspberry-pi and LED with pin-7 and pin-GND(WiringPi coding form);

5. run the webserver(uncomment the code in webapp__init__.py) and configure the raspberry-pi network with static ip;
```
app = create_app()
```

6. input the ip address on your phone and turn the LED on or off :wink:
