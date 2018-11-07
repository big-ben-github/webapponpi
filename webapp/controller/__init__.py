from flask import Blueprint

indexblueprint = Blueprint('index', __name__)

from . import index