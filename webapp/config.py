import os
from flask import current_app
basedir = os.path.abspath(os.path.dirname(__file__))
class Config:
    SECRET_KEY = os.environ.get('SECRET_KEY') or os.urandom(16)

class ProductionConfig(Config):
    pass
#   SQLALCHEMY_DATABASE_URI = os.environ.get('DATABASE_RUL') or 'sqlite:///' + os.path.join(basedir, 'data.sqlite')

# class DevelopmentConfig(Config):
#     DEBUG = True
#     SQLALCHEMY_DATABASE_URI = os.environ.get('DEV_DATABASE_RUL') or \'sqlite:///' + os.path.join(basedir, 'data-dev.sqlite')

# class TestingConfig(Config):
#     TESTING = True
#     SQLALCHEMY_DATABASE_URI = os.environ.get('TEST_DATABASE_RUL') or \'sqlite:///' + os.path.join(basedir, 'data-test.sqlite')
