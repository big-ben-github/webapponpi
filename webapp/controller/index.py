import functools
from flask import flash, g, redirect, render_template, request, session, url_for, jsonify
from werkzeug.security import check_password_hash, generate_password_hash
from webapp.model.db import get_db
from webapp.raspberry import led
from . import indexblueprint


def login_required(view):
    """View decorator that redirects anonymous users to the login page."""
    @functools.wraps(view)
    def wrapped_view(**kwargs):
        if g.user is None:
            return redirect(url_for('index.login'))
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


@indexblueprint.route('/register', methods=('GET', 'POST'))
def register():
    if request.method == 'POST':
        username = request.form['username']
        password = request.form['password']
        db = get_db()
        error = None

        if not username:
            error = 'Username is required.'
        elif not password:
            error = 'Password is required.'
        elif db.execute(
            'SELECT id FROM user WHERE username = ?', (username,)
        ).fetchone() is not None:
            error = 'User {} is already registered.'.format(username)

        if error is None:
            db.execute(
                'INSERT INTO user (username, password) VALUES (?, ?)',
                (username, generate_password_hash(password))
            )
            db.commit()
            return redirect(url_for('index.login'))

        flash(error)

    return render_template('register.html')


@indexblueprint.route('/login', methods=('GET', 'POST'))
def login():
    if request.method == 'POST':
        username = request.form['username']
        password = request.form['password']
        db = get_db()
        error = None
        user = db.execute(
            'SELECT * FROM user WHERE username = ?', (username,)
        ).fetchone()

        if user is None:
            error = 'Incorrect username.'
        elif not check_password_hash(user['password'], password):
            error = 'Incorrect password.'

        if error is None:
            session.clear()
            session['user_id'] = user['id']
            flash('You were successfully logged in')
            return redirect(url_for('index.index'))

        flash(error)

    return render_template('login.html')


@indexblueprint.route('/logout')
def logout():
    session.clear()
    return redirect(url_for('index.index'))


@indexblueprint.route('/')
def index():
    return render_template('index.html')


@indexblueprint.route('/toggleled', methods=['POST'])
def toggleled():
    print('here1')
    if request.method == 'POST':
        ledstatus = request.json['status']
        print('led status is %d' % ledstatus)
        if ledstatus == True:
            if 1 == led.toggle('ON'):
                return jsonify({'result': 'O N'})
        else:
            if 0 == led.toggle('OFF'):
                return jsonify({'result': 'OFF'})
    return {'result': 'fail'}
