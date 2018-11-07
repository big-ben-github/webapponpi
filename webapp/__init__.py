import os
from flask import Flask
from werkzeug.utils import import_string
from webapp.model import db

blueprints = [
    'webapp.controller:indexblueprint',
]

def create_app(config_name=None):
    # create and configure the app
    app = Flask(__name__, instance_relative_config=True)

    # ensure the instance folder exists
    try:
        os.makedirs(app.instance_path)
    except OSError:
        pass

    app.config.from_mapping(
        DATABASE=os.path.join(app.instance_path, 'webapp.sqlite')
    )
    
    if config_name is None:
        # load the instance config, if it exists, when not testing
        app.config.from_object('webapp.config.ProductionConfig')
    else:
        # load the test config if passed in
        app.config.from_mapping(config_name)

    # initiate the module
    db.init_app(app)

    # load blueprints
    for blueprint in blueprints:
        app.register_blueprint(import_string(blueprint))

    return app
