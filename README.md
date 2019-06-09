# Raspberry-Pi webapp project

This is a webapp running on Raspberry-Pi, you can access the web page from Pi and control the LED  on the Pi to turn it on or off or control or other devices do more things. This is a 'helloworld' or prototype project for now.

##### Note: All is with python3 and pip3.

### How to run in a development environment

The detailed process are as follows:

1. clone the repository and create the virtual environment with venv or virtualenv, then activate it:

```
python3 -m venv venv/ && . venv/bin/activate
```

2. install flask package:

```
pip install flask
```

3. run the app in different ways:

   - flask way

   ```
   export FLASK_APP=webapp
   ```

   - with manage.py

   ```
   python manage.py
   ```

   - with VSCode, open it and run

### Package the source code

package the project into module with python so it will be to deploy the package:

```
python3 -m pip install --user --upgrade setuptools wheel
python3 setup.py sdist bdist_wheel
```

then you get the xxx.whl and xxx.tar.gz file within dist/ and you can install it everywhere within virtual environment:

```
pip install xxx.whl
#or
tar -xzvf xxx.tar.gz
```

### Run on the Raspberry-Pi

1. create the virtual environment and activate it, put the source code or install the .whl module or unzip tar file on your raspberry-pi;
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

##### Note: you need a empty file "uwsgi.sock" (touch uwsgi.sock), and put it somewhere you can find.
##### Note: you need uncomment the code in "\__init__.py\__" within webapp/ before you run uwsgi server, open it and you can see it. 
you can put it in a "flsk_uwsgi.ini" (or other file names) file, then start the uwsgi server;

```
uwsgi -i flask_uwsgi.ini
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
put the configure at /etc/nginx/conf.d/ in a configuration file with name "flask_nginx.conf" and start the nginx;

```
sudo service nginx start
```

4. (TBD)Connect raspberry-pi and LED with pin-7 and pin-GND(WiringPi coding form);
copy adapter/ recursively to corresponding directory, you run these commands in your cmd:

```
make
./adapter
```
then you can see the APAPTER module is running.

5. (Optional)run the webserver and configure the raspberry-pi network with static ip at /etc/dhcpcd.conf like:

```
interface wlan0

static ip_address=192.168.0.200/24
static routers=192.168.0.1
static domain_name_servers=192.168.0.1
```

6. input the ip address and port on your phone browser and turn the LED on or off :wink:
