### Extracted from PS8 Finance - Begin ###
import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session, url_for
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash
from functools import wraps
from datetime import datetime


# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

def login_required(f):
    """
    Decorate routes to require login.

    http://flask.pocoo.org/docs/1.0/patterns/viewdecorators/
    """
    @wraps(f)
    def decorated_function(*args, **kwargs):
        if session.get("user_id") is None:
            return redirect("/login")
        return f(*args, **kwargs)
    return decorated_function

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///wikies.db")

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""
    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM user_info WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["user_id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")
### Extracted from ps8 finance - End ###



@app.route("/register", methods=["GET", "POST"])
def register():
    print("ARGGG")
    if request.method == "GET":
        print("ARGGG")
        return render_template("signIn.html")
    else:
        username = request.form.get("username")
        password = request.form.get("password")
        repeatedPassword = request.form.get("repeated-password")
        name = request.form.get("name")
        email = request.form.get("email")


        if isPasswordValid(password):

            if password == repeatedPassword:
                passwordHash = generate_password_hash(password)
                db.execute("INSERT INTO user_info (username, hash, name, email)"
                          +"VALUES (?, ?, ?, ?)", username, passwordHash, name, email)
                return redirect("/login")

            else:
                return apology("The passwords doesn't match. Try again!")

        else:
            return apology("Invalid password! The password must contain "
                          +"at least 6 characters, containing at least a letter and a number")


@app.route("/create", methods=["GET", "POST"])
@login_required
def create_wiki():
    if request.method == "GET":
        return render_template('create_wiki.html')
    else:
        wiki_name = request.form.get('wiki_name')
        wiki_intro = request.form.get('intro')

        db.execute("INSERT INTO wiki_info (wiki_name, wiki_intro) VALUES (?, ?)", wiki_name, wiki_intro)
        addUserToWiki(wiki_name)

        return redirect("/")


@app.route("/post_data", methods=["GET", "POST"])
@login_required
def post_data():
    if request.method == "POST":
        tags = request.form.get("tags")
        title = request.form.get("title")
        text = request.form.get("text")
        wiki_id = request.form.get("wiki_id")

        addDataToWiki(title, text, tags, wiki_id)

        return redirect(url_for("wiki", wiki_id=wiki_id))
    else:
        wiki_id = request.args.get('wiki_id')
        wiki_name = getWikiName(wiki_id)
        return render_template('post.html', wiki_id=wiki_id, wiki_name=wiki_name)

@app.route("/", methods=["GET"])
@login_required
def wiki_homepage():
    if request.method == "GET":
        wikies = getUserWikies()
        return render_template("wikies.html", wikies = wikies)
    else:
        pass


@app.route("/redirect", methods=["POST"])
@login_required
def redirectToWiki():
    if request.method == "POST":
        wiki_id = request.form.get("wiki_id")
        return redirect(url_for("wiki", wiki_id=wiki_id))
    else:
        pass


@app.route("/redirect_post", methods=["POST"])
@login_required
def redirectToPost():
    if request.method == "POST":
        wiki_id = request.form.get("wiki_id")
        return redirect(url_for("post_data", wiki_id=wiki_id))
    else:
        pass

@app.route("/redirect_posted", methods=["POST"])
@login_required
def redirectToPosted():
    if request.method == "POST":
        data_id = request.form.get("data_id")
        return redirect(url_for("read_data", data_id=data_id))
    else:
        pass

@app.route("/<wiki_id>", methods=["GET", "POST"])
@login_required
def wiki(wiki_id):
    if request.method == "GET":
        wiki_name = getWikiName(wiki_id)
        wiki = getWikiData(wiki_id)
        return render_template("wiki.html", wiki_id = wiki_id, wiki_name = wiki_name, wiki=wiki)
    else:
        if request.form['action'] == 'delete':
            deleteWiki(wiki_id)
        elif request.form['action'] == 'leave':
            removeUserFromWiki(wiki_id)
        else:
            pass

        return redirect("/")


@app.route("/wiki/<data_id>", methods=["GET", "POST"])
@login_required
def read_data(data_id):
    if request.method == "GET":
        data_posted = getPostedData(data_id)
        data = data_posted["data"]
        wiki_data_name = data_posted["wiki_data_name"]
        return render_template("posted.html", data=data, wiki_data_name=wiki_data_name, data_id = data_id)
    else:
        wiki_id = getWikiIdFromData(data_id)
        deleteWikiInfo(data_id)
        return redirect(url_for("wiki", wiki_id=wiki_id))

@app.route("/pending_requests", methods=["GET", "POST"])
@login_required
def pending_requests():
    if request.method == "GET":
        requests = getUserRequests()
        print(requests)
        return render_template("requests.html", requests=requests)
    else:
        wiki_name = request.form.get("wiki_name")
        request_id = request.form.get("request_id")

        if request.form['request_answer'] == 'confirm':
            addUserToWiki(wiki_name)
            deleteRequest(request_id)
        elif request.form['request_answer'] == 'delete':
            deleteRequest(request_id)
        else:
            pass

        return redirect("/")


@app.route("/invite", methods=["GET", "POST"])
@login_required
def invite():
    if request.method == "POST":
        wiki_id = request.form.get("wiki_id")
        wiki_name = getWikiName(wiki_id)
        user_id = session.get("user_id")
        return render_template("invite.html", wiki_id=wiki_id, wiki_name=wiki_name, user_id=user_id)
    else:
        pass

@app.route("/invited", methods=["GET", "POST"])
@login_required
def invited():
    if request.method == "POST":
        email = request.form.get("email")
        if isEmailValid(email):
            requester_id = request.form.get("user_id")
            wiki_id = request.form.get("wiki_id")
            placeRequest(requester_id, wiki_id, email)
            return redirect("/")
        else:
            return apology("Email not Found")
    else:
        pass

@app.route("/members", methods=["POST"])
@login_required
def members():
    if request.method == "POST":
        wiki_id = request.form.get("wiki_id")
        members = getWikiMembers(wiki_id)

        return render_template("members.html", members=members)
    else:
        pass

def isPasswordValid(password):
    if hasDigit(password) and hasChar(password) and hasProperLength(password):
        return True
    else:
        return False


def apology(message, code=400):
    return render_template("apology.html", top=code, bottom=message)

# Checks if the string has one ore more digits
def hasDigit(inputString):
    for symbol in inputString:
        if symbol.isdigit():
            return True

    return False


# Checks if the string has one ore more letter
def hasChar(inputString):
    for symbol in inputString:
        if symbol.isalpha():
            return True

    return False

# Check if the string has more than 6 chars
def hasProperLength(inputString):

    if len(inputString) < 6:
        return False

    return True

def getWikiId(wiki_name):
    return db.execute('SELECT wiki_id FROM wiki_info WHERE wiki_name = (?);', wiki_name)

def addUserToWiki(wiki_name):
    wiki_id = getWikiId(wiki_name)[0]['wiki_id']
    user_id = session.get("user_id")

    db.execute("INSERT INTO wiki_members (user_id, wiki_id) VALUES (?, ?)", user_id, wiki_id)

def getWikiIntro(wiki_name):
    db.execute("SELECT wiki_intro from wiki_info WHERE wiki_name = (?)", wiki_name)

def getUserWikies():
    user_id = session.get("user_id")
    return db.execute("SELECT wiki_name, wiki_intro, wiki_id FROM wiki_info WHERE wiki_id in (SELECT wiki_id FROM wiki_members WHERE user_id = (?))", user_id)

def getDataId(title):
    return db.execute("SELECT data_id FROM wiki_data WHERE wiki_data_name = (?)", title)

def addDataToWiki(title, text, tags, wiki_id):
    db.execute("INSERT INTO wiki_data (wiki_id, data, wiki_data_name) VALUES (?, ?, ?)", wiki_id, text, title)
    data_id = getDataId(title)[0]["data_id"]

    tags = tags.split(",")
    for tag in tags:
        db.execute("INSERT INTO wiki_tags (data_id, tag_name) VALUES (?, ?)", data_id, tag)

def getWikiName(wiki_id):
    return db.execute("SELECT wiki_name FROM wiki_info WHERE wiki_id = (?)", wiki_id)[0]["wiki_name"]

def getWikiData(wiki_id):
    return db.execute("SELECT wiki_data_name, tag_name, data.data_id FROM wiki_data as data JOIN wiki_tags as tags ON data.data_id ="
                        +"tags.data_id WHERE data.wiki_id = (?)", wiki_id)

def getPostedData(data_id):
    return db.execute("SELECT wiki_data_name, data FROM wiki_data WHERE data_id = (?)", data_id)[0]

def isEmailValid(email):
    if db.execute("SELECT email FROM user_info WHERE email = (?)", email):
        return True
    else:
        return False

def getUserName(user_id):
    return db.execute("SELECT name FROM user_info WHERE user_id = (?)", user_id)[0]["name"]

def getUserIdByEmail(email):
    return db.execute("SELECT user_id FROM user_info WHERE email = (?)", email)[0]["user_id"]

def placeRequest(requester_id, wiki_id, email):
    requester = getUserName(requester_id)
    requested_id = getUserIdByEmail(email)
    wiki_name = getWikiName(wiki_id)

    db.execute("INSERT INTO requests (requester, requester_id, requested_id, wiki_name, wiki_id) VALUES (?, ?, ?, ?, ?)",
                requester, requester_id, requested_id, wiki_name, wiki_id)

def getUserRequests():
    user_id = session.get("user_id")
    return db.execute("SELECT * FROM requests WHERE requested_id = (?)", user_id)

def deleteRequest(request_id):
    db.execute("DELETE FROM requests WHERE request_id = (?)", request_id)

def deleteWikiInfo(data_id):
    db.execute("DELETE FROM wiki_data WHERE data_id = (?)", data_id)

def getWikiIdFromData(data_id):
    return db.execute("SELECT wiki_id FROM wiki_data WHERE data_id = (?)", data_id)[0]["wiki_id"]

def getWikiMembers(wiki_id):
    return db.execute("SELECT name, email FROM user_info WHERE user_id IN (SELECT user_id from "
                        +"wiki_members where wiki_id = (?))", wiki_id)

def deleteWiki(wiki_id):
    db.execute("DELETE FROM wiki_data WHERE wiki_id = (?)", wiki_id)
    db.execute("DELETE FROM wiki_tags WHERE data_id IN (SELECT data_id FROM wiki_data WHERE wiki_id = (?))", wiki_id)
    db.execute("DELETE FROM wiki_members WHERE wiki_id = (?)", wiki_id)
    db.execute("DELETE FROM wiki_info WHERE wiki_id = (?)", wiki_id)

def removeUserFromWiki(wiki_id):
    user_id = session.get("user_id")
    db.execute("DELETE FROM wiki_members WHERE wiki_id = (?) AND user_id = (?)", wiki_id, user_id)