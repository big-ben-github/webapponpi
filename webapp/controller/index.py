import functools
from flask import flash, g, redirect, render_template, request, session, url_for, jsonify
from werkzeug.security import check_password_hash, generate_password_hash
from webapp.model.db import get_db
from webapp.devices import devices
from . import indexblueprint


def login_required(view):
    """View decorator that redirects anonymous users to the login page."""
    @functools.wraps(view)
    def wrapped_view(**kwargs):
        if g.user is None:
            return  # redirect(url_for('index.login'))
        return view(**kwargs)
    return wrapped_view


@indexblueprint.before_app_request
def load_logged_in_user():
    user_id = session.get('user_id')

    if user_id is None:
        g.user = None
    else:
        g.user = get_db().execute(
            'SELECT * FROM user WHERE id = ?', (user_id,)
        ).fetchone()


@indexblueprint.route('/register', methods=['POST'])
def register():
    if request.method == 'POST':
        username = request.json['username']
        password = request.json['password']
        print('registerusername:', username, 'registerpassword:', password)

        db = get_db()
        error = None

        if db.execute(
            'SELECT id FROM user WHERE username = ?', (username,)
        ).fetchone() is not None:
            error = 'Register failed: Username ({}) used.'.format(username)
        elif not username:
            error = 'Register failed: Username Null.'
        elif not password:
            error = 'Register failed: Password Null.'

        if error is None:
            db.execute(
                'INSERT INTO user (username, password) VALUES (?, ?)',
                (username, generate_password_hash(password))
            )
            db.commit()
            return jsonify({'result': 'OK'})

        return jsonify({'result': error})
    return jsonify({'result': 'Register failed: Wrong request.'})


@indexblueprint.route('/login', methods=['POST'])
def login():
    if request.method == 'POST':
        username = request.json['username']
        password = request.json['password']
        print('loginusername:', username, 'loginpassword:', password)

        error = None
        user = get_db().execute(
            'SELECT * FROM user WHERE username = ?', (username,)
        ).fetchone()
        if user is None:
            error = 'Incorrect username.'
        elif not check_password_hash(user['password'], password):
            error = 'Incorrect password.'
        elif g.user is not None:
            error = 'You have logined.'

        if error is None:
            session.clear()
            session['user_id'] = user['id']
            return jsonify({'result': 'OK'})

        return jsonify({'result': 'Login failed: ' + error})

    return jsonify({'result': 'Login failed: Wrong request.'})


@indexblueprint.route('/logout')
@login_required
def logout():
    session.clear()
    return redirect(url_for('index.index'))


@indexblueprint.route('/')
def index():
    return render_template('index.html')


@indexblueprint.route('/lampcontrol', methods=['POST'])
@login_required
def lampcontrol():
    if request.method == 'POST':
        lamp_op = request.json['operation']
        print('lamp mode is to be %s' % lamp_op)
        if devices.lampcontrol(lamp_op) is True:
            return jsonify({'result': 'ok'})
        else:
            return jsonify({'result': 'fail'})
    return jsonify({'result': 'fail'})
